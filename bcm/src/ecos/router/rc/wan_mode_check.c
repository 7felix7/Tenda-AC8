/***********************************************************
Copyright (C), 2013, Tenda Tech. Co., Ltd.
FileName: wan_surfing_check.c
Description: ����������绷���߳� 
Author: ly;
Version : 1.0
Date: 2013-11-05
Function List:   
1. int wan_mode_check()
2. int wan_mode_check_main_loop()
3. 

History:
<author>   <time>   <version >   <desc>
hqw        2013-11-05   1.0        �½��߳��ļ�
************************************************************/

#include <sys/param.h>
#include <stdio.h>
#include <bcmnvram.h>
#include <rc.h>
#include <sys/syslog.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <time.h>
#include <route_cfg.h>



extern int pppoe_pado_tag;
extern int pppoe_send_padi_num;
extern int pppoe0_send_padi_num;
extern int dhcp_offer_tag ;
extern int dhcp_send_discover_num ;
extern int change_tag;

int check_tag = 0;
int network_tpye = 3;
int dhcp_mode_check_tag = 0;

#define ING 				2
#define ON 					1
#define OFF 				0
#define PPPOE_CHECK_SIZE	12*1024
#define PPPOE_CHECK_MAX_NUM 3
#define DHCP_CHECK_MAX_NUM  3
#define _NONE_ 				0
#define _STATIC_ 			1
#define _DHCP_ 				2
#define _PPPOE_				3

static cyg_handle_t pppoe_daemon_handle;
static char pppoe_daemon_stack[PPPOE_CHECK_SIZE];
static cyg_thread pppoe_daemon_thread;

extern int ping_connet_check();
int wan_mode_check();
int static_first_check();
int dhcp_first_check();
int pppoe_first_check();


