# NetUtil
Util functions for socket programming in C. Originally used in uni assignments.

## Compile
Compile with `-std=gnu99` flag.
```
gcc *.c -std=gnu99
```

## Example
**Server**:
```c
#include "netutil.h"

// server config
const char port[] = "1234";

int main(void) {
    struct addrinfo hints = tcphints(AF_UNSPEC, AI_PASSIVE), *res;
    getaddrinfo(NULL, port, &hints, &res);

    int sockfd = addrsock(res);
    addrbind(sockfd, res);
    addrreuse(sockfd);
    freeaddrinfo(res);

    struct sockaddr_storage remote;
    socklen_t rsize = sizeof remote;
    listen(sockfd, 10);
    int accfd = accept(sockfd, TOSADDR(&remote), &rsize);
    // do network stuff with accfd
    return 0;
}
```

**Client**:
```c
#include "netutil.h"

// remote server config
const char addr[] = "192.168.1.1";
const char port[] = "1234";

int main(void) {
    struct addrinfo hints = tcphints(AF_UNSPEC, AI_PASSIVE), *res;
    getaddrinfo(addr, port, &hints, &res);

    int sockfd = addrsock(res);
    addrconn(sockfd, res);
    freeaddrinfo(res);
    // do network stuff with sockfd
    return 0;
}
```
