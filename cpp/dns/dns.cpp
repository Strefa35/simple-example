#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>


#define GET_FAMILY_NAME(_family) (      \
 (_family) == AF_PACKET ? "AF_PACKET" : \
 (_family) == AF_INET   ? "AF_INET"   : \
 (_family) == AF_INET6  ? "AF_INET6"  : \
                          "???"         \
)

/*
  struct addrinfo {
      int              ai_flags;
      int              ai_family;
      int              ai_socktype;
      int              ai_protocol;
      socklen_t        ai_addrlen;
      struct sockaddr *ai_addr;
      char            *ai_canonname;
      struct addrinfo *ai_next;
  };
*/

static int getIpAddr(const char* host, const char* port) {

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s;

    /* Obtain address(es) matching host/port */
    std::memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family   = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = 0;//SOCK_STREAM;
    hints.ai_flags    = AI_NUMERICSERV | AI_NUMERICHOST | AI_V4MAPPED | AI_ALL;
    hints.ai_protocol = 0;          /* Any protocol */

    s = getaddrinfo(host, port, nullptr/*&hints*/, &result);
    if (s != 0) {
        std::fprintf(stderr, "getaddrinfo: %d=%s\n", s, gai_strerror(s));
        return EXIT_FAILURE;
    }

    std::printf("====> DNS -> host: %s, port: %s\n", host ? host : "-", port ? port : "-");
    for (rp = result; rp != nullptr; rp = rp->ai_next) {

        std::printf("  +---> socket[%p](ai_family: %d [%s], ai_socktype: %d, ai_protocol: %d, ai_canonname: %p), ai_addrlen: %d\n", rp,
                    rp->ai_family, GET_FAMILY_NAME(rp->ai_family), rp->ai_socktype, rp->ai_protocol, rp->ai_canonname, rp->ai_addrlen);

        std::printf("    +---> len: %d\n", rp->ai_addrlen);

        if (rp->ai_addr && rp->ai_addrlen) {

            unsigned char* addr_ptr = (unsigned char*) rp->ai_addr;
            struct sockaddr_in   *ip4ptr = nullptr;
            struct sockaddr_in6  *ip6ptr = nullptr;

            std::printf("    +---> addr: \n");
            std::printf("      +---> [HEX]: ");
            for (int i = 0; i < rp->ai_addrlen; i++)
            {
              std::printf(" %02x ", addr_ptr[i]);
            }
            std::printf("\n");
            std::printf("      +---> [DEC]: ");
            for (int i = 0; i < rp->ai_addrlen; i++)
            {
              std::printf("%3d ", addr_ptr[i]);
            }
            std::printf("\n");

            switch (rp->ai_family) {
                case AF_INET: {
                    ip4ptr = (struct sockaddr_in *) rp->ai_addr;
                    if (ip4ptr) {
                        std::printf("      +---> IPv4\n");
                        std::printf("        +--->     addr: %d.%d.%d.%d\n", (ip4ptr->sin_addr.s_addr >> 0) & 0xff, (ip4ptr->sin_addr.s_addr >> 8) & 0xff, (ip4ptr->sin_addr.s_addr >> 16) & 0xff, (ip4ptr->sin_addr.s_addr >> 24) & 0xff);
                        std::printf("        +--->     port: %u\n", ntohs(ip4ptr->sin_port));
                        std::printf("        +--->   family: %d\n", ip4ptr->sin_family);
                        
                    }
                    break;
                } 
                case AF_INET6: {
                    ip6ptr = (struct sockaddr_in6 *) rp->ai_addr;
                    if (ip6ptr) {
                        std::printf("      +---> IPv6\n");
                        std::printf("        +--->     addr: %p\n", ip6ptr->sin6_addr.__in6_u.__u6_addr8);
                        std::printf("        +--->     port: %u\n", ntohs(ip6ptr->sin6_port));
                        std::printf("        +--->   family: %d\n", ip6ptr->sin6_family);
                        std::printf("        +---> flowinfo: %d\n", ip6ptr->sin6_flowinfo);
                        std::printf("        +---> scope_id: %d\n", ip6ptr->sin6_scope_id);
                    }
                    break;
                }
                default: {
                  break;
                }
            }

        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    int result = 0;

    if ((argc == 2) || (argc == 3)) {

        const char* host = nullptr;
        const char* port = nullptr;

        host = argv[1];
        if (argc == 3) {
            port = argv[2];
        }

        result = getIpAddr(host, port);
        if (result) {
            exit(result);
        }
    }
    else {
        std::printf("Wrong params -> should be dns <host> <port>\n");
    }
    return 0;
}