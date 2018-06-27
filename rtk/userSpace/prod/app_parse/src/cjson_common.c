#include "cJSON.h"
/*****************************************************************************
 �� �� ��  : cjson_get_number
 ��������  : ��json���ݸ�ʽ�а�ȫ�Ķ�ȡ���֣����һ��������Ĭ��ֵ
 �������  : cJSON *root          
             char *name           
             int defaultGetValue  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��26��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
int cjson_get_number(cJSON *root, char *name, int defaultGetValue)
{
	if (!name)
	{	
		return -1;
	}

	if(!root)
	{
		printf("root is NULL!!! get name %s\n",name);	
		return defaultGetValue;
	}
	cJSON *obj = cJSON_GetObjectItem(root, name);

	if (!obj)
	{
		return defaultGetValue;
	}
	else
	{
		return obj->valueint;
	}
}

/*****************************************************************************
 �� �� ��  : cjson_get_value
 ��������  : ��json���ݸ�ʽ�а�ȫ�Ķ�ȡ�ַ��������һ��������Ĭ��ֵ
 �������  : cJSON *root            
             char *name             
             char *defaultGetValue  
 �������  : ��
 �� �� ֵ  : char
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��26��
    ��    ��   : �ξ���
    �޸�����   : �����ɺ���

*****************************************************************************/
char *cjson_get_value(cJSON *root, char *name, char *defaultGetValue)
{
	if (!name)
	{
		return NULL;
	}

	if(!root)
	{
		printf("root is NULL!!! get name %s\n",name);	
		return defaultGetValue;
	}
	cJSON *obj = cJSON_GetObjectItem(root, name);

	if (!obj)
	{
		//printf("__%s__obj is not live",name);
		return defaultGetValue;
	}
	else
	{
		if (obj->valuestring)
		{
			//APP_MODULE_DEBUG("___in cjson_get_value  %s = %s\n", name, obj->valuestring);
			return obj->valuestring;
		}
		else
		{
			return "";
		}
	}
}

