/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cgi_config.c
  �� �� ��   : ����
  ��    ��   : liquan
  ��������   : 2016��12��8��
  ����޸�   :
  ��������   :

  ��������   : ����ҳ��set��get�Ľӿں���

  �޸���ʷ   :
  1.��    ��   : 2016��12��8��
    ��    ��   : liquan
    �޸�����   : �����ļ�

******************************************************************************/
#include "webs.h"
#include "cgi_common.h"
#include "flash_cgi.h"
extern CGI_MODULE_INFO cgi_set_public_modules[];
extern CGI_MODULE_INFO cgi_get_public_modules[];
extern CGI_MODULE_INFO cgi_set_private_modules[];

/*****************************************************************************
 �� �� ��  : formGet
 ��������  : ����ҳ���GET����
 �������  : webs_t wp
             char_t *path
             char_t *query
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��8��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
void formGet(webs_t wp, char_t *path, char_t *query)
{
    PIU32 index;
    PI8 searched = 0;
    cJSON *send = NULL;
    PI8_P modules = NULL;
    PI8_P send_buf = NULL;
    PI8_P cur_module = NULL;

    send = cJSON_CreateObject();
    if(NULL == send)
    {
        return ;
    }

    modules = websGetVar(wp, "modules", T(""));

    while(modules != NULL)
    {
        /* ��ȡ����ģ�飬ҳ�������ʽΪmodules=XXX,YYY */
        cur_module = strsep(&modules,",");
        if(cur_module)
        {
            for(index = 0; cgi_get_public_modules[index].type != CGI_NONE; ++index)
            {
                /* ���Ҷ�Ӧ������ģ�� */
                if(0 == strcmp(cur_module, cgi_get_public_modules[index].name))
                {
                    searched = 1;
                    CGI_GET_FUN(&cgi_get_public_modules[index])(wp, send, NULL);
                    break;
                }
            }
        }
    }

    websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain; charset=utf-8\nPragma: no-cache\nCache-Control: no-cache\n\n"));
    if(searched == 1)
    {
        send_buf = cJSON_Print(send);
        if(NULL != send_buf)
        {
            websWriteLongString(wp, send_buf);
        }

    }

    websDone(wp, 200);

    if(NULL != send)
    {
        cJSON_Delete(send);
        send = NULL;
    }
    if(searched == 1)
        FREE_P(&send_buf);

    return ;
}

/*****************************************************************************
 �� �� ��  : formSet
 ��������  : ����ҳ���set����
 �������  : webs_t wp
             char_t *path
             char_t *query
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��8��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
void formSet(webs_t wp, char_t *path, char_t *query)
{

    PIU32 index = 0;
    PIU32 module_index = 0;
    PI8_P cur_module = NULL;
    RET_INFO retval = RET_SUC;
    PI8 ret_buf[PI_BUFLEN_32] = {0};
    PI8 err_code[PI_BUFLEN_32] = {0};
    PI8 module_name[PI_BUFLEN_16] = {0};
    PI8 private_event[PI_BUFLEN_32]  = {0};
    CGI_MSG_MODULE msg_list[MAX_MSG_NUM];

    memset((char *)&msg_list, 0x0, MAX_MSG_NUM * sizeof(CGI_MSG_MODULE));
    for(index = 1; index <= MAX_MODULE_NUM; ++index)
    {
        memset(module_name, '\0', PI_BUFLEN_16 * sizeof(char));
        snprintf(module_name, PI_BUFLEN_16, "module%d", index);
        /* ��ȡ��ǰ��ģ���ģ���� */
        cur_module = websGetVar(wp, module_name, T(""));

        if(0 == strcmp(cur_module, ""))
        {
            continue;
        }

        for(module_index = 0; cgi_set_public_modules[module_index].type != CGI_NONE; ++module_index)
        {
            /* ���Ҷ�Ӧ������ģ�� */
            if(CGI_SET == cgi_set_public_modules[module_index].type && 0 == strcmp(cur_module, cgi_set_public_modules[module_index].name))
            {
                retval &= CGI_SET_FUN(&cgi_set_public_modules[module_index])(wp, msg_list, err_code,NULL);
                break;
            }
        }
    }

    for(module_index = 0; cgi_set_private_modules[module_index].type != CGI_NONE; ++module_index)
    {
        if(CGI_SET == cgi_set_private_modules[module_index].type && strstr(wp->path, cgi_set_private_modules[module_index].name))
        {
            /*����ǰ��˽��ģ��*/
            memset(private_event,0x0,PI_BUFLEN_32);
            CGI_SET_FUN(&cgi_set_private_modules[module_index])(wp, msg_list, err_code,private_event);
            break;
        }
    }

    _COMMIT();
    /* �ж�˽��ģ���ʱ�䣬��ǰʹ����skip_send����ʶ��˽��ģ�����Ѿ�ִ���˹���
    ģ�����Ϣ���ͣ��˴�����Ҫ������Ϣ���Ͳ��� */

    sprintf(ret_buf, "{\"errCode\":\"%s\"}", err_code);
    websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain; charset=utf-8\nPragma: no-cache\nCache-Control: no-cache\n\n"));
    websWrite(wp,"%s", ret_buf);
    websDone(wp, 200);
	
	for(index = 0; index < MAX_MSG_NUM; ++index)
    {
    	if(msg_list[index].id != 0)
        {
        	printf("=====msg_list[index].id:%d, msg_list[index].msg[%s]==========%s [%d]\n",msg_list[index].id, msg_list[index].msg, __FUNCTION__, __LINE__);
        	msg_send(MODULE_RC, msg_list[index].id, msg_list[index].msg);
        }
    }
	
    return ;
}