/************************************************************
Function:	 wan_mode_check               
Description:  ������绷����������

Input:                                          

Output: 	    

Return:      ��������Żظ�ǰ̨��ʾ���û�(�������ȫ�ֱ�����)  

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/
int wan_mode_check()
{
	char *wan_mode = NULL;
	int wan_mode_num = _DHCP_;
	wan_mode = nvram_safe_get("wan0_proto");

	//��ʼ�ж��Ƿ�ִ���������
	if(strcmp(nvram_safe_get("mode_need_switch"),"yes") != 0 
			||strcmp(nvram_safe_get("wl0_mode"),"ap") != 0 
				|| strcmp(nvram_safe_get("wl0_wds"),"") != 0)
	{
		check_tag = 1;
		return 0;
	}
	//��ȡĬ�ϵĽ��뷽ʽ
	if(strcmp(wan_mode,"pppoe") == 0)
	{
		wan_mode_num = _PPPOE_;
	}
	else if(strcmp(wan_mode,"dhcp") == 0)
	{
		wan_mode_num = _DHCP_;
	}
	else if(strcmp(wan_mode,"static") == 0)
	{
		wan_mode_num = _STATIC_;
	}
	else
	{
		wan_mode_num = _NONE_;
	}

	//����Ĭ�ϵĽ��뷽ʽѡ����Ӧ�ķ�֧
	switch(wan_mode_num)
	{
		case _PPPOE_:
			pppoe_first_check();
			break;
		case _DHCP_:
			dhcp_first_check();
			break;
		case _STATIC_:
			static_first_check();
			break;
		default:
			//���wan�ڽ��뷽ʽΪL2TP PPTP�ȣ���Ĭ��ʹ�þ�̬���(�����Ҫ���ģ����������)
			static_first_check();
			break;
	}
	return 0;
}

/************************************************************
Function:	 dhcp_first_check               
Description:  ���Ĭ�Ͻ��뷽ʽΪdhcp������øú������м��

Input:                                          

Output: 	    

Return:      ��������Żظ�ǰ̨��ʾ���û�(�������ȫ�ֱ�����)  

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/

int dhcp_first_check()
{
	char wan_connstatus[16]={0}, wan_ifname[16]={0}, wan_ipaddr[16]={0}, wan_mask[16]={0};
	static int wan_mode_check_tag = 0,ppp2_tag = ON;
	int dhcp_tag = 1;
	pppoe_pado_tag = 0;
	pppoe_send_padi_num = 0;
	dhcp_offer_tag = 0;
	dhcp_send_discover_num = 0;
	network_tpye = 3;
	int ping_tag = 1;
	char wan_mode_check_verdict[10] = "dhcp";

	while(1)
	{
		cyg_thread_delay(100);
		//���Ĭ�ϵ�DHCP���ӷ�ʽ�ڷ���3�����ֱ��Ķ�û���յ���Ӧ����ʱ��
		//����Ϊ��������DHCP��ʽֱ�ӿ���PPP2�̣߳�����Ƿ���PPPOE����
		if(dhcp_offer_tag != 1 && dhcp_send_discover_num > DHCP_CHECK_MAX_NUM)
		{
			dhcp_tag = 0;
			goto next;
		}
		//���wan���Ƿ��ȡ��IP�����û�л�ȡ��IP�����ѭ����⣬ֱ����ȡ��IP֮����DHCP�Ƿ���������
		strcpy(wan_connstatus, nvram_safe_get(_WAN0_CONNECT));
		if(strcmp(wan_connstatus,"Connected") != 0)
		{			
			continue;
		}
		if(change_tag == 1)
		{
			goto finish;
		}
		//ping �ٶȡ��ȸ�
		if(ping_tag == 1)
		{
			if(1 == ping_connet_check())
			{
				goto down;
			}
			if(change_tag == 1)
			{
				goto finish;
			}
			
			ping_tag = 0;
		}
next:	
		if(change_tag == 1)
		{
			goto finish;
		}
		//����ppp2�߳�	
		if(ppp2_tag == ON)
		{
			wan_pppoe_start2();
			ppp2_tag = ING;
		}
		//������ܵ���Ӧ�����л���PPPOE
		if(pppoe_pado_tag == 1)
		{
			//��ǰ������ʽΪpppoe
			diag_printf("\t %d wanmode is pppoe!\n",__LINE__);			
			network_tpye = 2;
			wan_mode_check_tag = 1;
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "pppoe" );
		}
		//�����PADI������5�����һ�û���յ���Ӧ��������DHCP���ӷ�ʽ
		else if(pppoe_send_padi_num >= PPPOE_CHECK_MAX_NUM)
		{
			if(dhcp_tag == 1)
			{
				diag_printf("\t %d wanmode is dhcp!\n",__LINE__);
				network_tpye = 1;
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "dhcp" );
			}
			else
			{
				diag_printf("\t %d wanmode is static ip!\n",__LINE__);
				network_tpye = 0;
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "static" );
			}
			wan_mode_check_tag = 1;
		}
		//�ر�ppp2�߳�
		if(wan_mode_check_tag == 1)
		{
			diag_printf("dowm ppp2!\n");
			wan_pppoe_down2("ppp2");
			ppp2_tag = OFF;
				
			sprintf(wan_ifname, "%s", nvram_safe_get("wan0_ifname"));
			sprintf(wan_ipaddr, "%s", nvram_safe_get("wan0_ipaddr"));
			sprintf(wan_mask, "%s", nvram_safe_get("wan0_netmask"));
			if (strcmp(nvram_safe_get("wan0_ifname"), "vlan2") == 0){
				ifconfig(wan_ifname, IFF_UP,wan_ipaddr, wan_mask);
				 route_add(wan_ifname, 0, "0.0.0.0", nvram_safe_get("wan0_gateway"), "0.0.0.0");
			}
			nvram_set("wan0_isonln","no");
			goto down;
		}
	}
down:	
	nvram_set("mode_need_switch","no");
finish:	
	if(ppp2_tag == ING)
	{
		diag_printf("dowm ppp2!\n");
		wan_pppoe_down2("ppp2");
	}
	ppp2_tag = OFF;
	check_tag = 1;
	nvram_set("wan_mode_check_verdict",wan_mode_check_verdict);
	nvram_commit();
	return 0;
}
/************************************************************
Function:	 pppoe_first_check               
Description:  ���Ĭ�Ͻ��뷽ʽΪpppoe������øú������м��

Input:                                          

Output: 	    

Return:      ��������Żظ�ǰ̨��ʾ���û�(�������ȫ�ֱ�����)  

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/

int pppoe_first_check()
{
	pppoe_pado_tag = 0;
	pppoe_send_padi_num = 0;
	dhcp_offer_tag = 0;
	dhcp_send_discover_num = 0;
	network_tpye = 3;
	int dhcp_check_tag = ON;
	char wan_mode_check_verdict[10] = "pppoe";
	
	while(1)
	{
		cyg_thread_delay(100);
		if(pppoe_pado_tag == 1 && pppoe0_send_padi_num <= PPPOE_CHECK_MAX_NUM)
		{
			//��ǰ������ʽΪpppoe
			diag_printf("\t %d wanmode is pppoe!\n",__LINE__);			
			network_tpye = 2;
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "pppoe" );
			goto down;
		}
		if(change_tag == 1)
		{
			goto finish;
		}
		if(pppoe_pado_tag != 1 && pppoe0_send_padi_num <= PPPOE_CHECK_MAX_NUM)
		{
			continue;
		}
		//����dhcp������������
		if(dhcp_check_tag == ON)
		{
			dhcp_mode_check_tag = 1;
			dhcpc_start("vlan2", "wandhcpc", "Tenda");
			dhcp_check_tag = ING;
		}
		//������յ�offer���Ĳ��ҷ�����������3��������dhcp����
		if(dhcp_offer_tag == 1 && dhcp_send_discover_num <= DHCP_CHECK_MAX_NUM)
		{
			diag_printf("\t %d wanmode is dhcp!\n",__LINE__);
			network_tpye = 1;
			dhcpc_stop("vlan2");
			dhcp_check_tag = OFF;
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "dhcp" );
			goto down; 
		}
		//
		if(change_tag == 1)
		{
			goto finish;
		}
		if(dhcp_offer_tag != 1 && dhcp_send_discover_num <= DHCP_CHECK_MAX_NUM)
		{
			continue;
		}
		//����Ϊ��̬IP
		{
			diag_printf("\t %d wanmode is static ip!\n",__LINE__);
			network_tpye = 0;
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "static" );
			goto down;
		}
	}
down:	
	nvram_set("mode_need_switch","no");
finish:	
	if(dhcp_check_tag == ING && strcmp(nvram_safe_get("wan0_proto"),"dhcp") != 0)
	{
		dhcpc_stop("vlan2");
	}
	dhcp_check_tag = OFF;
	check_tag = 1;
	nvram_set("wan_mode_check_verdict",wan_mode_check_verdict);
	nvram_commit();
	return 0;
}
/************************************************************
Function:	 static_first_check               
Description:  ���Ĭ�Ͻ��뷽ʽΪstatic������øú������м��

Input:                                          

Output: 	    

Return:      ��������Żظ�ǰ̨��ʾ���û�(�������ȫ�ֱ�����)  

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/

int static_first_check()
{
	static int ppp2_tag = 1;
	pppoe_pado_tag = 0;
	pppoe_send_padi_num = 0;
	dhcp_offer_tag = 0;
	dhcp_send_discover_num = 0;
	network_tpye = 3;
	char wan_mode_check_verdict[10] = "static";
	int dhcp_check_tag = ON;

	while(1)
	{
		cyg_thread_delay(100);
		if(change_tag == 1)
		{
			goto finish;
		}
		if(ppp2_tag == 1)
		{
			wan_pppoe_start2();
			ppp2_tag = 0;
		}
		if(pppoe_pado_tag == 1 && pppoe_send_padi_num <= PPPOE_CHECK_MAX_NUM)
		{
			//��ǰ������ʽΪpppoe
			diag_printf("\t %d wanmode is pppoe!\n",__LINE__);			
			network_tpye = 2; 
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "pppoe" );
			goto down;
		}
		if(change_tag == 1)
		{
			goto finish;
		}
		if(pppoe_pado_tag != 1 && pppoe_send_padi_num <= PPPOE_CHECK_MAX_NUM)
		{
			continue;
		}
		//����dhcp������������
		if(dhcp_check_tag == ON)
		{
			dhcp_mode_check_tag = 1;
			dhcpc_start("vlan2", "wandhcpc", "check");
			dhcp_check_tag = ING;
		}
		//������յ�offer���Ĳ��ҷ�����������3��������dhcp����
		if(dhcp_offer_tag == 1 && dhcp_send_discover_num <= DHCP_CHECK_MAX_NUM)
		{
			diag_printf("\t %d wanmode is dhcp!\n",__LINE__);
			network_tpye = 1;
			dhcpc_stop("vlan2");
			dhcp_check_tag = OFF;
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "dhcp" );
			goto down; 
		}
		if(change_tag == 1)
		{
			goto finish;
		}
		if(dhcp_offer_tag != 1 && dhcp_send_discover_num <= DHCP_CHECK_MAX_NUM)
		{
			continue;
		}
		//����Ϊ��̬IP
		{
			diag_printf("\t %d wanmode is static ip!\n",__LINE__);
			network_tpye = 0;
			snprintf(wan_mode_check_verdict , sizeof(wan_mode_check_verdict) , "static" );
			goto down;
		}
	}
down:	
	nvram_set("mode_need_switch","no");
finish:	
	if(dhcp_check_tag == ING && strcmp(nvram_safe_get("wan0_proto"),"dhcp") != 0)
	{
		dhcpc_stop("vlan2");
	}
	dhcp_check_tag = OFF;
	check_tag = 1;
	nvram_set("wan_mode_check_verdict",wan_mode_check_verdict);
	nvram_commit();
	return 0;
}
/************************************************************
Function:	 wan_mode_check_main_loop               
Description:  ������绷���Ĵ����̺߳�������RC.C
			�е��øú����������߳�

Input:                                          

Output: 	    

Return:     

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/

int wan_mode_check_main_loop()
{	
	cyg_thread_create(
		8, 
		(cyg_thread_entry_t *)wan_mode_check,
		0, 
		"wan_mode_check",
		pppoe_daemon_stack, 
		sizeof(pppoe_daemon_stack), 
		&pppoe_daemon_handle, 
		&pppoe_daemon_thread);
	cyg_thread_resume(pppoe_daemon_handle);

	cyg_thread_delay(1);
printf("function[%s] , line[%d] , start end \n" , __FUNCTION__ , __LINE__);
	return 0;
}
