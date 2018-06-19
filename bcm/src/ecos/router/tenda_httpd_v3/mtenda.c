/*
* mtenda.c for bcm ecos, 2010/08/25 roy
*/

/********************************* Includes ***********************************/

#include <stdio.h>
#include <bcmnvram.h>
#include <iflib.h>
#include <ecos_oslib.h>
#include <net/if_var.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <net/if_dl.h>
#include <net/radix.h>
#include <net/route.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <sys/sockio.h>
#include <autoconf.h>

#include  "uemf.h"
#include  "wsIntrn.h"
#include  "route_cfg.h"
#include  "flash_cgi.h"
#include  "../ddns/include/ddns.h"
#include  "chlan.h"
#include  "../rc/rc.h"
#include  "webs.h"


/******************************** extern Description *********************************/

/******************************** local Description *********************************/

/*
 * The number of bytes in an ethernet (MAC) address.
 */
#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6
#endif

#ifndef DHCPD_STATIC_LEASE_NU
#define DHCPD_STATIC_LEASE_NU 16
#endif 

#ifndef VTS_MAX_NUM_1
#define VTS_MAX_NUM_1 10 //VTS_MAX_NUM in natcmd.c
#endif

#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V11.13.01.08_T"
#else
#define W311R_ECOS_SV "V11.13.01.08"		//"V5.07.53"
#endif


//�޸�Cookie ©������
//Cookie �����ı����Ϊ���뾭base64 ���ܺ�ĩβ��3 λ����ִ�
/*
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.53s"
#else
#define W311R_ECOS_SV "V1.0.0.0"		//"V5.07.53"
#endif
*/

/*//���dns����˳��������
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.51s"
#else
#define W311R_ECOS_SV "V5.07.51"
#endif
*/
/*
//ADSL�������⣬PPPOE Tags�ֶε��޸ģ�service name ����host unique

#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.50s"
#else
#define W311R_ECOS_SV "V5.07.50"
#endif
*/
/*
//flash ˫���ݷ���������ϵ綪����

#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.49s"
#else
#define W311R_ECOS_SV "V5.07.49"
#endif
*/
/*/* 
w316r w308r V4.0 ��ҳ��
�����½��ȫ��֤
�����N��·��Զ��web����ʧЧ������

#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.46s"
#else
#define W311R_ECOS_SV "V5.07.46"
#endif
*/

/*//����ڶ�OEM�г�����BUG

#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.43s"
#else
#define W311R_ECOS_SV "V5.07.43"
#endif
*/

/*��w316r �� w308r ����bug by ldm*/
/*
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.42s"
#else
#define W311R_ECOS_SV "V5.07.42"
#endif


#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.41s"
#else
#define W311R_ECOS_SV "V5.07.41"
#endif
*/

//Ϊ���nex�ͻ�����cable modem��ȡIP �쳣���ⷢ����nex�ͻ�һ�����԰汾���汾��Ҳ��V5.07.39
//�޸�����Ϊ:���dhcp client�ͻ��˷������option12��option60ѡ�� 
//�ð汾��V5.07.38�����ϸ���---20121022


/*�����Ը���*/
/*#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.39s"
#else
#define W311R_ECOS_SV "V5.07.39"
#endif
*/
/*
*���WPS���к󣬵����ҳ���棬���߹ر����߹��ܣ����WPS�����쳣���Լ�WPS LED����ʾ�쳣���
*/
/*
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.38s"
#else
#define W311R_ECOS_SV "V5.07.38"
#endif
*/


/*
1.����w316r����˹PPPOE���ŷ�ʽ����̨�������
2.�޸�SSID֧�������ַ�
*/

/*
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.37s"
#else
#define W311R_ECOS_SV "V5.07.37"
#endif
*/
/*
���wps�رպ� ��λ��ť�������õ�����
W303R�����������л�����ӿ�

#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.36s"
#else
#define W311R_ECOS_SV "V5.07.36"
#endif
*/
/*���̨�������Ž�WDS�ڶ���MAC��ַ��Ч�� BUG*/
/*
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.35s"
#else
#define W311R_ECOS_SV "V5.07.35"
#endif
*/

/*������ҳ�������ӷ�ʽ�ɹ������ʾҳ��*/

/*#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.34s"
#else
#define W311R_ECOS_SV "V5.07.34"
#endif

*/
/*�޸���ҳ������ʽһֱ��ʾPPPOE��BUG*
/*��������2T2R�����л��ӿ�*/
/*
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.33s"
#else
#define W311R_ECOS_SV "V5.07.33"
#endif
*/

/*�޸�A5��N3/W316R��N30/W368RӢ����ҳ��ʾ��Ϣ*/
/*
#ifdef __CONFIG_TENDA_WLJB__

#define W311R_ECOS_SV "V5.07.32s"
#else
#define W311R_ECOS_SV "V5.07.32"
#endif
*/

/*�޸�����WPS PIN ��BUG��ֻ��ͨ������ӿڸı�PIN���ֵ
��������������ƽⲹ�����������Ӵ���*/
/*
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.31s"
#else
#define W311R_ECOS_SV "V5.07.31"
#endif
*/

/*�޸�����WPS PIN ��BUG��ֻ��ͨ������ӿڸı�PIN���ֵ
��������������ƽⲹ�����������Ӵ���*/

/*#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.30s"
#else
#define W311R_ECOS_SV "V5.07.30"
#endif
*/

/*�޸�����WPS PIN ��BUG��ÿ�λָ��������ö��������µ�PIN��
��������������ƽⲹ�����������Ӵ���*/

/*#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.29s"
#else
#define W311R_ECOS_SV "V5.07.29"
#endif
*/

/*
�޸�DHCP�ͻ���BUG,�����ۺͼǿ�Ƶ�޷�DHCP������


#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.28s"
#else
#define W311R_ECOS_SV "V5.07.28"
#endif
//�����ɫ��l2tp�κ��������⣬l2tp�κ�DNS������������
*/

/*
#ifdef __CONFIG_TENDA_WLJB__
#define W311R_ECOS_SV "V5.07.27s"
#else
#define W311R_ECOS_SV "V5.07.27"
#endif
*/

/*
1.�޸İ�ȫ����  �����հ�ť���ܱ��������
2.Ӣ�İ汾DDNS  ��88ip�ĳ� dyndns
3.reset button ���������ε�����
4.�����ļ���SSIDΪ�գ��ָ����������д�SSID��ʱ��,��SSIDû�б��ָ�Ϊ��
5.��������wanģʽ�£������߻��������б���ʱ�ŵ�����Զ�ѡ�������
*/

//#ifdef __CONFIG_TENDA_WLJB__
//#define W311R_ECOS_SV "V5.07.26s"
//#else
//#define W311R_ECOS_SV "V5.07.26"
//#endif

/*
1,w316r use old wds,w308r use apclient replace wds
2,change wds to old way
3,release w316r cn, (w316r en, w308r cn, w308r en, have not finished testing)
*/
//#define W311R_ECOS_SV "V5.07.25"

/*
1,add patch for wps and upnp,
Following files changed:

1. src/ecos/router/rc/bcmdhcpc.c
2. src/ecos/router/rc/rc.h
3. src/router/libupnp/upnp/upnp.c
4. src/wps/brcm_apps/apps/wps_libupnp.c
5. add no-ip ddns for english version
6. fix wps bug on win7
7. add a5 v2 version
8. change wds to apclinet wet mode
*/
//#define W311R_ECOS_SV "V5.07.24"

/*
1. �޸�ҳ��BUG��ȥ�������еĽ�����
2. �����ж��߳�run���㷨thread_check.c
3��   �������BUG,ȥ��logoff����
4��   ��ȫ����ֻȥ��URL�Ľ�����
5��   �Ż����RUN�����Ļ���
6,����a5v1���İ汾
*/

//#define W311R_ECOS_SV "V5.07.23"

/*
1. ���g only����
2. �޸��ϸ��汾��5������ lcp_echos_pending = 0;
3. ҳ��BUG��ȥ���˳���ť
*/
//#define W311R_ECOS_SV "V5.07.22"

/*
1.    SSID can��t find
2.    SSID can appear but can��t connect
3.    Can��t ping Lan IP( unresolved on this version)
4.	Base SDK:5.110.27.21
5.    add mechanism in lcp_extcode()->lcp_echo_fails = 0
6.    remove (ifp->if_flags & IFF_PROMISC) != 0 in if_ethersubr.c for filtering unicast package is not for us
*/

//#define W311R_ECOS_SV	"V5.07.21"

/*
1,���1MFlash���������⣬�Ѳ����ĵ�λ�ĳ�4KB
2,����8021X���뷽ʽ
3,�޸Ĳ�������ӿڣ�ֻ�ڲ�����MAC��ַ�����Flash����Ķ���
4,Tenda���İ汾���˳���¼�ĳ��˳�����¼����
5,URL����ʱ����������ȫ����Ч�����⣬��www.baidu.com���޸�urlfilter.c
6,��ֹ��WAN�����ĵ�LAN��
7,����NAT����������ģ��(NAT_PER_IP_LIMIT)
*/
//#define W311R_ECOS_SV	"V5.07.20"

/*
1,����A5 V1�汾
*/
//#define W311R_ECOS_SV	"V5.07.19"

/*
1,�����ɺϰ汾
2,�޸���������ͳ��BUG����ip_forward���������Ӻ���ʵ��
3�����²������
4���޸�DDNSͨ����������·������BUG
5������״̬ҳ��20M/40M��ʾ����BUG
6��ϵͳpanic��ԭ���ǽ�����ѭ�������ڸĳ�����ϵͳ����support.c��
*/
//#define W311R_ECOS_SV	"V5.07.18"

/*
1,update obj-wl_ap.o.gz for g only mode problem
2,modify web page for PPTP/L2TP server ip length limited
*/

//#define W311R_ECOS_SV	"V5.07.17"

/*
1������NAT����������ģ��(NAT_PER_IP_LIMIT)
2���޸�����ģ��BUG
3������r17�����������(obj-wl_apsta.o.gz)�����g only���� 
*/

//#define W311R_ECOS_SV	"V5.07.16"

/*
1,���ɼ���O���ļ�
2,W316r������԰汾�޸ģ�����3���汾����webcomp.c
3,��cluster�����ĳ�1600,�Ż���������
*/

//#define W311R_ECOS_SV	"V5.07.15"

/*
1,�޸�lanip��DMZ�;�̬·�ɶ�ʧ
2,lanip���Ըĳ��ѱ���̬�󶨵�ip�����������ip��ͻ
3,lanip���Ըĳɺ�wanipͬ���ε�ip
*/
/*
 1���޸��ð�ť�ָ���������ʱ�ڴ治�����������
 2��Ӣ�İ汾ֻ�ṩһ�����ԣ��������԰汾�ṩ�������Ժ�Ӣ����������
*/
/*
 1����webs�ṹ������accLang����������԰汾
 2������envram������޸Ĳ�����
 3������TC��������ģ�飬�����޸�
 4��SSID�������뺺��
*/
//#define W311R_ECOS_SV	"V5.07.14"

/*
1�������ֶ���¡MAC��ַ 8��1��
2������WEP���ܷ�ʽ 8��2��8��3�գ�����30%��
3��PPPOE�ʺ�����ʱΪ���ģ������Ϊ���� 8��4��
4���ڰ�װ��ҳ���߼��ܵ�������������֡�Ĭ������Ϊ12345678��8��4��
6������ҳ�����������ѡ��߼����á��������߼����á��������óɳ����ӵ��߼����ý��� 8��4��
7��WAN�ڿ�֧��10/100M�ֶ�ѡ��Ĭ��ΪAUTO 8��9��10��
8�� LAN��IP�ı�ʱ���޸�������������ò���
9�� LAN��IP�ı�ʱ���޸ķ���ǽ���ò���
*/
//#define W311R_ECOS_SV	"V5.07.13"

// 1,remove pl and ko in multi language;2,add new compile method(check Readme.txt);
// 3,del unuseful codes;4,renew new wireless driver for apclient 72MB problem
//#define W311R_ECOS_SV	"V5.07.12"

//ɾ����N3�����ã�������W309R_IDC_cn���������İ汾��
//#define W311R_ECOS_SV	"V5.07.11"

//���g only �������⣬wds ���߲�����������
//#define W311R_ECOS_SV	"V5.07.10"

//�޸�303r/308r boot���������дmac��������
//#define W311R_ECOS_SV	"V5.07.09"

//WDS���⣬����WDS������������Ϊ����MAC��ַ֮��Ҫ�ÿո�ֿ���WDS����������������ΪWMM��APSD���ó���off
//#define W311R_ECOS_SV	"V5.07.08"

// �޸���ҳBUG������ͳ��ҳ�治׼������ (�޸�do_stream_timer�����Ķ�ʱʱ��)
//#define W311R_ECOS_SV	"V5.07.07"

//�޸�IP����ʱ�˿ڲ���ƥ������� ��ipfilter����1><80ʱ��������1��80
//#define W311R_ECOS_SV	"V5.07.06"

// 1,�޸�upnp������Ϣ��2,pppoe �������Ӵ�5����һ�θĳ�һ��һ��
//#define W311R_ECOS_SV	"V5.07.05"

//ȥ����N3��WPS���ã���__CONFIG_WPS_LED__ ѡ��ȷ����û�����ѡ���û��WPS
//#define W311R_ECOS_SV	"V5.07.04"

//���ϵͳ�Ʋ���������(ϵͳ��Ϊgpio6��Ӳ���Ͻӷ���)
//#define W311R_ECOS_SV	"V5.07.03"

//wan�ڲ���ʹ�ã���Ϊ5356c0�õ���5357оƬ�Ĳü��棬���԰�default�����û���w308r�ģ�������
//#define W311R_ECOS_SV	"V5.07.02"

//���µ�SDK 5357c0.5.100.27.7(5357c0���������5356c0��)
//#define W311R_ECOS_SV	"V5.07.01"

// 1.�޸������������2.�������ARP����
// 3. �޸�DHCP��Լʱ��BUG(dhcpd_ecos.c)
//#define W311R_ECOS_SV	"V5.19.07"
//#define W311R_ECOS_SV	"V5.19.07s"

// �޸��������汾Ѹ���������߶�������
//�޹�������ں�DNS�ض���
//#define W311R_ECOS_SV	"V5.19.06s"
//#define W311R_ECOS_SV	"V5.19.06"

// ��pppoe���ʻ���������������Ǻ�
//#define W311R_ECOS_SV	"V5.19.05"

// ��pppoe���ʻ���������ĳ�128�ֽڣ���wps��ʱ��ĳ�140��
// ϵͳĬ�������룬��¼������½�򡣶�����԰��Զ���ȡ�����������Ϣ��
//#define W311R_ECOS_SV	"V5.19.04"

// PPPOE�κ�ʱ�������AC NAME
//#define W311R_ECOS_SV	"V5.19.03"

//�޸ĺ�IP��ַ��ص��ж�����
//#define W311R_ECOS_SV	"V5.19.02"

//���µ�SDK19�汾
//#define W311R_ECOS_SV	"V5.19.01"

//���µ�SDK17�汾
//#define W311R_ECOS_SV	"V5.17.01"

//���µ�SDK16�汾
//#define W311R_ECOS_SV	"V5.16.01"

//���wpa����������Կ���ֻ������62�������� 
//#define W311R_ECOS_SV	"V5.12.02"

//��������������SDK��12�汾��������߷���·������WEB��������
//#define W311R_ECOS_SV	"V5.12.01"

//MAC��ַ�����ڽ�������ֲ���ʱ�䷶Χ��ʱ���ܷ���·����WEB������ 
//#define W311R_ECOS_SV	"V5.11.07"

//�޸ı��ݻָ��������ù��ܣ��޸�CFG_write_prof����
//#define W311R_ECOS_SV	"V5.11.06"

//pppoe�κ�ʱ������ͳ�ƿ��Ʋ����������� 
//���ϵͳ�ð�ť��λʱ����������(��Ҫstop_wan) 
//#define W311R_ECOS_SV	"V5.11.05"

//LOG�Զ���պ�ϵͳ��־ͣ��15ҳ������� 
//#define W311R_ECOS_SV	"V5.11.004"

//�޸�DMZ��80�˿ڵĳ�ͻ���� (ip_nat.c->check_localport)
//#define W311R_ECOS_SV	"V5.11.003"

//������+DDNS+����ʱ��+PPPOE����ʱ��+��������
//#define W311R_ECOS_SV	"V5.11.002"

//����SDK����10�汾
//#define W311R_ECOS_SV	"V5.10.001"

//����SDK����9�汾
//#define W311R_ECOS_SV	"V5.09.001_CN"

//����dns������Ϣ
//�޸�����ͳ��BUG
//�޸�WPS���ú���ʹ��reset��BUG
//�޸�DHCP������BUG
//#define W311R_ECOS_SV	"V5.07.002_CN"

//����SDK��ecos-router-5[1].70.56.7.tar.gz
//#define W311R_ECOS_SV	"V5.07.001_CN"

//���DHCP��ú�LAN��IPͬһ����IP������
//���DMZ��·�ɱ��ؽ����Л_ͻ����
//�������ͳ�����ú�ҳ��û����ʾ���õ�����
//DMZ��·�ɱ��ؽ����Л_ͻ
//�������ƻ�������
//#define W311R_ECOS_SV	"V5.05.005_CN"

//�޸�WAN��MTU��Ч��BUG
//�������������,ϵͳ��λ��ťBUG
//#define W311R_ECOS_SV	"V5.05.004_CN"

//����PPPOE��������
//#define W311R_ECOS_SV	"V5.05.003_CN"

//LOG�ĳɷǻ���,��150����0
//����DHCP,PPPOE��״̬ҳ��ĶϿ�,���ӿ���
//�޸�gethostbyname�޷���������BUG,��dns_impl.inl�й�
//#define W311R_ECOS_SV	"V5.05.002_CN"

