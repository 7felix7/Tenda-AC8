
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <kautoconf.h>//in bsp/include
#include <router_net.h>
#include <netinet/if_ether.h>

#include "flash_cgi.h"
#include "webs.h"
#include "uemf.h"
#include "bcmwifi.h"
#include "cJSON.h"

#include "pi_common.h"
#include "sys_module.h"
#include "rc_module.h"
#include <wl_utility_rltk.h>//Realtek����������ȡ��Ϣʹ��

#include "cgi_common.h"
#include "sys_module.h"
#include "wifi.h"



#ifdef __CONFIG_SUPPORT_GB2312__
//hong add for full-width character
#include "cgi_encode.h"
extern WIFI_STA_LIST_SSID_ENCODE_ARRAY encode_gb_list[WDS_SAN_LIST_LENGTH];
#endif

static int need_prompt_wlpwd = 1;

#define IFVALUE(value)	 if(value==NULL)	\
							value=""
#ifdef __CONFIG_WL_LED__
extern void wl_led_test_on(void);
extern void wl_led_test_off(void);
#endif
#ifdef __CONFIG_WPS_LED__
extern void wps_led_test_off();
extern void wps_led_test_on();
#endif
static void clear_security_parameter(void);

static void set_wireless_oob(webs_t wp, char_t *path, char_t *query);


#ifdef CONFIG_WL_USE_APSTA
void new_set_wireless_apclient(webs_t wp);//fh modify
void new_set_wireless_wisp(webs_t wp);//fh modify
#endif
static void clear_wds_settings(void);
void set_extra_channel(int channel);//fh modify
static void formSetWifiRelay(webs_t wp, char_t *path, char_t *query);
static void set_extra_ssid(webs_t wp, char *wlname);
extern void formGetWifiRelay(webs_t wp, char_t *path, char_t *query);
extern void formGetWifiScan(webs_t wp, char_t *path, char_t *query);

extern void  wps_oob(const char *flag);
#ifdef __CONFIG_WPS__
extern void tenda_stop_wps_timer(void);
#endif

/*��changeWeekForm2������cgi_wireless_ctlû��ʹ��*/
void changeWeek(char *in_week, char *out_week)
{
	char *ptr;
	if(in_week == NULL ||out_week == NULL )
		return;
	int n = 0;
	ptr = in_week; 
	while( *ptr!= '\0')
	{
		switch(*ptr)
		{	
			case ',':
				n ++;
				break;
			case '-':
				break;
			case '8':
			case '9':
			case '0':
				printf("illegal character %c\n",*ptr);
				break ;
			case '7':
				out_week[n] = '1';
				n ++;
				break;
			default:
				if(*ptr >= '1' && *ptr <= '6')//�����쵽������
				{
				
					out_week[n] = *ptr + 1;
					n ++;
				}
				
				break;
		}
		ptr++;
	}	

	int i,j;
	char temp; 

	for(i=0; i<strlen(out_week) ;  i+=2) 
	{
		for(j=i+2; j<strlen(out_week);  j+=2)
		{
			if(out_week[i]>out_week[j]) 
			{ 
				temp=out_week[i]; 
				out_week[i]=out_week[j]; 
				out_week[j]=temp; 
			} 
		}
	}
	return;
}

//add by liangia SSIDȫ�ַ�֧��
char_t *encodeSSID(char_t *SSID, char_t *buf)
{
	//buf[>256]����ȫ�����ַ�ʱ����Խ��
	char_t *ch = SSID;
	char temp[512] = {0};

	while (*ch != '\0')
	{

		//��SSID �����ַ�����ʵ�����
		switch (*ch)
		{
			case '"':
				strcat(buf, "&#34;");
				break;

			case '%':
				strcat(buf, "&#37;");
				break;

			case '&':
				strcat(buf, "&#38;");
				break;

			case '\'':
				strcat(buf, "&#39;");
				break;

			case '<':
				strcat(buf, "&#60;");
				break;

			case '>':
				strcat(buf, "&#62;");
				break;

			case '\\':
				strcat(buf, "&#92;");
				break;

			case ' '://ʹHTML��ǩ�пո񲻺ϲ�����������&#160;���������
				strcat(buf, "&#160;");
				break;

			default:
				sprintf(temp, "%s%c", buf, *ch);
				strcpy(buf,temp);

		}

		ch++;
	}

	//	diag_printf("buf=%s\n", buf);
	return buf;
}

