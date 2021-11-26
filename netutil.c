///////////////////////////////////////////////////////////////////////////////
// Helper functions for networking.
// https://github.com/PotatoMaster101/netutil
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>          // fputs()
#include <errno.h>          // errno
#include "netutil.h"

// Size per chunk when doing IO reading.
#define FCHUNKSIZE 1024

// Validates the given IPv4 address.
//
// PARAMS:
// ip - the IPv4 address to validate
//
// RETURN:
// 1 if the IPv4 address is valid, 0 otherwise.
_Bool valid4(const char *ip) {
    struct sockaddr_in sa;
    return (inet_pton(AF_INET, ip, &(sa.sin_addr)) == 1);
}

// Validates the given IPv6 address.
//
// PARAMS:
// ip - the IPv6 address to validate
//
// RETURN:
// 1 if the IPv6 address is valid, 0 otherwise.
_Bool valid6(const char *ip) {
    struct sockaddr_in6 sa;
    return (inet_pton(AF_INET, ip, &(sa.sin6_addr)) == 1);
}

// Sends all the content from the buffer using the given socket.
//
// PARAMS:
// sockfd - the socket file descriptor
// buf    - the buffer to send
// n      - size of the buffer
//
// RETURN:
// The number of bytes sent, or SIZE_MAX on error.
size_t sendall(int sockfd, const char *buf, size_t n) {
    if (buf == NULL)
        return SIZE_MAX;

    int sendres = 0;    // return value from send()
    size_t curr = 0, left = n;
    while (curr < n) {
        sendres = send(sockfd, buf + curr, left, 0);
        if (sendres == -1)
            return SIZE_MAX;        // send() error
        curr += sendres;
        left -= sendres;
    }
    return curr;
}

// Sends all the content from the file stream using the given socket.
//
// PARAMS:
// sockfd - the socket file descriptor
// fs     - the file stream to send
//
// RETURN:
// The number of bytes sent, or SIZE_MAX on error.
size_t sendfs(int sockfd, FILE *fs) {
    if (fs == NULL)
        return SIZE_MAX;

    size_t sent = 0, fres = 0;
    char block[FCHUNKSIZE];
    while ((fres = (fread(block, sizeof(char), sizeof block, fs))) > 0) {
        size_t sendres = sendall(sockfd, block, fres);
        if (sendres == SIZE_MAX)
            return SIZE_MAX;        // send() error
        sent += sendres;
    }
    return sent;
}

// Sends a long integer (converted to string) using the given socket.
//
// PARAMS:
// sockfd - the socket file descriptor
// n      - the number to send
//
// RETURN:
// The number of bytes sent, or SIZE_MAX on error.
size_t sendlong(int sockfd, long n) {
    char num[25];
    int res = snprintf(num, sizeof num, "%ld", n);
    return (res > 0) ? sendall(sockfd, num, res + 1) : SIZE_MAX;
}

// Sends a double (converted to string) using the given socket.
//
// PARAMS:
// sockfd - the socket file descriptor
// n      - the number to send
//
// RETURN:
// The number of bytes sent, or SIZE_MAX on error.
size_t senddouble(int sockfd, double n) {
    char num[50];
    int res = snprintf(num, sizeof num, "%lf", n);
    return (res > 0) ? sendall(sockfd, num, res + 1) : SIZE_MAX;
}

// Returns an addrinfo using the given settings.
//
// PARAMS:
// fam   - the address family, use AF_UNSPEC for both IPv4/IPv6
// stype - the socket type
// flags - extra flags
//
// RETURN:
// The addrinfo created using the given settings.
struct addrinfo gethints(int fam, int stype, int flags) {
    struct addrinfo ret;
    memset(&ret, 0, sizeof ret);
    ret.ai_family = fam;
    ret.ai_socktype = stype;
    ret.ai_flags = flags;
    return ret;
}

// Returns a general addrinfo for TCP connections. This call is equivalent to
// gethints(fam, SOCK_STREAM, flags).
//
// PARAMS:
// fam   - the address family, use AF_UNSPEC for both IPv4/IPv6
// flags - extra flags
//
// RETURN:
// The addrinfo created using the given setting.
struct addrinfo tcphints(int fam, int flags) {
    return gethints(fam, SOCK_STREAM, flags);
}

// Returns a general addrinfo for UDP connections. This call is equivalent to
// gethints(fam, SOCK_DGRAM, flags).
//
// PARAMS:
// fam   - the address family, use AF_UNSPEC for both IPv4/IPv6
// flags - extra flags
//
// RETURN:
// The addrinfo created using the given setting.
struct addrinfo udphints(int fam, int flags) {
    return gethints(fam, SOCK_DGRAM, flags);
}

// Creates a socket using the addrinfo and returns the socket file descriptor.
//
// PARAMS:
// addr - the addrinfo to create the socket
//
// RETURN:
// The socket file descriptor, or -1 on error.
int addrsock(struct addrinfo *addr) {
    return socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
}

// Binds the given socket to the addrinfo.
//
// PARAMS:
// sockfd - the socket to bind
// addr   - the addrinfo to bind to
//
// RETURN:
// 0 on success, -1 on error.
int addrbind(int sockfd, struct addrinfo *addr) {
    return bind(sockfd, addr->ai_addr, addr->ai_addrlen);
}

// Connects to the given socket to the addrinfo.
//
// PARAMS:
// sockfd - the socket to connect
// addr   - the addrinfo to connect to
//
// RETURN:
// 0 on success, -1 on error.
int addrconn(int sockfd, struct addrinfo *addr) {
    return connect(sockfd, addr->ai_addr, addr->ai_addrlen);
}

// Allows the given socket to reuse addresses.
//
// PARAMS:
// sockfd - the socket to allow address reuse
//
// RETURN:
// 0 on success, -1 on error.
int addrreuse(int sockfd) {
    int yes = 1;
    return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
}

// Returns the IP and port strings for the given address.
//
// PARAMS:
// addr - the address to convert to string
// len  - the length of the address
//
// RETURN:1
// The string representations of IP and port of the given address.
addrstr_t getaddrstr(struct sockaddr *addr, socklen_t len) {
    addrstr_t ret = { .ip = { 0 }, .port = { 0 } };
    getnameinfo(addr, len, ret.ip, sizeof ret.ip, ret.port, sizeof ret.port,
            NI_NUMERICHOST | NI_NUMERICSERV);
    return ret;
}

// Returns a hostent with information about the current host.
//
// RETURN:
// The host entry, or NULL on error.
struct hostent *gethostentry() {
    char hname[1024];
    if (gethostname(hname, sizeof hname) == -1)
        return NULL;
    return gethostbyname(hname);
}

// Returns a fd_set with the given file descriptors set.
//
// PARAMS:
// fds - the file descriptors to set
// n   - the size of the file descriptors
//
// RETURN:
// A new fd_set with the file descriptors set.
fd_set newfds(const int *fds, size_t n) {
    fd_set ret;
    FD_ZERO(&ret);
    if (fds != NULL)
        for (size_t i = 0; i < n; i++)
            FD_SET(fds[i], &ret);
    return ret;
}

// Resets the given fd_set, resetting all the file descriptors.
//
// PARAMS:
// fdset - the fd_set to reset
// fds   - the file descriptors to set
// n     - size of the file descriptors
void clrfds(fd_set *fdset, const int *fds, size_t n) {
    if (fdset != NULL && fds != NULL)
        for (size_t i = 0; i < n; i++)
            FD_SET(fds[i], fdset);
}
