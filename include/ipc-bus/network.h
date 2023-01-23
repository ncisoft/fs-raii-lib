#pragma once

#include <ev.h>
#include <sys/un.h>
#include <unistd.h>

extern int tcp_new();
extern int tcp_connect(const char *host, int port);
extern int tcp_bind(int fd, const char *host, short int port);
extern int tcp_accept(int sfd);
extern int tcp_listen(int sfd, int backlog);
extern int tcp_setnonblock(int fd);
extern ssize_t tcp_read(int fd, void *buf, size_t count);
