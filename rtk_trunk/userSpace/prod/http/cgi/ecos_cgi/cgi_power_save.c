/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cgi_power_save.c
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2017��12��15��
  ����޸�   :
  ��������   :

  ��������   : ���߲����Ļ�ȡ������

  �޸���ʷ   :
  1.��    ��   : 2017��11��15��
    ��    ��   : 
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/

#include "cgi_lib.h"
#ifdef __CONFIG_LED__
RET_INFO cgi_power_save_led_set(webs_t wp, CGI_MSG_MODULE * msg, char * err_code, void * info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO set_info;
	PIU8 modules[] = 
	{
		MODULE_SET_LED_INFO,	
	};	

	set_info.wp = wp;
	set_info.root = NULL;
	set_info.modules = modules;
	set_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_set(set_info,msg,err_code,NULL);
	
	return RET_SUC;	
}

RET_INFO cgi_power_save_led_get(webs_t wp, cJSON *root, void * info)
{
	cJSON *obj = NULL;
	CGI_LIB_INFO get_info;
	PIU8 modules[] = 
	{
		MODULE_GET_LED_INFO,	
	};

	cJSON_AddItemToObject(root, "LEDControl", obj = cJSON_CreateObject());

	get_info.wp = wp;
	get_info.root = obj;
	get_info.modules = modules;
	get_info.module_num = ARRAY_SIZE(modules);
	cgi_lib_get(get_info, NULL);

	return RET_SUC;
}
#endif

