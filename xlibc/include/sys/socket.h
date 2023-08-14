#pragma once

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sockaddr {
};

int accept(int sockfd, struct sockaddr * addr, socklen_t * addrlen);
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int socket(int domain, int type, int protocol);


#ifdef __cplusplus
}
#endif
