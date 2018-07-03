#include "cgi_lib.h"

extern CGI_LIB_MODULE_INFO cgi_lib_get_modules[];
extern CGI_LIB_MODULE_INFO cgi_lib_set_modules[];
/*****************************************************************************
 �� �� ��  : cgi_lib_get
 ��������  : ����lib�⺯���Ľӿں�����������Ҫ���õ�ģ��������
 �������  : CGI_LIB_INFO get_info  
             void *info             
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��15��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_get(CGI_LIB_INFO get_info, void *info)
{
	PIU8 index = 0;
	PIU8 module_serch = 0;
	PIU8 module_index = 0;
	while(index < get_info.module_num)
	{
		module_index = get_info.modules[index];
		module_serch = 0;
		while(module_serch < MODULE_GET_END)
		{
			if(cgi_lib_get_modules[module_serch].module_index == module_index)
			{
				CGI_LIB_GET_FUN(cgi_lib_get_modules[module_serch])(get_info.wp,get_info.root, info);
				break;
			}
			module_serch++;
		}
		index++;
	}
	return RET_SUC;
}
/*****************************************************************************
 �� �� ��  : cgi_lib_set
 ��������  : ����lib�����Ľӿں������ڽ�������ʱ���õģ�ҳ��ʹ��wp��appʹ��-
             root
 �������  : CGI_LIB_INFO set_info  
             CGI_MSG_MODULE *msg    
             char *err_code         
             void *info             
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��15��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_set(CGI_LIB_INFO set_info,CGI_MSG_MODULE *msg, char *err_code, void *info)
{
	PIU8 index = 0;
	PIU8 module_serch = 0;
	PIU8 module_index = 0;
	while(index < set_info.module_num)
	{
		module_index = set_info.modules[index];
		module_serch = 0;
		while(module_serch < MODULE_SET_END)
		{
			if(cgi_lib_set_modules[module_serch].module_index == module_index)
			{
				CGI_LIB_SET_FUN(cgi_lib_set_modules[module_serch])(set_info.wp,set_info.root,msg,err_code, info);
			}
			module_serch++;
		}
		index++;
	}
	return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_lib_get_var
 ��������  : ����ԭ�е�websGetvar����������app
 �������  : webs_t wp                
             cJSON*root               
             char_t *var              
             char_t *defaultGetValue  
 �������  : ��
 �� �� ֵ  : char_t
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��15��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
char_t *cgi_lib_get_var(webs_t wp,cJSON*root, char_t *var, char_t *defaultGetValue)
{
	char* value = NULL;
	cJSON* Item = NULL;
	if(wp != NULL)
	{
		value =  websGetVar(wp, var, defaultGetValue);
	}
	else if(root != NULL)
	{
		Item = cJSON_GetObjectItem(root,var);
		if(Item == NULL)
		{
			cJSON_AddStringToObject(root,var,defaultGetValue);
			Item = cJSON_GetObjectItem(root,var);	
		}
		value = Item->valuestring;
	}
	else
		value = NULL;
	return value;
}


