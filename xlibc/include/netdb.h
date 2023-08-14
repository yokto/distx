#pragma once

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOL_SOCKET 1
#define SO_REUSEADDR 2
#define AF_INET 3
#define SOCK_STREAM 4
#define AI_PASSIVE 5

struct addrinfo {
    int ai_flags;
    int ai_family;
    int ai_socktype;
    int ai_protocol;
    size_t ai_addrlen;
    char * ai_canonname;
    struct sockaddr * ai_addr;
    struct addrinfo * ai_next;
};

int getaddrinfo(const char * node, const char * service, const struct addrinfo * hints, struct addrinfo ** res);

const char *gai_strerror(int errcode);

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

#ifdef __cplusplus
}
#endif
