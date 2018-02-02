#include "userfunc.hpp"
#include "FixSseType.hpp"

namespace xele_md
{
    extern void Print_Sse_MarketData_Stock(FILE* , struct Sse_MarketData_Stock obj);
    extern void Print_Sse_MarketData_Other(FILE* logfd,struct Sse_MarketData_Other obj);
    extern void Print_Sse_MarketData_Option(FILE* logfd,struct Sse_MarketData_Option obj);

    queue_buf queue_buffer[queue_length + 1];
    int queue_point;

    XeleMdFixApi::XeleMdFixApi(struct userinfo &u_info)
    {
         UserInfo = &u_info;
         local_ipaddr = UserInfo->localaddress;
         udp_ipaddr = UserInfo->mcastaddress;

         local_ipaddr = local_ipaddr.substr(local_ipaddr.find("/") + 2,local_ipaddr.size() - local_ipaddr.find("/") - 2);
         local_port = atoi(local_ipaddr.substr(local_ipaddr.find(":") + 1,local_ipaddr.size() - local_ipaddr.find(":") - 1).c_str()); 
         local_ipaddr = local_ipaddr.substr(0,local_ipaddr.find(":"));

         udp_ipaddr = udp_ipaddr.substr(udp_ipaddr.find("/") + 2,udp_ipaddr.size() - udp_ipaddr.find("/") - 2);
         udp_port = atoi(udp_ipaddr.substr(udp_ipaddr.find(":") + 1,udp_ipaddr.size() - udp_ipaddr.find(":") - 1).c_str());
         udp_ipaddr = udp_ipaddr.substr(0,udp_ipaddr.find(":"));
        
         stop_flag = 0;
         memset(queue_buffer,0, sizeof(queue_buffer));
         log_fd = fopen("marketinfo.log","w+");
         if (log_fd == NULL)
         {
             perror("log file open failed..");
         }
    }

    XeleMdFixApi::~XeleMdFixApi()
    {
         fclose(log_fd);
    }

    void* XeleMdFixApi::recv_marketinfo(void* _ptr)
    {
         queue_point = -1;

        XeleMdFixApi* ptr_recv = (XeleMdFixApi*) _ptr;

         int ret = ptr_recv->connect_udp_socket();
         if (ret != 0)
         {
             printf("udp connect failed\n");
             return 0;
         }

         ret = ptr_recv->recv_udp_msg();
         if (ret != 0)
         {
             printf("recv marketinfo failed\n");
             return 0;
         }

         return _ptr;
    }
    //
    void* XeleMdFixApi::write_marketlog(void* _ptr)
    {
        XeleMdFixApi* ptr_write = (XeleMdFixApi*) _ptr;
         int q_point = 0;
        
         while (1)
         {
             while (queue_buffer[q_point].num > 0 && ptr_write->stop_flag == 0)
             {
                 //queue_buffer[q_point].num = -1; 只有num大于0,才会继续读取数据。
                 switch (queue_buffer[q_point].num)
                 {
                     case 1:
                     {
                         Print_Sse_MarketData_Stock(ptr_write->log_fd,*(Sse_MarketData_Stock*)queue_buffer[q_point].buffer);
                         queue_buffer[q_point].num = -1;
                         ++q_point;
                         if (q_point > queue_length)
                         {
                             q_point = 0;
                         }
                         break;
                     }
                     case 2:
                     {
                         Print_Sse_MarketData_Other(ptr_write->log_fd,*(Sse_MarketData_Other*)queue_buffer[q_point].buffer);
                         queue_buffer[q_point].num = -1;
                         ++q_point;
                         if (q_point > queue_length)
                         {
                             q_point = 0;
                         }
                         break;
                     }
                     case 3:
                     {
                         Print_Sse_MarketData_Option(ptr_write->log_fd,*(Sse_MarketData_Option*)queue_buffer[q_point].buffer);
                         queue_buffer[q_point].num = -1;
                         ++q_point;
                         if (q_point > queue_length)
                         {
                             q_point = 0;
                         }
                         break;
                     }
                 }
             }

             if (ptr_write->stop_flag != 0)
             {
                 break;
             }
         }

         return _ptr;
    }

