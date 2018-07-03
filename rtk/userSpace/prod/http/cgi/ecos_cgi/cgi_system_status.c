/*************************************************************************
	> Copyright (C) 2016, Tenda Tech. Co., All Rights Reserved.
	> File Name: cgi_system_status.c
	> Description: 
	> Author: zhuhuan
	> Mail: zhuhuan_IT@outlook.com
	> Version: 1.0
	> Created Time: Tue 15 Mar 2016 10:02:28 AM CST
	> Function List: 

	> History:
    <Author>      <Time>      <Version>      <Desc>
    
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cgi_lib.h"	 

#include "webs.h"
#include "uemf.h"

#include "wifi.h"
#include <tenda_arp.h>
#include <wl_utility_rltk.h>
#include <shutils.h>
#include "systools.h"
#include "wan.h"
#include "cgi_tc.h"

#include "cgi_api.h"   //���������ӿ�
#include "cgi_lib.h"	//��CGI��


RET_INFO cgi_system_get_internet_status(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] = 
	{
		MODULE_GET_INTERNET_STATUS,	
	};

	cJSON_AddItemToObject(root, "internetStatus", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
    return RET_SUC;

}

RET_INFO cgi_system_get_system_info(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] = 
	{
		MODULE_GET_WAN_TYPE,
		MODULE_GET_STATUS_WANMAC,
		MODULE_GET_NATIVE_HOST_MAC,
		MODULE_GET_LAN_INFO,
		MODULE_GET_SYSTEM_STATUS_INFO,
		MODULE_GET_SOFTWARE_VERSION,
		MODULE_GET_WAN_CONNECT_TIME,
	};

	cJSON_AddItemToObject(root, "systemInfo", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
	
	return RET_SUC;	
}

#ifdef __CONFIG_TENDA_APP__
/*����������������ص�,����ʹ��cgi_api.h�еĽӿڣ��������app�����*/
/*****************************************************************************
 �� �� ��  : cgi_notNow_upgrade_set
 ��������  : �����ݲ�����
 �������  : webs_t wp	
			 cJSON *root 
			 void *info
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ	   :
  1.��	  ��   : 2016��12��13��
	��	  ��   : luorilin
	�޸�����   : �����ɺ���
*****************************************************************************/
RET_INFO cgi_notNow_upgrade_set(webs_t wp, CGI_MSG_MODULE * msg, char * err_code, void *info)
{	
	cJSON *obj = NULL;
	CGI_LIB_INFO set_info;
	PIU8 modules[] = 
	{
		MODULE_SET_NO_NOW_UPGRADE,	
	};
	
	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);     /*err_code��lib�����Ѿ�����*/
	
	return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_ucloud_version_get
 ��������  : ��ȡ����°汾
 �������  : webs_t wp	
			 cJSON *root 
			 void *info
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ	   :
  1.��	  ��   : 2016��12��13��
	��	  ��   : luorilin
	�޸�����   : �����ɺ���
*****************************************************************************/
RET_INFO cgi_ucloud_version_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] = 
	{
		MODULE_GET_UCLOUD_VERSION 
	};
	cJSON_AddItemToObject(root, "hasNewSoftVersion", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
	
	return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_ucloud_upgrade_get
 ��������  : ��������
 �������  : webs_t wp	
			 cJSON *root 
			 void *info
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ	   :
  1.��	  ��   : 2016��12��13��
	��	  ��   : luorilin
	�޸�����   : �����ɺ���
*****************************************************************************/
RET_INFO cgi_ucloud_upgrade_get(webs_t wp, cJSON *root, void *info)
{	
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] = 
	{
		MODULE_GET_UCLOUD_UPGRADE 
	};
	cJSON_AddItemToObject(root, "onlineUpgradeReady", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
	return RET_SUC;
}
#endif //__CONFIG_TENDA_APP__
