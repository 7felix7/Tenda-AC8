#include "cgi_lib.h"
#if 0
PIU8 set_modules_macFilter[] =
{
    MODULE_SET_MACFILTER,
    MODULE_SET_END,
};

PIU8 get_modules_macFilter[] =
{
    MODULE_GET_MACFILTER_MODE,
    MODULE_GET_MACFILTER_LIST,
    MODULE_GET_END,
};
#endif
/*****************************************************************************
 �� �� ��  : cgi_macfilter_set
 ��������  : ����mac���˵�ui�ӿں���
 �������  : webs_t wp
             CGI_MSG_MODULE *msg
             char *err_code
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��11��27��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO  cgi_macfilter_set(webs_t wp, CGI_MSG_MODULE *msg, char *err_code, void *info)
{
    cJSON *obj = NULL;
    CGI_LIB_INFO set_info;
    PIU8 modules[] =
    {
        MODULE_SET_MACFILTER,
    };

    set_info.wp = wp;
    set_info.root = NULL;
    set_info.modules = modules;
    set_info.module_num = ARRAY_SIZE(modules);
    cgi_lib_set(set_info,msg,err_code,NULL);
    return RET_SUC;
}
/*****************************************************************************
 �� �� ��  : cgi_get_macfilter_list
 ��������  : ҳ���ȡmacfilter�б�ĺ����ӿڣ���������AP��routeģʽ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��11��27��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_get_macfilter_list(webs_t wp, cJSON *root, void *info)
{
    cJSON *obj = NULL;
    CGI_LIB_INFO get_info;
    PIU8 modules[] =
    {
        MODULE_GET_MACFILTER_MODE,
        MODULE_GET_MACFILTER_LIST,
    };

    cJSON_AddItemToObject(root, T("macFilter"), obj = cJSON_CreateObject());

    get_info.wp = wp;
    get_info.root = obj;
    get_info.modules = modules;
    get_info.module_num = ARRAY_SIZE(modules);
    cgi_lib_get(get_info,NULL);
    return RET_SUC;
}

