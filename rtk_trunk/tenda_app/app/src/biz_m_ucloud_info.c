#include "cgi_module_interface.h"
#include "biz_m_ucloud_info.h"

/*************************************************************************
  ����: �����ƹ���sn���к�
  ����: data  sn���к�ָ��
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_ucloud_info_sn_set_cb(api_cmd_t *cmd, 
						   					unsigned int data_len,
						   					void *data,
						   					void *privdata)
{
	int ret = 0;
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	
	if (!data) 
	{
		printf("[%s][%d]data is null!\n",__func__,__LINE__);
		return 1;
	}
    char *sn = (char *)data;
	
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_UCLOUD_INFO_SN));

	cJSON_AddStringToObject(cj_set, "uc_serialnum",sn);
	
	
	ret = app_set_module_param(cj_query,cj_set);
	
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
	
	return ret;
}
/*************************************************************************
  ����: ��ȡ�ƹ��������Ϣ
  ����: data  �����ƹ��������Ϣ
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_ucloud_info_basic_get(cloud_basic_info_t *data)
{
	int ret = 0;
	cJSON *cj_get	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	
    if (!data)
	{
		return 1;
	}
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_UCLOUD_BASIC_INFO));
	
	ret = app_get_module_param(cj_query,cj_get);
    /* �ƹ����Ƿ��� 1-����0-�� */
	data->enable = cjson_get_number(cj_get,"uc_manage_en",1);
	/* �ƹ���sn���к� */
	strncpy(&data->sn[0], cjson_get_value(cj_get,"uc_serialnum",""), sizeof(data->sn));
	/*mac ȥ��ð�ź��mac*/
	strncpy(&data->mac[0], cjson_get_value(cj_get,"host_mac",""), sizeof(data->mac));
	/*����汾��*/
	strncpy(&data->ver[0],cjson_get_value(cj_get,"version",""), sizeof(data->ver));
	/*��˾*/
	strncpy(&data->company[0], cjson_get_value(cj_get,"company","Tenda"), sizeof(data->company));
	/*/�ͺ�*/
	strncpy(&data->product[0], 	cjson_get_value(cj_get,"product","F9"), sizeof(data->product));
	//�������� ����ʱ ͨ���ű������������
	strncpy(&data->ver_passwd[0], cjson_get_value(cj_get,"update_random",""), sizeof(data->ver_passwd)); 
	//strcpy(&data->ver_passwd[0], "cpoej5");
	
	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	
	return ret;  
}

/*************************************************************************
  ����: ��ȡ�ƹ�����
  ����: manage_en �����ƹ�����״̬
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_ucloud_info_manage_en_get_cb(
						   const api_cmd_t *cmd,
						   m_cloud_info_manage_en_t *manage_en,
						   void *privdata)
{

	int ret = 0;
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	
	int uc_enable = 1;//�ƹ���Ĭ�Ͽ���
	
	if (!manage_en) 
	{
		printf("[%s][%d]manage_en is null!\n",__func__,__LINE__);
		return 1;	
	} 

	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_UCLOUD_INFO_MESSAGE_EN));
	
	ret = app_get_module_param(cj_query,cj_get);

	uc_enable = cjson_get_number(cj_get,"uc_manage_en",1);
	manage_en->en = uc_enable;
	
	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	return ret;
}

/*************************************************************************
  ����: �����ƹ�����
  ����: manage_en ���������ƹ�����״̬
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_ucloud_info_manage_en_set_cb(
							   const api_cmd_t *cmd,
							   m_cloud_info_manage_en_t *manage_en,
							   void *privdata)
{
	int ret = 0;
	cJSON *cj_set	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	
	if (!manage_en) 
	{
		return 1;	
	} 

	int en = !! manage_en->en;
	char buf[8] = {0};
	snprintf(buf, 8, "%d", en);
	
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_UCLOUD_INFO_MESSAGE_EN));

	cJSON_AddStringToObject(cj_set, "uc_manage_en",buf);
	
	
	ret = app_set_module_param(cj_query,cj_set);
	
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
	return ret;
}

/*************************************************************************
  ����: �����ж��Ƿ���Ҫ������˺�
  ����: ����
  ����ֵ: 0-�ɹ���-1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
static int biz_ucloud_info_send_clear_acc(int already_exec)
{
	if (!already_exec)
	{
		if (-1 == uc_api_m_cloud_info_send_clear_account()) 
		{
			printf("[%s][%d]send clear account failure!\n",__func__,__LINE__);
			return -1;
		}
	}
	return 0;
}

/*************************************************************************
  ����: �����ж��Ƿ���Ҫ������˺�
  ����: ����
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
#ifdef CONFIG_APP_COSTDOWN
int biz_ucloud_info_try_clear_acc(int *need_clear)
#else
int biz_ucloud_info_try_clear_acc()
#endif
{
	int ret = 0;
	char value[32] = {0};
	int clear;
	
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_UCLOUD_INFO_TRY_CLEAR));
	
	ret = app_get_module_param(cj_query,cj_get);

	strncpy(value, cjson_get_value(cj_get,"ucloud_need_clear_acc",""), sizeof(value));
	clear = !!atoi(value);
	
	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
#ifdef CONFIG_APP_COSTDOWN
	if (need_clear)
		*need_clear = !clear;
	return ret;
#else
	return biz_ucloud_info_send_clear_acc(clear);
#endif
}

/*************************************************************************
  ����: ������˺ź��������˺�����ձ�־
  ����: ����
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_cloud_info_clear_account_ack_cb (
										const api_cmd_t *cmd, 
										void *privdata)
{
	int ret = 0;
	cJSON *cj_set	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_UCLOUD_INFO_CLEAR_ACCOUNT));

	cJSON_AddStringToObject(cj_set, "ucloud_need_clear_acc","1");
	
	
	ret = app_set_module_param(cj_query,cj_set);
	
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);		
	return ret;
}