char_t *changeStr2CJSON(char_t *SSID, char_t *buf)
{
	//buf[>256]����ȫ�����ַ�ʱ����Խ��
	char_t *ch = SSID;
	char_t *p = buf;

	while (*ch != '\0')
	{
		//��SSID �����ַ�����ʵ�����
		switch (*ch)
		{

			case '"':
			case '\\':
				*p++ = '\\';
				*p++ = *ch;
				break;

			default:
				*p++ = *ch;
		}

		ch++;
	}

	*p = '\0';

	return buf;
}

char *strtouppor(char *str)
{
	char *p = NULL;
	p = str;

	while ('\0' != *p || NULL != *p)
	{
		*p = toupper(*p);
		p++;
	}

	return str;
}

#ifdef __CONFIG_WPS__
extern void stop_wps_when_failed(void);
#endif

void OOB(void)
{
	char_t *wlunit = NULL;

	clear_security_parameter();
#ifdef __CONFIG_WPS__
	wps_oob("unconfigured");/*reset OOB*/
#endif

	nvram_set(WPS_METHOD, "pbc");
	nvram_set(WPS_AP_PIN, "");
	return;
}
static void set_wireless_oob(webs_t wp, char_t *path, char_t *query)
{
	OOB();
#ifdef __CONFIG_WPS__
	tenda_stop_wps_timer();
#endif
	cyg_thread_delay(300);

	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain; charset=gb2312\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websDone(wp, 200);
	return ;
}

static void clear_security_parameter(void)
{
    /*
    lrl ��ʱע�� wps������û��ȷ�� ȷ�����޸�
	nvram_set(WLN0_SECURITY_TYPE, "");
	nvram_set(WLN0_ENCRYP_TYPE, "");
	nvram_set(WLN0_WPA_PSK1, "");
	*/
	return;
}