    int XeleMdFixApi::fin()
    {
        int ret = close_udp_socket();
        stop_flag = 1;
        if (ret != 0)
        {
            return 1;
        }

        return 0;
    }

    int XeleMdFixApi::connect_udp_socket()
    {
        struct sockaddr_in clientaddr;
        struct ip_mreq mreq;
        sock_udp_fd = socket(AF_INET,SOCK_DGRAM,0);
        if (sock_udp_fd < 0)
        {
            perror("Opening datagram socket error");
            return 1;
        }
        bzero(&mreq, sizeof(struct ip_mreq));
        mreq.imr_multiaddr.s_addr = inet_addr(udp_ipaddr.c_str());
        mreq.imr_interface.s_addr = inet_addr(local_ipaddr.c_str());
        //mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (setsockopt(sock_udp_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq)) != 0)
        {
            perror("setsockopt");
            return 1;
        }
        bzero(&clientaddr,sizeof(clientaddr));
        clientaddr.sin_family = AF_INET;
        clientaddr.sin_port = htons(udp_port);
        if (inet_pton(AF_INET,udp_ipaddr.c_str(),&clientaddr.sin_addr) <= 0)
        {
            perror("Wrong dest IP address!");
            return 1;
        }
        if (bind(sock_udp_fd, (struct sockaddr *) &clientaddr,sizeof(struct sockaddr_in)) == -1)
        {
            printf("Bind error\n");
            return 1;
        }

        return 0;
    }

    //接受udp行情， 并且将行情写入环stack中
    int XeleMdFixApi::recv_udp_msg()
    {
        u_char buffer[max_buffer_length];
        memset(buffer, 0, sizeof(buffer));
        printf("begin udp thread recv.\n");
        while (recvfrom(sock_udp_fd, buffer, max_buffer_length, 0, NULL, NULL) >= 0)
        {
            u_char msg_type[MD_STREAMID_LEN];
            memcpy(msg_type, buffer + SEQ_NUM_LEN, MD_STREAMID_LEN);
            if (strcmp((char*)(&msg_type), "MD001") == 0)
            {
                if (queue_point == queue_length)
                {
                    queue_point = -1;
                }
                memcpy(&queue_buffer[queue_point + 1].buffer, buffer, sizeof(Sse_MarketData_Stock));
                queue_buffer[queue_point + 1].num = 1;
                ++queue_point;
            }
            else if (strcmp((char*)(&msg_type),"M0301") == 0)
            {
                if (queue_point == queue_length)
                {
                    queue_point = -1;
                }
                memcpy(&queue_buffer[queue_point + 1].buffer, buffer, sizeof(Sse_MarketData_Option));
                queue_buffer[queue_point + 1].num = 3;
                ++queue_point;
            }
            else if ((strcmp((char*)(&msg_type),"MD002") == 0)
                    || (strcmp((char*)(&msg_type),"MD003") == 0)
                    || (strcmp((char*)(&msg_type),"MD004") == 0))
            {
                if (queue_point == queue_length)
                {
                    queue_point = -1;
                }
                memcpy(&queue_buffer[queue_point + 1].buffer, buffer, sizeof(Sse_MarketData_Other));
                queue_buffer[queue_point + 1].num = 2;
                ++queue_point;
            }
            else
                fprintf(stderr, "wrong message type :%s\n", msg_type);
        }
        return 0;
    }

    int XeleMdFixApi::close_udp_socket()
    {
        if (sock_udp_fd == -1)
        {
            return 1;
        }

        close(sock_udp_fd);
        return 0;
    }

    void XeleMdFixApi::print_userinfo()
    {
         printf("--------------------------------\n");
         fprintf(stderr, "LOCALADDRESS:%s\n", UserInfo->localaddress);
         fprintf(stderr, "MCASTADDRESS:%s\n", UserInfo->mcastaddress);
         printf("--------------------------------\n");
    }

}

