/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cgi_wifi.c
  �� �� ��   : ����
  ��    ��   : fh
  ��������   : 2016��12��13��
  ����޸�   :
  ��������   :

  ��������   : ���߲����Ļ�ȡ������

  �޸���ʷ   :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

#include "cgi_lib.h"

#if 0
PIU8 get_modules_wifiEn[] =
{
	MODULE_GET_WIFI_EN,
	MODULE_GET_END,
};

PIU8 get_modules_isWifiClients[] = 
{		
	MODULE_GET_ISWIFICLIENT,
	MODULE_GET_END,
};	

PIU8 get_modules_wifiBasicCfg[] =
{
	MODULE_GET_WIFI_BASIC,
	MODULE_GET_END,
};
	
PIU8 get_modules_wifiPower[] =
{
	MODULE_GET_WIFI_POWER,
	MODULE_GET_END,
};

PIU8 get_modules_wifiTime[] =
{
	MODULE_GET_WIFI_SCHED,
	MODULE_GET_WIFI_RELAY_TYPE,
	MODULE_GET_END,
};

PIU8 get_modules_wifiAdvCfg[] =
{
	MODULE_GET_WIFI_ADV_CFG,
	MODULE_GET_WIFI_RELAY_TYPE,
	MODULE_GET_END,
};

PIU8 get_modules_wifiWPS[] =
{
	MODULE_GET_WIFI_WPS,
	MODULE_GET_END,
};


PIU8 set_modules_wifiEn[] =
{
	MODULE_SET_WIFI_EN,
	MODULE_SET_END,
};

PIU8 set_modules_wifiBasicCfg[] =
{
	MODULE_SET_WIFI_BASIC,
	MODULE_SET_END,
};

PIU8 set_modules_wifiPower[] =
{
	MODULE_SET_WIFI_POWER,
	MODULE_SET_END,
};

PIU8 set_modules_wifiTime[] =
{
	MODULE_SET_WIFI_SCHED,
	MODULE_SET_END,
};

PIU8 set_modules_wifiAdvCfg[] =
{
	MODULE_SET_WIFI_ADV_CFG,
	MODULE_SET_END,
};