//bcm�İ汾��Ϊ5.10.56.68,����ȡ�׺�β,Ȼ��������ѵ���λ�汾�ź�OEM��Ϣ

//#define W311R_ECOS_SV	"V5.05.001_CN"

#define W311R_ECOS_HV	""
#define W311R_ECOS_BV	"V5.100.27.21" //sdk version

#define TWL300N_FIRMWARE_BUILD_DATE		__DATE__

#ifdef __CONFIG_TENDA_MULTI__
#define LAN_WAN_IP_ERROR 				"ERROR: WAN NET is same as LAN"
#define WAN_IP_ERROR 				"ERROR: Wrong WAN IP Settings"
#define LOGIN_USERS_FULL 				"Warning: Up to 4 users can log in to the router at a time!"
#define INDEX_USERS_FULL 				"Warning: Up to 4 users can log in to the router at a time!"
#else
#define LAN_WAN_IP_ERROR 				"����: WAN IP������LAN IP����һ��"
#define WAN_IP_ERROR 				"����:WAN IP���ô���"
#define LOGIN_USERS_FULL 				"����: �޷���½·��������½��������4��!"
#define INDEX_USERS_FULL 				"����: �޷�����·��������½��������4��!"
#endif


//#define WPS_BASE                    0x1000
//#define WPS_SUCCESS                 WPS_BASE+1


static char gWLErrorMsg[64] = {0};
static char gWLErrorRtnUrl[64] = {0};

static int g_user_pass_Flag = 0;
static char bForceLogout = 1;
static int g_login_where = 0;
//syslog
static int logNum;
static int gLog_cur_page = 1;

char_t g_User[64],g_Pass[64];

int dns_redirect_dag = 0;//add for redirect to advance.asp by ldm 20121127

extern login_ip_time loginUserInfo[MAX_USER_NUM];

extern int g_cur_wl_radio_status;


//roy +++2010/09/07
extern void sys_restart(void);
extern void sys_reboot(void);
extern int    arpioctl(int req, caddr_t data, struct proc *p);
extern void show_network_tables2(int (*mtenda_routing_entry)(struct radix_node *, void *),void *pr);
extern int	    	syslog_clear();
extern int 	syslog_open();
extern int 	syslog_close();
extern int 	syslog_get2(char *log_time,char *log_type,char *log_data);
extern int get_argv(char *string, char *argv[]);

//start: add by z10312  ���� ���ⰴ������&LED�Ʋ��� �ӿ�2015-01-27
extern void set_all_led_on( void );
extern void set_all_led_off( void );
//end: add by z10312  ���� ���ⰴ������&LED�Ʋ��� �ӿ�2015-01-27
//extern uint32 wps_gen_pin(char *devPwd, int devPwd_len);
/* ARP ioctl structure */
struct arp_arpentry {
	struct in_addr addr;
	char enaddr[ETHER_ADDR_LEN];
};
//roy+++

/*
 *asp define here
 */
static int  	aspTendaGetStatus(int eid, webs_t wp, int argc, char_t **argv);
static int 		aspmGetRouteTable(int eid, webs_t wp, int argc, char_t **argv);
static int 		aspmNatPortSegment(int eid, webs_t wp, int argc, char_t **argv);
static int 		aspTendaGetDhcpClients(int eid, webs_t wp, int argc, char_t **argv);
static int  	sysTendaGetStatus(int eid, webs_t wp, int argc, char_t **argv);
static int 		aspErrorMsg(int eid,webs_t wp,int argc,char_t **argv);
static int 		aspErrorRedirect(int eid,webs_t wp,int argc,char_t **argv);
static int 		aspSysLogGet(int eid, webs_t wp, int argc, char_t **argv);

/*
 *form define here
 */
static void	fromAdvSetLanip(webs_t wp, char_t *path, char_t *query);
static void	fromWizardHandle(webs_t wp, char_t *path, char_t *query);
static void 	fromAdvSetMacClone(webs_t wp, char_t *path, char_t *query);
static void 	fromAdvSetWan(webs_t wp, char_t *path, char_t *query);
static void 	fromAdvSetDns(webs_t wp, char_t *path, char_t *query);
static void	fromVirSerDMZ(webs_t wp, char_t *path, char_t *query);
static void 	fromVirSerUpnp(webs_t wp, char_t *path, char_t *query);
static void	fromVirSerSeg(webs_t wp, char_t *path, char_t *query);
static void	fromSysToolTime(webs_t wp, char_t *path, char_t *query);
static void 	fromalgform(webs_t wp, char_t *path, char_t *query);
static void	fromsetWanSpeed(webs_t wp, char_t *path, char_t *query);//huangxiaoli modify
//roy+++
static void 	fromDhcpSetSer(webs_t wp, char_t *path, char_t *query);
static void 	fromDhcpListClient(webs_t wp, char_t *path, char_t *query);
//beili+++
static void 	fromSysToolChangePwd(webs_t wp, char_t *path, char_t *query);
static void 	fromSysToolReboot(webs_t wp, char_t *path, char_t *query);
static void 	fromSysToolRestoreSet(webs_t wp, char_t *path, char_t *query);
static void 	fromSysToolSysLog(webs_t wp, char_t *path, char_t *query);
static void 	fromSysToolDDNS(webs_t wp, char_t *path, char_t *query);
static void 	fromRouteStatic(webs_t wp, char_t *path, char_t *query);
static void 	fromSysAted(webs_t wp, char_t *path, char_t *query);
static void 	fromSysStatusHandle(webs_t wp, char_t *path, char_t *query);

void asp_define()
{ 
    	websAspDefine(T("aspTendaGetStatus"), aspTendaGetStatus);	
	websAspDefine(T("mGetRouteTable"), aspmGetRouteTable);	
	websAspDefine(T("mNatPortSegment"), aspmNatPortSegment);
	websAspDefine(T("TendaGetDhcpClients"), aspTendaGetDhcpClients);
	websAspDefine(T("sysTendaGetStatus"), sysTendaGetStatus);
	websAspDefine(T("asp_error_message"),aspErrorMsg);
	websAspDefine(T("asp_error_redirect_url"),aspErrorRedirect);
	websAspDefine(T("aspSysLogGet"),aspSysLogGet);
}

void goform_define()
{
	websFormDefine(T("AdvSetLanip"), fromAdvSetLanip);
	websFormDefine(T("WizardHandle"), fromWizardHandle);
	websFormDefine(T("AdvSetMacClone"), fromAdvSetMacClone);
	websFormDefine(T("AdvSetWan"), fromAdvSetWan);
	websFormDefine(T("AdvSetDns"), fromAdvSetDns);
	websFormDefine(T("VirSerDMZ"), fromVirSerDMZ);
	websFormDefine(T("VirSerUpnp"), fromVirSerUpnp);
	websFormDefine(T("VirSerSeg"), fromVirSerSeg);
	websFormDefine(T("SysToolTime"), fromSysToolTime);
	websFormDefine(T("algform"), fromalgform);
	websFormDefine(T("setWanSpeed"),fromsetWanSpeed);
//roy+++
	websFormDefine(T("DhcpSetSer"), fromDhcpSetSer);
	websFormDefine(T("DhcpListClient"), fromDhcpListClient);
//beili+++
	websFormDefine(T("SysToolChangePwd"), fromSysToolChangePwd);
	websFormDefine(T("SysToolReboot"), fromSysToolReboot);
	websFormDefine(T("SysToolRestoreSet"), fromSysToolRestoreSet);
	websFormDefine(T("SysToolSysLog"), fromSysToolSysLog);
	websFormDefine(T("SysToolDDNS"), fromSysToolDDNS);
	websFormDefine(T("RouteStatic"), fromRouteStatic);
	websFormDefine(T("ate"), fromSysAted);
	websFormDefine(T("SysStatusHandle"), fromSysStatusHandle);
	
}




void wl_change_states( int status )
{
	if ( WL_RADIO_OFF == status)
	{
		g_cur_wl_radio_status = WL_RADIO_OFF;
	}
	else
	{
		g_cur_wl_radio_status = WL_RADIO_ON;
	}
	
	
	sys_restart();

}



void SetErrorArg(char *msg,char *url)
{
	memset(gWLErrorMsg,0,sizeof(gWLErrorMsg));
	memset(gWLErrorRtnUrl,0,sizeof(gWLErrorRtnUrl));
	strcpy(gWLErrorMsg,msg);
	strcpy(gWLErrorRtnUrl,url);
}

static int aspErrorMsg(int eid,webs_t wp,int argc,char_t **argv)
{

	if(gWLErrorMsg[0] == 0)
	{
		strcpy(gWLErrorMsg,"system error!");
		strcpy(gWLErrorRtnUrl,"system_status.asp");
	}
	websWrite(wp,T(gWLErrorMsg));
	memset(gWLErrorMsg,0,sizeof(gWLErrorMsg));

	return 0;
}

static int aspErrorRedirect(int eid,webs_t wp,int argc,char_t **argv)
{
	if(gWLErrorRtnUrl[0] == 0)
	{
		strcpy(gWLErrorRtnUrl,"system_status.asp");
	}
	return websWrite(wp,T(gWLErrorRtnUrl));
}

/***********************************************************************/


void _mask(struct sockaddr *sa, char *buf, int _len)//roy modify
{
    unsigned char *cp = ((char *)sa) + 4;
    int len = sa->sa_len - 4;
    int tot = 0;

    while (len-- > 0) {
        if (tot) *buf++ = '.';
        buf += diag_sprintf(buf, "%d", *cp++);
        tot++;
    }

    while (tot < 4) {
        if (tot) *buf++ = '.';
        buf += diag_sprintf(buf, "%d", 0);
        tot++;
    }
}

/***********************************************************************/

/*����ϵͳ״̬ҳ��õ�WAN�ڵ���Ϣ*/
extern char change_ip[];
static int  	sysTendaGetStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*item=NULL, *type=NULL;
	int retv = 0,wan_status,wan_type;
	unsigned int time_diff;
	char val2[64];
	char *v=NULL;
	
	if (ejArgs(argc, argv, T("%s %s"), &type, &item) < 2) {
	}

	//diag_printf("==%s=type=[%s]===item=[%s]\n",__FUNCTION__,type,item);

	wan_status = get_wan_connstatus();
	wan_type = get_wan_type();

	if (strcmp(type,"wan") == 0){
		if(strcmp(item,"contstatus") == 0){
			/*0:dic,1:connecting,2:connected*/
			retv = websWrite(wp,T("%d"),wan_status);
		}
		else if(strcmp(item,"linkstatus") == 0){
			/*0:dic,1:connecting,2:connected*/
			retv = websWrite(wp,T("%d"),get_wan_linkstatus());
		}
		else if(strcmp(item,"wanip") == 0){
			if(wan_status == 2)
				retv = websWrite(wp,T("%s"),NSTR(SYS_wan_ip));
			else
				retv = websWrite(wp,T(""));
		}
		else if(strcmp(item,"wanmask") == 0){
			if(wan_status == 2)
				retv = websWrite(wp,T("%s"),NSTR(SYS_wan_mask));
			else
				retv = websWrite(wp,T(""));
		}
		else if(strcmp(item,"gateway") == 0){
			if(wan_status == 2)
				retv = websWrite(wp,T("%s"),NSTR(SYS_wan_gw));
			else
				retv = websWrite(wp,T(""));
		}
		else if(strcmp(item,"dns1") == 0){
			if(wan_status == 2)
				if(wan_type == PPPOEMODE ||wan_type == PPTPMODE ||
					wan_type == L2TPMODE||wan_type==PPPOEMODE2	){
					retv = websWrite(wp,T("%s"),SYS_dns_1);
				}else{
					memset(val2,0,sizeof(val2));
					get_wan0_dns(1,val2);
					retv = websWrite(wp,T("%s"),val2);
				}
			else
				retv = websWrite(wp,T(""));
		}
		else if(strcmp(item,"dns2") == 0){
			if(wan_status == 2)
				if(wan_type == PPPOEMODE ||wan_type == PPTPMODE ||
					wan_type == L2TPMODE||wan_type==PPPOEMODE2	){
					retv = websWrite(wp,T("%s"),SYS_dns_2);
				}else{
					memset(val2,0,sizeof(val2));
					get_wan0_dns(2,val2);
					retv = websWrite(wp,T("%s"),val2);
				}
			else
				retv = websWrite(wp,T(""));
		}
		else if(strcmp(item,"connetctime") == 0){
			if(wan_status == 2){
				if (SYS_wan_conntime)
					time_diff = time(0) - SYS_wan_conntime;
				else
					time_diff = 0;
			}else
				time_diff = 0;
			
			retv = websWrite(wp,T("%u"),time_diff);
		}
		else if(strcmp(item,"login_where") == 0){
			
			retv = websWrite(wp,T("%d"),g_login_where);
		}
		else if(strcmp(item,"change-ip"))
		{
			retv = websWrite(wp,T("%s"),change_ip);
		}
	}
	else if(strcmp(type,"lan") == 0){
		if(strcmp(item,"lanip") == 0){
			retv = websWrite(wp,T("%s"),NSTR(SYS_lan_ip));
		}
		else if(strcmp(item,"lanmask") == 0){
			retv = websWrite(wp,T("%s"),NSTR(SYS_lan_mask));
		}
	}
	else if(strcmp(type,"pppoe")==0)
	{
		if(strcmp(item,"err_check")==0)
			{
			retv=websWrite(wp,T("%s"),_SAFE_GET_VALUE("err_check",v));
			}
		else if(strcmp(item,"index")==0)
			{
			retv=websWrite(wp,T("%s"),_SAFE_GET_VALUE("pppoe_index",v));
			_SET_VALUE("pppoe_index","0");
			}
	}

	return retv;
}

