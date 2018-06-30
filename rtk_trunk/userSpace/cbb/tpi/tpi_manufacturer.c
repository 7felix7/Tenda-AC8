#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <network.h>
#include <string.h>
#include <netdb.h>
#include <arp_clients.h>
#include "tenda_arp.h"
#include "cJSON.h"
#include "wl_utility_rltk.h"
#include "debug.h"
#include "manufacturer.h"

static RET_INFO tpi_manufacturer_start();
RET_INFO tpi_manufacturer_first_init()
{
    return tpi_manufacturer_start();
}

RET_INFO tpi_manufacturer_action(RC_MODULES_COMMON_STRUCT *var)
{
    PI_PRINTF(TPI,"op=%d\n",var->op);
    switch(var->op)
    {
        case OP_START:
            tpi_manufacturer_start();
            break;
        default:
            PI_ERROR(TPI,"op[%d] donnot have handle!\n",var->op);
            break;
    }

    return RET_SUC;
}
/*****************************************************************************
 �� �� ��  : connect_to_cloud_server
 ��������  : ���ӵ�������
 �������  : int *sock
             char *host
             unsigned short port
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��2��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
int connect_to_cloud_server(int *sock, char *host, unsigned short port)
{
    struct sockaddr_in addr;
    int len;
    int result;
    struct hostent *hostinfo;

    if((*sock=socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        return(-1);
    }

    hostinfo = gethostbyname(host);

    if(!hostinfo)
    {
        RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"gethostbyname error\n");
        close(*sock);
        return(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;
    addr.sin_port = htons(port);
    memset(addr.sin_zero,0,sizeof(addr.sin_zero));

    len = sizeof(addr);
    if((result=connect(*sock, (struct sockaddr *)&addr, len)) == -1)
    {
        perror("connect");
        close(*sock);
        return(-1);
    }

    return 0;
}
/*****************************************************************************
 �� �� ��  : output_to_cloud
 ��������  : �����������ݱ���
 �������  : int fd
             void *buf
 �������  : ��
 �� �� ֵ  : static

 �޸���ʷ      :
  1.��    ��   : 2016��12��2��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
static void output_to_cloud(int fd, void *buf)
{
    fd_set writefds;
    int max_fd;
    struct timeval tv;
    int ret;

    FD_ZERO(&writefds);
    FD_SET(fd, &writefds);
    max_fd = fd;

    tv.tv_sec = 0;
    tv.tv_usec = 20000;

    ret = select(max_fd + 1, NULL, &writefds, NULL, &tv);

    if(ret == -1)
    {
        RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"select error ret:%d\n",ret);

    }
    else if(ret == 0)
    {
        RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"select timeout ret:%d\n",ret);
    }
    else
    {
        if(FD_ISSET(fd, &writefds))
        {
            if(send(fd, buf, strlen(buf), 0) == -1)
                RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"error send()ing request\n");
        }
        else
            RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"socket was not exist!!\n");
    }
}
/*****************************************************************************
 �� �� ��  : read_input_by_cloud
 ��������  : ��ȡ�������Ļ�Ӧ����
 �������  : int fd
             void *buf
             int len
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��2��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
int read_input_by_cloud(int fd, void *buf, int len)
{
    fd_set readfds;
    int max_fd;
    struct timeval tv;
    int ret;
    int bread = -1;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    max_fd = fd;

    tv.tv_sec = 9;
    tv.tv_usec = 0;
    ret = select(max_fd + 1, &readfds, NULL, NULL, &tv);

    if(ret == -1)
    {
        RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"[cloud]: select error\n");
    }
    else if(ret == 0)
    {
        RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"select timeout\n");
    }
    else
    {
        if(FD_ISSET(fd, &readfds))
        {
            if((bread=recv(fd, buf, len, 0)) == -1)
            {
                RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"error recv()ing reply\n");
            }
        }
        else
            RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"socket was not exist!!\n");
    }

    return(bread);
}
static void make_request(struct mac_node* client_mac, char *request)
{
    char mac_tmp[1024] = {0};
    char mac[6][4];
    struct mac_node* temp_node = client_mac;
    cJSON   *pJson = NULL;
    cJSON   *pArray = NULL;
    cJSON   *pItem = NULL;
    char*  p_send_buf;
    int first_node = 1;
    int request_mac_num = 0;
    pJson = cJSON_CreateObject();
    cJSON_AddItemToObject(pJson, "mac", pArray = cJSON_CreateArray());
    /* һ���������ͻ��˵��豸������Ϣ */
    while(temp_node)
    {
        if (strlen(temp_node->mac))
        {
            if (sscanf(temp_node->mac, "%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%s", mac[0], mac[1],mac[2], mac[3],mac[4], mac[5]) == 6)
            {
            	if(request_mac_num > 49)
					break;
                sprintf(mac_tmp, "%s-%s-%s-%s-%s-%s", mac[0], mac[1],mac[2], mac[3],mac[4], mac[5]);
                cJSON_AddItemToArray(pArray, pItem = cJSON_CreateString(mac_tmp));
		  		request_mac_num++;
		  
            }
        }
        temp_node = temp_node->next;
    }
    p_send_buf = cJSON_Print(pJson);
    /* ��װ������Ϣ */
    sprintf(request, "POST %s HTTP/1.0\r\n"
            "Connection: Keep-Alive\r\n"
            "Accept: */*\r\n"
            "User-Agent: Mozilla/5.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n"
            "Content-Length: %d\r\n"
            "Host: %s\r\n\r\n"
            "%s\r\n",
            CLOUD_REQUST_URL, 2+strlen(p_send_buf), CLOUD_SERVER_NAME, p_send_buf);
    if(NULL != pJson)
    {
        cJSON_Delete(pJson);
        pJson = NULL;
    }

    FREE_P(&p_send_buf);
}

