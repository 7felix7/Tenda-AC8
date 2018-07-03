/***********************************************************
	Copyright (C), 1998-2015, Tenda Tech. Co., Ltd.
	FileName: tai_clientlist.c
	Description:tenda alibaba api
	Author: Lvliang;
	Version : 1.0
	Date: 2015.1.27
	Function List:
	History:
	<author>   <time>     <version >   <desc>
	Lvliang    2015.4.28   		1.0          learn from xuwu(ecos1.0)
************************************************************/
#include <stdio.h>
#include <stdlib.h>


#define IN
#define OUT

#define NOT_IN_USE          0
#define IN_USE              1

#define CLIENT_TBL_ADD      1
#define CLIENT_TBL_DEL      2

#define L2_TYPE_WIRED       0
#define L2_TYPE_WIRELESS    1


#define MAX_CLIENT_LIST_NUM   128


#define TPI_BUFLEN_64       64    //!< buffer length 64




#define TPI_MACLEN	6					/* MAC���� */
#define TPI_MAC_STRING_LEN	18			/* MAC�ַ������� */
#define TPI_IP_STRING_LEN	16			/* IP�ַ������� */
#define TPI_IFNAME_LEN		16			/* �ӿ����Ƴ��� */


typedef struct tpi_all_clients{
        int in_use;                                //now in use
        unsigned char mac[TPI_MAC_STRING_LEN];     //mac address
        unsigned char ip[TPI_IP_STRING_LEN];       //ip address
		unsigned char hostname[TPI_BUFLEN_64];     //hostname
		int l2type;                                //layer2 type: wired or wireless 
		
}TPI_ALL_CLIENTS, *PTPI_ALL_CLIENTS;


typedef struct tpi_lan_status{
	char ifname[TPI_IFNAME_LEN];	
	//char mac[TPI_MAC_STRING_LEN];
	char ip[TPI_IP_STRING_LEN];
	char netmask[TPI_IP_STRING_LEN];
}TPI_LAN_STATUS, *PTPI_LAN_STATUS;


/* �ӿں�������ֵ���� */
typedef enum tpi_ret{
	TPI_RET_OK = 0,						/* �ɹ� */
	TPI_RET_APP_RUNNING = 1,			/* ģ���������� */
	TPI_RET_APP_DEAD = 2,				/* ģ���Ѿ��˳� */
	TPI_RET_NULL_POINTER = 1001,		/* ��ָ����� */
	TPI_RET_INVALID_PARAM = 1002,		/* �Ƿ����� */
	TPI_RET_ERROR = 0xff				/* ʧ�� */
}TPI_RET;



//dhcp clients
/* ������ */
#define MAX_DHCPS_CLIENTS  253

typedef enum tpi_dhcps_bound_type{
	AUTO_ALLOCATION,								/* ��̬���� */
	STATIC_BOUND									/* ��̬�� */
}TPI_DHCPS_BOUND_TYPE;

/* �ӿڽ������� */
typedef enum tpi_l2type{
	WIRED,
	WIRELESS
}TPI_L2TYPE;


struct lease_t {
	unsigned char	last;
	unsigned char	flag;
	unsigned char	mac[6];
	unsigned int 	ipaddr;
	unsigned int	expiry;
	unsigned char	hostname[64];
};


/* �ͻ����б�Ԫ�� */
typedef struct tpi_dhcps_client {
	unsigned char 	bound;			/*������*/
	TPI_L2TYPE l2type;						/* �������� */
	char	mac[TPI_MAC_STRING_LEN];			/*MAC��ַ*/
	char    ipaddr[TPI_IP_STRING_LEN];			/*IP��ַ*/
	unsigned int	expiry;						/*��Լʱ��*/
	unsigned char	hostname[TPI_BUFLEN_64];	/*������*/
}TPI_DHCPS_CLIENT, *PTPI_DHCPS_CLIENT;


/* �ͻ����б� */
typedef struct tpi_dhcps_clients{
	int total;
	TPI_DHCPS_CLIENT clients[MAX_DHCPS_CLIENTS];
}TPI_DHCPS_CLIENTS, *PTPI_DHCPS_CLIENTS;





