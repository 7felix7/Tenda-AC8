/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cgi_wps_dispatch.c
  �� �� ��   : ����
  ��    ��   : fh
  ��������   : 2016��10��13��
  ����޸�   :
  ��������   : 

  ��������   : wps,��һ����UI��ʱû��ģ�黯

  �޸���ʷ   :
  1.��    ��   : 2016��10��13��
    ��    ��   : fh
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include <webs.h>
#include "cgi_common.h"
#include "flash_cgi.h"


/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/


void fromSetWps(webs_t wp, char_t *path, char_t *query)
{
	char_t *security_mode=NULL,  *wpsenable=NULL,*wifienable_2G=NULL,*wifienable_5G=NULL,*action=NULL;
	char_t *wlunit;
	

	action = websGetVar(wp, T("action"), T("pbc"));

	_GET_VALUE("wps_mode",wpsenable);
	_GET_VALUE("wl0_radio",wifienable_2G);
    _GET_VALUE("wl1_radio",wifienable_5G);

	printf("wpsenable=%s,wifienable_2G=%s,wifienable_5G=%s,action=%s,\n",wpsenable,wifienable_2G,wifienable_5G,action);

	if( (0== strcmp(wpsenable, "enabled")) && ((0== strcmp(wifienable_2G, "1")) || (0== strcmp(wifienable_5G, "1"))) && (0== strcmp(action, "pbc")) )
	{	
		printf("start pbc wps.................\n");
		msg_send(MODULE_RC, RC_WPS_MODULE, "string_info=startpbc");
	}
	else
	{
		printf("can't start pbc wps.................\n");
	}

 	websWrite(wp, T("HTTP/1.0 200 OK\r\n\r\n")); 
	websWrite(wp, T("%s"), "{\"errCode\":\"0\"}"); 
	websDone(wp, 200);
			
	return;
	
}

