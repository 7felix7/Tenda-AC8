/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cgi_lib_sysmanage.c
  �� �� ��   : ����
  ��    ��   : �ξ���
  ��������   : 2016��12��13��
  ����޸�   :
  ��������   :

  ��������   : sysmanage����С���ܵ�Ԫ��get��set��

  �޸���ʷ   :
  1.��    ��   : 2016��12��13��
    ��    ��   : �ξ���
    �޸�����   : �����ļ�

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <router_net.h>
#include "flash_cgi.h"
#include "webs.h"
#include "cJSON.h"
#include "cgi_common.h"
#include "sys_module.h"
#include "http.h"
#include <autoconf.h>
#include "cgi_lib.h"


extern int sys_led_turn;

/*****************************************************************************
 �� �� ��  : cgi_lib_set_led
 ��������  : led��ʱ�ر�
 �������  : webs_t wp
             cJSON *root
             CGI_MSG_MODULE *msg
             char *err_code
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��11��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_set_led(webs_t wp,cJSON *root,CGI_MSG_MODULE *msg, char *err_code,void *info)
{
	char_t *ledType,*led_timer;
	char led_ctl_type[16] = {0};
	char timer[64] = {0};
	int restart = 0;
	

    ledType = cgi_lib_get_var(wp,root, T(LIB_LED_STATUS),"");
    led_timer = cgi_lib_get_var(wp,root, T(LIB_LED_CLOSE_TIME),"");

	if(strcmp(nvram_safe_get(SAVE_POWER_LED_TYPE),ledType))
	{
		nvram_set(SAVE_POWER_LED_TYPE,ledType);
		restart = 1;
	}
	//LED����Ϊ��ʱ�ر�
	if(0 == strcmp("2",ledType))
	{
		if(strcmp(nvram_safe_get(SAVE_POWER_LED_TIME),led_timer))
		{
			nvram_set(SAVE_POWER_LED_TIME,led_timer);
			restart = 1;
		}
	}
	
	if(restart)
    {
        CGI_MSG_MODULE msg_tmp;
        msg_tmp.id = RC_LED_MODULE;
        sprintf(msg_tmp.msg, "op=%d", OP_RESTART);
        add_msg_to_list(msg, &msg_tmp);
    }

	
	 if(!err_code[0])
    {
        strcpy(err_code, "0");
    }
	return RET_SUC;
}


/*****************************************************************************
 �� �� ��  : cgi_lib_get_led
 ��������  : ��ȡLED״̬��Ϣ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��11��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

********	*********************************************************************/
RET_INFO cgi_lib_get_led(webs_t wp, cJSON *root, void *info)
{
	char iterm_value[PI_BUFLEN_64] = {0};
	//LED�������� 0:���� 1:���� 2:��ʱ�ر�
	strcpy(iterm_value,nvram_safe_get(SAVE_POWER_LED_TYPE));
	cJSON_AddStringToObject(root,LIB_LED_STATUS,iterm_value);

	//LED���ƹر�ʱ���
	memset(iterm_value,0x0,sizeof(iterm_value));
	strcpy(iterm_value,nvram_safe_get(SAVE_POWER_LED_TIME));
	cJSON_AddStringToObject(root,LIB_LED_CLOSE_TIME,iterm_value);

	return RET_SUC;
}


/*****************************************************************************
 �� �� ��  : cgi_lib_get_led
 ��������  : ����APP��ȡLED״̬��Ϣ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��11��6��
    ��    ��   : ������
    �޸�����   : �����ɺ���

********	*********************************************************************/
RET_INFO cgi_lib_get_led_app(webs_t wp, cJSON *root, void *info)
{
	char iterm_value[PI_BUFLEN_64] = {0};
	//LED�������� 0:���� 1:���� 2:��ʱ�ر�
	strcpy(iterm_value,nvram_safe_get(SAVE_POWER_LED_TYPE));
	//��LED������Ϊ��ʱ�ر�ʱ,ϵͳ�Ʋ�������Ϊ��LEDȫ��,����Ϊȫ��
	if(0 == strcmp("2",iterm_value))
	{
		if(0 == sys_led_turn)
		{
			cJSON_AddStringToObject(root,LIB_LED_STATUS,"1");
		}
		else
		{
			cJSON_AddStringToObject(root,LIB_LED_STATUS,"0");
		}
	}
	else
	{
		cJSON_AddStringToObject(root,LIB_LED_STATUS,iterm_value);
	}

	return RET_SUC;
}