int getRedirectWlsecStatus()
{
	char_t *wlSec = NULL;
	char_t *never_prompt_wlpwd = NULL;

	_GET_VALUE("wl0_akm", wlSec);
	_GET_VALUE("never_prompt_wlpwd", never_prompt_wlpwd);

	if (!strcmp(wlSec, "") && need_prompt_wlpwd == 1
	    && atoi(never_prompt_wlpwd) != 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

RET_INFO cgi_wizard_firstbridge_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;

	if (root == NULL)
	{
		return RET_ERR;
	}

	cJSON_AddItemToObject(root, T("isFirstBridge"), obj = cJSON_CreateObject());

	if (0 == strcmp(nvram_safe_get(SYSCONFIG_QUICK_SET_ENABLE), "1"))
	{
		cJSON_AddTrueToObject(obj, T("isFirstBridge"));
	}
	else
	{
		cJSON_AddFalseToObject(obj, T("isFirstBridge"));
	}

	return RET_SUC;
}

RET_INFO cgi_wizard_wifiscan_get(webs_t wp, cJSON *root, void *info)
{
	RET_INFO ret = RET_SUC;

	if (root)
	{
		cgi_wifi_scanresult_get(root);
	}
	else
	{
		ret = RET_ERR;
	}

	return ret;
}
/*****************************************************************************
 �� �� ��  : send_wifi_msg_handle
 ��������  : ����wifiģ����Ϣ		
 �������  : int wifi_type (2.4G/5G)
             int enable    
             CGI_MSG_MODULE *msg
 �������  : ��
 �� �� ֵ  : ��
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��23��
    ��    ��   : lrl
    �޸�����   : �����ɺ���

*****************************************************************************/
void send_wifi_msg_handle(char *ifname,int msg_op_code,CGI_MSG_MODULE *msg)
{
	if(0 != strncmp(ifname,TENDA_WLAN24_AP_IFNAME,strlen(TENDA_WLAN24_AP_IFNAME)) 
		&& 0 != strncmp(ifname,TENDA_WLAN5_AP_IFNAME,strlen(TENDA_WLAN5_AP_IFNAME))
		&& 0 != strncmp(ifname,TENDA_WLAN24_GUEST_IFNAME,strlen(TENDA_WLAN24_GUEST_IFNAME))
		&& 0 != strncmp(ifname,TENDA_WLAN5_GUEST_IFNAME,strlen(TENDA_WLAN5_GUEST_IFNAME))
		&& 0 != strncmp(ifname,TENDA_WLAN24_REPEATER_IFNAME,strlen(TENDA_WLAN24_REPEATER_IFNAME))
		&& 0 != strncmp(ifname,TENDA_WLAN5_REPEATER_IFNAME,strlen(TENDA_WLAN5_REPEATER_IFNAME)))
	{
		strcpy(ifname,"");
	}

	if (COMMON_MSG_MAX != msg_op_code)
	{
		CGI_MSG_MODULE msg_tmp;
		msg_tmp.id = RC_WIFI_MODULE;
		sprintf(msg_tmp.msg, "op=%d,wlan_ifname=%s", msg_op_code,ifname);
		add_msg_to_list(msg, &msg_tmp);
	}
	return;
}
/*****************************************************************************
 �� �� ��  : 	gen_encryp_type
 				gen_security_type
 				config_security
 ��������  : һ�¼�����������������ü��ܷ�ʽ�ģ����ϱ�ǵĺ���
 				���ں���ֻ��һ��
 				config_wlan_security
 				
 �������  : char*cipher_str  
             char *encryp     
 �������  : ��
 �� �� ֵ  : static
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��25��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
static void gen_encryp_type( char*cipher_str,char *encryp)
{
	char_t *p = NULL;
	char_t *cipher = NULL;

	cipher = cipher_str;
	cipher = strtouppor(cipher);

	if (0 == strncmp(cipher, "WPA", 3) && NULL != (p = strchr(cipher, '/')))
	{
		++p;
	}

	if (NULL == p || encryp == NULL)
	{
		return;
	}

	if ( 0 == strcmp(p, "TKIP"))
	{
		sprintf(encryp,"tkip");
	}
	else if (0 == strcmp(p, "AES"))
	{
		sprintf(encryp,"aes");
	}
	else if (0 == strcmp(p, "AESTKIP"))
	{
		sprintf(encryp,"tkip+aes");
	}

	return ;

}

static void gen_security_type( char* security,char *security_value)
{
	char *security_web = NULL;
	char *p_str = NULL; 

	security_web = security;
	
	security_web = strtouppor(security_web);
	sprintf(security_value, "%s" , security_web);
	
	if (NULL != (p_str = strchr(security_value, '/')))
	{
		*p_str = '\0';
	}

	return ;

}

static inline void config_security(char * ifname,
	char *wep,char *akm,char *passwd,char *crypto)
{
	if(!strcmp(ifname,TENDA_WLAN24_AP_IFNAME))
	{
		nvram_set(WLAN24G_WEP, wep);
		nvram_set(WLAN24G_AKM, akm);
		nvram_set(WLAN24G_PASSWD, passwd);
		nvram_set(WLAN24G_CRYPTO, crypto);
	}
	else if(!strcmp(ifname,TENDA_WLAN24_REPEATER_IFNAME))
	{
		nvram_set(WLAN24G_REPEATER_WEP, wep);
		nvram_set(WLAN24G_REPEATER_AKM, akm);
		nvram_set(WLAN24G_REPEATER_PASSWD, passwd);
		nvram_set(WLAN24G_REPEATER_CRYPTO, crypto);
	}
	else if(!strcmp(ifname,TENDA_WLAN24_GUEST_IFNAME))
	{
		nvram_set(WLAN24G_GUEST_WEP, wep);
		nvram_set(WLAN24G_GUEST_AKM, akm);
		nvram_set(WLAN24G_GUEST_PASSWD, passwd);
		nvram_set(WLAN24G_GUEST_CRYPTO, crypto);
	}
	else if(!strcmp(ifname,TENDA_WLAN5_AP_IFNAME))
	{
		nvram_set(WLAN5G_WEP, wep);
		nvram_set(WLAN5G_AKM, akm);
		nvram_set(WLAN5G_PASSWD, passwd);
		nvram_set(WLAN5G_CRYPTO, crypto);
	}
	else if(!strcmp(ifname,TENDA_WLAN5_REPEATER_IFNAME))
	{
		nvram_set(WLAN5G_REPEATER_WEP, wep);
		nvram_set(WLAN5G_REPEATER_AKM, akm);
		nvram_set(WLAN5G_REPEATER_PASSWD, passwd);
		nvram_set(WLAN5G_REPEATER_CRYPTO, crypto);
	}
	else if(!strcmp(ifname,TENDA_WLAN5_GUEST_IFNAME))
	{
		nvram_set(WLAN5G_GUEST_WEP, wep);
		nvram_set(WLAN5G_GUEST_AKM, akm);
		nvram_set(WLAN5G_GUEST_PASSWD, passwd);
		nvram_set(WLAN5G_GUEST_CRYPTO, crypto);
	}
}

/*****************************************************************************
 �� �� ��  : config_wlan_security
 ��������  : ���ö�Ӧ�ӿڵļ��ܷ�ʽ�Լ������㷨�����룬�������sercurity����
             ʽ��ҪΪ���ܷ�ʽ/�����㷨���������������Ž�ģʽҳ���·�������
             ��ʽ
 �������  : char* ifname           
             char *security         
             char *pass_phrase_str  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��25��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
void config_wlan_security(char* ifname,char *security,char *pass_phrase_str)
{	
	char_t security_value[16] = {0};
	char_t encryp[16] = {0};

	gen_security_type(security,security_value);
	
	if (0 == strcmp(security_value, "NONE") )  				// !--- Disable Mode ---
	{
		config_security(ifname,DISABLE,"","","");	
	}
	else if (0 == strcmp(security_value, "WPA2") ) 			// !---  WPA2 Personal Mode ----
	{
		gen_encryp_type(security,encryp);
		config_security(ifname,DISABLE,"psk2",pass_phrase_str,encryp);
	}
	else if (0 == strcmp(security_value, "WPAWPA2") )	  	//! ----   WPA PSK WPA2 PSK mixed
	{
		gen_encryp_type(security,encryp);
		config_security(ifname,DISABLE,"psk psk2",pass_phrase_str,encryp);
	}
	else if (0 == strcmp(security_value, "WPA")) 			// !---  WPA Personal Mode ---
	{	
		gen_encryp_type(security,encryp);
		config_security(ifname,DISABLE,"psk",pass_phrase_str,encryp);
	}
	else
	{
		diag_printf("Unsupport security.\n");
	}
}
/*****************************************************************************
 �� �� ��  : get_wlan_security_mode
 ��������  : ��ȡ���ܷ�ʽ,���ܷ�ʽ��ʽ��:WPA/AES;���Ž�ģʽҳ���·������ݱ���һ��
 �������  : WIFI_INFO_STRUCT wifi_info         
             char *security_mode           
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��25��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
void get_wlan_security_mode(P_WIFI_SSID_CFG wifi_basic_info,char *security_mode)
{
	if(NULL == security_mode || NULL == wifi_basic_info)
	{
		return;
	}
    if (strcmp(wifi_basic_info->wpa_psk_type, "psk") == 0)
    {
        snprintf(security_mode,"%s","WPA/AES");
    }
    else if (strcmp(wifi_basic_info->wpa_psk_type, "psk2") == 0)
    {
        snprintf(security_mode,"%s","WPA2/AES");
    }
    else if (strcmp(wifi_basic_info->wpa_psk_type, "psk psk2") == 0)
    {
        snprintf(security_mode,"%s","WPAWPA2/AES");
    }

    if(strcmp(wifi_basic_info->security, "none") == 0)
    {
        snprintf(security_mode,"%s","NONE" );
    }
	return;
}

/*****************************************************************************
 �� �� ��  : assemble_channelList_cJsonArray
 ��������  : �ŵ��б���ϳ����鷵��web
 �������  : cJSON *array         
             PIU16 *channel_list
             int channel_count
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��10��25��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
int assemble_channelList_cJsonArray(cJSON *array, PIU16 *channel_list, int channel_count)
{
	int i = 0;
	
	if( array == NULL || channel_list == NULL)
	{
		return -1;
	}
	/*ҳ�� �Լ�����Զ��ŵ�*/
	/*add auto channel*/
	//if( channel_count > 0)
	//{	
	//	cJSON_AddNumberToObject(array, "XXX", 0);
	//}
	/*add channel list*/
	for ( i=0; i< channel_count; i++)
	{
		cJSON_AddNumberToObject(array, "XXX", channel_list[i]);
	}
	
	return 0;
}

char *inttostr(char *str,int n,int len)
{
	if(NULL == str)
	{
		return "";
	}

	memset(str,0x0,len);
	snprintf(str,len,"%d",n);

	return str;
}
