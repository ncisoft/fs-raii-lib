#include <unistd.h>
#include <stdio.h>
#include <fs_raii.h>
#include <logger.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <utils.h>
#define g_warning logger_debug

struct sock_ev_serv {
  ev_io io;
  int fd;
  struct sockaddr_un socket;
  int socket_len;
};

struct sock_ev_client {
  ev_io io;
  int fd;
  int index;
  struct sock_ev_serv* server;
};

static void client_cb(EV_P_ ev_io *w, int revents)
{
  // a client has become readable

  struct sock_ev_client* client = (struct sock_ev_client*) w;

  int n;
  char str[BUFSIZ] = ".\0";

  printf("[r]");
  n = recv(client->fd, str, BUFSIZ, 0);
  if (n <= 0) {
    if (0 == n) {
      // an orderly disconnect
      logger_debug("orderly disconnect\n");
      ev_io_stop(EV_A_ &client->io);
      close(client->fd);
    }  else if (EAGAIN == errno) {
      puts("should never get in this state with libev");
    } else {
      perror("recv");
    }
    return;
  }
  logger_debug("socket client said: %s\n", str);

  // Assuming that whenever a client is readable, it is also writable ?
  if (send(client->fd, str, n, 0) < 0) {
    perror("send");
  }
}
inline static struct sock_ev_client* client_new(int fd)
{
  struct sock_ev_client* client;

  client = realloc(NULL, sizeof(struct sock_ev_client));
  client->fd = fd;
  //client->server = server;
  tcp_setnonblock(client->fd);
  ev_io_init(&client->io, client_cb, client->fd, EV_READ);

  return client;
}

static void not_blocked(EV_P_ ev_periodic *w, int revents)
{
  logger_debug("I'm ev_periodic\n");
}


static void server_cb(EV_P_ ev_io *w, int revents);
static void server_init(struct sock_ev_serv* server, const char *host, int port)
{
  EV_P  = ev_default_loop(0);
  int sfd = tcp_new();
  tcp_bind(sfd, "0.0.0.0", 8001);
  tcp_listen(sfd, 10);
  tcp_setnonblock(sfd);
  server->fd = sfd;
  ev_io_init(&server->io, server_cb, server->fd, EV_READ);
  ev_io_start(EV_A_ &server->io);
}

static void server_cb(EV_P_ ev_io *w, int revents)
{
  logger_debug("unix stream socket has become readable\n");

  int client_fd;
  struct sock_ev_client* client;

  // since ev_io is the first member,
  // watcher `w` has the address of the
  // start of the sock_ev_serv struct
  struct sock_ev_serv* server = (struct sock_ev_serv*) w;

  while (1)
  {
    client_fd = tcp_accept(server->fd);
    if( client_fd == -1 )
    {
      if( errno != EAGAIN && errno != EWOULDBLOCK )
      {
        logger_warn("accept() failed errno=%i (%s)\n",  errno, strerror(errno));
        exit(EXIT_FAILURE);
      }
      break;
    }
    logger_info("accepted a client\n");
    client = client_new(client_fd);
    client->server = server;
    ev_io_start(EV_A_ &client->io);
  }
}

int main()
{
  struct sock_ev_serv server;
  struct ev_periodic every_few_seconds;

  logger_init(NULL, LOGGER_LEVEL_DEBUG);

  EV_P  = ev_default_loop(0);
  ev_periodic_init(&every_few_seconds, not_blocked, 0, 5, 0);
  ev_periodic_start(EV_A_ &every_few_seconds);
  server_init(&server, "0.0.0.0", 8001);
  ev_loop(EV_A_ 0);
  return 0;
}
