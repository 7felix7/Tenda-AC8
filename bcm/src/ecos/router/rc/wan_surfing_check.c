/***********************************************************
Copyright (C), 2013, Tenda Tech. Co., Ltd.
FileName: wan_surfing_check.c
Description: �������״̬�߳� 
Author: ly;
Version : 1.0
Date: 2013-11-05
Function List:   
1. int get_real_ip()
2. int ping_connet_check()
3. int wan_surfing_check()

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
#include <string.h>
#include <stdlib.h>

#define SURFING_CHECK_SIZE	8*1024

#define GETHOSTBYNAME_DONAME_EN	0 /*add by lqz for not gethostbyname,only ping ntp services*/

#define	NUM_SNTP_SERVER					(4+4)
#define ONLINE_CONFIRM_CHECKCOUNT		2
#define NO_ONLINE_CONFIRM_CHECKCOUNT	3
#define PING_DELAY	      					3000

#if GETHOSTBYNAME_DONAME_EN
#define PING_HOSTNAME_QQ			"www.qq.com"
#define PING_HOSTNAME_BAIDU		"www.baidu.com"
#define PING_HOSTNAME_GOOGLE		"www.google.com"
#define PING_HOSTNAME_MICROSOFT	"www.microsoft.com"  //add by z10312 20150526 ��Ӻ���tenda�汾 ping ��������
#define	NVRAMNAME_WL_COUNTRYCODE	"wl0_country_code"
static char the_second_hostname[32] = {0};/*������:baidu���ڶ�������:����qq,����google�����ȸ�����Ҫ���е���*/
#endif

#define NVRAMNAME_NTP_SERVER		"ntp_server"

static cyg_handle_t wan_surfing_daemon_handle;
static char wan_surfing_daemon_stack[SURFING_CHECK_SIZE];
static cyg_thread wan_surfing_daemon_thread;

static char ntp_server_ip[NUM_SNTP_SERVER][17];
static char record_ok_ip[17] = {0};
static int had_init_value = 0;

extern int network_tpye;
extern int check_tag;
extern int change_tag;
extern int dns_res_init_flag;

int 	wan_surfing_check();
void 	init_ntpserverip_and_secondhostname(void);
int 	ping_connet_check(void);
char * 	tenda_gethostbyname(char *str);

extern int 	icmp_ping(unsigned int dst, int seq, int timeout);


