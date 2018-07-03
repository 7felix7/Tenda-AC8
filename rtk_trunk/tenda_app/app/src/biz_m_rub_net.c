#include "cgi_module_interface.h"
#include "biz_m_rub_net.h"

#define RUB_NET_BLACKLIST_MAX 10 /* ����һ�������������ֵ������10�� */

/*************************************************************************
  ����: ʵ����Ӻ�����
  ����: mac���ڽ��豸��ӵ�������
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
static int biz_m_rub_net_add_blacklist(const char *mac)
{	
	int ret = 0;
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	
	if(NULL == mac)
	{
		printf("[%s][%d]mac is null!\n",__func__,__LINE__);
		return 1;
	}
	
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_RUB_NET_ADD_BLACKLIST));
	cJSON_AddStringToObject(cj_set,"mac",mac);
	
	ret = app_set_module_param(cj_query,cj_set);

	
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
	
	return ret;
}

/*************************************************************************
  ����: ʵ��ɾ��������
  ����: mac���ڽ��豸�Ӻ�������ɾ��
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
static int biz_m_rub_net_delete_blacklist(const char *mac)
{	
	int ret = 0;
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	
	if(NULL == mac)
	{
		printf("[%s][%d]mac is null!\n",__func__,__LINE__);
		return 1;
	}
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_RUB_NET_DEL_BLACKLIST));

	cJSON_AddStringToObject(cj_set,"mac",mac);
	
	ret = app_set_module_param(cj_query,cj_set);
	
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
	
	return ret;
}
/*************************************************************************
  ����: ʵ�ֹر���������
  ����: mac���ڸ��豸������Σ����ε��豸���߾Ͳ�����
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
static int biz_m_rub_net_add_to_trustlist(const char *mac)
{		
	int ret = 0;
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	
	if(NULL == mac)
	{
		printf("[%s][%d]mac is null!\n",__func__,__LINE__);
		return 1;
	}
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_RUB_NET_ADD_TO_TRUSTLIST));

	cJSON_AddStringToObject(cj_set,"mac",mac);

	ret = app_set_module_param(cj_query,cj_set);

	
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
	
	return ret;	
}


/*************************************************************************
  ����: ʵ�ֿ�����������
  ����: mac���ڽ��豸��������ɾ���������ε��豸���ߺ�ͻ�����
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
static int biz_m_rub_net_delete_from_trustlist(const char *mac)
{
	int ret = 0;
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	if(NULL == mac)
	{
		printf("[%s][%d]mac is null!\n",__func__,__LINE__);
		return 1;
	}
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_RUB_NET_DEL_TO_TRUSTLIST));

	cJSON_AddStringToObject(cj_set,"mac",mac);
	
	ret = app_set_module_param(cj_query,cj_set);

	
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);

	return ret;
}

/*************************************************************************
  ����: ʵ����Ӻ�������ɾ������������ӵ������б�ɾ�������б���
  ����: ��Ҫ����access_user_t�е����ݵ�·����
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_access_user_set_cb( const api_cmd_t *cmd,
									     access_user_t *user_info,
										 void *privdata)
{
	int ret = 0;
	//����coverity��������޸ģ�ԭ��Ϊ��Ч���ж�:if (user_info && user_info->mac)  2017/1/11 F9��Ŀ�޸�
	if (user_info != NULL) {
		switch (user_info->op) {
			case RUB_NET_OPT_ADD_TO_BLACKLIST:	/*���������*/
				ret = biz_m_rub_net_add_blacklist(user_info->mac);
			    break;
			case RUB_NET_OPT_RM_FROM_BLACKLIST: /*�Ӻ�������ɾ��*/
				ret = biz_m_rub_net_delete_blacklist(user_info->mac);
				break;
				
			case RUB_NET_OPT_ADD_TO_TRUSTLIST: /* �ر��������� */
				ret = biz_m_rub_net_add_to_trustlist(user_info->mac);
				break;
				
			case RUB_NET_OPT_RM_FROM_TRUSTLIST: /* ������������ */
				ret = biz_m_rub_net_delete_from_trustlist(user_info->mac);
				break;
				
			default:
				ret = -1;
				break;
		}
	}

	return ret;
}
/*************************************************************************
  ����: ʵ�ֻ�ȡ����������
  ����: ��Ҫ����������black_list�ṹ�������漰����̬�ڴ�����
  ����ֵ: 0-�ɹ���1-ʧ�ܣ���80-������������
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_rub_net_black_list_get_cb(
									const api_cmd_t *cmd,
									black_list_t **black_list, 
#ifdef CONFIG_APP_COSTDOWN
									int head_keep_size,
#endif
									void *privdata)
{
	int ret = 0;
    int i = 0;
    int count = 0;
	char *p_a_mem = NULL;
    cJSON *black_listInfo = NULL;
    cJSON *child_obj = NULL;
	
	cJSON *cj_get	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	cJSON *extern_data = NULL;
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_RUB_NET_BLACK_LIST));

    ret = app_get_module_param(cj_query,cj_get);

	if (black_list)
	{
		/*��ȡ����*/	
		black_listInfo = cJSON_GetObjectItem(cj_get,"black_list_info");
		count = cJSON_GetArraySize(black_listInfo);
	
		/* �������ĺ����������Ŀռ� */