static int  aspTendaGetStatus(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*item=NULL, *type=NULL;
	int retv=0;
	char *v = NULL;
	char *u = NULL;
	
 	if (ejArgs(argc, argv, T("%s %s"), &type, &item) < 2) 
	{
		return 0;
	}

	if (item == NULL || type == NULL)
	{
		return 0;
	}
	if(strcmp(type,"lan") == 0)
	{
		if(strcmp(item,"lanip") == 0)
		{
			return websWrite(wp,T("%s"),_SAFE_GET_VALUE(_LAN0_IP,v));
		}
		else if(strcmp(item,"lanmask") == 0)
		{
			return websWrite(wp,T("%s"),_SAFE_GET_VALUE(_LAN0_NETMASK,v));
		}
		else if(strcmp(item,"dhcps") == 0)
		{
			return websWrite(wp,T("%d"),get_dhcpd_en());
		}
		else if(strcmp(item,"dhcps_start") == 0)
		{
			 _SAFE_GET_VALUE(_LAN0_DHCPD_START,v);
		}
		else if(strcmp(item,"dhcps_end") == 0)
		{
			_SAFE_GET_VALUE(_LAN0_DHCPD_END,v);
		}
		else if(strcmp(item,"lease_time") == 0)
		{
			_SAFE_GET_VALUE(_LAN0_DHCPD_LEASET,v);
		}
		else if(strcmp(item,"dmzen") == 0)
		{
			_SAFE_GET_VALUE(_FW_DMZ_IPADDR_EN,v);
		}
		else if(strcmp(item,"dmzip") == 0)
		{
			_SAFE_GET_VALUE(_FW_DMZ_IPADDR,v);
		}
		
		return websWrite(wp, T("%s"),v);
	}
	else if(strcmp(type,"sys") == 0)
	{
		if(strcmp(item,"lanmac") == 0)
		{
			_SAFE_GET_VALUE(_LAN0_HWADDR, v);
		}
		//add by ldm
		else if(strcmp(item,"clnway") == 0)
		{
			_SAFE_GET_VALUE("clnway", v);
		}
		else if(strcmp(item,"config_num") == 0)
		{
			_SAFE_GET_VALUE("config_index", v);
		}
		//end
		else if(strcmp(item,"wanmac") == 0)
		{
			_SAFE_GET_VALUE("wl_mode", u);
			if(strcmp(u,"sta")==0)
				_SAFE_GET_VALUE("wl0.1_hwaddr", v);
			else
				_SAFE_GET_VALUE(_WAN0_HWADDR, v);
#if 0			
			char wan_mac[6]={0,0,0,0,0,0};
			_SAFE_GET_VALUE(_WAN0_IFNAME, v);
			
			iflib_getifhwaddr(v,wan_mac);

			retv = websWrite(wp,T("%02X:%02X:%02X:%02X:%02X:%02X"),
				wan_mac[0]&0XFF,
				wan_mac[1]&0XFF,
				wan_mac[2]&0XFF,
				wan_mac[3]&0XFF,
				wan_mac[4]&0XFF,
				wan_mac[5]&0XFF);
				return retv;
#endif
		}
		else if(strcmp(item,"manmac") == 0)
		{
			char pcMac[20];
			((struct in_addr*) pcMac)->s_addr=inet_addr(wp->ipaddr);
				
			if(arpioctl(SIOCGARPRT, pcMac, NULL) == 0)
			{
				retv = websWrite(wp,T("%02X:%02X:%02X:%02X:%02X:%02X"),
							pcMac[4]&0XFF,
							pcMac[5]&0XFF,
							pcMac[6]&0XFF,
							pcMac[7]&0XFF,
							pcMac[8]&0XFF,
							pcMac[9]&0XFF);
			}
			else
			{
				retv = websWrite(wp,T("%02X:%02X:%02X:%02X:%02X:%02X"),
							0,
							0,
							0,
							0,
							0,
							0);//wrong value
			}
			return retv;
		}
		else if(strcmp(item,"fmac") == 0)
		{
			char fmmac[6]={0,0,0,0,0,0};
			
			iflib_getifhwaddr("br0",fmmac);

			retv = websWrite(wp,T("%02X:%02X:%02X:%02X:%02X:%02X"),
				fmmac[0]&0XFF,
				fmmac[1]&0XFF,
				fmmac[2]&0XFF,
				fmmac[3]&0XFF,
				fmmac[4]&0XFF,
				fmmac[5]&0XFF);
			return retv;
		}
		else if(strcmp(item,"upnpen") == 0)
		{
			_SAFE_GET_VALUE(_FW_UPNP_EN, v);
		}
		else if(strcmp(item,"timezone") == 0){
			_SAFE_GET_VALUE(_SYS_TZONE,v);
		}
		else if(strcmp(item,"timeMode") == 0){
			_SAFE_GET_VALUE(_SYS_NTPTYPE,v);
		}
		else if(strcmp(item,"sysver") == 0){
#if defined(__CONFIG_WEB_VERSION__)
			return websWrite(wp,T("%s_%s"),W311R_ECOS_SV,__CONFIG_WEB_VERSION__);
#else
			return websWrite(wp,T("%s"),W311R_ECOS_SV);
#endif
		}
		else if(strcmp(item,"hardwarever") == 0){
			return websWrite(wp,T("%s"),W311R_ECOS_HV);
		}
		else if(strcmp(item,"compimetime") == 0){
			return websWrite(wp,T("%s"),TWL300N_FIRMWARE_BUILD_DATE);
		}
		else if(strcmp(item,"runtime") == 0){
			return websWrite(wp, T("%u"),(unsigned int)(cyg_current_time()/100));
		}
		else if(strcmp(item,"username") == 0){
			_SAFE_GET_VALUE(HTTP_USERNAME, v);
		}
		/*add by ldm for system_password*/
		else if(strcmp(item,"password") == 0){
			_SAFE_GET_VALUE(HTTP_PASSWD, u);
			if(strcmp(u,"")==0)
				return websWrite(wp, T("%s"),"0");
			else
				return websWrite(wp, T("%s"),"1");
		}
		/*add end*/
		else if(strcmp(item,"systime") == 0){
			char time_info[40];
			time_t now;
			struct tm TM;
			now = time(0);
			gmtime_r(&now,&TM);
			//diag_printf("%04d-%02d-%02d \n", TM.tm_year,TM.tm_mon, TM.tm_mday);
			if(TM.tm_year == 70)
			{
				sprintf(time_info,"%04d-%02d-%02d %02d:%02d:%02d",
				TM.tm_year + 1900+41,TM.tm_mon + 1+3,TM.tm_mday,
					TM.tm_hour,TM.tm_min,TM.tm_sec);
			}
			else
			{
				sprintf(time_info,"%04d-%02d-%02d %02d:%02d:%02d",
				TM.tm_year + 1900,TM.tm_mon + 1,TM.tm_mday,
					TM.tm_hour,TM.tm_min,TM.tm_sec);
			}
			
			return websWrite(wp, T("%s"), time_info);
		}
		else if(strcmp(item,"conclient") == 0){
			char conclient[20];
			struct arp_arpentry *arp_ent;
			memset(conclient,0,sizeof(conclient));
				
			if(arpioctl(SIOCGARPNU, conclient, NULL) == 0)
			{
				arp_ent = (struct arp_arpentry *)conclient;
				return websWrite(wp, T("%d"),arp_ent->addr.s_addr);
			}else{
				return websWrite(wp, T("%d"),1);
			}
		}
		return websWrite(wp, T("%s"),v);
			
	}
	else if (strcmp(type,"wan") == 0)
	{
		if(strcmp(item,"connecttype") == 0){
			return websWrite(wp, T("%d"), get_wan_type());
		}
		else if(strcmp(item,"connectsave") == 0){
			return websWrite(wp, T("%d"), get_wan_type_index());
		}
		else if(strcmp(item,"dns1") == 0){
			//wan_dns.asp
			char dns1[20]={0};
			get_wan0_dns(1,dns1);
			v = dns1;
		}
		else if(strcmp(item,"dns2") == 0){
			//wan_dns.asp
			char dns2[20]={0};
			get_wan0_dns(2,dns2);
			v = dns2;
		}
		else if(strcmp(item,"dnsen") == 0){
			_SAFE_GET_VALUE(_WAN0_DNS_FIX, v);	
		}
		else if(strcmp(item,"wanip")==0)
		{
			 _SAFE_GET_VALUE(_WAN0_IPADDR,v);
		}
		else if(strcmp(item,"wanmask")==0)
		{
			_SAFE_GET_VALUE(_WAN0_NETMASK,v);
		}
		else if(strcmp(item,"staticgateway")==0)
		{ 
			 _SAFE_GET_VALUE(_WAN0_GATEWAY,v);
		}
		else if(strcmp(item,"staticMTU")==0)
		{ 
			 _SAFE_GET_VALUE(_WAN0_MTU,v);
		}
		else if(strcmp(item,"dynamicMTU")==0)
		{ 
			_SAFE_GET_VALUE(_WAN0_MTU,v);
		}
		else if(strcmp(item,"l2tpIP")==0)
		{
			_SAFE_GET_VALUE(_WAN0_l2TP_SERVER_NAM,v);
		}
		else if(strcmp(item,"l2tpPUN")==0)
		{
			_SAFE_GET_VALUE(_WAN0_l2TP_USERNAME,v);
		}
		else if(strcmp(item,"l2tpPPW")==0)
		{
			_SAFE_GET_VALUE(_WAN0_l2TP_PASSWD,v);
		}
		else if(strcmp(item,"l2tpAdrMode")==0)
		{
			_SAFE_GET_VALUE(_WAN0_l2TP_STATIC,v);
		}
		else if(strcmp(item,"l2tpMTU")==0)
		{
			_SAFE_GET_VALUE(_WAN0_l2TP_MTU,v);
		}
		else if(strcmp(item,"l2tpWANIP")==0)
		{
			_SAFE_GET_VALUE(_WAN0_l2TP_IPADDR,v);
		}
		else if(strcmp(item,"l2tpWANMSK")==0)
		{
			_SAFE_GET_VALUE(_WAN0_l2TP_NETMASK,v);
		}
		else if(strcmp(item,"l2tpWANGW")==0)
		{
			_SAFE_GET_VALUE(_WAN0_l2TP_GATEWAY,v);
		}
		else if(strcmp(item,"l2tpDNS1")==0)
		{
			char l2tpdns1[20]={0};
			
			get_l2tp_dns(1,l2tpdns1);
			v=l2tpdns1;
		}
		else if(strcmp(item,"l2tpDNS2")==0)
		{
			char l2tpdns2[20]={0};
			
			get_l2tp_dns(2,l2tpdns2);
			v=l2tpdns2;
		}
		else if(strcmp(item,"pptpIP")==0){
			_SAFE_GET_VALUE(_WAN0_PPTP_SERVER_NAME,v);
		}
		else if(strcmp(item,"pptpPUN")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPTP_USERNAME,v);
		}
		else if(strcmp(item,"pptpPPW")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPTP_PASSWD,v);
		}
		else if(strcmp(item,"pptpAdrMode")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPTP_STATIC,v);
		}
		else if(strcmp(item,"pptpMTU")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPTP_MTU,v);
		}
		else if(strcmp(item,"pptpWANIP")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPTP_IPADDR,v);
		}
		else if(strcmp(item,"pptpWANMSK")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPTP_NETMASK,v);
		}
		else if(strcmp(item,"pptpWANGW")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPTP_GATEWAY,v);
		}
		else if(strcmp(item,"pptpMPPE")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPTP_MPPE_EN,v);
		}
		else if(strcmp(item,"pptpDNS1")==0)
		{
			char pptpdns1[20]={0};
			
			get_pptp_dns(1,pptpdns1);
			v=pptpdns1;
		}
		else if(strcmp(item,"pptpDNS2")==0)
		{
			char pptpdns2[20]={0};
			
			get_pptp_dns(2,pptpdns2);
			v=pptpdns2;
		}
		else if(strcmp(item,"wan0_route") == 0){
			_SAFE_GET_VALUE(_WAN0_ROUTE,v);
		}
		/*huangxiaoli modify*/
		else if(strcmp(item, "wanspeed") == 0)
		{
			_SAFE_GET_VALUE("wan_speed",v);
		}
		/*end modify*/
		/* huangxiaoli add for 8021x */
		else if(strcmp(item, "x1name") == 0)
		{
			_SAFE_GET_VALUE("wan0_1x_username",v);
		}
		else if(strcmp(item, "x1pwd") == 0)
		{
			_SAFE_GET_VALUE("wan0_1x_password",v);
		}
		else if(strcmp(item, "x1mode") == 0)
		{
			_SAFE_GET_VALUE("wan0_1x_ardmode",v);
		}
		else if(strcmp(item, "x1mtu") == 0)
		{
			_SAFE_GET_VALUE("wan0_1x_mtu",v);
		}
		/* end add */
		return websWrite(wp, T("%s"),v);
	}
	else if(strcmp(type,"ppoe") == 0){
		int h_s,m_s,h_e,m_e;
		if(strcmp(item,"userid")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_USERNAME, v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"pwd")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_PASSWD, v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"idletime")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_IDLETIME, v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"conmode")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_DEMAND, v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"mtu")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_MTU, v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"sev")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_SERVICE, v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"ac")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_AC, v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"pppoeAdrMode")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE2_STATIC,v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"pppoeWANIP")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE2_IPADDR,v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"pppoeWANMSK")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE2_NETMASK,v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"pppoeWANGW")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE2_GATEWAY,v);
			retv = websWrite(wp, T("%s"),v);
		}
		else if(strcmp(item,"h_s")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_ST, v);
			h_s = atoi(v)/3600;
			retv = websWrite(wp,T("%d"),h_s);
		}
		else if(strcmp(item,"m_s")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_ST, v);
			h_s = atoi(v)%3600;
			m_s = h_s/60;
			retv = websWrite(wp,T("%d"),m_s);
		}
		else if(strcmp(item,"h_e")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_ET, v);
			h_e = atoi(v)/3600;
			retv = websWrite(wp,T("%d"),h_e);
		}
		else if(strcmp(item,"m_e")==0)
		{
			_SAFE_GET_VALUE(_WAN0_PPPOE_ET, v);
			h_e = atoi(v)%3600;
			m_e = h_e/60;
			retv = websWrite(wp,T("%d"),m_e);
		}

		return retv;

	}
	else if(strcmp(type,"ddns") == 0){

		char *arglists[9];
		char ddns_value[128];
		memset(ddns_value,0,sizeof(ddns_value));
		if(strcmp(item,"en") == 0){
			_SAFE_GET_VALUE(_DDNS_ENABLE,v);
			
			if(atoi(v) >= 1){
				retv = websWrite(wp,T("%d"),1);
			}else
				retv = websWrite(wp,T("%d"),0);
		}else if(strcmp(item,"provide")==0){
			_SAFE_GET_VALUE(_DDNS_ENABLE,v);
			if(atoi(v) >= 1){
				retv = websWrite(wp,T("%d"),atoi(v) -1);
			}else
				retv = websWrite(wp,T("%d"),0);
		}else if(strcmp(item,"url") == 0){
			_SAFE_GET_VALUE(_DDNS_SET1,v);
			strncpy(ddns_value,v,sizeof(ddns_value));
			
			if(str2arglist(ddns_value, arglists, ';', 9) == 9){
				retv = websWrite(wp,T("%s"),arglists[2]);
			}else
				retv = websWrite(wp,T("%s"),"");
			
		}else if(strcmp(item,"user_name") == 0){
			_SAFE_GET_VALUE(_DDNS_SET1,v);
			strncpy(ddns_value,v,sizeof(ddns_value));
			
			if(str2arglist(ddns_value, arglists, ';', 9) == 9){
				retv = websWrite(wp,T("%s"),arglists[3]);
			}else
				retv = websWrite(wp,T("%s"),"");
		}else if(strcmp(item,"pwd") == 0){
			_SAFE_GET_VALUE(_DDNS_SET1,v);
			strncpy(ddns_value,v,sizeof(ddns_value));
			
			if(str2arglist(ddns_value, arglists, ';', 9) == 9){
				retv = websWrite(wp,T("%s"),arglists[4]);
			}else
				retv = websWrite(wp,T("%s"),"");
		}
		return retv;

	}
	//add by z10312 FH456V2.0 V2 TDE ��ֲucd ���߹���&��ʱ����  20150707
	else if(strcmp(type,"wifi") == 0){

		if(strcmp(item,"HighPower") == 0){

			retv = websWrite(wp,T("%s"),get_product_pwr_info());
		}
		return retv;
		
	}
	//add done
	
	return websWrite(wp,T(""));
}

static int aspmNatPortSegment(int eid, webs_t wp, int argc, char_t **argv)
{
	int which;
	char value[128]={'\0'},newv[128]={'\0'};
	int retv = 0;
	char *v;
	
	if(ejArgs(argc, argv, T("%d"),&which) < 1)
	{
	}
	if(which >=1 && which <=10){
		which -= 1;
		_SAFE_GET_VALUE(_FW_FORWARD_PORT(which),v);
	}
	else{
		v=NULL;
	}
	//forward_port0=50-60>192.168.1.23:50-60,tcp,on
	if(v){
		strcpy(value,v);
		if(parse_portforward2webstr(value,newv))
			retv = websWrite(wp,T("%s"),newv);
	}
	
	return retv;
}

//0x27-->'
static void dhcpd_entry2(char *buf, int id, void *arg1)
{
	webs_t req = (webs_t )arg1;
	char *p = buf;
	
	if (id>1)
		websWrite(req, T("%s"),",");

	websWrite(req, T("%c"),0x27);
	
	for(;*p != '\0';p++){				
		if(*p != 0x27) websWrite(req, T("%c"), *p);
	}
	websWrite(req, T("%c"),0x27);
}

extern int show_lan_leases(void *wp, void (*dhcpd_entry2)(char *buf, int id, void *arg1));

static int 		aspTendaGetDhcpClients(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t *type=NULL;
	int i;
	char *value = NULL;
	//char name[32];
	ejArgs(argc, argv, T("%s"), &type);

	if(strcmp(type,"staticlist")==0)
	{
		for(i = 1;i<=DHCPD_STATIC_LEASE_NU;i++){
			//sprintf(name,"dhcp_static_lease%d",i);
			//_SAFE_GET_VALUE(name,value);
			_SAFE_GET_VALUE(LAN0_DHCP_SATIC(i),value);
			
			if(value && strlen(value) >4/*simple check*/ && i > 1)
				websWrite(wp, T("%s"),",");
			
			if(strlen(value) >4/*simple check*/)websWrite(wp, T("'%s'"),value);
		}
	}
	else if(strcmp(type,"list")==0)
	{	
		show_lan_leases(wp, dhcpd_entry2);
	}
	else if(strcmp(type,"dhcpipmacbind")==0)
	{
		websWrite(wp,"");
	}

	return 0;
}

void set_gLog_cur_page(int cur)
{
//call from syslog.c
	gLog_cur_page = cur;
}

static int aspSysLogGet(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t	*item, *type;
	int retv = 0;
	int index = 0;
	char log_time[32],log_type[32],log_data[__CONFIG_SYSLOG_LOG_SIZE__];
	char *p;
	
	if(ejArgs(argc, argv, T("%s %s"), &type, &item) < 2) {
	}

	if(strcmp(type,"system")==0)//return log context
	{
		logNum = 0;
		syslog_open();
		memset(log_time,0,sizeof(log_time));
		memset(log_type,0,sizeof(log_type));
		memset(log_data,0,sizeof(log_data));
		while(syslog_get2(log_time,log_type,log_data))
		{	
			index++;
			logNum++;
			
			if(index >= (10 * (gLog_cur_page - 1) + 1) && index < (10 * gLog_cur_page + 1))
			{
				websWrite(wp,T("<tr><td width=20>&nbsp;%d</td>"),index);
				websWrite(wp,T("<td width=120>&nbsp;%s</td>"),log_time);
				websWrite(wp,T("<td width=50>&nbsp;%s</td>"),log_type);
				p = strchr(log_data,'\n');
				if(p) *p = '\0';//remove '\n'
				websWrite(wp,T("<td>&nbsp;%s</td ></tr>"),log_data);	
			}
			memset(log_time,0,sizeof(log_time));
			memset(log_type,0,sizeof(log_type));
			memset(log_data,0,sizeof(log_data));		
		}

		syslog_close();
	}
	else if(strcmp(type,"count")==0)//retrun log number
	{
		retv = websWrite(wp,T("%d"),logNum);
	}
	else if(strcmp(type,"curcount")==0)//return cur page number
	{
		retv = websWrite(wp,T("%d"),gLog_cur_page);
	}
	return retv;
}


