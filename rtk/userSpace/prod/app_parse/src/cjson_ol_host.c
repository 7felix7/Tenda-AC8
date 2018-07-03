#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi_lib.h"
#include "cgi_tc.h"

/*����hostType�����꣬��app��hostTypeö������ƥ��*/
#define DEVICE_TYPE_ANDROID 0
#define DEVICE_TYPE_UNKNOWN 4

/*����������͵ĺ�,����,2G,5G,2G�ÿ�,5G�ÿ�,��APP��Э��Լ��*/
#define WIRES	0
#define WIFI_2G	10
#define WIFI_5G	11
#define WIFI_2G_GUEST	12
#define WIFI_5G_GUEST	13
extern char *cjson_get_value(cJSON *root, char *name, char *defaultGetValue);
extern int cjson_get_number(cJSON *root, char *name, int defaultGetValue);

extern int is_mac_trust(const char *m_mac);

/*****************************************************************************
 �� �� ��  : app_get_ol_host_info
 ��������  : ��ȡ�������ߡ������豸��Ϣ
 �������  : webs_t wp,
 			 cJSON *root
 			 void *info
 �������  : ��
 �� �� ֵ  : 

 �޸���ʷ      :
  1.��    ��   : 2016��12��20��
    ��    ��   : lrl
    �޸�����   : �����ɺ���

*****************************************************************************/
extern int g_speedtest_web_update_tc_timeout;
void app_get_ol_host_info(cJSON *recv_root,cJSON *send_root,void *info)
{
	int count = 0, i = 0;
	int host_type = DEVICE_TYPE_ANDROID;
	float up_limit = 0;	
	float down_limit = 0;
	int bw_limited = 1;
	char mac[PI_MAC_STRING_LEN] = {0};
	char  manufactory[PI_BUFLEN_32] = {0};
	cJSON *online_expired_list =NULL;
	char value[32] = {0};
	
	online_expired_list  = cJSON_CreateArray(); /*���ڱ������е����������豸��Ϣ*/
	
	CGI_LIB_INFO get_info;
	PIU8 modules[] = 
	{
		MODULE_GET_TC_ONLINE_LIST,
		MODULE_GET_EXPIRED_INFO,
		#ifdef __CONFIG_GUEST__
		MODULE_GET_GUEST_ONLINE_LIST,
		#endif
	};
	
	/*����cgi,��ȡ���е������б�*/
	get_info.wp = NULL;
	get_info.root = online_expired_list;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
	g_speedtest_web_update_tc_timeout = 30;	
	count = cJSON_GetArraySize(online_expired_list);

    cJSON *online_expired_child_obj = NULL;
	cJSON *root = NULL;
	cJSON *obj = NULL;

	cJSON_AddItemToObject(send_root,"hostInfo", root = cJSON_CreateArray());
	for(i = 0; i < count; i++)
	{
		cJSON_AddItemToArray(root, obj = cJSON_CreateObject());
		
		online_expired_child_obj = cJSON_GetArrayItem(online_expired_list,i);	
		/*hostname*/
		cJSON_AddStringToObject(obj,"hostname", cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_HOSTNAME,""));	
		/*remark*/
		cJSON_AddStringToObject(obj,"remark", cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_REMARK,""));
		/*ip*/
		cJSON_AddStringToObject(obj,"ip", cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_IP,""));
	
		strcpy(mac,cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_MAC,""));
        cJSON_AddStringToObject(obj,"mac", mac);/*mac*/
		
		strcpy(value,cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_CONN_TYPE,"wifi"));
		if(0 == strcmp("wires",value))
		{
			cJSON_AddNumberToObject(obj,"ConnectType",WIRES);/*����*/
		}
		else
		{
			memset(value,0x0,sizeof(value));
			strcpy(value,cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_ACCESS_TYPE,""));
			if(0 == strcmp(value,"wifi_2G"))	//2G
			{
				cJSON_AddNumberToObject(obj,"ConnectType",WIFI_2G);
			}
			else if(0 == strcmp(value,"wifi_5G"))	//5G
			{
				cJSON_AddNumberToObject(obj,"ConnectType",WIFI_5G);
			}
			#ifdef __CONFIG_GUEST__
			else if(0 == strcmp(value,"wifi_guest_2G"))	//2G�ÿ� 
			{
				cJSON_AddNumberToObject(obj,"ConnectType",WIFI_2G_GUEST);
			}
			else if(0 == strcmp(value,"wifi_guest_5G"))
			{
				cJSON_AddNumberToObject(obj,"ConnectType",WIFI_5G_GUEST);
			}
			#endif
		}

		/*�Ƿ��������б�*/
		if(1 == is_mac_trust(mac))
		{
			cJSON_AddNumberToObject(obj,"is_mac_trust",1);
		}
		else
		{
			cJSON_AddNumberToObject(obj,"is_mac_trust",0);
		}
		/*connectTime*/
		cJSON_AddNumberToObject(obj,"ConnetTime",cjson_get_number(online_expired_child_obj,LIB_QOS_LIST_CONNECT_TIME,0));
		
		/*�ж��Ƿ�����	38528 * 128KB/s��ʾ������*/
        down_limit = atof(cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_DOWN_LIMIT,"38528"));
		up_limit = atof(cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_UP_LIMIT,"38528"));
	    
		/*app��-1��ʾ������*/
		if( (UP_RATE_LIMIT * 128 == up_limit) && (DOWN_RATE_LIMIT * 128 == down_limit))
		{
			bw_limited = 0;  /*������*/
			
			down_limit = -1;
			up_limit = -1;	
		}
		else
		{
			bw_limited = 1;  /*����*/
			
			if(UP_RATE_LIMIT * 128 == up_limit)
				up_limit = -1;
			if(DOWN_RATE_LIMIT * 128 == down_limit)
				down_limit = -1;
		}
		cJSON_AddNumberToObject(obj,"down_limit",down_limit);
		cJSON_AddNumberToObject(obj,"up_limit",up_limit);
		cJSON_AddNumberToObject(obj,"bw_limited",bw_limited);
		
		cJSON_AddNumberToObject(obj,"curr_down_rate",atof(cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_DOWN_SPEED,"")));
		cJSON_AddNumberToObject(obj,"curr_up_rate",atof(cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_UP_SPEED,"")));

		/*�豸����*/
		strcpy(manufactory,cjson_get_value(online_expired_child_obj,LIB_QOS_LIST_MANUFACTURER,""));
		if(0 == strcmp(manufactory,"other"))
		{
			strcpy(manufactory,"unknown");
			host_type = DEVICE_TYPE_UNKNOWN;  //δ֪��������
		}
		cJSON_AddStringToObject(obj,"manufactory",manufactory); 
		
		cJSON_AddNumberToObject(obj,"hostTpye",host_type);   //�豸���� 
		/**/
		if(0 == cjson_get_number(online_expired_child_obj,LIB_EXPIRED_IS,1))
		{		
			cJSON_AddNumberToObject(obj,"online",0);    //�豸����
			/*������ģʽ*/
			if(-1 != is_mac_filter(mac,"deny"))
			{
				cJSON_AddNumberToObject(obj,"mac_blocked",1);
			}
			else
			{
				/*�����豸�ǲ����ڰ�����ģʽ*/
				cJSON_AddNumberToObject(obj,"mac_blocked",0);
			} 
		}
		else
		{		
			cJSON_AddNumberToObject(obj,"online",1);    //�豸����
			/*����ע��ڰ�����ģʽ*/
	        if(!strcmp("deny",nvram_safe_get("filter_mac_mode"))) //������ģʽ
	        {
				cJSON_AddNumberToObject(obj,"mac_blocked",0); 
	        }
			/*Ҫȷ�� app ��mac_blocked����ʲô????*/
	        else  /*������ģʽ*/
	        {
				/*�ж��Ƿ���mac��ַ������*/
	            if(-1 == is_mac_filter(mac,"deny"))
	            {
					/*���豸���ں�����ģʽ��*/
	            	cJSON_AddNumberToObject(obj,"mac_blocked",0);
	            }
	            else
	            {
	                cJSON_AddNumberToObject(obj,"mac_blocked",1);
	            }
	        }
		}
	}
		
	cJSON_Delete(online_expired_list);  
    return ;
}

