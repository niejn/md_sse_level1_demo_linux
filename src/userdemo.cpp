#include "userfunc.hpp"
#include "FixSseApiStruct.hpp"
#include <pthread.h>

static int loadConfigFile(xele_md::userinfo &u_info)
{
    int ini_flag = 0;
    memset(&u_info, 0, sizeof(u_info));

    FILE* ini = popen("sed 's/LOCALADDRESS=\\(.*\\)/\\1/g;tx;d;:x' refer.ini", "r");
    fscanf(ini, "%s\n", u_info.localaddress);
    pclose(ini);
    if (strlen(u_info.localaddress) == 0)
    {
        ini_flag = -1;
    }

    ini = popen("sed 's/MCASTADDRESS=\\(.*\\)/\\1/g;tx;d;:x' refer.ini", "r");
    fscanf(ini, "%s\n", u_info.mcastaddress);
    pclose(ini);
    if (strlen(u_info.mcastaddress) == 0)
    {
        ini_flag = -1;
    }

    return ini_flag;
}

int main()
{
    /*read refer.ini*/
    FILE* fd = fopen("refer.ini","r");
    if (fd == NULL)
    {
        printf("no config file..\n");
        return -1;
    }
    xele_md::userinfo user_info;
    int ret = loadConfigFile(user_info);
    if (ret != 0)
    {
        printf("user config file error..\n");
        return -1;
    }

    xele_md::XeleMdFixApi* api = new xele_md::XeleMdFixApi(user_info);
    api->print_userinfo();

    pthread_t ptr_recv;
    pthread_t ptr_write;
    int p_msg = pthread_create(&ptr_recv,NULL,api->recv_marketinfo,api);
//    int p_msg = pthread_create(&ptr_recv,NULL,api->recv_markwrite_marketlogetinfo,api);
    if (p_msg != 0)
    {
        printf("create market thread failed\n");
        delete api;
        api = NULL;
        return -1;
    }

    int p_write = pthread_create(&ptr_write,NULL,api->write_marketlog,api);
    if (p_write != 0)
    {
        printf("create writestore thread failed\n");
        api->fin();
        delete api;
        api = NULL;
        return -1;
    }

    char usr_str[255] = "";
    printf("Input 'q' to disconnect API:");
    while (1)
    {
        scanf("%s",usr_str);
        if (strlen(usr_str) == 1 && strcmp(usr_str,"q") == 0)
        {
            api->fin();
            break;
        }
        else
        {
            printf("Input 'q' to disconnect API:");
        }
    }

    delete api;
    api = NULL;

    return 0;
}