static void	fromAdvSetLanip(webs_t wp, char_t *path, char_t *query)
{
	char_t  *lan_ip, *go,*lan_mask,*value;
	char_t dhcp_ip_start[20],dhcp_ip_end[20];
	unsigned int dhcp_ip[4],lan_ip2[4];

	char_t old_lan_ip[20],old_lanmask[20],wan_ip[20],wan_mask[20];

	//unsigned int ul_dhcp_start_ip,ul_dhcp_end_ip,ul_lanip,ul_lanmask;
	//int which;

	struct in_addr;
	 //struct inip;

	lan_ip = websGetVar(wp, T("LANIP"), T("192.168.0.1")); 
	lan_mask = websGetVar(wp,T("LANMASK"),T("255.255.255.0"));
	go = websGetVar(wp, T("GO"), T("")); 

	strncpy(old_lan_ip,_GET_VALUE(_LAN0_IP,value),sizeof(old_lan_ip));
	strncpy(old_lanmask,_GET_VALUE(_LAN0_NETMASK,value),sizeof(old_lanmask));

	_SET_VALUE(_LAN0_IP,lan_ip);
	_SET_VALUE(_LAN0_NETMASK,lan_mask);

	if(get_wan_type() == STATICMODE){
		strcpy(wan_ip,_GET_VALUE(_WAN0_IPADDR,value));
		strcpy(wan_mask,_GET_VALUE(_WAN0_NETMASK,value));
		if (CGI_same_net_with_lan(inet_addr(wan_ip),inet_addr(wan_mask)))
		{
			_SET_VALUE(_LAN0_IP,old_lan_ip);
			_SET_VALUE(_LAN0_NETMASK,old_lanmask);
			
			SetErrorArg(LAN_WAN_IP_ERROR,"lan.asp");
			websRedirect(wp, T("error.asp"));
			return;
		}
	}

	//�����ԭ����LAN���ò���ͬһ����,�����LAN��صĲ���
	if(CGI_same_net_with_lan(SYS_lan_ip,SYS_lan_mask) == 0){
		//change dhcp settings
		strcpy(dhcp_ip_start,_GET_VALUE(_LAN0_DHCPD_START,value));
		strcpy(dhcp_ip_end,_GET_VALUE(_LAN0_DHCPD_END,value));

		sscanf(lan_ip, "%d.%d.%d.%d", &lan_ip2[0], &lan_ip2[1], &lan_ip2[2], &lan_ip2[3]);
		
		sscanf(dhcp_ip_start, "%d.%d.%d.%d", &dhcp_ip[0], &dhcp_ip[1], &dhcp_ip[2], &dhcp_ip[3]);
		sprintf(dhcp_ip_start,"%d.%d.%d.%d",lan_ip2[0], lan_ip2[1], lan_ip2[2],dhcp_ip[3]);

		sscanf(dhcp_ip_end, "%d.%d.%d.%d", &dhcp_ip[0], &dhcp_ip[1], &dhcp_ip[2], &dhcp_ip[3]);
		sprintf(dhcp_ip_end,"%d.%d.%d.%d",lan_ip2[0], lan_ip2[1], lan_ip2[2],dhcp_ip[3]);
		
#if 0		
		ul_dhcp_start_ip = inet_addr(dhcp_ip_start);
		ul_dhcp_end_ip = inet_addr(dhcp_ip_end);
		ul_lanip = inet_addr(lan_ip);

		ul_dhcp_start_ip = ((ul_dhcp_start_ip & 0xff000000) | (ul_lanip & 0x00ffffff));
		ul_dhcp_end_ip = ((ul_dhcp_end_ip & 0xff000000) | (ul_lanip & 0x00ffffff));

		memset(dhcp_ip_start,0,sizeof(dhcp_ip_start));
		memset(dhcp_ip_end,0,sizeof(dhcp_ip_end));
		inip.s_addr = ul_dhcp_start_ip;
		strcpy(dhcp_ip_start,inet_ntoa(inip));
		inip.s_addr = ul_dhcp_end_ip;
		strcpy(dhcp_ip_end,inet_ntoa(inip));
#endif
		_SET_VALUE(_LAN0_DHCPD_START,dhcp_ip_start);
		_SET_VALUE(_LAN0_DHCPD_END,dhcp_ip_end);
		
		//set filter
		modify_filter_virtual_server(lan_ip);
#if 0		
		//clean static dhcp lease
		for(which = 0; which<= DHCPD_STATIC_LEASE_NU; which++){
			_SET_VALUE(LAN0_DHCP_SATIC(which),"");
		}

		//clean url filter
		_SET_VALUE(_FW_FLT_URL_EN,"disable");
		_SET_VALUE(_FW_FLT_URL_CUR_NU,"1");
		for(which=0;which<= 9;which++){
			_SET_VALUE(_FW_FILTER_URL(which), "");	
		}

		//clean ip filter
		_SET_VALUE(_FW_FLT_CLN_EN,"disable");
		_SET_VALUE(_FW_FLT_CLN_CUR_NU,"1");
		for(which=0;which<= 9;which++){
			_SET_VALUE(_FW_FILTER_CLIENT(which), "");	
		}
		//clean mac filter
		_SET_VALUE(_FW_FLT_MAC_CUR_NU,"1");
		for(which=0;which<= 9;which++){
			_SET_VALUE(_FW_FILTER_MAC(which), "");	
		}
#endif
		//clean dmz
		//_SET_VALUE(_FW_DMZ_IPADDR_EN,"0");
		//_SET_VALUE(_FW_DMZ_IPADDR,"");

		//clean static route
		//_SET_VALUE(_WAN0_ROUTE,"");
	}
	
	_SET_VALUE("err_check","0");
	_COMMIT();
	websRedirect(wp, T("/direct_reboot.asp"));	
	
	cyg_thread_delay(200);
		
	sys_reboot();
}
/*huangxiaoli modify*/
static void	fromsetWanSpeed(webs_t wp, char_t *path, char_t *query)
{
	char_t *wanspeed;
	int speed;

	wanspeed =websGetVar(wp, T("ws"), T("0"));
	speed = atoi(wanspeed);
	_SET_VALUE("wan_speed", wanspeed);
	_COMMIT();
	ifr_set_link_speed2(speed);
	websRedirect(wp, T("/wan_speed.asp"));
}
/*end modify*/
extern void set_wireless_secur_from_indexasp(char *pass_phrase_str);

