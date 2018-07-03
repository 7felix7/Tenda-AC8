
#ifndef __ARP_CLIENTS_H__
#define	__ARP_CLIENTS_H__


#define TPI_BUFLEN_64       64    //!< buffer length 64
#define TPI_BUFLEN_16       16    
#define TPI_BUFLEN_32      32    //!< buffer length 64
#define TPI_BUFLEN_8      	    8   
#define TPI_MACLEN	6					/* MAC���� */
#define TPI_MAC_STRING_LEN	18			/* MAC�ַ������� */
#define TPI_IP_STRING_LEN	16			/* IP�ַ������� */
#define TPI_IFNAME_LEN		16			/* �ӿ����Ƴ��� */
#define MAX_CLIENT_LIST_NUM   128
#define MAX_PARENTCTL_NUM   10

#define MAX_CLIENT_NUMBER 					255

typedef struct client_info{
        int in_use;                                //now in use
        unsigned char mac[TPI_MAC_STRING_LEN];     //mac address
        unsigned char ip[TPI_IP_STRING_LEN];       //ip address
	unsigned char hostname[TPI_BUFLEN_64];     //hostname
	unsigned char mark[TPI_BUFLEN_64];     //mark
	int l2type;                                //layer2 type: wired or wireless 
	time_t time; //sta��һ�ν���·������ʱ��
	time_t interval_time;//sntp����ǰ��ʱ����	
	int limitenable;
    time_t update_time; //���һ�θ���ʱ��
    u_long rmx_expire;  //��ʱʱ��
}arp_client_info;



typedef struct static_dhcp_list
{
    int index;		//��ʶnvram����
    char mac[TPI_MAC_STRING_LEN];
    char ip[TPI_IP_STRING_LEN];
	char host_name[TPI_BUFLEN_64];
    struct static_dhcp_list *next;
}Static_dhcp_list, *pStatic_dhcp_list;

struct static_dhcp_table
{
    int count;
    struct static_dhcp_list *gStatic_dhcp_list;
};


#endif
