#include <stdio.h>
#include <stdlib.h>
#include <bcmnvram.h>
#include "flash_cgi.h"

#include "cgi_handle_module.h"
#include "cgi_lib_config.h"

/*****************************************************************************
 �� �� ��  : app_get_safe_grade
 ��������  : app��ȡ��ȫ�ȼ�
 			 ע��:DNS�ٳֺ�ddos�����޷��ж�
 �������  : cJSON *recv_root  
             cJSON *send_root  
             void *info        
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��28��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
void app_get_safe_grade(cJSON *recv_root,cJSON *send_root, void *info)
{
	CGI_LIB_INFO get_info;
	PIU8 modules[] = 
	{
		MODULE_GET_WIFI_BASIC,
	};

	get_info.wp = NULL;
	get_info.root = recv_root;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
	

	cJSON *obj = cJSON_GetObjectItem(get_info.root, LIB_WIFI_AP_NO_PWD);
	//���nvramֵ"wifiNoPwd"��false˵�������룬�����ture���ʾû������
	if(!strcmp(obj->valuestring,"false"))
	{
		obj = cJSON_GetObjectItem(get_info.root, LIB_WIFI_AP_PWD);
		cJSON_AddStringToObject(send_root, "wifi_password",obj->valuestring);
	}
	else
		cJSON_AddStringToObject(send_root, "wifi_password","");

	//��ȡ���ĵĵ�¼����
	char login_depw[64] = {0},*login_enpw =NULL;
	_GET_VALUE("http_passwd",login_enpw);
	base64_decode(login_enpw,login_depw,64);
	cJSON_AddStringToObject(send_root, "decode_login_password",login_depw);

}






