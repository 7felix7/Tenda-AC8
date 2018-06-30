#include "cgi_module_interface.h"

#include "biz_m_sys.h"
#include "biz_m_ucloud_info.h"


/*************************************************************************
  ����: ��ȡϵͳ������Ϣ
  ����: basic ����ϵͳ������Ϣ����
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_sys_basic_info_get_cb(const api_cmd_t *cmd, 
				   						    sys_basic_info_t *basic,
				   							void *privdata)
{

	int ret = 0;
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;

	if (!basic) {
		printf("func param is null\n");
		return 1;
	}
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_SYS_BASIC_INFO));

	app_get_module_param(cj_query,cj_get);
	strcpy(basic->sn, cjson_get_value(cj_get, "sn_number", ""));  /* ��ȡ�ƹ���sn���к� */
	strcpy(basic->mac, cjson_get_value(cj_get,"mac", "")); /* mac��ַ */
	basic->wl_mode = 	cjson_get_number(cj_get,"wl_mode",0); /* �����м�ģʽ */
	strcpy(basic->product.hard_ver, cjson_get_value(cj_get,"hard_ver", ""));     /* Ӳ���汾�ţ�û������ַ��� */
	basic->product.release_date = cjson_get_number(cj_get,"release_date",0);  /* �������ʱ���ɵ�ʱ�䣬ʱ��� */
	strcpy(basic->product.firm, cjson_get_value(cj_get,"product_firm", "")); /* ��˾���� */
	strcpy(basic->product.model, cjson_get_value(cj_get, "product_model", "")); /* ��Ʒ��F9 */
	snprintf((basic->product).soft_ver, sizeof((basic->product).soft_ver), ("%s"), cjson_get_value(cj_get,"soft_ver", ""));
	basic->init.guide_done = cjson_get_number(cj_get,"guid_done", 1);	/* �Ƿ���й������� 1-���й� 0-δ���й� */
	strcpy(basic->supt_wan_type, cjson_get_value(cj_get,"supt_wan_type", "")); /* wan��֧�ֵĽ������� */
	basic->dev_mode = cjson_get_number(cj_get,"dev_mode", DEV_MODE_ROUTER); /* DUT��ǰ�Ƿ�ΪAPģʽ */
	SET_SUPT_WAN_TYPE(basic);

	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	return 0;

}

/*************************************************************************
  ����: ��ȡϵͳ�߼���Ϣ
  ����: advance ����ϵͳ�߼���Ϣ����
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_sys_advance_info_get_cb(const api_cmd_t *cmd, 
                   						 	sys_advance_info_t *advance,
                   						    void *privdata)
{
   	int ret = 0;
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;

	if (!advance) {
		printf("func param is null\n");
		return 1;
	}
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_SYS_ADVANCE_INFO));

	app_get_module_param(cj_query,cj_get);
	memset(advance, 0, sizeof(sys_advance_info_t)); 	
	advance->systime = cjson_get_number(cj_get,"systime",0); /* unixʱ��� */
	advance->uptime = cjson_get_number(cj_get,"uptime",0);  /* ϵͳ��������ʱ�����ڵ�ʱ�� s*/
	advance->cpu_info.max_freq = cjson_get_number(cj_get,"cpu_max_freq",0);  /* cpuƵ�� MHz */ 
	advance->cpu_info.curr_idle = cjson_get_number(cj_get,"cpu_curr_idle",0); /* cpu�����ʣ�60% */
	advance->mem_info.total = cjson_get_number(cj_get,"mem_total",0); /* ���ڴ��С KB*/
	advance->mem_info.used =cjson_get_number(cj_get,"mem_used",0);   /* ʹ���˶����ڴ� KB*/

	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	return ret;
}
#ifdef CONFIG_APP_COSTDOWN
void biz_m_sys_reset(void)
{
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_RESET));
	cJSON_AddStringToObject(cj_set, "action","restore");
	
	app_set_module_param(cj_query,cj_set);
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
}

void biz_m_sys_reboot(void)
{
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_REBOOT));
	cJSON_AddStringToObject(cj_set, "action","reboot");
	
	app_set_module_param(cj_query,cj_set);
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
}

#else
/*************************************************************************
  ����:ʵ�ָֻ������ĺ���
  ����: ����Ҫʹ��
  ����ֵ: ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
static void biz_m_sys_reset(
	const api_cmd_t * cmd, 
	int data_len, 
	int ret)
{
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_RESET));
	cJSON_AddStringToObject(cj_set, "action","restore");
	
	app_set_module_param(cj_query,cj_set);
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
}

/*************************************************************************
  ����: ʵ�ָֻ������Ļص�����
  ����: ����Ҫʹ��
  ����ֵ: Ĭ�Ϸ��� 10
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_sys_reset_cb(const api_cmd_t *cmd, void *privdata)
{
	sys_common_ack_t sys_ack;
	memset(&sys_ack, 0, sizeof(sys_ack));
	sys_ack.err_code = 0;
	uc_api_lib_cmd_resp_notify(cmd, 0, sizeof(sys_ack), &sys_ack, biz_m_sys_reset); /* �ڻָ�����֮ǰ֪ͨ�ֻ�app�ָ������·��ɹ� */
	return COMPLETE_RET; /* 10 */
}

/*************************************************************************
  ����:ʵ��·���������Ĺ��ܵ�ʵ�ֺ���
  ����: ����Ҫʹ��
  ����ֵ: ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
static void biz_m_sys_reboot(
	const api_cmd_t * cmd, 
	int data_len, 
	int ret)
{
	cJSON *cj_set 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query 	= NULL;
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_REBOOT));
	cJSON_AddStringToObject(cj_set, "action","reboot");
	
	app_set_module_param(cj_query,cj_set);
	cJSON_Delete(cj_set);
	cJSON_Delete(cj_query);
}

/*************************************************************************
  ����: ʵ�������Ļص�����
  ����: ����Ҫʹ��
  ����ֵ: Ĭ�Ϸ��� 10
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_sys_reboot_cb(const api_cmd_t *cmd, void *privdata)
{
	sys_common_ack_t sys_ack;
	memset(&sys_ack, 0, sizeof(sys_ack));
	sys_ack.err_code = 0;
	uc_api_lib_cmd_resp_notify(cmd, 0, sizeof(sys_ack), &sys_ack, biz_m_sys_reboot); /* �ڻָ�����֮ǰ֪ͨ�ֻ�app�ָ������·��ɹ� */
	return COMPLETE_RET; /* 10 */
}
#endif