/************************************************************
Function:	 tenda_gethostbyname               
Description:  ͨ�������ҵ�IP

Input:           Ҫ���ҵ�����                        

Output: 

Return:         ���ҵ�������Ӧ��IP�ַ�����ָ��

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/


char * tenda_gethostbyname(char *str)
{
	struct hostent *hptr;
	static char dst[32] = {0};
	if((hptr = gethostbyname(str)) == NULL)
	{
		return NULL;
	}
	else
	{
		inet_ntop(hptr->h_addrtype,hptr->h_addr,dst,sizeof(dst));
		return dst;
	}
}

/************************************************************
Function:	 ping_connet_check               
Description:  ping�ٶȡ��ȸ�

Input:                                          

Output: 

Return:         pingͨ�κ�һ������1������Ϊ0

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/

int check_addr(int ip0,int ip1)
{
	if(ip0 != 10 && ip0 != 172 && ip0 != 192)
	{
		return 1;
	}
	else
	{
		if((ip0 == 172) && ((ip1 < 16) || (ip1>31)))
		{
			return 1;
		}
		if((ip0 == 192) && (ip1 != 168))
		{
			return 1;
		}
		return 0;
	}
}


/************************************************************
Function:	 wan_surfing_check               
Description:  	  ���������������ÿ��25s���һ��

Input:                                          

Output: 

Return:         

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/
int wan_surfing_check()
{  
	char *tmp_username = NULL,*tmp_passwd = NULL,*dail_username = NULL,*dail_passwd = NULL;
	char wan0_isonln[32] = {0};	
	static int not_online_count = 0,online_count = 0;

	int ping_result = 0;

	nvram_set("wan0_isonln","yes");

	while(1)
	{
		strcpy(wan0_isonln, nvram_safe_get("wan0_isonln"));
		//����������У�������״̬�û���������	
		if(strcmp(nvram_safe_get("wan0_connect"),"Disconnected") == 0
			|| strcmp(nvram_safe_get("wan0_connect"),"") == 0)
		{
			if(strcmp(wan0_isonln,"no") != 0)
			{
				nvram_set("wan0_isonln","no");
			}
			cyg_thread_delay(100);
			continue;
		}
		else if(strcmp(nvram_safe_get("wan0_connect"),"Connecting") == 0)
		{
			if(strcmp(wan0_isonln,"yes-no") != 0)
			{
				nvram_set("wan0_isonln","yes-no");
			}
			cyg_thread_delay(100);
			continue;
		}
		
		//����Ƿ���pingͨ�ٶȡ��ȸ衢qq.com
		//dns�����ɹ�������Ϊ������(ҳ����ʾ)��Ȼ���ٽ���ping����
		//���ϸ��ĸ��ݿͻ���������

RECHECK:
		ping_result = ping_connet_check();

		if (1 == ping_result)
		{
			not_online_count = 0;
			online_count ++ ;

			if(strcmp(wan0_isonln,"yes") != 0)
			{
				network_tpye = 4;
				//nvram_set("wan0_isonln","yes");//isonln ��ǿ�������
				lan_pppoe_down("ppp1");
			

				if(online_count >= ONLINE_CONFIRM_CHECKCOUNT)
				{
					nvram_set("wan0_isonln","yes");

					online_count = 0;
				}
				else
				{					
					cyg_thread_delay(100);
					
					goto RECHECK;
				}
			}
		}
		else
		{
			online_count = 0;
			
			not_online_count ++ ;

			if(not_online_count >= NO_ONLINE_CONFIRM_CHECKCOUNT)
			{
				if(check_tag == 0)
				{
					nvram_set("wan0_isonln","yes-no");
				}
				else
				{			
					nvram_set("wan0_isonln","no");
				}
				
				not_online_count = 0;
			}
			else
			{
				cyg_thread_delay(500);
			
				goto RECHECK;
			}
		}
		//����Ϊ֮ǰ�ļ��弴�õĹ���
		if (strcmp(nvram_safe_get("wan0_proto"),"pppoe") == 0 && strcmp(nvram_safe_get("err_check"),"2") == 0)//ADSL��֤����
		{															
			tmp_username = nvram_safe_get("wan0_tmp_pppoe_username");
			tmp_passwd = nvram_safe_get("wan0_tmp_pppoe_passwd");
			dail_username = nvram_safe_get("wan0_pppoe_username");
			dail_passwd = nvram_safe_get("wan0_pppoe_passwd");
			if(strcmp(tmp_username,"") != 0 && strcmp(tmp_passwd,"") != 0)
			{	
				if(strcmp(tmp_username,dail_username) != 0 || strcmp(tmp_passwd,dail_passwd) != 0)
				{
					nvram_set("wan0_pppoe_username",tmp_username);
					nvram_set("wan0_pppoe_passwd",tmp_passwd);
					nvram_set(_WAN0_CHECK,"0");
					nvram_commit();
					stop_wan();
					start_wan();
					diag_printf("Authente fail,dail again!!!!\n");
				}
			}
		}

		cyg_thread_delay(1500);
	}
	return 0;
}

/************************************************************
Function:	 wan_surfing_check_main_loop               
Description:  	  ������ⴴ���̺߳�����ڣ������߳�
			  RC.C�е���
Input:                                          

Output: 

Return:         

Others:
History:
<author>   <time>    <version >   <desc>
hqw        2013-11-05   1.0        �½�����

************************************************************/

int wan_surfing_check_main_loop()
{	
	init_ntpserverip_and_secondhostname();
	
	cyg_thread_create(
		8, 
		(cyg_thread_entry_t *)wan_surfing_check,
		0, 
		"wan_surfing_check",
		wan_surfing_daemon_stack, 
		sizeof(wan_surfing_daemon_stack), 
		&wan_surfing_daemon_handle, 
		&wan_surfing_daemon_thread);
	cyg_thread_resume(wan_surfing_daemon_handle);
       
	cyg_thread_delay(1);
printf("function[%s] , line[%d] , start end \n" , __FUNCTION__ , __LINE__);
	return 0;
}

void init_ntpserverip_and_secondhostname(void)
{
	if (0 == had_init_value)
	{     
		memset(ntp_server_ip,0,sizeof(ntp_server_ip));

		char server_names[256] = {0};
		char *token = NULL, *value = NULL;
		int i = 0;
		
		strncpy(server_names, nvram_safe_get(NVRAMNAME_NTP_SERVER), sizeof(server_names)-1);
		value = server_names;
		
		for (i = 0; i < 4; i++) {
			token = strsep(&value, " ");
			if (NULL == token)
				break;

			strncpy(ntp_server_ip[i],token,sizeof(ntp_server_ip)-1);
		}
		
		/*�����������PING 180.76.76.76 223.5.5.5 119.29.29.29 114.114.114.114*/
		strcpy(ntp_server_ip[4],"180.76.76.76");
		strcpy(ntp_server_ip[5],"223.5.5.5");
		strcpy(ntp_server_ip[6],"119.29.29.29");
		strcpy(ntp_server_ip[7],"114.114.114.114");
		
#if GETHOSTBYNAME_DONAME_EN	
		memset(the_second_hostname,0,sizeof(the_second_hostname));

		#ifdef __CONFIG_TENDA_HTTPD_UCD_MULTI_EN__   //add by z10312 20150526  ���ֹ����ⷢ��ͬ��dns����
			strcpy(the_second_hostname,PING_HOSTNAME_GOOGLE);/*����*/
		#else
			value = nvram_safe_get(NVRAMNAME_WL_COUNTRYCODE);
									
			if (strlen(value) > 4) //g.cn		
			{	
				if (0 == strcasecmp(value,"ALL") || 0 == strcasecmp(value,"CN"))
				{
					strcpy(the_second_hostname,PING_HOSTNAME_BAIDU);/*����*/
				}
				else
				{
					strcpy(the_second_hostname,PING_HOSTNAME_GOOGLE);/*����*/
				}
			}
			else
			{
				strcpy(the_second_hostname,PING_HOSTNAME_BAIDU);/*default is ����*/
			}		
		#endif
#endif
		
		had_init_value = 1;
	}		
	
	return;
}


