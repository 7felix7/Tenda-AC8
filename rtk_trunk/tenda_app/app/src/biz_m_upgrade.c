#include "biz_m_upgrade.h"
#include "cgi_module_interface.h"

#include <trxhdr.h>


/*************************************************************************
  ����: ��ȡ����������������ĵ�ַ
  ����: download_path_t  ������Ϣ�Ľṹ
  ����ֵ: 0-�ɹ���-1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_ucloud_info_download_path_get (
								  struct download_path_t * dpath)
{
	int ret = 0;
	int len = 0;
	char *path = NULL;
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_UPGRADE_IMAGE_PATH));
	
	ret = app_get_module_param(cj_query,cj_get);

	len = cjson_get_number(cj_get,"uprade_image_path_len",0);  /*�����ַ����*/
	path = cjson_get_value(cj_get,"uprade_image_path",""); /*�����ַ*/
    
	dpath->len = len;      /* ��ַ���� */
	dpath->path = (char *)malloc((len+1) * sizeof(char));
	strncpy(dpath->path, path, len); 
	dpath->path[len] = '\0';

	/*�ͷ��ڴ�*/
	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	
	return ret;
}

/*************************************************************************
  ����: ��������ǰ����ڴ��Ƿ����
  ����: memory_state  ������Ϣ�Ľṹ
  	img_size  ����Ĵ�С
  ����ֵ: 0-�ɹ���-1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_ucloud_info_memory_check_cb (
						struct mem_state_t *memory_state,
						int img_size)
{
	int ret = 0;
	int enough_memory = 0;
	
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	cJSON *extern_data = NULL;
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_UPGRADE_MEMORY_STATE));
    cJSON_AddItemToObject(cj_query, "extern_data", extern_data = cJSON_CreateObject());
	cJSON_AddNumberToObject(extern_data,"img_size",img_size);
	
	ret = app_get_module_param(cj_query,cj_get);

	enough_memory = cjson_get_number(cj_get,"memory_state",0);

	memory_state->enough_memory = enough_memory;

	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	
	return ret;
}

extern int do_upgrade_check(char *stream, int offset_in, int *flash_offset);
extern void uc_ol_upgrade_mem_free(char *head);

/*************************************************************************
  ����: ʵ�����������Ļص�����
  ����: data_len  δʹ�� 
  	 data      �������ݽṹ��ַ
  ����ֵ: 0-�ɹ���-1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_ucloud_begin_upgrade_cb (api_cmd_t *cmd, 
										unsigned int data_len, 
										void *data,
										void *privdata)
{	
	int offset = 0;   //ͷ��ƫ���� ��������data�Ѿ�ȥ��ͷ���˵�
	int flash_offset = 0;

	//��������ʱ��Ϊ����ʾ������ �Ƴ���������������ʱ��
	cyg_thread_delay(100);
		
	tapf_watchdog_disable(); //�رտ��Ź�
	
	char *query = *(size_t *)data;
   
	if(do_upgrade_check(query,offset,&flash_offset)<0)
	{
		printf("[%s][%d]check error!\n",__func__,__LINE__);
		uc_ol_upgrade_mem_free(query);
		sys_reboot();
		return -1;
	}
	
#ifdef REALTEK
	/*realtek ��������ͷ,add*/
	offset += sizeof(struct trx_header);
#endif

	if(tenda_upload_fw(query,offset,flash_offset) < 0)
	{
		printf("[%s][%d]upload error!\n",__func__,__LINE__);
		sys_reboot();
		uc_ol_upgrade_mem_free(query);
		return -1;
	}
	
	uc_ol_upgrade_mem_free(query);   
	sys_reboot();

	return 0;
}

