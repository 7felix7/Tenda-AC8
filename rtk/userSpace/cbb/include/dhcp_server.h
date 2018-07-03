#ifndef __DHCP_SERVER_H__
#define __DHCP_SERVER_H__

#ifndef __PI_COMMON_H__
#include "pi_common.h"
#endif

#ifndef __RC_MODULE_H__
#include "rc_module.h"
#endif

#define DHCPD_STATIC_LEASE_NU 19
#define PI_REMARK_STRING_LEN (64 + 1)			/* ���������ע���� */

/* ��̬IP���ò��� */
typedef struct static_ip_list
{
    PI8 ip_addr[PI_IP_STRING_LEN];
    PI8 mac_addr[PI_MAC_STRING_LEN];
	PI8 remark[PI_REMARK_STRING_LEN];
    struct static_ip_list *next;
}STATIC_IP_LIST, *P_STATIC_IP_LIST;

/* DHCP���������ò��� */
 typedef struct dhcp_server_info_struct{
	PIU8 enable;                      	 			/*����*/
	PI8 ifname[PI_BUFLEN_16];          			/*�󶨽ӿ�,����br0,br1*/
	PIU32 lease_time;							 	/*��Լ*/

	SYS_WORK_MODE wl_mode;								/*���߹���ģʽ*/

	PI8 lan_ip[PI_IP_STRING_LEN];					/*lan��IP*/
	PI8 lan_mask[PI_IP_STRING_LEN];				/*lan ����������*/
	PI8 start_ip[PI_IP_STRING_LEN];			 	/*��ʼIP*/
	PI8 end_ip[PI_IP_STRING_LEN];					/*����IP*/
	PI8 gateway[PI_IP_STRING_LEN];					/*Ĭ������*/
	PI8 dns[3*PI_IP_STRING_LEN];						/*DNS*/

	/*��̬IP��*/
	P_STATIC_IP_LIST static_ip_bind_info;
}DHCP_SERVER_INFO_STRUCT,*P_DHCP_SERVER_INFO_STRUCT; 

/*API*/

/*GPI*/

/*TPI*/
extern RET_INFO tpi_dhcp_server_update_info();
extern RET_INFO tpi_dhcp_server_struct_init();
extern RET_INFO tpi_dhcp_server_first_init();
extern RET_INFO tpi_dhcp_server_action(RC_MODULES_COMMON_STRUCT *var);
#ifdef __CONFIG_TC__
extern RET_INFO tpi_tc_dhcp_server_action(RC_MODULES_COMMON_STRUCT *var);
#endif

extern RET_INFO tpi_static_ip_update_info();
extern P_STATIC_IP_LIST tpi_static_ip_get_info();

P_DHCP_SERVER_INFO_STRUCT tpi_dhcp_server_get_info();

#endif/*__DHCP_SERVER_H__*/