//	(1)����ֱ��ping sntp������IP;
//	(2)�ٿ��Ƿ���pingͨ�ٶȡ�qq���߹ȸ��΢��
//	(3)��ӿ���ping������
//	�ɹ�����1��ʧ�ܷ���0
int ping_connet_check(void)  
{	 
	char *ip_addr = NULL;
	unsigned int dst = 0;
	int i = 0;

	//Add by hqw
	change_tag = 0;
	
	if (0 == had_init_value)
	{
		init_ntpserverip_and_secondhostname();
	}

	//printf("==>record_ok_ip[%s]\n",record_ok_ip);

	if (strlen(record_ok_ip) >= 7)/*����ping��,���ټ��,wwk*/
	{
		dst = inet_addr(record_ok_ip);
		dst = ntohl(dst);
		if(icmp_ping(dst, 0, PING_DELAY) == 0)
		{
			goto SUCESS_PING;
		}
		else
		{
			memset(record_ok_ip,0,sizeof(record_ok_ip));

			//return 0;
		}
	}
	
	if (dns_res_init_flag || change_tag)/*��ϵͳdns��ص�ģ������˳�*/
		goto EXIT_PING;
	
	for (i = 0; i < NUM_SNTP_SERVER; i++)/*ping sntp������IP*/
	{
		if (strlen(ntp_server_ip[i]) >= 7)
		{
			//printf("==>ntp_server_ip[%d] = %s\n",i,ntp_server_ip[i]);
			ip_addr = ntp_server_ip[i];
			dst = inet_addr(ip_addr);
			dst = ntohl(dst);
			if(0 == icmp_ping(dst, 0, PING_DELAY))
			{
				memcpy(record_ok_ip,ip_addr,sizeof(record_ok_ip));
				goto SUCESS_PING;
			}
		}

		if (dns_res_init_flag || change_tag)/*��ϵͳdns��ص�ģ������˳�*/
			goto EXIT_PING;
	}

#if GETHOSTBYNAME_DONAME_EN
	char ip_str0[4] = {0};
	char ip_str1[4] = {0};
	char ip_str2[4] = {0};
	char ip_str3[4] = {0};

	if (dns_res_init_flag || change_tag)/*��ϵͳdns��ص�ģ������˳�*/
		goto EXIT_PING;	
	
	#ifdef __CONFIG_TENDA_HTTPD_UCD_MULTI_EN__   //add by z10312 20150526  ���ֹ����ⷢ��ͬ��dns����
	if((ip_addr = tenda_gethostbyname(PING_HOSTNAME_MICROSOFT)) != NULL)	
	#else
	if((ip_addr = tenda_gethostbyname(PING_HOSTNAME_QQ)) != NULL)	
	#endif
	{
		sscanf(ip_addr,"%[^.].%[^.].%[^.].%s",ip_str0,ip_str1,ip_str2,ip_str3);
		if(check_addr(atoi(ip_str0),atoi(ip_str1)))
		{
			dst = inet_addr(ip_addr);
			dst = ntohl(dst);
			if(0 == icmp_ping(dst, 0, PING_DELAY)) 
			{
				memcpy(record_ok_ip,ip_addr,sizeof(record_ok_ip));
				goto SUCESS_PING;
			}
		}
	}	
	
	if (dns_res_init_flag || change_tag)/*��ϵͳdns��ص�ģ������˳�*/
		goto EXIT_PING;
	
	if (strlen(the_second_hostname) > 0)
	{			
		if((ip_addr = tenda_gethostbyname(the_second_hostname)) != NULL)
		{
			sscanf(ip_addr,"%[^.].%[^.].%[^.].%s",ip_str0,ip_str1,ip_str2,ip_str3);
			if(check_addr(atoi(ip_str0),atoi(ip_str1)))
			{
				dst = inet_addr(ip_addr);
				dst = ntohl(dst);
				if(0 == icmp_ping(dst, 0, PING_DELAY))
				{
					memcpy(record_ok_ip,ip_addr,sizeof(record_ok_ip));
					goto SUCESS_PING;
				}
			}
		}
	}
#endif
	
EXIT_PING:		
	return 0;
	
SUCESS_PING:	
	return 1;
}
