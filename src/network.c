#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>         /* See NOTES */
#include <sys/socket.h>
#include <sys/types.h>         /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <libcork/core.h>
#include <logger.h>
#include <utils.h>

void _die(ut_logger_context_t *ut_ctx, int exit_code, const char *fmt, ...)
{
  va_list arg;
  if (ut_ctx != NULL)
    printf("%s:%s:%d ", ut_ctx->m_file, ut_ctx->m_func, ut_ctx->m_lineno);
  va_start(arg, fmt);
  vfprintf(stdout, fmt, arg);
  va_end(arg);
  abort();
  return;
}

extern int tcp_new()
{
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(fd > 0 );
  logger_debug("fd=%d\n", fd);
  return fd;
}

extern int tcp_connect(const char *host, int port)
{
  return 0;
}

extern int tcp_bind(int fd, const char *host, short int port)
{
  int optval = 1;
  struct sockaddr_in serv_addr;
  int rc;

  if (host != NULL) {
      struct cork_ip ip;
      if (cork_ip_init(&ip, host) != -1) {
          if (ip.version == 4) {
              logger_debug("cork_ip_init\n");
              memset(&serv_addr, 0, sizeof(struct sockaddr_in));
              struct sockaddr_in *addr = &serv_addr;
              inet_pton(AF_INET, host, &addr->sin_addr);
              addr->sin_family = AF_INET;
              logger_info("binding to outbound IPv4 addr: %s\n", host);
          } else if (ip.version == 6) {
              /*
              memset(storage_v6, 0, sizeof(struct sockaddr_storage));
              struct sockaddr_in6 *addr = (struct sockaddr_in6 *)storage_v6;
              inet_pton(AF_INET6, host, &addr->sin6_addr);
              addr->sin6_family = AF_INET6;
              LOGI("binding to outbound IPv6 addr: %s", host);
              */
            die(1, "use IPv6\n");
              return 0;
          }
      }
  }
  else
    {
      die(1, "ipv6");
    }

  serv_addr.sin_family = AF_INET;
  //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  rc = bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
  if (rc < 0)
    {
      die(1, "bind() fail\n");
    }
  setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
  return rc;
}

extern int tcp_listen(int sfd, int backlog)
{
 return listen(sfd, backlog);
}

extern int tcp_accept(int sfd)
{
 int client_fd = accept(sfd, NULL, NULL);
 return client_fd;
}

int tcp_setnonblock(int fd)
{
  int flags;

  flags = fcntl(fd, F_GETFL);
  flags |= O_NONBLOCK;
  return fcntl(fd, F_SETFL, flags);
}
ssize_t tcp_read(int fd, void *buf, size_t count)
{
  return read(fd, buf, count);

}