PIU8 set_modules_wifiWPS[] =
{
	MODULE_SET_WIFI_WPS,
	MODULE_SET_END,
};
#endif
/*****************************************************************************
 �� �� ��  : cgi_wifiEn_get
 ��������  : ���߿���״̬�Ļ�ȡ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  : RET_INFO

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiEn_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_EN,
	};

	cJSON_AddItemToObject(root, "wifiEn", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}
/*�Ƿ������߿ͻ���*/
RET_INFO cgi_isWifiClient_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;	
	PIU8 modules[] = 
	{		
		MODULE_GET_ISWIFICLIENT,
	};	
	cJSON_AddItemToObject(root, "isWifiClients", obj = cJSON_CreateObject());
	get_info.wp = wp;	
	get_info.root = obj;	
	get_info.modules = modules;	
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info,NULL);
	return RET_SUC;
}
/*****************************************************************************
 �� �� ��  : cgi_wifiBasic_get
 ��������  : ���߻���������ȡ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiBasic_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_BASIC,
	};

	cJSON_AddItemToObject(root, "wifiBasicCfg", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_wifiPower_get
 ��������  : ���߹��ʵȼ���ȡ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiPower_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_POWER,
	};

	cJSON_AddItemToObject(root, "wifiPower", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}


/*****************************************************************************
 �� �� ��  : cgi_wifiSched_get
 ��������  : ���߶�ʱ������Ϣ��ȡ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiSched_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_SCHED,
		MODULE_GET_WIFI_RELAY_TYPE,
	};

	cJSON_AddItemToObject(root, "wifiTime", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}


/*****************************************************************************
 �� �� ��  : cgi_wifiParam_get
 ��������  : ���߸߼�������ȡ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiParam_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_ADV_CFG,
		MODULE_GET_WIFI_RELAY_TYPE,
	};

	cJSON_AddItemToObject(root, "wifiAdvCfg", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_wps_hasmodule
 ��������  : ��ȡ�Ƿ�֧��wpsģ��
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��03��21��
    ��    ��   : lrl
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wps_hasmodule(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	cJSON_AddItemToObject(root, "wpsModule", obj = cJSON_CreateObject());
#ifdef __CONFIG_WPS_RTK__
	//֧��wpsģ��
	cJSON_AddStringToObject(obj,LIB_WPS_HAS_MODE,"true");
#else
	//֧��wpsģ��
	cJSON_AddStringToObject(obj,LIB_WPS_HAS_MODE,"false");
#endif
	return RET_SUC;
}

#ifdef __CONFIG_WPS_RTK__
/*****************************************************************************
 �� �� ��  : cgi_wifiWps_get
 ��������  : ��ȡWPS��Ϣ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiWps_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_WPS,
	};

	cJSON_AddItemToObject(root, "wifiWPS", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_wifiWps_set
 ��������  : ����WPS����
 �������  : webs_t wp
             CGI_MSG_MODULE *msg
             char *err_code
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiWps_set(webs_t wp, CGI_MSG_MODULE *msg, char *err_code, void *info)
{
	CGI_LIB_INFO set_info;
	PIU8 modules[] =
	{
		MODULE_SET_WIFI_WPS,
	};

	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, NULL);
	return (RET_SUC);
}
#endif

/*****************************************************************************
 �� �� ��  : cgi_wifiBasic_set
 ��������  : �������߻�������
 �������  : webs_t wp
             CGI_MSG_MODULE *msg
             char *err_code
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiBasic_set(webs_t wp, CGI_MSG_MODULE *msg, char *err_code, void *info)
{
	CGI_LIB_INFO set_info;
	PIU8 modules[] =
	{
		MODULE_SET_WIFI_BASIC,
	};

	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, NULL);
	return (RET_SUC);
}

/*****************************************************************************
 �� �� ��  : cgi_wifiPower_set
 ��������  : �������߹���
 �������  : webs_t wp
             CGI_MSG_MODULE *msg
             char *err_code
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiPower_set(webs_t wp, CGI_MSG_MODULE *msg, char *err_code, void *info)
{
	CGI_LIB_INFO set_info;
	PIU8 modules[] =
	{
		MODULE_SET_WIFI_POWER,
	};

	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, NULL);
	return (RET_SUC);
}

/*****************************************************************************
 �� �� ��  : cgi_wifiSched_set
 ��������  : �������߶�ʱ����
 �������  : webs_t wp
             CGI_MSG_MODULE *msg
             char *err_code
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiSched_set(webs_t wp, CGI_MSG_MODULE *msg, char *err_code, void *info)
{
	CGI_LIB_INFO set_info;
	PIU8 modules[] =
	{
		MODULE_SET_WIFI_SCHED,
	};

	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, NULL);
	return (RET_SUC);
}

/*****************************************************************************
 �� �� ��  : cgi_wifiParam_set
 ��������  : �������߸߼�����
 �������  : webs_t wp
             CGI_MSG_MODULE *msg
             char *err_code
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifiParam_set(webs_t wp, CGI_MSG_MODULE *msg, char *err_code, void *info)
{
	CGI_LIB_INFO set_info;
	PIU8 modules[] =
	{
		MODULE_SET_WIFI_ADV_CFG,
	};

	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, NULL);
	return (RET_SUC);
}

#ifdef __CONFIG_AUTO_CONN_CLIENT__
/*****************************************************************************
 �� �� ��  : cgi_auto_sync_info_get
 ��������  : �Զ�����ͬ����Ϣ��ȡģ��
 �������  : webs_t wp    
             cJSON *root  
             void *info   
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��3��25��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_auto_sync_info_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_AUTO_SYNC_INFO,
	};

	cJSON_AddItemToObject(root, "synchroStatus", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}
#endif

#ifdef __CONFIG_GUEST__
/*****************************************************************************
 �� �� ��  : cgi_wifi_guest_info_get
 ��������  : ��ȡ�ÿ����������Ϣ
 �������  : webs_t wp    
             cJSON *root  
             void *info   
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��10��
    ��    ��   : lrl
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifi_guest_info_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_WIFI_GUEST_INFO,
	};

	cJSON_AddItemToObject(root, "wifiGuest", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}
/*****************************************************************************
 �� �� ��  : cgi_wifi_guest_info_set
 ��������  : set�ÿ����������Ϣ
 �������  : webs_t wp    
             CGI_MSG_MODULE *msgt  
             char *err_code
             void *info    
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��10��
    ��    ��   : lrl
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_wifi_guest_info_set(webs_t wp, CGI_MSG_MODULE *msg, char *err_code, void *info)
{
	CGI_LIB_INFO set_info;
	PIU8 modules[] =
	{
		MODULE_SET_WIFI_GUEST_INFO,
	};

	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, NULL);

	return RET_SUC;
}
#endif

#ifdef __CONFIG_WL_BEAMFORMING_EN__
/*****************************************************************************
 �� �� ��  : cgi_beaforming_enable_get
 ��������  : ��ȡbeaforming����
 �������  : webs_t wp    
             cJSON *root  
             void *info   
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��10��
    ��    ��   : lrl
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_beaforming_enable_get(webs_t wp, cJSON *root, void *info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] =
	{
		MODULE_GET_BEAFORMING_ENABLE,
	};

	cJSON_AddItemToObject(root, "wifiBeamforming", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);
	return RET_SUC;
}
/*****************************************************************************
 �� �� ��  : cgi_beaforming_enable_set
 ��������  : set beaforming
 �������  : webs_t wp    
             CGI_MSG_MODULE *msgt  
             char *err_code
             void *info   
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��10��
    ��    ��   : lrl
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_beaforming_enable_set(webs_t wp, CGI_MSG_MODULE *msg, char *err_code, void *info)
{
	CGI_LIB_INFO set_info;
	PIU8 modules[] =
	{
		MODULE_SET_BEAFORMING_ENABLE,
	};

	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info, msg, err_code, NULL);

	return RET_SUC;
}
#endif