/*****************************************************************************
 �� �� ��  : parse_response_string
 ��������  : �������ݰ�������������
 �������  : char* response
             struct mac_node* dev_mac
 �������  : ��
 �� �� ֵ  : static

 �޸���ʷ      :
  1.��    ��   : 2016��12��2��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
struct facture_name facture_name_head;
static int parse_response_string(char* response,struct mac_node* dev_mac)
{
    int         i = 0;
    int         nCount = 0;
    char      *pos = NULL;
    cJSON   *pJson = NULL;
    cJSON   *pArray = NULL;
    cJSON   *pItem = NULL;
    struct facture_name* temp = NULL;
    struct facture_name* head = &facture_name_head;
    unsigned char ether_mac[6] = {0};
    struct mac_node* dev_mac_temp = dev_mac;


    pos = strstr(response, "\r\n\r\n");

    if(NULL == pos)
    {
        return -1;
    }

    pJson   = cJSON_Parse(pos);
    if(pJson == NULL)
    {
        return -1;
    }

    pArray  = cJSON_GetObjectItem (pJson, "result" );
    if(pArray == NULL)
    {
    	 cJSON_Delete(pJson);
        return -1;
    }

    nCount  = cJSON_GetArraySize (pArray);
    for(i = 0; i < nCount; i++)
    {
        pItem = cJSON_GetArrayItem(pArray,i);
        if(pItem->valuestring!= NULL)
        {
            temp = (struct facture_name*)malloc(sizeof(struct facture_name));
            if(temp == NULL)
            {
                return -1;
            }
            memset(temp,0x0,sizeof(struct facture_name));
            ether_atoe(dev_mac_temp->mac, ether_mac);
            temp->mac[0] = ether_mac[0];
            temp->mac[1] = ether_mac[1];
            temp->mac[2] = ether_mac[2];
            if(strcmp(pItem->valuestring,"Null") == 0)
            {
                /* ����鲻����MAC��Ӧ���豸���̣��򽫸�MAC��¼Ϊother����ֹÿһ
                   �ζ�Ҫ�Ը��豸���в�ѯ */
                temp->name = (char*)malloc(strlen("other")+1);
                strcpy(temp->name,"other");
            }
            else
            {
                temp->name = (char*)malloc(strlen(pItem->valuestring)+1);
                strcpy(temp->name,pItem->valuestring);
            }
            temp->next = head->next;
            head->next = temp;
        }
        dev_mac_temp = dev_mac_temp->next;
    }
    cJSON_Delete(pJson);
}
int get_menufacture_by_cloud(struct mac_node* dev_mac,int fd)
{
    int bytes, btot, ret;
    char request[2048] = {0};
    char buf[2048] = {0};

    if(dev_mac == NULL)
    {
        close(fd);
        return 1;
    }
    /* ��װ������ */
    make_request(dev_mac,request);
    RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"%s\n",request);


    /* ���������� */
    output_to_cloud(fd, request);
    /* ���ձ��� */
    if((bytes=read_input_by_cloud(fd, buf, 2048)) > 0)
    {
        /* ���ݽ��� */
        RC_MODULE_DEBUG(RC_MANUFACTURER_MODULE,TPI,"%s\n",buf);

        parse_response_string(buf,dev_mac);
    }
    close(fd);

    return 1;
}
/*****************************************************************************
 �� �� ��  : router_mode_get_online_list
 ��������  : ·��ģʽ�»�ȡ���߿ͻ���
 �������  : struct mac_node** mac_list
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��2��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
int  router_mode_get_online_list(struct mac_node** mac_list)
{
    int client_num = 0;
    int index = 0;
    char facturer_name[16] = {0};
    time_t now_time = time(0);
    struct client_info  *clients_list = NULL;
    struct mac_node     *temp_node = NULL;
    struct mac_node     *mac_list_head = *mac_list;
    /* ��ȡ���߿ͻ��� */
    clients_list = (struct client_info *)malloc(MAX_CLIENT_NUMBER * sizeof(arp_client_info));
    if(clients_list != NULL )
    {
        memset(clients_list, 0x0 , sizeof(arp_client_info) * MAX_CLIENT_NUMBER);
        client_num = get_all_client_info(clients_list , MAX_CLIENT_NUMBER);
    }
    else
    {
        client_num = 0;
    }

    for(index = 0; index < client_num; ++index)
    {
        /* �ڵ�ǰ�б��в�ѯ����ѯ��������ӵ�ѯ�������� */
        if(!get_menufacture_name(clients_list[index].mac,facturer_name))
        {
            temp_node = (struct mac_node*)malloc(sizeof(struct mac_node));
            if(temp_node == NULL)
            {
                goto exit;
            }
            memset(temp_node,0x0,sizeof(struct mac_node));
            strcpy(temp_node->mac,clients_list[index].mac);
            temp_node->next = mac_list_head;
            mac_list_head = temp_node;
        }
    }