#ifdef CONFIG_APP_COSTDOWN
		p_a_mem = (char *)malloc(head_keep_size + sizeof(black_list_t)+count*sizeof(black_user_t)); 
		if (!p_a_mem) 
#else
		*black_list = (black_list_t *)malloc(sizeof(black_list_t)+count*sizeof(black_user_t)); 
		if (!(*black_list)) 
#endif
		{
		    printf("black_list alloc mem is fail\n");
			*black_list= NULL;
			ret = 1; /* �ڲ�ʵ�ִ��� */
			goto out;
		}
#ifdef CONFIG_APP_COSTDOWN
		*black_list = p_a_mem + head_keep_size;
#endif
		memset(*black_list, 0, sizeof(black_list_t)+count*sizeof(black_user_t));
		
        black_user_t *black_user_list = (*black_list)->buser;

		(*black_list)->n_mac = count; /*��������*/
		
		for(i = 0; i < count; i++)
		{		
		    child_obj = cJSON_GetArrayItem(black_listInfo,i);

			strcpy(black_user_list[i].mac,cjson_get_value(child_obj, "mac",""));  /*mac*/		
			strcpy(black_user_list[i].dev_name,cjson_get_value(child_obj, "dev_name",""));  /*dev_name*/

			//printf("====>>>>black_list_mac:%s,black_list_dev_name:%s\n",black_user_list[i].mac,black_user_list[i].dev_name);
		}	
	}

out:
	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);

	return ret;
}

/*************************************************************************
  ����: ��ȡ�����ߵ�İ��������Ϣ
  ����: info ��İ��������Ϣ���뵽�ýṹ����
  ����ֵ: 0-�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_strange_host_info_get(rub_strange_host_info_t **rub_info,int *n_host)
{
	int ret = 0;
	cJSON *cj_get	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	cJSON *extern_data = NULL;
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_STRANGE_INFO));

    ret = app_get_module_param(cj_query,cj_get);

	int i = 0;
	int count = 0;
    int len = 0;

	cJSON *strange_host_info = NULL;
	cJSON *child_obj = NULL;	

	strange_host_info = cJSON_GetObjectItem(cj_get,"strang_host_info");
	if(NULL == strange_host_info)
	{
		cJSON_Delete(cj_get);
		cJSON_Delete(cj_query);
		return 1;
	}
	count = cJSON_GetArraySize(strange_host_info); /*İ����������*/
	
	if(count > 0)
	{
		len = count * sizeof(rub_strange_host_info_t);
		*rub_info= (rub_strange_host_info_t *)malloc(len);	
		memset(*rub_info, 0, len);

		*n_host = count;
		for(i = 0; i < count; i++)
	    {	
	    	child_obj = cJSON_GetArrayItem(strange_host_info,i);

			strcpy((*rub_info)[i].dev_name,cjson_get_value(child_obj, "strange_name",""));  /*mac*/	
			strcpy((*rub_info)[i].mac,cjson_get_value(child_obj, "strange_mac",""));  /*mac*/		
			strcpy((*rub_info)[i].date,cjson_get_value(child_obj, "strange_date",""));  /*date*/
			strcpy((*rub_info)[i].time,cjson_get_value(child_obj, "strange_time",""));  /*time*/
			strcpy((*rub_info)[i].ssid,cjson_get_value(child_obj, "strange_ssid","")); /*ssid*/
			
			(*rub_info)[i].type = cjson_get_number(child_obj, "strange_type",0);
			
			strcpy((*rub_info)[i].serial_num,cjson_get_value(child_obj, "uc_serialnum",""));
			
			(*rub_info)[i].dtype = cjson_get_number(child_obj, "strange_dtype",0);
		}	
	}

	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	
	return ret;
}

