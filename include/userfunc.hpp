#ifndef USERFUNC_HPP
#define USERFUNC_HPP

#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>  
#include <sys/socket.h> 
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <iconv.h>
#include <locale.h>
#include <sstream>
#include <exception>
#include "FixSseApiStruct.hpp"

#define MEMB_SIZEOF(st_type, member) sizeof(((st_type*)0)->member)

namespace xele_md 
{
    struct userinfo
    {
          char localaddress[40];
          char mcastaddress[40];
    };

    struct queue_buf
    {
        int num;
        u_char buffer[max_buffer_length];
    };


    class XeleMdFixApi
    {
    public:
          XeleMdFixApi(userinfo &);
          ~XeleMdFixApi();

          int fin();
          static void* recv_marketinfo(void* _ptr);
          static void* write_marketlog(void* _ptr);

          void print_userinfo();

    protected:
          struct userinfo* UserInfo;

          int connect_udp_socket();
          int recv_udp_msg();
          int close_udp_socket();

          int sock_udp_fd;
          FILE* log_fd;

          std::string local_ipaddr;
          std::string udp_ipaddr;
          int local_port;
          int udp_port;
          int stop_flag;

    };
}

#endif