exit:
    *mac_list = mac_list_head;
    FREE_P(&clients_list);
}

/*****************************************************************************
 �� �� ��  : ap_mode_get_online_list
 �������  : struct mac_node** mac_list
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��2��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
int ap_mode_get_online_list(struct mac_node** mac_list)
{
    unsigned int i;
    char dev_mac[18] = {0};
    char facturer_name[16] = {0};
    struct mac_node     *temp_node = NULL;
    struct mac_node     *mac_list_head = *mac_list;
    WLAN_STA_INFO_T pInfo[MAX_STA_NUM + 1];
    unsigned char mac[18] = {0};

    memset(pInfo,0x0,sizeof(WLAN_STA_INFO_T)*(MAX_STA_NUM + 1));
    /* ��ȡ����·���������߿ͻ��� */
    getWlStaInfo( "wlan0", pInfo);

    for (i=1; i<=MAX_STA_NUM; i++)
    {

        if (pInfo[i].aid && (pInfo[i].flag & STA_INFO_FLAG_ASOC))
        {
            memset(mac,0x0,18);
            sprintf(mac,"%02X:%02X:%02X:%02X:%02X:%02X",
                    pInfo[i].addr[0],pInfo[i].addr[1],pInfo[i].addr[2],
                    pInfo[i].addr[3],pInfo[i].addr[4],pInfo[i].addr[5]);
            /* �ڵ�ǰ�б��в�ѯ����ѯ��������ӵ�ѯ�������� */
            if(!get_menufacture_name(mac,facturer_name))
            {
                temp_node = (struct mac_node*)malloc(sizeof(struct mac_node));
                if(temp_node == NULL)
                {
                    goto exit;
                }
                memset(temp_node,0x0,sizeof(struct mac_node));
                strcpy(temp_node->mac,mac);
                temp_node->next = mac_list_head;
                mac_list_head = temp_node;
            }
        }
    }
exit:
    *mac_list = mac_list_head;
    return 0;
}
/*****************************************************************************
 �� �� ��  : cloud_manufacturer_main_loop
 ��������  : �����豸���̵���ѭ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : static

 �޸���ʷ      :
  1.��    ��   : 2016��12��2��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
static void cloud_manufacturer_main_loop()
{
    int fd = 0;
    struct mac_node *mac_list = NULL;
    struct mac_node *temp_mac_node = NULL;
    struct mac_node *next_mac_node = NULL;
    while(1)
    {
        if(nvram_match(SYSCONFIG_WORKMODE, "client+ap") 
		|| nvram_match(SYSCONFIG_WORKMODE, "bridge"))
        {
            /* ap����apclientģʽ�»�ȡ���߿ͻ��� */
            ap_mode_get_online_list(&mac_list);
        }
        else
        {
            /* ·��ģʽ�»�ȡ���߿ͻ��� */
            router_mode_get_online_list(&mac_list);
        }

        if(mac_list != NULL)
        {
             /* ���ӵ������� */
            if(connect_to_cloud_server(&fd,CLOUD_SERVER_NAME, CLOUD_SERVER_PORT) != 0)
            {
            	goto wait_next;
            }
            /* ���±��ز���ʶ����豸 */
            get_menufacture_by_cloud(mac_list,fd);
            /* �����ڴ� */
wait_next:
            temp_mac_node = mac_list;
            while(temp_mac_node)
            {
                next_mac_node = temp_mac_node->next;
                free(temp_mac_node);
                temp_mac_node = next_mac_node;
            }
            mac_list = NULL;
        }
        cyg_thread_delay(500);
    }
}

static int cloud_manufacturer_thread_type = 0;
static char cloud_manufacturer_daemon_stack[10240];
static cyg_handle_t cloud_manufacturer_daemon_handle;
static cyg_thread cloud_manufacturer_daemon_thread;
static RET_INFO tpi_manufacturer_start()
{
    if(cloud_manufacturer_thread_type == 0)
    {
        cyg_thread_create( 9,
                           (cyg_thread_entry_t *)cloud_manufacturer_main_loop,
                           0,
                           "cloud_manufacturer",
                           &cloud_manufacturer_daemon_stack,
                           sizeof(cloud_manufacturer_daemon_stack),
                           &cloud_manufacturer_daemon_handle,
                           &cloud_manufacturer_daemon_thread);
        cyg_thread_resume(cloud_manufacturer_daemon_handle);
        cloud_manufacturer_thread_type = 1;
    }
    return RET_SUC;
}