static void fromWizardHandle(webs_t wp, char_t *path, char_t *query)
{
	int want;
	int restart_wan = 0;
	//int rc;

	char_t *rebootFlag, *indexssid,*mac;
	//char_t *v;
	char_t *wantstr, *go, *wlpassword;
	char_t *wps;
#if 1
	if(strcmp("ko", __CONFIG_WEB_VERSION__) == 0)
	{
		indexssid = websGetVar(wp, T("SSID"), T("Tenda_WIFI"));
	}
#endif
	mac = websGetVar(wp, T("MACC"), T("")); 
	if(strcmp(mac,"") != 0)
			_SET_VALUE(_WAN0_HWADDR,mac);//mac clone

	rebootFlag = websGetVar(wp, T("rebootTag"), T("1"));

	wantstr = websGetVar(wp, T("WANT1"), T("127"));

	go = websGetVar(wp, T("GO"), T(""));

	wlpassword =  websGetVar(wp, T("wirelesspassword"), T("12345678"));

	/*wantstr = 1 static ip, =2 DHCP, =3 PPPOE, =4 PPTP, =5 L2TP, =6 1x, =7 DHCP+ */

	want = atoi(wantstr);
#if 1
	if (strcmp("ko", __CONFIG_WEB_VERSION__) == 0)
	{
		if(STATICMODE == want)//static ip
		{
			char_t *wanip,*wanmsk, *gw, *dns1, *dns2,dns[40];
			wanip = websGetVar(wp, T("WANIP"), T("0.0.0.0"));
			wanmsk = websGetVar(wp, T("WANMSK"), T("0.0.0.0"));
			gw = websGetVar(wp, T("WANGW"), T("0.0.0.0"));
			dns1 = websGetVar(wp, T("DS1"), T(""));
			dns2 = websGetVar(wp, T("DS2"), T(""));
			//diag_printf("[%s]:: ssid=%s, %s, %s,%s,%s,%s,\n", __FUNCTION__, 
			//			indexssid, wanip,wanmsk,	gw, dns1, dns2);
			if(CGI_same_net_with_lan(inet_addr(wanip),inet_addr(wanmsk)))
			{
				SetErrorArg(LAN_WAN_IP_ERROR, "wizard.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}
			if (strcmp(wanip,gw)==0||strcmp(wanip,dns1)==0||strcmp(wanip,dns2)==0)
			{
				SetErrorArg(WAN_IP_ERROR,"wan_connected.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}

			_SET_VALUE(_WAN0_IPADDR, wanip);
			_SET_VALUE(_WAN0_NETMASK, wanmsk);
			_SET_VALUE(_WAN0_GATEWAY, gw);
				
			//_SET_VALUE(_WAN0_DNS_FIX, "0");

			sprintf(dns,"%s %s",dns1,dns2);
			 _SET_VALUE(_WAN0_DNS, dns);

			//_SET_VALUE(_C8021X_EN,"0");
				
		}
		_SET_VALUE("wl_ssid", indexssid);	
		_SET_VALUE("wl0_ssid", indexssid);	
	}
#endif
	if(get_wan_type() != want)
		restart_wan = 1;

	if(DHCPMODE == want)//dhcp
	{
		_SET_VALUE(_WAN0_MTU,"1500");//default

		//rc = _SET_VALUE(_C8021X_EN,"0");
		//_SET_VALUE(_WAN0_DNS_FIX,"0");
	}
	else if(want == PPPOEMODE)
	{//pppoe
		char_t *pppoeuser, *pppoepwd, *conmode;

		pppoeuser = websGetVar(wp, T("PUN"), T("")); 
		pppoepwd = websGetVar(wp, T("PPW"), T("")); 
		conmode = websGetVar(wp, T("PCM"), T("0"));
		char_t *v12_time = websGetVar(wp,T("v12_time"),T("0"));

		if(strcmp(pppoeuser, nvram_safe_get(_WAN0_PPPOE_USERNAME)) != 0 ||
			strcmp(pppoepwd, nvram_safe_get(_WAN0_PPPOE_PASSWD)) != 0)
			restart_wan = 1;

		_SET_VALUE(_WAN0_PPPOE_USERNAME,pppoeuser);
		_SET_VALUE(_WAN0_PPPOE_PASSWD,pppoepwd);
		//conmode:0,auto;1,traffic;2,hand;3,time
		_SET_VALUE(_WAN0_PPPOE_DEMAND,"0");//auto,default value
#if defined(CONFIG_CHINA_NET_CLIENT)
		_SET_VALUE(_WAN0_PPPOE_XKJX_TIME,v12_time);
#endif
		//_SET_VALUE(_WAN0_DNS_FIX,"0");
		//rc = _SET_VALUE(_C8021X_EN,"0");
	}
#if 0
	else if(want == PPTPMODE)
	{//pptp
		char_t  *pptpip, *pptpuser, *pptppwd, *pptpmtu, *pptpsrv, *pptpmode, *pptpmask, *pptpgw,
				pptpdns[40],*pptpdns1,*pptpdns2;
		pptpsrv = websGetVar(wp, T("pptpIP"), T("0.0.0.0"));
		pptpuser = websGetVar(wp, T("pptpPUN"), T(""));
		pptppwd = websGetVar(wp, T("pptpPPW"), T(""));
		//wan��ģʽ
		pptpmode = websGetVar(wp, T("pptpAdrMode"), T("1"));

		pptpip = websGetVar(wp, T("pptpWANIP"), T(""));
		pptpmask = websGetVar(wp, T("pptpWANMSK"), T(""));
		pptpgw = websGetVar(wp, T("pptpWANGW"), T(""));
		pptpdns1 = websGetVar(wp, T("pptpDNS1"), T(""));
		pptpdns2 = websGetVar(wp, T("pptpDNS2"), T(""));

		if(strcmp(pptpmode,"1") == 0){
			if (CGI_same_net_with_lan(inet_addr(pptpip),inet_addr(pptpmask)))
			{
				SetErrorArg(LAN_WAN_IP_ERROR,"wizard.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}
		}

		rc = _SET_VALUE(_WAN0_PPTP_SERVER_NAME,pptpsrv);
		rc = _SET_VALUE(_WAN0_PPTP_USERNAME,pptpuser);
		rc = _SET_VALUE(_WAN0_PPTP_PASSWD,pptppwd);
		rc = _SET_VALUE(_WAN0_PPTP_STATIC,pptpmode);
		if(strcmp(pptpmode,"1") == 0){
			rc = _SET_VALUE(_WAN0_PPTP_IPADDR,pptpip);
			rc = _SET_VALUE(_WAN0_PPTP_NETMASK,pptpmask);
			rc = _SET_VALUE(_WAN0_PPTP_GATEWAY,pptpgw);
			sprintf(pptpdns,"%s %s",pptpdns1,pptpdns2);
			 _SET_VALUE(_WAN0_PPTP_DNS,pptpdns);
		}

		rc = _SET_VALUE(_WAN0_DNS_FIX,"0");

		//rc = _SET_VALUE(_C8021X_EN,"0");
	}
	else if(want == L2TPMODE)
	{//l2tp
		char_t  *l2tpip, *l2tpuser, *l2tppwd, *l2tpmode, *l2tpmtu, *l2tpwip, *l2tpwmask, *l2tpwgw,
			l2tpdns[40],*l2tpdns1,*l2tpdns2;

		l2tpip = websGetVar(wp, T("l2tpIP"), T(""));

		l2tpuser = websGetVar(wp, T("l2tpPUN"), T(""));
		l2tppwd = websGetVar(wp, T("l2tpPPW"), T(""));

		//wan��ģʽ
		l2tpmode = websGetVar(wp, T("l2tpAdrMode"), T("1"));

		l2tpwip = websGetVar(wp, T("l2tpWANIP"), T(""));
		l2tpwmask = websGetVar(wp, T("l2tpWANMSK"), T(""));
		l2tpwgw = websGetVar(wp, T("l2tpWANGW"), T(""));
		l2tpdns1 = websGetVar(wp, T("l2tpDNS1"), T(""));
		l2tpdns2 = websGetVar(wp, T("l2tpDNS2"), T(""));

		if(strcmp(l2tpmode,"1") == 0){
			if (CGI_same_net_with_lan(inet_addr(l2tpwip),inet_addr(l2tpwmask)))
			{
				SetErrorArg(LAN_WAN_IP_ERROR,"wizard.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}
		}

		rc = _SET_VALUE(_WAN0_l2TP_SERVER_NAM,l2tpip);
		rc = _SET_VALUE(_WAN0_l2TP_USERNAME,l2tpuser);
		rc = _SET_VALUE(_WAN0_l2TP_PASSWD,l2tppwd);
		rc = _SET_VALUE(_WAN0_l2TP_STATIC,l2tpmode);
		if(strcmp(l2tpmode,"1") == 0){
			rc = _SET_VALUE(_WAN0_l2TP_IPADDR,l2tpwip);
			rc = _SET_VALUE(_WAN0_l2TP_NETMASK,l2tpwmask);
			rc = _SET_VALUE(_WAN0_l2TP_GATEWAY,l2tpwgw);
			sprintf(l2tpdns,"%s %s",l2tpdns1,l2tpdns2);
			 _SET_VALUE(_WAN0_l2TP_DNS,l2tpdns);
		}

		rc = _SET_VALUE(_WAN0_DNS_FIX,"0");
		//rc = _SET_VALUE(_C8021X_EN,"0");
	}
#endif
	set_wan_str(want);
	set_wan_str_index(want);

	set_wireless_secur_from_indexasp(wlpassword);
		
			
		//mac clone
	stop_wireless_wps();
#ifdef __CONFIG_WPS_LED__
	wps_led_test_off();		
#endif

	nvram_set(WLN0_WPS_OOB, "disabled");
	_SET_VALUE("err_check","0");
	_SET_VALUE("config_index","1");
	_SET_VALUE("wan0_check","0");
	
	_COMMIT();

	cyg_thread_delay(100);

	if(restart_wan == 1)
		sys_restart2();
	else
		sys_restart();

#ifdef __CONFIG_WEB_VERSION__
	if(strcmp(__CONFIG_WEB_VERSION__,"cn") == 0)
	{
		SetErrorArg("���óɹ���","advance.asp");
	}
	else if(strcmp(__CONFIG_WEB_VERSION__,"ko") == 0)
	{
		SetErrorArg("1","index.asp");//1->Successfully Configure! 
	}
	else
	{
		SetErrorArg("Successfully Configure! ","advance.asp");
	}
#else
	SetErrorArg("Successfully Configure! ","advance.asp");
#endif
	cyg_thread_delay(100);
/*�޸�Ϊ�װ�װby ldm*/
		websRedirect(wp, T("advance.asp"));	
		//websRedirect(wp, T("notice.asp"));
}

static int rte_num1;

static int mtenda_routing_entry(struct radix_node *rn, void *vw)
{
   struct rtentry *rt = (struct rtentry *)rn;   
    
    struct sockaddr *dst, *gate, *netmask, *genmask;
    char addr[32]={0}, addr1[32]={0}, addr2[32]={0},ifname[32]={0};
    webs_t req = (webs_t )vw;

    dst = rt_key(rt);
    gate = rt->rt_gateway;
    netmask = rt_mask(rt);
    genmask = rt->rt_genmask;
    if ((rt->rt_flags & (RTF_UP | RTF_LLINFO)) == RTF_UP) 
	{
	        _inet_ntop(dst, addr, sizeof(addr));

	        if (netmask != NULL) {
	            _mask(netmask, addr2, sizeof(addr2));
	        } else {
	        	if(rt->rt_flags & RTF_HOST) 
	        		sprintf(addr2, "255.255.255.255");
	        	else
	            	sprintf(addr2, " ");
	        }
	       
	        if (gate != NULL && gate->sa_family == AF_INET) {
	            _inet_ntop(gate, addr1, sizeof(addr1));
	        } else if(gate != NULL && gate->sa_family == AF_LINK) {
	        	_inet_ntop((struct sockaddr *)rt_key(rt), addr1, sizeof(addr1));
	        } else {
	            sprintf(addr1,"unknown");
	        }

		 if_indextoname(rt->rt_ifp->if_index, ifname);
		 if(strcmp(ifname,"lo0") != 0){
		 	//don't show lo0 interface
			if (rte_num1++!=0)
				websWrite(req,",");
				
		        websWrite(req, T("'%s,%s,%s,%d,"), addr,addr2,addr1,rt->rt_rmx.rmx_hopcount);
			
			 websWrite(req, T("%s'"), ifname);
		 }
    }
    return 0;
}


/*��̬·�ɱ�++*/
static int aspmGetRouteTable(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t  *type, *item;
	
	if(ejArgs(argc, argv, T("%s %s"), &type, &item) < 2)
	{
	}

	if(strcmp(type,"sys") == 0)
	{
		websWrite(wp,"");//write a default value first
		rte_num1 = 0;
		show_network_tables2(mtenda_routing_entry,wp);
	}

	return 0;
}


//����ϵͳ�Ͽ�ҳ���ϵ�"�ͷ�" "����" ��ť
//*/
static void fromSysStatusHandle(webs_t wp, char_t *path, char_t *query)
{
	char_t  *ac, *go;
	int iac = 0;
	char_t *m;

	ac = websGetVar(wp, T("action"), T(""));
	go = websGetVar(wp, T("GO"), T(""));
	iac = atoi(ac);
	_SET_VALUE("pppoe_index",ac);	

	
	CGI_do_wan_connect_tenda(iac);

		cyg_thread_delay(200);
	

	websRedirect(wp, T(go));
}

/******************************************************************************
����Wan�ڵĸ��ֲ���
*/

static void fromAdvSetWan(webs_t wp, char_t *path, char_t *query)

{
	int t,wanmod;
	//int rc;
	char val[128];
	char dns3[40];
	char_t *mac;

	char *want = websGetVar(wp, T("WANT2"), T("127")); 

	mac = websGetVar(wp, T("MACC"), T("NULL")); 
	
	wanmod = atoi(want);//get_wan_type();

	printf(".....Debug,wanmod=%d\n",wanmod);

	if(C8021XMODE == wanmod)
	{
		char_t  *x1name,*x1pwd,*x1ardmode,*wanip, *wanmsk, *gw,*dns1, *dns2,*c8021xMtu;
		x1name = websGetVar(wp, T("x1_name"), T("")); 
		x1pwd = websGetVar(wp, T("x1_pwd"), T("")); 
		x1ardmode = websGetVar(wp, T("x1AdrMode"), T("")); 
		wanip = websGetVar(wp, T("WANIP"), T("0.0.0.0")); 
		wanmsk = websGetVar(wp, T("WANMSK"), T("0.0.0.0")); 
		gw = websGetVar(wp, T("WANGW"), T("0.0.0.0")); 
		dns1 = websGetVar(wp, T("DS1"), T(""));
		dns2 = websGetVar(wp, T("DS2"), T(""));
		c8021xMtu = websGetVar(wp, T("x1_MTU"), T("1500"));
		if (CGI_same_net_with_lan(inet_addr(wanip),inet_addr(wanmsk)))
		{
			SetErrorArg(LAN_WAN_IP_ERROR,"wan_connected.asp");
			websRedirect(wp, T("error.asp"));
			return;
		}
		_SET_VALUE("wan0_1x_username",x1name);
		_SET_VALUE("wan0_1x_password",x1pwd);
		_SET_VALUE("wan0_1x_ardmode",x1ardmode);
		_SET_VALUE(_WAN0_IPADDR,wanip);
		_SET_VALUE(_WAN0_NETMASK,wanmsk);
		_SET_VALUE(_WAN0_GATEWAY,gw);

		sprintf(dns3,"%s %s",dns1,dns2);
		_SET_VALUE(_WAN0_DNS,dns3);
		_SET_VALUE("wan0_1x_mtu",c8021xMtu);
		_SET_VALUE(_WAN0_MTU,c8021xMtu);
	}
	else if( STATICMODE ==wanmod)
	{
		char_t  *wanip, *wanmsk, *gw,*dns1, *dns2,*staticMtu;	
		wanip = websGetVar(wp, T("WANIP"), T("0.0.0.0")); 
		wanmsk = websGetVar(wp, T("WANMSK"), T("0.0.0.0")); 
		gw = websGetVar(wp, T("WANGW"), T("0.0.0.0")); 
		dns1 = websGetVar(wp, T("DS1"), T(""));
		dns2 = websGetVar(wp, T("DS2"), T(""));
		staticMtu = websGetVar(wp, T("staticMTU"), T("1500"));
		if (CGI_same_net_with_lan(inet_addr(wanip),inet_addr(wanmsk)))
		{
			SetErrorArg(LAN_WAN_IP_ERROR,"wan_connected.asp");
			websRedirect(wp, T("error.asp"));
			return;
		}
		//
		if (strcmp(wanip,gw)==0||strcmp(wanip,dns1)==0||strcmp(wanip,dns2)==0)
		{
			SetErrorArg(WAN_IP_ERROR,"wan_connected.asp");
			websRedirect(wp, T("error.asp"));
			return;
		}
		_SET_VALUE(_WAN0_IPADDR,wanip);
		_SET_VALUE(_WAN0_NETMASK,wanmsk);
		_SET_VALUE(_WAN0_GATEWAY,gw);

		sprintf(dns3,"%s %s",dns1,dns2);
		_SET_VALUE(_WAN0_DNS,dns3);
		_SET_VALUE(_WAN0_MTU,staticMtu);
	}
	else if(DHCPMODE == wanmod)
	{
		char_t  *DynStaticMtu;

		DynStaticMtu = websGetVar(wp, T("dynamicMTU"), T("1500"));

		 _SET_VALUE(_WAN0_MTU,DynStaticMtu);
	}
	else if(PPPOEMODE == wanmod)
	{//pppoe
		char_t *user_id, *pwd, *mtu, *ac, *sev, *conmode;
		char_t *idle_time, *hour_s, *min_s, *hour_e, *min_e;
		char_t *v12_time;
		int i_conmode = 0 ;

		user_id = websGetVar(wp, T("PUN"), T("")); 
		pwd = websGetVar(wp, T("PPW"), T("")); 
		mtu = websGetVar(wp, T("MTU"), T("1492"));
		ac = websGetVar(wp, T("AC"), T(""));
		sev = websGetVar(wp, T("SVC"), T(""));
		conmode = websGetVar(wp, T("PCM"), T("0"));
		idle_time = websGetVar(wp, T("PIDL"), T("60"));
		
		hour_s = websGetVar(wp, T("hour1"), T("0"));
		min_s = websGetVar(wp, T("minute1"), T("0"));
		hour_e = websGetVar(wp, T("hour2"), T("0"));
		min_e = websGetVar(wp, T("minute2"), T("0"));
		v12_time = websGetVar(wp,T("v12_time"),T("0"));

		_SET_VALUE(_WAN0_PPPOE_USERNAME,user_id);
		_SET_VALUE(_WAN0_PPPOE_PASSWD,pwd);
		_SET_VALUE(_WAN0_PPPOE_MTU,mtu);
		_SET_VALUE(_WAN0_PPPOE_MRU,mtu);//important for mtu
		_SET_VALUE(_WAN0_PPPOE_AC,ac);
		_SET_VALUE(_WAN0_PPPOE_SERVICE,sev);
		//conmode:0,auto;1,traffic;2,hand;3,time
		_SET_VALUE(_WAN0_PPPOE_DEMAND,conmode);
		//to be added idle_time
		i_conmode = atoi(conmode);
		
		_SET_VALUE(_WAN0_PPPOE_MANAGE_FLAG,"0");
		
		if(i_conmode == PPPOE_TRAFFIC){
			_SET_VALUE(_WAN0_PPPOE_IDLETIME,idle_time);
		}
		else if(i_conmode == PPPOE_BY_TIME){
			t = atoi(hour_s)*3600 + atoi(min_s)*60;
			sprintf(val,"%d",t);
			
			_SET_VALUE(_WAN0_PPPOE_ST,val);

			t = atoi(hour_e)*3600 + atoi(min_e)*60;
			sprintf(val,"%d",t);

			_SET_VALUE(_WAN0_PPPOE_ET,val);
		}
		
#ifdef __CONFIG_TENDA_HTTPD_V3__
		else if(i_conmode == PPPOE_BY_HAND){
			_SET_VALUE(_WAN0_PPPOE_MANAGE_FLAG,"1");

		}
#endif		
#if defined(CONFIG_CHINA_NET_CLIENT)
//wan0_pppoe_xkjx_time
		_SET_VALUE(_WAN0_PPPOE_XKJX_TIME,v12_time);
#endif
	}
	else if(PPPOEMODE2 == wanmod)
	{//pppoe
		char_t *user_id, *pwd, *p2MTU,*pppoeip,*pppoemode, *pppoemask, *pppoegw;
		//char_t pppoedns[40],*pppoedns1,*pppoedns2;
		char_t *pppoe2ac,*pppoe2ser,*pppoe2hostname,*pppoe2mtu2;

		user_id = websGetVar(wp, T("PUN"), T("")); 
		pwd = websGetVar(wp, T("PPW"), T("")); 
		p2MTU = websGetVar(wp, T("MTU"), T("1492"));

		//wan��ģʽ
		pppoemode = websGetVar(wp, T("pppoeAdrMode"), T("1"));
		pppoeip = websGetVar(wp, T("pppoeWANIP"), T(""));
		pppoemask = websGetVar(wp, T("pppoeWANMSK"), T(""));
		pppoegw = websGetVar(wp, T("pppoeWANGW"), T(""));

		pppoe2ac = websGetVar(wp, T("AC"), T("1"));
		pppoe2ser = websGetVar(wp, T("SVC"), T(""));
		pppoe2hostname = websGetVar(wp, T("pppoehostname"), T(""));
		pppoe2mtu2 = websGetVar(wp, T("dynamicMTU"), T("1460"));
	
		
		if(strcmp(pppoemode,"1") == 0){
			if (CGI_same_net_with_lan(inet_addr(pppoeip),inet_addr(pppoemask)))
			{
				SetErrorArg(LAN_WAN_IP_ERROR,"wan_connected.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}
		}
				
		_SET_VALUE(_WAN0_PPPOE2_USERNAME,user_id);
		_SET_VALUE(_WAN0_PPPOE2_PASSWD,pwd);
		_SET_VALUE(_WAN0_PPPOE2_MTU,p2MTU);
		_SET_VALUE(_WAN0_PPPOE2_MRU,p2MTU);
		_SET_VALUE(_WAN0_PPPOE2_AC,pppoe2ac);
		_SET_VALUE(_WAN0_PPPOE2_SERVICE,pppoe2ser);

		
		_SET_VALUE(_WAN0_PPPOE2_STATIC,pppoemode);

		if(strcmp(pppoemode,"1") == 0){
			_SET_VALUE(_WAN0_PPPOE2_IPADDR,pppoeip);
			_SET_VALUE(_WAN0_PPPOE2_NETMASK,pppoemask);
			_SET_VALUE(_WAN0_PPPOE2_GATEWAY,pppoegw);
		}else{
			_SET_VALUE(_WAN0_PPPOE2_HAOTNAME,pppoe2hostname);
		}
		_SET_VALUE(_WAN0_PPPOE2_MTU2,pppoe2mtu2);
		
	}
	else if(wanmod == PPTPMODE || wanmod == PPTPMODE2)
	{//pptp
		char_t  *pptpip, *pptpuser, *pptppwd, *pptpmtu, *pptpsrv, *pptpmode, *pptpmask, *pptpgw, *pptpMppe;
		char_t pptpdns[40],*pptpdns1,*pptpdns2;
		
		pptpsrv = websGetVar(wp, T("pptpIP"), T("0.0.0.0"));
		pptpuser = websGetVar(wp, T("pptpPUN"), T(""));
		pptppwd = websGetVar(wp, T("pptpPPW"), T(""));
		pptpmtu = websGetVar(wp, T("pptpMTU"), T("1452"));
		//wan��ģʽ
		pptpmode = websGetVar(wp, T("pptpAdrMode"), T("1"));

		pptpip = websGetVar(wp, T("pptpWANIP"), T(""));
		pptpmask = websGetVar(wp, T("pptpWANMSK"), T(""));
		pptpgw = websGetVar(wp, T("pptpWANGW"), T(""));

		pptpdns1 = websGetVar(wp, T("pptpDNS1"), T(""));
		pptpdns2 = websGetVar(wp, T("pptpDNS2"), T(""));

		pptpMppe = websGetVar(wp, T("mppeEn"), T("0"));

		if(strcmp(pptpmode,"1") == 0){
			if (CGI_same_net_with_lan(inet_addr(pptpip),inet_addr(pptpmask)))
			{
				SetErrorArg(LAN_WAN_IP_ERROR,"wan_connected.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}
			if (strcmp(pptpip,pptpgw)==0||strcmp(pptpip,pptpdns1)==0||strcmp(pptpip,pptpdns2)==0)
			{
				SetErrorArg(WAN_IP_ERROR,"wan_connected.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}
		}
	

		_SET_VALUE(_WAN0_PPTP_SERVER_NAME,pptpsrv);
		_SET_VALUE(_WAN0_PPTP_USERNAME,pptpuser);
		_SET_VALUE(_WAN0_PPTP_PASSWD,pptppwd);

		_SET_VALUE(_WAN0_PPTP_MTU,pptpmtu);
		_SET_VALUE(_WAN0_PPTP_MRU,pptpmtu);//important for mtu

		_SET_VALUE(_WAN0_PPTP_STATIC,pptpmode);

		if(strcmp(pptpmode,"1") == 0){
			_SET_VALUE(_WAN0_PPTP_IPADDR,pptpip);
			_SET_VALUE(_WAN0_PPTP_NETMASK,pptpmask);
			_SET_VALUE(_WAN0_PPTP_GATEWAY,pptpgw);
			sprintf(pptpdns,"%s %s",pptpdns1,pptpdns2);
			 _SET_VALUE(_WAN0_PPTP_DNS,pptpdns);
		}
		
		 _SET_VALUE(_WAN0_PPTP_MPPE_EN,pptpMppe);
				
	}
	else if(wanmod == L2TPMODE)
	{//l2tp
		char_t  *l2tpip, *l2tpuser, *l2tppwd, *l2tpmode, *l2tpmtu, *l2tpwip, *l2tpwmask, *l2tpwgw;
		char_t l2tpdns[40],*l2tpdns1,*l2tpdns2;
		
		l2tpip = websGetVar(wp, T("l2tpIP"), T(""));

		l2tpuser = websGetVar(wp, T("l2tpPUN"), T(""));
		l2tppwd = websGetVar(wp, T("l2tpPPW"), T(""));
		l2tpmtu = websGetVar(wp, T("l2tpMTU"), T("1400"));

		//wan��ģʽ
		l2tpmode = websGetVar(wp, T("l2tpAdrMode"), T("1"));

		l2tpwip = websGetVar(wp, T("l2tpWANIP"), T(""));
		l2tpwmask = websGetVar(wp, T("l2tpWANMSK"), T(""));
		l2tpwgw = websGetVar(wp, T("l2tpWANGW"), T(""));

		l2tpdns1 = websGetVar(wp, T("pptpDNS1"), T(""));
		l2tpdns2 = websGetVar(wp, T("pptpDNS2"), T(""));


		if(strcmp(l2tpmode,"1") == 0){
			if (CGI_same_net_with_lan(inet_addr(l2tpwip),inet_addr(l2tpwmask)))
			{
				SetErrorArg(LAN_WAN_IP_ERROR,"wan_connected.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}
			if (strcmp(l2tpwip,l2tpwgw)==0||strcmp(l2tpwip,l2tpdns1)==0||strcmp(l2tpwip,l2tpdns2)==0)
			{
				SetErrorArg(WAN_IP_ERROR,"wan_connected.asp");
				websRedirect(wp, T("error.asp"));
				return;
			}
		}

		_SET_VALUE(_WAN0_l2TP_SERVER_NAM,l2tpip);
		_SET_VALUE(_WAN0_l2TP_USERNAME,l2tpuser);
		_SET_VALUE(_WAN0_l2TP_PASSWD,l2tppwd);

		_SET_VALUE(_WAN0_l2TP_MTU,l2tpmtu);
		_SET_VALUE(_WAN0_l2TP_MRU,l2tpmtu);//important for mtu

		_SET_VALUE(_WAN0_l2TP_STATIC,l2tpmode);
		
		if(strcmp(l2tpmode,"1") == 0){
			_SET_VALUE(_WAN0_l2TP_IPADDR,l2tpwip);
			_SET_VALUE(_WAN0_l2TP_NETMASK,l2tpwmask);
			_SET_VALUE(_WAN0_l2TP_GATEWAY,l2tpwgw);
			sprintf(l2tpdns,"%s %s",l2tpdns1,l2tpdns2);
			 _SET_VALUE(_WAN0_PPTP_DNS,l2tpdns);
		}

	}
	// 1x remain added	
	_SET_VALUE("err_check","0");
	set_wan_str(wanmod);
	if(strcmp(mac,"NULL") != 0)
		_SET_VALUE(_WAN0_HWADDR,mac);

	_SET_VALUE("config_index","1");
	_SET_VALUE("wan0_check","0");

	_COMMIT();
	
	sys_restart2();
	
	websRedirect(wp, T("/system_status.asp"));
}


static void 	fromAdvSetMacClone(webs_t wp, char_t *path, char_t *query)
{
	char_t  *mac, *go,*way;
//huangxiaoli modify;ȥ��MAC��¡������·��1,����Ҫ����
/*
	char_t *rebootFlag;

	rebootFlag = websGetVar(wp, T("rebootTag"), T("1"));
*/
	mac = websGetVar(wp, T("WMAC"), T("")); 
	go = websGetVar(wp, T("GO"), T(""));
	way= websGetVar(wp, T("autoenable"), T("0"));
	
	_SET_VALUE(_WAN0_HWADDR,mac);
	_SET_VALUE("clnway",way);

	_COMMIT();
	sys_restart2();
//huangxiaoli modify;ȥ��MAC��¡������·��2
/*
	if(rebootFlag){
		websRedirect(wp, T("/direct_reboot.asp"));
		sys_reboot();
	}else{
		websRedirect(wp, T("/mac_clone.asp"));
	}
*/
	websRedirect(wp, T("/mac_clone.asp"));

}


static void 	fromAdvSetDns(webs_t wp, char_t *path, char_t *query)
{
	char_t  *dns1, *dns2, *enable, *go; 
	char_t *rebootFlag;
	char_t *wan_type;
	char_t dns3[40];
	
	rebootFlag = websGetVar(wp, T("rebootTag"), T("1"));

	dns1 = websGetVar(wp, T("DS1"), T("")); 
	dns2 = websGetVar(wp, T("DS2"), T("")); 

	enable = websGetVar(wp, T("DSEN"), T("0")); 

	go = websGetVar(wp, T("GO"), T("")); 

	_GET_VALUE(_WAN0_PROTO,wan_type);
	_SET_VALUE(_WAN0_DNS_FIX,enable);
	
	if(strcmp(enable, "1") == 0)
	{
		sprintf(dns3,"%s %s",dns1,dns2);
		_SET_VALUE(_WAN0_DNS,dns3);
	}
	_COMMIT();
	
	sys_restart2();

	websRedirect(wp, T("/wan_dns.asp"));	
}

static void fromVirSerDMZ(webs_t wp, char_t *path, char_t *query)
{
	char_t  *dmzip, *go, *en;
	go = websGetVar(wp, T("GO"), T("")); 
	dmzip = websGetVar(wp, T("dmzip"), T("")); 
	en = websGetVar(wp, T("en"), T("0")); 
		
	_SET_VALUE(_FW_DMZ_IPADDR_EN,en);
	_SET_VALUE(_FW_DMZ_IPADDR,dmzip);

	_COMMIT();

	sys_restart();

	websRedirect(wp, T("/nat_dmz.asp"));	
}

static void fromVirSerUpnp(webs_t wp, char_t *path, char_t *query)
{
	char_t  *go, *upnpstatu;
	char_t  *val;
	//int rc;

	go = websGetVar(wp, T("GO"), T("0")); 
 
	upnpstatu = websGetVar(wp, T("UpnpStatus"), T("0"));  

	_SAFE_GET_VALUE(_FW_UPNP_EN,val);

	if(strncmp(val,upnpstatu,1) != 0){
		_SET_VALUE(_FW_UPNP_EN,upnpstatu);
		_COMMIT();

		sys_restart();
		
		websRedirect(wp, T("/upnp_config.asp"));	
	}else{
		websRedirect(wp, T("/upnp_config.asp"));
	}	
}

static void fromVirSerSeg(webs_t wp, char_t *path, char_t *query)
{
	char name[32] ={'\0'};
	int i = 0;
	char_t  *stritem;
	char oldvalue[64],newvalue[64];

	for(i = 0;i < VTS_MAX_NUM_1;i ++)
	{
		sprintf(name,"PL%d",i + 1);
		stritem = websGetVar(wp,T(name),T(""));
		if(strlen(stritem) <5/*simple check*/){
			//�����������
			_SET_VALUE(_FW_FORWARD_PORT(i),"");	
			continue;
		}
		strcpy(oldvalue,stritem);

		if(!parse_webstr2portforward(oldvalue, newvalue))
			continue;

		//diag_printf("=dfdf=%s==%s==[%s]\n",__FUNCTION__,newvalue,_FW_FORWARD_PORT(i));

		_SET_VALUE(_FW_FORWARD_PORT(i),newvalue);			
	}

	_COMMIT();

	sys_restart();

	websRedirect(wp, T("/nat_virtualportseg.asp"));
}

extern void update_application_time(void);
extern int wanRunTime;
extern int update_application_flag;
static void fromSysToolTime(webs_t wp, char_t *path, char_t *query)
{
	char_t  *timezone,*timezonesel,*go,*mode,*manual;
	int manual_time;

	go = websGetVar(wp, T("GO"), T("")); 
	timezone = websGetVar(wp, T("TZ"), T("57"));//57:beijing 
	timezonesel = websGetVar(wp, T("TZSel"), T("")); 
	mode = websGetVar(wp,T("manualEN"),T("0"));//0:sntp mode
	manual = websGetVar(wp,T("time"),T("1275384461"));
	
	_SET_VALUE(_SYS_NTPTYPE,mode);

	if(atoi(mode)){
		//by hand
		_COMMIT();

		manual_time = strtol(manual, NULL, 10);
		update_dhcp_client_lease(manual_time);
		wanRunTime = time(0) - primary_wan_ip_set.conn_time;
		cyg_libc_time_settime(manual_time);
		update_application_flag = 0;
		update_application_time();
		
		websRedirect(wp, T("/system_hostname.asp"));

	}else{
		_SET_VALUE(_SYS_TZONE,timezone);

		_COMMIT();

		sys_restart2();

		websRedirect(wp, T("/system_hostname.asp"));	
	}
}

static void fromalgform(webs_t wp, char_t *path, char_t *query)
{

}

static void 	fromDhcpSetSer(webs_t wp, char_t *path, char_t *query)
{
	char_t *dhen=NULL,*dips=NULL,*dipe=NULL,*dhlt=NULL,*go=NULL;
	go = websGetVar(wp, T("GO"), T("")); 

	dhen = websGetVar(wp,T("dhcpEn"),T("0"));
	dips = websGetVar(wp,T("dips"),T(""));
	dipe = websGetVar(wp,T("dipe"),T(""));
	dhlt = websGetVar(wp,T("DHLT"),T("3600"));

	set_dhcpd_en(atoi(dhen));

	_SET_VALUE(_LAN0_DHCPD_START,dips);
	_SET_VALUE(_LAN0_DHCPD_END,dipe);
	_SET_VALUE(_LAN0_DHCPD_LEASET,dhlt);

	_COMMIT();
	websRedirect(wp, T("/direct_reboot.asp"));	
	
	cyg_thread_delay(200);
	
	sys_reboot();
	
}

//lan_dhcp_clients.asp&list1='192.168.0.200;22:33:44:55:66:77'&list2='192.168.0.150;24:33:44:55:66:77'&IpMacEN=11&LISTLEN=2
static void fromDhcpListClient(webs_t wp, char_t *path, char_t *query)
{
	char_t	*lease_str=NULL, *listcnt=NULL;
	int i = 0, cnt;
	char strlist[32];	
	
	listcnt = websGetVar(wp, T("LISTLEN"), T("0")); 
	cnt = atoi(listcnt);

	for(i=1; i<=cnt; i++)
	{	
		memset(strlist, 0, sizeof(strlist));
		sprintf(strlist,"%s%d","list",i);
		lease_str = websGetVar(wp, T(strlist), T(""));
		if(lease_str == NULL || lease_str[0] == '\0' || strlen(lease_str) <5)
			break;
	
		_SET_VALUE(LAN0_DHCP_SATIC(i),lease_str);
	}

	for(i = cnt+1; i<= DHCPD_STATIC_LEASE_NU; i++){
		_SET_VALUE(LAN0_DHCP_SATIC(i),"");
	}
	
	_COMMIT();
	
	//sys_restart();
	add_dhcpd_static_lease();

	websRedirect(wp, T("/lan_dhcp_clients.asp"));
}

static void fromSysToolChangePwd(webs_t wp, char_t *path, char_t *query)
{
	char_t  /**user_id=NULL,*/ *old_pwd=NULL, /**new_user_id=NULL,*/ *new_pwd1=NULL,
		*new_pwd2=NULL , *go=NULL;
	//char val[512];

	go = websGetVar(wp, T("GO"), T("")); 

	//user_id = websGetVar(wp, T("SYSUN"), T("")); 

	old_pwd = websGetVar(wp, T("SYSOPS"), T("")); 

	//new_user_id= websGetVar(wp, T("SYSUN1"), T("")); 
	new_pwd1 = websGetVar(wp, T("SYSPS"), T("")); 
	new_pwd2 = websGetVar(wp, T("SYSPS2"), T("")); 

	if(strcmp(g_Pass,old_pwd)!=0) 
	{
		SetErrorArg("Password error!","system_password.asp");
		websRedirect(wp, T("error.asp"));
	} else
	{
		//_SET_VALUE(HTTP_USERNAME, user_id);
		_SET_VALUE(HTTP_PASSWD, new_pwd1);
		_COMMIT();
		//strncpy(g_User,user_id,sizeof(g_User));
		strncpy(g_Pass,new_pwd1,sizeof(g_Pass));

		websRedirect(wp,T("system_password.asp"));
	}
}

static void fromSysToolReboot(webs_t wp, char_t *path, char_t *query)
{
	sys_reboot();
}



/*check wpspin is reasonable*/

static void fromSysToolRestoreSet(webs_t wp, char_t *path, char_t *query)
{

	//�ָ���������
	char *v = NULL;
	_SET_VALUE(_RESTORE_DEFAULTS,"1");
	_SET_VALUE(_LAN0_IP,_GET_DEFAULT(_LAN0_IP,v));

	//A5 wan/lan�л�����308��316�Ƿ���Ӱ��?
#ifdef A5
	_SET_VALUE("vlan1ports", "1 2 3 4 5*");
	_SET_VALUE("vlan2ports", "0 5");
#endif 

	_COMMIT();
	
	websRedirect(wp, T("/direct_reboot.asp"));

	sys_reboot();
	
	
}
void  reset_button(void){
	//char *v = NULL;
	_SET_VALUE(_RESTORE_DEFAULTS,"1");

#ifdef A5
	_SET_VALUE("vlan1ports", "1 2 3 4 5*");
	_SET_VALUE("vlan2ports", "0 5");
#endif 
	//_SET_VALUE(_LAN0_IP,_GET_DEFAULT(_LAN0_IP,v));
	_COMMIT();
	
	diag_printf("%s\n", __FUNCTION__);
	sys_reboot();
	return;
}

static void fromSysToolSysLog(webs_t wp, char_t *path, char_t *query)
{
	char_t *go=NULL,*optType=NULL,*curpage=NULL;
	go = websGetVar(wp, T("GO"), T("")); 
	optType = websGetVar(wp,T("TYPE"),T(""));//0�������־��1���鿴��ǰҳ
	curpage = websGetVar(wp,T("curPage"),T(""));


	if(strcmp(optType,"0") == 0)
	{
		syslog_clear();
		logNum=0;
		gLog_cur_page = 1;
	}
	else if(strcmp(optType,"1") == 0)
	{
		int cur = atoi(curpage);
		if(0 == cur)
			cur = 1;
		
		gLog_cur_page = cur;
	}

	websRedirect(wp, T(go));
}

static void fromSysToolDDNS(webs_t wp, char_t *path, char_t *query)
{
	char_t  *go=NULL, *ddnsen=NULL, *userName=NULL, *password=NULL,
		*hostName=NULL, *serverName=NULL;
	char_t val[128];
	char_t *serName = NULL;
	
	go = websGetVar(wp, T("GO"), T("")); 

	ddnsen = websGetVar(wp, T("ddnsEnable"), T("0")); 
	userName = websGetVar(wp, T("userName"), T("")); 
	password = websGetVar(wp, T("password"), T("")); 
	hostName = websGetVar(wp, T("hostName"), T("")); 
	//0 = oray ; 1 = dyndns
	serverName = websGetVar(wp,T("serverName"),T("0"));

	memset(val,0,sizeof(val));
	if (atoi(ddnsen)) {
		//���ã�
		if (atoi(serverName) == 0) {
			//oray.net
			_SET_VALUE(_DDNS_ENABLE,"1");
			serName = ORAY_NAME;
		} else if (atoi(serverName) == 1) {
			//dyndns.org
			_SET_VALUE(_DDNS_ENABLE,"2");
			serName = DYNDNS_NAME;
		} else if (atoi(serverName) == 2) {
			//88ip.com
			_SET_VALUE(_DDNS_ENABLE,"3");
			serName = M88IP_NAME;
		} else if (atoi(serverName) == 3) {
			//freedns.afraid.org
			_SET_VALUE(_DDNS_ENABLE,"4");
			serName = "afraid";
		} else if (atoi(serverName) == 4) {
			//zoneedit.com 
			_SET_VALUE(_DDNS_ENABLE,"5");
			serName = ZOE_NAME;
		} else if (atoi(serverName) == 5) {
			//no-ip.com
			_SET_VALUE(_DDNS_ENABLE,"6");
			serName = NOIP_NAME;
		} else if (atoi(serverName) == 6) {
			//3322.org
			_SET_VALUE(_DDNS_ENABLE,"7");
			serName = QDNS_NAME;
		} else if (atoi(serverName) == 7) {
			//gnhostlinux
			_SET_VALUE(_DDNS_ENABLE,"8");
			serName = "gnhostlinux";
		} 
/* from ddns.htm*****
if(f.serverName.value =="oray")
	{
		setCfg("ddns1", "1;"+
					f.serverName.value+";"+
					f.hostName.value+";"+
					f.userName.value+";"+
					f.passWord.value+";"+
					";"+";"+";60");
	}
*/
		snprintf(val,sizeof(val),"%s;%s;%s;%s;%s;%s;%s;%s;%s",
			"1",serName,hostName,userName,password,"","","",
			/*strcmp(serName,ORAY_NAME)?"":*/"60"
		);
		//diag_printf("ddns:%s\n", val);
		
		_SET_VALUE(_DDNS_SET1,val);
		_SET_VALUE(_DDNS_HOST_NAME,hostName);
	} else {
		_SET_VALUE(_DDNS_ENABLE,"0");
	}

	_COMMIT();

	sys_restart();
	
	websRedirect(wp, T("/ddns_config.asp"));	
}

//192.168.5.0:255.255.255.0:192.168.5.1:1 192.168.6.0:255.255.255.0:192.168.6.1:1
static void fromRouteStatic(webs_t wp, char_t *path, char_t *query)
{
	char_t  *go, *str, *cmd;
	char *oldvalue;

	go = websGetVar(wp, T("GO"), T("")); 
	cmd = websGetVar(wp, T("cmd"), T(""));

	str= websGetVar(wp, T("wan0_route"), T("")); 

	_SAFE_GET_VALUE(_WAN0_ROUTE,oldvalue);

	//��������Ϊ��ɾ��LAN�ھ�̬·��
	_SET_VALUE(_LAN0_ROUTE,oldvalue);

	_SET_VALUE(_WAN0_ROUTE,str);

	_COMMIT();

	sys_restart2();

	websRedirect(wp, T("/routing_static.asp"));	
}

#if 0
int TWL300NWebsSecurityHandler(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg, 
						char_t *url, char_t *path, char_t *query)
{
	char_t			*userid=NULL, *password=NULL;
	//char_t			*wwsuserid=NULL, *wwspassword=NULL;
	char_t 			*value;
	
	char bReLogin;

	static clock_t save_security_time = 0;
	clock_t security_time_val;

	a_assert(websValid(wp));
	a_assert(url && *url);
	a_assert(path && *path);

	if(g_user_pass_Flag == 0)
	{
		memset(g_User,0,sizeof(g_User));
		memset(g_Pass,0,sizeof(g_Pass));

		strncpy( g_User,_SAFE_GET_VALUE(HTTP_USERNAME,value),sizeof(g_User));
		strncpy( g_Pass,_SAFE_GET_VALUE(HTTP_PASSWD,value),sizeof(g_Pass));

		g_user_pass_Flag = 1;
	}

	password = websGetRequestPassword(wp);
	userid = websGetRequestUserName(wp);

	security_time_val = time(0) - save_security_time;
	if (security_time_val > 2 * 60 * 100 )
		bReLogin = 1;
	else
		bReLogin = 0;

	if (userid == NULL)
	{//û�������û���
	//����������û���������
			goto ERROREXIT;
	} else
	{
		if ((gstrcmp(userid,g_User) != 0) || (bReLogin)  || (bForceLogout))
		{//�û�����һ��
			goto ERROREXIT;
		}
	}

	if (password == NULL)
	{//û�������û�����
		//������������룬����
		goto ERROREXIT;
	} else
	{
		if ((gstrcmp(password,g_Pass) != 0) || (bReLogin) || (bForceLogout))
		{//�û����벻һ��
			goto ERROREXIT;
		}
	}

	save_security_time = time(0);
	bForceLogout = 0;
	return 0;

ERROREXIT:
	save_security_time = time(0);
	bForceLogout = 0;
 

	websResponse(wp, 401, NULL, NULL);
	return 0;
}
#else
extern int dns_redirect_disable;


//asp �ļ�δʹ��
#define PUBLIC_FOLDER "/public"

const char cookie_suffix[COOKIE_SUFFIX_NUM][4] = {"1qw","ded","ert","fcv","vgf","nrg","xcb","jku","cvd","wdv","2dw","njk","ftb","efv","azx","tuo","cvb","bcx","eee","mfw",
"qdr","bhu","il0","3g7","xnh","7ht","23f","cmj","njf","3t8","ad9","zxq","tgb","nyj","ilo","tfh","cnd","wrx","421","vmy",
"bhy","dfj","rmx","skh","3yz","xwa","etf","cxn","mji","gkt","fva","qpn","xdw","oir","lkf","bde","tew","mnb","dex","ves",
"yjd","nmk","xcv","9hd","5gk","2fs","78v","03h","1mz","tcs","ula","rns","xow","alb","89v","344","7dw","bbf","ttt","opp"};

int TWL300NWebsSecurityByCookieHandler(webs_t wp, char_t *urlPrefix, char_t *webDir, int arg, 
						char_t *url, char_t *path, char_t *query)
{
	char_t *userid, *password;
	char *pfiletype = NULL;
	char urlbuf[256] = {0}, *purlbuf;
	char *pcookie = NULL;
	int i = 0;

	char *p_user_cookie, *p_cookie_tmp, *value;
	int k = 0;
	char user_cookie[512]={0};
	
	char lan_ip[20]={0};
	char wan_ip[20]={0};
	char redirect_login[64]={0}, wan_redirect_login[128]={0}, redirect_index[64]={0}, wan_redirect_index[128] = {0};
	char default_Pwd[64];
	char http_true_passwd[256] = {0};

	a_assert(websValid(wp));
	a_assert(url && *url);
	a_assert(path && *path);
	//���ӹ���������֤	
	/*  QQ�����6.0���������⣬/favicon.ico Ҳ��Ҫ������֤ (add by chenwb,2012-1-11)  */
	if(strncmp(url,PUBLIC_FOLDER,strlen(PUBLIC_FOLDER))==0
		|| strcmp(url,"/favicon.ico")==0 || wp->host_str == NULL) 
		return 0;

	if(g_user_pass_Flag == 0)
	{
		memset(g_User,0,sizeof(g_User));
		memset(g_Pass,0,sizeof(g_Pass));

		strncpy(g_User,_SAFE_GET_VALUE(HTTP_USERNAME,value),sizeof(g_User));
		strncpy(g_Pass,_SAFE_GET_VALUE(HTTP_PASSWD,value),sizeof(g_Pass));

		g_user_pass_Flag = 1;
	}
	strncpy(default_Pwd,_SAFE_GET_VALUE(HTTP_DEFAULTPWD,value),sizeof(default_Pwd));
	strncpy(lan_ip,NSTR(SYS_lan_ip),sizeof(lan_ip));
	strncpy(wan_ip,NSTR(SYS_wan_ip),sizeof(wan_ip));

	snprintf(redirect_login,sizeof(redirect_login),"http://%s/login.asp",lan_ip);
	snprintf(redirect_index,sizeof(redirect_index),"http://%s/index.asp",lan_ip);
	snprintf(wan_redirect_login,sizeof(wan_redirect_login),"http://%s/login.asp",wp->host_str);
	snprintf(wan_redirect_index,sizeof(wan_redirect_index),"http://%s/index.asp",wp->host_str);
	
		
	memcpy(urlbuf, url, 255);
	if ((purlbuf = strchr(urlbuf, '?')) != NULL)
		*purlbuf = '\0';
#if 0
	if(wp->cookie)
	{
		k = 0;
		p_cookie_tmp = wp->cookie;
		while( k<32 && *p_cookie_tmp && *p_cookie_tmp!=':' )
		{	 
			user_cookie[k] = *p_cookie_tmp;
			if(user_cookie[k] == 0x20)
				k = 0;
			else
				k++;
			p_cookie_tmp++;
		}
		user_cookie[k] = '\0' ;
		pcookie = user_cookie;
	}
#else
	
	if(wp->cookie)
	{
		p_user_cookie = wp->cookie;
		while ((p_cookie_tmp = strstr(p_user_cookie,"ecos_pw="))!=NULL)
		{
			pcookie = p_cookie_tmp + strlen("ecos_pw=");
			p_user_cookie = pcookie;
		}
	}
#endif

	if (strlen(urlbuf)>3)
	{
		pfiletype = strchr(urlbuf, '.');
		if (pfiletype)
		{
			pfiletype++;
			if (!memcmp(pfiletype, "gif", 3)||!memcmp(pfiletype, "js", 2)||!memcmp(pfiletype, "css", 3)||!memcmp(pfiletype, "png", 3) ||!memcmp(pfiletype, "jpg", 3))
				return 0;
		}
	}

	/*
    	    	������COOKIE��֤����,������ҳ���޹�
      		��������ԭʼ����һ��( strcmp(pwd,"admin" ) == 0)��
      		������COOKIE��֤
    	*/
    if ( g_Pass!=NULL && (strcmp(g_Pass,default_Pwd) == 0) && wp->url!=NULL && (strstr(wp->url,"/goform/ate")!=NULL) )
    	 return 0;

	for (i=0; i<MAX_USER_NUM; i++)
	{
		if(!strcmp(loginUserInfo[i].ip, wp->ipaddr)) 
		{
			//loginUserInfo[i].ip�Ѿ���¼��.
			break;
		}
	}
	if(i<MAX_USER_NUM)
	{
		//������
		if(strcmp(g_Pass, "") == 0)
		{
			if((!memcmp(urlbuf, "/login.asp", 10) || (strlen(urlbuf)==1&&urlbuf[0]=='/')) || (!memcmp(urlbuf, "/LoginCheck", 11)))
			{
				goto LOGINOK;
			} 
			else
			{
				loginUserInfo[i].time = (unsigned int)cyg_current_time();
				return 0;
			}
		}
		
		//�Ѿ���¼��.��δ��ʱ

		//IE�����cookieд���쳣����
		userid = websGetVar(wp, T("Username"), T(""));
		
		if ( wp->ipaddr!=NULL)
		{
			sprintf(http_true_passwd,"%s%s",g_Pass,cookie_suffix[((unsigned int)inet_addr(wp->ipaddr))%COOKIE_SUFFIX_NUM]);
		}
		else
		{
			sprintf(http_true_passwd,"%s",g_Pass);
		}
		
		if(pcookie && !strncmp(pcookie, http_true_passwd,strlen(http_true_passwd)))
		{
			if(strlen(urlbuf)!=0 && !memcmp(urlbuf, "/login.asp", 10) || (strlen(urlbuf)==1 && urlbuf[0]=='/') || strlen(urlbuf)!=0 && !memcmp(urlbuf, "/LoginCheck", 11))
			{
				goto LOGINOK;
			} 
			else
			{
				loginUserInfo[i].time = (unsigned int)cyg_current_time();
				return 0;
			}	
			
		}
		else  //pcookie����ʱ
	    {
			if(strstr(wp->url,"login")!=NULL || strstr(wp->url,"lang")!=NULL || !memcmp(urlbuf, "/LoginCheck", 11))
			{
				goto RELOGIN;
			}
			else{
				if(wp->host_str!=NULL && strlen(lan_ip)!=0 && strncmp(wp->host_str,lan_ip,strlen(lan_ip))==0){
					websResponse(wp,302,NULL,T(redirect_login));
				}
				else{
					websResponse(wp,302,NULL,T(wan_redirect_login));
				}
				loginUserInfo[i].time = (unsigned int)cyg_current_time();
				return 0;
			}
		}
		
		
	} 
	else
	{
		if(strstr(wp->url,".asp") != NULL 
	   || strstr(wp->url,".php") != NULL 
       || strstr(wp->url,".htm") != NULL 
       || strstr(wp->url,".html") != NULL 
       || strstr(wp->url,"goform") != NULL 
       || strstr(wp->url,"cgi-bin") != NULL
       || strstr(wp->url,"userRpm") != NULL)
		{
			if(strcmp(g_Pass, "") == 0)
			{
				if ((!memcmp(urlbuf, "/error.asp", 10)) || (!memcmp(urlbuf, "/index.asp", 10)))
					return 0;
				
				goto NOPASSWORD;
			}
			
			if ( strncmp(wp->host_str,lan_ip,strlen(lan_ip))!=0 && strncmp(wp->host_str,wan_ip,strlen(wan_ip))!=0 )
			{
				//printf("----%d----redirect_login\n", __LINE__);
				if(2  !=  get_wan_connstatus())
				{
					websResponse(wp,302,NULL,T(redirect_login));
					return 0;
				}
			}
		}
		
RELOGIN:
		//δ��¼
		if (!memcmp(urlbuf, "/lang", 5))
		{
			return 0;
		}
		else if (!memcmp(urlbuf, "/login.asp", 10))
		{
		#if 0
			if(!wp->cookie)
			{
				if(2  ==  get_wan_connstatus())
				{
					websResponseWithCookie(wp,302,NULL,T(wan_redirect_login),0);
				}
				else
				{
					websResponseWithCookie(wp,302,NULL,T(redirect_login),0);
				}
			}
		#endif
			return 0;
		}
		else if (strlen(urlbuf)==1&&urlbuf[0]=='/')
			goto ERROREXIT;
		else if (!memcmp(urlbuf, "/error.asp", 10))
			return 0;
		else if (!memcmp(urlbuf, "/LoginCheck", 11))
		{
			password = websGetVar(wp, T("Password"), T(""));
			userid = websGetVar(wp, T("Username"), T(""));
			if (userid == NULL||password == NULL)
			{
				goto ERROREXIT;
			}
			else
			{
			
				if ((gstrcmp(userid, g_User)==0) && (gstrcmp(password, g_Pass) == 0))
				{
					for(i=0; i<MAX_USER_NUM; i++)
					{
						if (strlen(loginUserInfo[i].ip) == 0)
						{
							memcpy(loginUserInfo[i].ip , wp->ipaddr, IP_SIZE);		//����û�		
							
							loginUserInfo[i].time = (unsigned int)cyg_current_time();
							goto LOGINOK;
							break;
							
						}	
					}
					if(i == MAX_USER_NUM) //�û�������
					{
						SetErrorArg(LOGIN_USERS_FULL,"login.asp");
						websRedirect(wp, T("error.asp"));
						
						return 0;
					}

				}else
				{
					
					goto LOGINERR;
				}		
			}
		}
		else{
			goto ERROREXIT;
		}
	}
	return 0;
	
ERROREXIT:
	websRedirect(wp, T("login.asp")); 
	return 0;

LOGINOK:
	if(2  ==  get_wan_connstatus())
	{	
		websResponseWithCookie(wp,302,NULL,T(wan_redirect_index), 1);
	}
	else
	{
		websResponseWithCookie(wp,302,NULL,T(redirect_index),1);
	}
	return 0;
	
LOGINERR:
	websRedirect(wp, T("login.asp?0"));
	return 0;
	
NOPASSWORD:
	//������
	if(strcmp(g_Pass, "") == 0)
	{
		for(i=0; i<MAX_USER_NUM; i++)
		{
			if (strlen(loginUserInfo[i].ip) == 0)
			{
				memcpy(loginUserInfo[i].ip , wp->ipaddr, IP_SIZE);		//����û�						
				loginUserInfo[i].time = (unsigned int)cyg_current_time();

				if((!memcmp(urlbuf, "/login.asp", 10) || (strlen(urlbuf)==1&&urlbuf[0]=='/')) || (!memcmp(urlbuf, "/LoginCheck", 11)))
				{
					goto LOGINOK;
				} 
				else
				{
					loginUserInfo[i].time = (unsigned int)cyg_current_time();
					return 0;
				}	

				break;
				
			}	
		}

		if(i == MAX_USER_NUM) //�û�������
		{
			if((!memcmp(urlbuf, "/login.asp", 10) || (strlen(urlbuf)==1&&urlbuf[0]=='/')) || (!memcmp(urlbuf, "/LoginCheck", 11)))
			{
				websRedirect(wp, T("index.asp"));
			}
			else if(memcmp(urlbuf, "/index.asp", 10))
			{
				SetErrorArg(INDEX_USERS_FULL,"index.asp");
				websRedirect(wp, T("error.asp"));	
			}
		}
		return 0;
	}
}
#endif


typedef uint16 chanspec_t;
#define WLC_UP					2
#define WLC_DOWN				3
#define BCME_OK				0	/* Success */
#define WLC_GET_UP				162

int ate_is_doing = 0;	//add flag for tenda_v3
extern void mfg_start(void);
static void 	fromSysAted(webs_t wp, char_t *path, char_t *query)
{
	mfg_start();
	ate_is_doing = 1;
	websWrite(wp,T("load mfg success."));
	websDone(wp,200);
}

typedef struct
{
	uint8 ant_config[4];	/* antenna configuration */
	uint8 num_antcfg;	/* number of available antenna configurations */
} wlc_antselcfg_t;

//����ģ��
//����ģ��ҳ��goform/ate����Ҫ���ƹ�COOKIE��֤������TWL300NWebsSecurityHandler()������
//�����ļ�����ʵ�ֶ�bootloader�Ĵ�ȡ��ֱ������

#define SUCCESS  0
#define ERROR    -1
#define INFO     1
extern int envram_get(int argc, char* argv[]);
extern int envram_set(int argc, char* argv[]);
extern int envram_commit(int argc, char* argv[]);

static int mfg_set_value(const char *name, const char *value) 		//�ں˲�������ֵ�ӿڣ�//�ɹ����� 0 == SUCCESS,ʧ�ܷ��� -1 == ERROR
{	
    if( nvram_set(name, value) == 0 )
	{
		return SUCCESS;
	}
	return ERROR;
}
static char *mfg_get_value(char *name) 	        //�ں˲�����ȡֵ�ӿڣ��ɹ�value����ֵ
{
    return nvram_safe_get(name);
}
static int mfg_commit() 		//�ύ���ں�	�ɹ����� 0 == SUCCESS,ʧ�ܷ��� -1 == ERROR
{
    if( nvram_commit()==0 )//_COMMIT();
	{   
	    return SUCCESS;
	}
    return ERROR;
}
static void mfg_html_info(char *buf) 	//ҳ����Ϣ
{
    /*��˳������ƴ�Ӻ󷵻�����a���м��ԡ�������������������ֵ������дĬ��ֵ0
         HV=V1.0��SV=V1.0CN��LANMAC=C8:3A:35:00:00:20;WANMAC=C8:3A:35:00:00:21;LANGUAGEINFO=�ڴ����߱�Яʽ·����;
         SSID=Tenda_111;DATA=1000,2000��(dateΪADSL����������)
    */
      char *hv,*lanmac,*wanmac,*language,*ssid,*pin,*date;
	  hv = W311R_ECOS_HV;
      lanmac = mfg_get_value("et0macaddr");
	  wanmac = mfg_get_value("wan0_hwaddr");
	  ssid = mfg_get_value("wl_ssid");
	  pin = mfg_get_value("wps_device_pin");  
	  language = "";
	  date = "";
      sprintf(buf,"HV=%s;SV=%s_%s;LANMAC=%s;WANMAC=%s;LANGUAGEINFO=%s;SSID=%s;PIN=%s;DATA=%s",
	  	hv,W311R_ECOS_SV,__CONFIG_WEB_VERSION__,lanmac,wanmac,language,ssid,pin,date);
	  printf("buf = %s\n",buf);
	  return;
}
static void mfg_reboot() 	      //�ɹ����� 0 == SUCCESS,ʧ�ܷ��� -1 == ERROR
{
   sys_reboot();	
}

static int  wl_wps_pin_check(char *pin_string)
{
    unsigned long pin = strtoul(pin_string, NULL, 10);
    unsigned long int accum = 0;
    unsigned int len = strlen(pin_string);
    if (len != 4 && len != 8)
        return 	ERROR;
    if (len == 8)
    {
        accum += 3 * ((pin / 10000000) % 10);
        accum += 1 * ((pin / 1000000) % 10);
        accum += 3 * ((pin / 100000) % 10);
        accum += 1 * ((pin / 10000) % 10);
        accum += 3 * ((pin / 1000) % 10);
        accum += 1 * ((pin / 100) % 10);
        accum += 3 * ((pin / 10) % 10);
        accum += 1 * ((pin / 1) % 10);
        if (0 == (accum % 10))
            return SUCCESS;
    }
    else if (len == 4)
        return SUCCESS;
    return ERROR;
}//end PIN check


#define EFFECT_LEVEL 0 
#define WPS_RESET_BUTTON_GPIO 20

static int ate_button_poll()
{
	unsigned long btn_mask;	
	unsigned long value;
	unsigned int utimes = 0;
	int flag = 0;
	int gpio = 0;	

	for(utimes = 0; utimes < 2; utimes++)
	{	
		gpio = WPS_RESET_BUTTON_GPIO;
		bcmgpio_connect(gpio, 0);
		btn_mask = ((unsigned long)1 << gpio);		
		bcmgpio_in(btn_mask, &value);		
		value >>= gpio;			

		/* Other gpio Here. */

		if(value == EFFECT_LEVEL)
		{
			flag = 1;
			printf("\n---Button pressed!!!\n\n");	

			printf("ledtest  off \n");
		
            set_all_led_off();
			cyg_thread_delay(20); // 0.2 s
	
			printf("ledtest  on \n");
			
			set_all_led_on();	
			break;
		}
		cyg_thread_delay(10); // 0.1 s
	}	

	return 0;
}
void mfg_mainloop(void)
{
    int mfgfd;
    int ilen;
	struct sockaddr_in local;
	struct sockaddr remote;
    char recvbuf[256], sendbuf[256];
	int			argc;
	char *argv[32];
	//char *macaddr1=NULL;
	char *bootwait=NULL;
    char cmdbuf[256];
	
    struct ether_addr *hwaddr;
    unsigned char macaddr[32];
	fd_set rfds;
	struct timeval tv={2,0};
       int retval;
    int mfgflag;

    memset( &local, 0, sizeof(local) );         //��յ�ַ�ṹ
    local.sin_family = AF_INET;                 //��ַ����ΪAF_INET��IPV4��
    local.sin_port = htons(7329);               //�˿�7329
    local.sin_addr.s_addr = SYS_lan_ip;         //ȡ��̨LAN��IP

    mfgfd = socket( AF_INET, SOCK_DGRAM, 0 );	//ʹ�ú���socket()�������׽����ļ�������
    if ( mfgfd < 0 )                           //��ʼ�����
    {
        diag_printf("MfgThread socket error.\n");
        return ;
    }
    if (bind(mfgfd, (struct sockaddr *)&local, sizeof(local)) < 0)  	 //�󶨵�ַ���
    {
        diag_printf("MfgThread bind error.\n");
		close(mfgfd);//hqw add for tcp 2014.01.24
        return;
    }
    printf("MfgThread start loop.\n");
	

    while (1)
    {
        FD_ZERO(&rfds);
        FD_SET(mfgfd, &rfds);
        tv.tv_sec = 0;
        tv.tv_usec = 200 *1000; //0.2 second
        ate_button_poll();
		
        retval = select(mfgfd + 1, &rfds, NULL, NULL, &tv);

        if (retval <= 0 || !FD_ISSET(mfgfd, &rfds))
            continue;

        memset(recvbuf,0,sizeof(recvbuf));   //���
        ilen = sizeof(remote);
        ilen = recvfrom(mfgfd, recvbuf, sizeof(recvbuf), 0, &remote, &ilen); //&sizeof(remote)�Ƿ���ȷ

        if (ilen < 10)			    //������� Tenda_mfg reboot�ų��ո��ĳ���
            continue;
        if (ilen == 256 )
            recvbuf[255] = 0;

        printf("MfgThread recv %d[%s]\n",ilen,recvbuf);
		 
        memset(argv, 0, sizeof(argv));
        argc = get_argv(recvbuf, argv);

        mfgflag = ERROR;
	
        switch (argc)
        {
        case 2://Tenda_mfg
			if ( strcmp(argv[0], "Tenda_mfg")==0 )
			{
				if( strcmp(argv[1], "reboot")==0 )
	            {
	                 strcpy(sendbuf, "success");
	                 ilen= strlen(sendbuf);
	                 sendto(mfgfd, (char *)sendbuf, ilen, 0, (struct sockaddr *)&remote, sizeof(remote));
                     mfg_reboot();
	            }
	            else if( strcmp(argv[1], "default")==0 )
	            {			
					mfg_set_value(_RESTORE_DEFAULTS, "1");
					mfg_set_value(_LAN0_IP,mfg_get_value(_LAN0_IP));					
#ifdef A5
					mfg_set_value("vlan1ports", "1 2 3 4 5*");
					mfg_set_value("vlan2ports", "0 5");
#endif		
					mfgflag = mfg_commit(); 
                    printf("default = %s ,mfgflag = %d\n",mfg_get_value(_RESTORE_DEFAULTS),mfgflag);
					sys_reboot();

	            }
	            else if ( strcmp(argv[1], "htmlVersionInfo")==0 )
	            {
                     mfg_html_info(cmdbuf);
                     mfgflag = INFO;
	            }
				
			}
                //start: add by z10312  ���� ���ⰴ������&LED�Ʋ��� �ӿ�2015-01-27
                else if(strcmp(argv[0], "ledtest")==0)
                {
                    if(strcmp(argv[1], "on")==0)
                    {
                        set_all_led_on();
                        mfgflag = SUCCESS;
                    }else if(strcmp(argv[1], "off")==0)
                    {
                        set_all_led_off();
                        mfgflag = SUCCESS;
                    }
                }
                //end: add by z10312  ���� ���ⰴ������&LED�Ʋ��� �ӿ�2015-01-27
            break;
        case 3://wlctrl
        	//Tenda_mfg check ResetButton
        	if (0 == strcmp(argv[0], "Tenda_mfg"))
        	{
        		if (0 == strcmp(argv[1], "Check")) 
        		{
					if(0 == strcmp(argv[2], "ResetButton"))
	        		{
						printf("Check ResetButton Now---------\n");
						mfgflag = ERROR;
						if(ate_check_button(20))
						{
							printf("ResetButton test OK\n");
							mfgflag = SUCCESS;
						}
						break;
	        		}
					else if(0 == strcmp(argv[2], "WPSButton"))
	        		{
	        			printf("Check WPSButton Now---------\n");
						mfgflag = ERROR;
						if(ate_check_button(20))
						{
							printf("WPSButton test OK\n");
							mfgflag = SUCCESS;
						}
						break;
	        		}
        		}
        	}
            else if (strcmp(argv[0], "wlctrl") == 0)
            {
                if (strcmp(argv[1], "set_ant") == 0)
                {
                    int nget1, nget2;
                    int nset;
                    int nreturn;

                    wl_set("eth1", WLC_DOWN, &nset, sizeof(nset));
                    wl_get("eth1", WLC_GET_UP, &nget1, sizeof(nget1));
                    if (nget1 != 0)
                    {
                        printf("wl status is not [down] status,return false!!!\n");
                        mfgflag = ERROR;
                        break;
                    }
					
                    if (strcmp(argv[2], "0") == 0 || strcmp(argv[2], "1") == 0)
                    {
                        nset = atoi(argv[2]) + 1;

                        wl_iovar_setint("eth1", "txchain", nset);
                        wl_iovar_setint("eth1", "rxchain", nset);

                        wl_iovar_getint("eth1", "rxchain", &nget1);
                        wl_iovar_getint("eth1", "txchain", &nget2);
                        if ( nget1 == nset && nget2 == nset )
                            mfgflag = SUCCESS;
                        else
                            mfgflag = ERROR;

                        printf("wl txchain get:%d  rxchain get:%d,mfgflag value:%d\n", nget1, nget2, mfgflag);
                    }
#ifdef __CONFIG_UTILS__
                    else if (strcmp(argv[2], "2") == 0)
                    {
                        wlc_antselcfg_t val = {{0}, 0};
                        val.ant_config[0] = (uint8)strtol("0x12", NULL, 0);
                        val.ant_config[1] = (uint8)strtol("0x12", NULL, 0);
                        val.ant_config[2] = (uint8)strtol("0x12", NULL, 0);
                        val.ant_config[3] = (uint8)strtol("0x12", NULL, 0);
                        nreturn= wlu_iovar_set("eth1", "phy_antsel", &val, sizeof(wlc_antselcfg_t));
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        nreturn = wl_iovar_setint("eth1", "txchain", 1);
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        nreturn = wl_iovar_setint("eth1", "rxchain", 1);
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        else
                            mfgflag = SUCCESS;
                        printf("nreturn = %d, mfgflag = %d\n", nreturn, mfgflag );
                    }
                    else if (strcmp(argv[2], "3") == 0)
                    {
                        nreturn = wl_iovar_setint("eth1", "txchain", 2);
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        nreturn = wl_iovar_setint("eth1", "rxchain", 2);
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        wlc_antselcfg_t val = {{0}, 0};
                        val.ant_config[0] = (uint8)strtol("0x20", NULL, 0);
                        val.ant_config[1] = (uint8)strtol("0x20", NULL, 0);
                        val.ant_config[2] = (uint8)strtol("0x20", NULL, 0);
                        val.ant_config[3] = (uint8)strtol("0x20", NULL, 0);
                        nreturn = wlu_iovar_set("eth1", "phy_antsel", &val, sizeof(wlc_antselcfg_t));
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        nreturn = wl_iovar_setint("eth1", "txchain", 2);
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        nreturn = wl_iovar_setint("eth1", "rxchain", 2);
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        else
                            mfgflag = SUCCESS;
                        printf("nreturn = %d, mfgflag = %d\n", nreturn, mfgflag );
                    }
#endif
                    else if (strcmp(argv[2], "4") == 0)
                    {
                        nreturn = wl_iovar_setint("eth1", "txchain", 3);
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        nreturn = wl_iovar_setint("eth1", "rxchain", 3);
                        if (nreturn != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        if (nreturn == 0)
                        {
                            mfgflag = SUCCESS;
                        }
                        else
                        {
                            mfgflag = ERROR;
                            break;
                        }
                        printf("wl txchain get:3 rxchain get:3,mfgflag value:%d\n", mfgflag );
                    }
					
			wl_set("eth1", WLC_UP, &nset, sizeof(nset));
	                wl_get("eth1", WLC_GET_UP, &nget1, sizeof(nget1));
	                if ( nget1 != 1)
	                {
	                    printf("wl status is not [up] status,return false!!!\n");
	                    mfgflag = ERROR;
	                    break;
	                }
				} //end set_ant
				else if (strcmp(argv[1], "set_channel") == 0)
                {    
                     mfg_set_value("wl0_channel", argv[2]);   //�޸�ҳ��channel;				 
					 mfg_set_value("wl_channel", argv[2]);    //�޸ĺ�̨channel;
					 if( atoi(argv[2]) < 5 )                  //���UI�ŵ���ʵ����������
                     {
				          mfg_set_value("wl_nctrlsb", "lower");
						  mfg_set_value("wl0_nctrlsb", "lower");
                     }
			         else 
					 {
				          mfg_set_value("wl_nctrlsb", "upper");
						  mfg_set_value("wl0_nctrlsb", "upper");
			         }
			  
					 mfg_commit();
					 cyg_thread_delay(100);               
                     strcpy(argv[1],mfg_get_value("wl0_channel"));	
                     if (strcmp(argv[1],argv[2]) == 0)
                     {                           
                         mfgflag = SUCCESS;
						 printf("argv1channel = %s,mfgflag = %d\n",argv[1],mfgflag);
						 sys_restart();						
                     }
                     else
                     {                       
                        mfgflag = ERROR;
						break;
                     }  
					 
                }//end set_channel             
            }//end wlctrl
            break;

        case 4://nvram set && Tenda_mfg check USB

            memset(cmdbuf,0,sizeof(cmdbuf));

            if (strcmp(argv[0], "nvram") == 0 && strcmp(argv[1], "set") == 0)
            {
                if (strcmp(argv[2], "PIN") == 0)
                {
                    char wps_pin[16] = {0};
                    if (wl_wps_pin_check(argv[3]) == 0)
                    {
                        sprintf(cmdbuf, "wps_device_pin=%s", argv[3]);//��argv[2]����cmdbuf
                        argv[2] = cmdbuf;     
                        mfgflag = envram_set(3, argv);                  //set����argv[2]��cmdbuf��ʽ�Ⱥ������ұ�����ֵ 
                        mfgflag = envram_commit(0, NULL);
						if(mfgflag != SUCCESS)
						{
							break;
						}
						
                        cyg_thread_delay(200);               
					
                        mfg_set_value("wps_device_pin",argv[3]);					
                        mfg_commit();
		
                        strcpy(wps_pin, mfg_get_value("wps_device_pin"));
                        if (strcmp(wps_pin, argv[3]) == 0)
                        {                          
                            mfgflag = SUCCESS;
							printf("mfg_get : wps_pin = %s,mfgflag = %d\n",wps_pin,mfgflag);
                        }
                        else
                        {   
                            mfgflag = ERROR;
                        }
			
                    }//end PINcheck
                }//end PIN
                else if (strcmp(argv[2], "MAC") == 0)
                { 
                    hwaddr = ether_aton(argv[3]);
                    if (hwaddr)
                    {
                        //�������MAC,ȷ��MAC��ʽΪXX:XX:XX:XX:XX:XX
                     
                        memset(macaddr, 0, sizeof(macaddr));
                        snprintf(macaddr, sizeof(macaddr), "%02X:%02X:%02X:%02X:%02X:%02X",
                                 hwaddr->octet[0] & 0XFF,
                                 hwaddr->octet[1] & 0XFF,
                                 hwaddr->octet[2] & 0XFF,
                                 hwaddr->octet[3] & 0XFF,
                                 hwaddr->octet[4] & 0XFF,
                                 hwaddr->octet[5] & 0XFF);

                        sprintf(cmdbuf, "et0macaddr=%s", macaddr);
                        argv[2] = cmdbuf;
                        mfgflag = envram_set(3, argv);
						if(mfgflag != SUCCESS) 
						{
                           printf("set et0macaddr error\n");
						   break;
						}

                        sprintf(cmdbuf, "sb/1/macaddr=%s", macaddr);
                        argv[2] = cmdbuf;
                        mfgflag = envram_set(3, argv);
						if(mfgflag != SUCCESS) 
						{
                           printf("set sb/1/macaddr error\n");
						   break;
						}
                        mfgflag = envram_commit(0, NULL);
						if(mfgflag != SUCCESS) 
						{
                           printf("commit error\n");
						   break;
						}
						
                        cyg_thread_delay(50);
						
                        macaddr[17] = '\0';
                        //�ж�bootloader��������MAC��ַ�Ƿ�д����ȷ			
                        sprintf(cmdbuf, "%s", "sb/1/macaddr");
                        argv[2] = cmdbuf;
                        envram_get(3, argv);
                        if (strcmp(argv[2], macaddr) != 0)
                        {   
                            mfgflag = ERROR;
                            break;
                        }
                        sprintf(cmdbuf, "%s", "et0macaddr");
                        argv[2] = cmdbuf;
                        envram_get(3, argv);
                        if (strcmp(argv[2], macaddr) != 0)
                        {   
                            mfgflag = ERROR;
                            break;
                        }
						
						mfgflag = SUCCESS;

                    }//end hwaddr
                    if (mfgflag == SUCCESS)  	//���MAC��ַ�޸ĳɹ�������Ҫ�ָ�����ֵ��ȷ��SSID�ܸ���Ϊ�µ�MAC�ĺ���λ
                    {
                        mfg_set_value(_RESTORE_DEFAULTS, "1");
#ifdef A5
                        mfg_set_value("vlan1ports", "1 2 3 4 5*");
                        mfg_set_value("vlan2ports", "0 5");
#endif
                        mfg_set_value("sb/1/macaddr", macaddr);
                        mfg_set_value("et0macaddr", macaddr);    
					    mfg_commit();
                    }
                }//end MAC
                else if (strcmp(argv[2], "PLC_PW") == 0)
                {
                    //
                    //PLC:�޸�����(Ŀǰֻ��PW201A)
                    //nvram set PLC_PW OSPM-ENPK-EQXH-FADA
                    //
                }//end PLC_PW
                else if (strcmp(argv[2], "WAN_MODE") == 0)
                {
                    //
                    //ģʽ�л�������WAN������WAN��3GWANģʽ�л��ӿ� 3G611R 3G622R�ȣ�
                    //nvram set Wan_Mode WAN/3GWAN/WireWAN
                    //
                }//end WAN_MODE
                else if (strcmp(argv[2], "SSID") == 0)
                {
                    char ssid[64]={0};							
                    mfg_set_value("wl0_ssid",argv[3]);
					mfg_set_value("wl_ssid",argv[3]);
                    mfg_commit();
					
                    strcpy(ssid, mfg_get_value("wl0_ssid"));//ҳ��SSID
					strcpy(ssid, mfg_get_value("wl_ssid"));//��̨SSID
                    if (strcmp(ssid,argv[3]) == 0)
                    {                           
                        mfgflag = SUCCESS;
                    }
                    else
                    {                       
                        mfgflag = ERROR;
                    }                 
                }//end SSID
                else if (strcmp(argv[2], "boot_wait") == 0)
                {
                    if (strcmp(argv[3], "on") == 0 || strcmp(argv[3], "off") == 0)
                    {
                        sprintf(cmdbuf, "boot_wait=%s", argv[3]);
                        argv[2] = cmdbuf;
                        envram_set(3, argv);
                        envram_commit(0, NULL);

                        //�ж�bootloader��������"boot_wait"�Ƿ�д����ȷ
						sprintf(cmdbuf,"%s","boot_wait");
						argv[2] = cmdbuf;                        			
                        envram_get(3, argv);
                        if (strcmp(argv[2], argv[3]) != 0)
                        {
                            mfgflag = ERROR;
                            break;
                        }
						
                        mfg_set_value("boot_wait", argv[3]);
                        mfg_commit();			
						
                        //�ж��ں˲�������"boot_wait"�Ƿ�д����ȷ
                        bootwait = mfg_get_value("boot_wait");
                        if (strcmp(bootwait, argv[3]) == 0)
                        {   
                            mfgflag = SUCCESS;
							printf("bootwait = %s,mfgflag = %d\n",bootwait,mfgflag);
                        }
                        else
                        {
                            mfgflag = ERROR;
                        }                      
                    }//end on/off
                    else if (strcmp(argv[3], "lan") == 0)
                    {
                        // add for A5 V1
                        mfg_set_value("vlan1ports", "0 1 2 3 4 5*");
                        mfg_set_value("vlan2ports", "5");
                        mfg_commit();
                        mfgflag = SUCCESS;
                    }//end lan
                    else if (strcmp(argv[3], "wan") == 0)
                    {
                        mfg_set_value("vlan1ports", "1 2 3 4 5*");
                        mfg_set_value("vlan2ports", "0 5");
                        mfg_commit();
                        mfgflag = SUCCESS;                      
                    } //end wan
                }//en boot_wait
                else if (strcmp(argv[2], "WPS") == 0)
                {
                    if (strcmp(argv[3],"off") == 0)
                    {    
                         char wps[16] = {0};
                         mfg_set_value("lan_wps_oob", "enabled");
						 mfg_set_value("wl_wps_mode", "disabled");
                         mfg_set_value("wl0_wps_mode", "disabled");
                         mfg_set_value("wl_wps_method", "");
                         mfg_commit();
						 strcpy(wps,mfg_get_value("wl0_wps_mode"));
                         if (strcmp(wps,"disabled") == 0)
                         {
                             mfgflag = SUCCESS;
							 printf("wps = %s,mfgflag=%d\n",wps,mfgflag);
						 }
						 else
                         {
                            mfgflag = ERROR;
                         }                    
                    }
                }//end WPS
            }//end nvram set    
            else if (strcmp(argv[0], "Tenda_mfg") == 0 && strcmp(argv[1], "check" )== 0&& strcmp(argv[2], "USB") == 0)
            {
                  //
                  //USB���ӿ�
                  //Tenda_mfg check USB Test.txt
                  //        
            }           
        break;

    default:
            mfgflag = ERROR;

        }//end switch
    if (mfgflag == SUCCESS)
    {
        //sucess
        strcpy(sendbuf, "success");
    }
    else if (mfgflag == INFO)
    {    
         //htmlVersionInfo
         strcpy(sendbuf, cmdbuf);
    }
	else
    {
         //error
          strcpy(sendbuf, "error");
		  printf("�����ˡ�������");
    }
    ilen = strlen(sendbuf);
    sendto(mfgfd, (char *)sendbuf, ilen, 0, (struct sockaddr *)&remote, sizeof(remote));

    continue;

    }//end while


    return;

}//end main

static void set_oob_status(char *ssid, char *pwd)
{
	char *value = nvram_get("default_ssid");	
	int ischange = 0;
	
	if(ssid == NULL || pwd == NULL)
		return;

	if(value && strcmp(value, ssid) != 0)
		ischange = 1;
	
	if(strcmp(pwd, "") != 0)
		ischange = 1;

	/*
	 * if ssid has been changed or wlpwd has been set
	 */
	if(ischange){
		nvram_set(WLN0_WPS_OOB, "disabled");
	}

	return;
}