/*************************************************************************
  ����: ��ȡ�ڰ�����ģʽ
  ����: mode->enable (mac ����ģʽ�Ƿ�򿪣�1-������0-δ����)
  	mode->mac_mode	(��ǰ��Ч�ĺڰ�����ģʽ��1-��������2-������)
  	mode->supt_mac_mode (·����֧�ֵ�ģʽ��1-��֧�ֺ�������2-��֧�ְ�������3-�ڰ�������֧��)
  ����ֵ: 0-�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_rub_net_mf_mode_get_cb(mac_filter_mode_t * mode, 
											  void * privdata)
{
	cJSON *cj_query = NULL;
	cJSON *cj_get = NULL;
	cJSON *module = NULL;
	int ret = 0;

	if (mode == NULL) {
		return -1;
	}

	cj_query = cJSON_CreateObject();
	if(NULL == cj_query)
	{
		printf("line:%d create fail\n",__LINE__);
		return 1;
	}
	
	cj_get = cJSON_CreateObject();
	if(NULL == cj_get)
	{
		cJSON_Delete(cj_query);
		printf("line:%d create fail\n",__LINE__);
		return 1;
	}
	
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_MAC_FILTER_MODE));

	ret = app_get_module_param(cj_query,cj_get);	//���û�ȡ�Ĺ�������

	mode->enable = cjson_get_number(cj_get,"enable",1);
	SET_MACFILTER_ENABLE(mode);

	mode->mac_mode = cjson_get_number(cj_get,"mac_mode",1); /* ��ǰ��Ч�ĺڰ�����ģʽ��1-��������2-������ */
	SET_MACFILTER_MODE(mode);

	
	mode->supt_mac_mode = cjson_get_number(cj_get,"supt_mac_mode",1); /* ·����֧�ֵ�ģʽ��1-��֧�ֺ�������2-��֧�ְ�������3-�ڰ�������֧�� */
	SET_MACFILTER_SUPT_MODE(mode);

	cJSON_Delete(cj_query);
	cJSON_Delete(cj_get);
	return 0;
}

/*************************************************************************
  ����: ���úڰ�����ģʽ
  ����: mode->mac_mode	(��ǰ��Ч�ĺڰ�����ģʽ��1-��������2-������)
  ����ֵ: 0-�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/

int biz_m_rub_net_mf_mode_set_cb(mac_filter_mode_t * mode,
											  void * privdata)
{
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	int ret = 0;

	if (mode == NULL) {
		return -1;
	}

	cj_query = cJSON_CreateObject();
	if(NULL == cj_query)
	{
		printf("create cj_query fail\n");
		return 1;
	}
	cj_set = cJSON_CreateObject();
	if(NULL == cj_set)
	{
		printf("create cj_set fail\n");
		cJSON_Delete(cj_query);
		return 1;
	}
	
	if (HAS_MACFILTER_ENABLE(mode))  //��̨��ʱ��֧������ֶΣ�ʲôҲ����
	{
		
	}
	if (HAS_MACFILTER_MODE(mode)) 
	{
		if((2 == mode->mac_mode) || (1 == mode->mac_mode))
		{
			cJSON_AddNumberToObject(cj_set,"mac_mode",mode->mac_mode);
		}
		else
		{
			cJSON_Delete(cj_query);
			cJSON_Delete(cj_set);
			return 1;
		}
	}

	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_MAC_FILTER_MODE));
	

    ret = app_set_module_param(cj_query,cj_set);

	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);
	
	return 0;
}

/*************************************************************************
  ����: ��պ�����
  ����: ��
  ����ֵ: 0-�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_rub_net_black_list_flush_cb(void * privdata)
{
	int ret = 0;
	cJSON *module = NULL;
	cJSON *cj_query = NULL;
	cJSON *cj_set = NULL;

	cj_query = cJSON_CreateObject();
	if(NULL == cj_query)
	{
		printf("line[%d] create cj_query fail\n",__LINE__);
		return 1;
	}
	cj_set = cJSON_CreateObject();
	if(NULL == cj_set)
	{
		cJSON_Delete(cj_query);
		printf("line[%d] create cj_set fail\n",__LINE__);
		return 1;
	}

	cJSON_AddItemToObject(cj_query,"module",module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_RUB_NET_FLUSH_BLACKLIST));

	ret = app_set_module_param(cj_query,cj_set);
	
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);
	/* ������к������е��豸 */
	return 0;
}

