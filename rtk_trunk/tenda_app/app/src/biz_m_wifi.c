#include "cgi_module_interface.h"

#include "process_api.h"

/*************************************************************************
  ����: ʵ�ֻ�ȡwifi�ź�ǿ�ȵĻص�����
  ����: ��Ҫ��дwifi_power_t�ṹ
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wifi_power_get_cb(const api_cmd_t *cmd, 
					                     wifi_power_t *power,
					                     void *privdata)
{
	int ret = 0;
	cJSON *cj_get	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;

	if (!power) {
		printf("[%s][%d]func param is null\n",__func__,__LINE__);
		return 1;
	}
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();

	if(NULL == cj_query || NULL == cj_get)
	{
		printf("[%s][%d]cj_query or cj_get is null\n",__func__,__LINE__);
		return 1;
	}
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_WL_POWER));
	
	ret = app_get_module_param(cj_query,cj_get);	
	power->wifi_2g_power = cjson_get_number(cj_get,"wifi_2g_power",0);
	power->wifi_5g_power = cjson_get_number(cj_get,"wifi_5g_power",0);
	SET_WIFI_POWER_5G(power);
	
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_get);
	return ret; 
}

/*************************************************************************
  ����: ����wifi�ź�ǿ�Ⱥ�ʹ������Ч�ĺ���
  ����: ��ʹ��
  ����ֵ: ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
#ifdef CONFIG_APP_COSTDOWN
void biz_m_wifi_power_set_process(void)
#else
static void biz_m_wifi_power_set_process(
											const api_cmd_t *cmd,
											int data_len,
											int ret)
#endif
{
	int biz_ret = 100;
	cJSON *cj_set	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
#ifndef CONFIG_APP_COSTDOWN
	cyg_thread_delay(50); /* ��ʱ��Ч����ֹ��������ʱ��ʾʧ������ */
#endif
	
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WL_POWER_PROCESS));
	
	biz_ret = app_set_module_param(cj_query,cj_set);
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);
}

/*************************************************************************
  ����: ʵ������wifi�ź�ǿ�ȵĻص�����
  ����: ��Ҫ����wifi_power_t�����ݵ�·����
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wifi_power_set_cb(const api_cmd_t *cmd, 
					                     wifi_power_t *power,
					                     void *privdata)
{
	int ret = 0;
	cJSON *cj_set	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;

	if (!power) {		
		printf("[%s][%d]func param is null\n",__func__,__LINE__);
 		return 1;
	}
	
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WL_POWER));
	
	cJSON_AddNumberToObject(cj_set,"wifi_2g_power",power->wifi_2g_power);

	if(HAS_WIFI_POWER_5G(power))
	{
		cJSON_AddNumberToObject(cj_set,"wifi_5g_power",power->wifi_5g_power);
	}
	ret = app_set_module_param(cj_query,cj_set);
	
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);

	//3.(��δ��뱣�����ɣ�����Ҫ�޸�)���������ź�ǿ����Ϣ�󣬵�ʹ������Чʱ����ֹ�ֻ�app���·�����Ͽ����ӣ����������ֻ�app�ظ����óɹ���Ϣ����ʹ������Ч��
	// ��ʱ���صĴ�����Ϊ10
	#ifndef CONFIG_APP_COSTDOWN
	wifi_common_ack_t wifi_ack = {0};
	wifi_ack.err_code = 0;
	
	if (ret != 0) {
    	return ret;
	} else {
		uc_api_lib_cmd_resp_notify( cmd, 0, sizeof(wifi_common_ack_t),
					&wifi_ack, biz_m_wifi_power_set_process); /* ֪ͨapp�������óɹ� */
		return COMPLETE_RET; /* 10 */

	}
	#endif
	return ret;

}

/*************************************************************************
  ����: ʵ�ֵõ�wifi������Ϣ�Ļص�����
  ����: ��Ҫ��дwifi_basic_t�ṹ
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wifi_basic_info_get_cb(const api_cmd_t *cmd, 
											  wifi_basic_t *basic_info,
											  void *privdata)
{  
	cJSON *cj_get 	= NULL;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    char *passwd = NULL;
    cJSON *sec_option = NULL;
    int n_sec_option = 0;
    int i = 0;
    cJSON *obj = NULL;
	
	if (!basic_info) 
	{
        printf("func:%s line:%d basic_info if NULL\n",__func__,__LINE__);
 		return 1;
	}
    
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_WIFI_BASIC));

    //���ù���get������ȡ����
	app_get_module_param(cj_query,cj_get);

	/***************2.4G������Ϣ*****************/
    basic_info->wifi_detail[WIFI_2G].type = WIFI_2G;
    SET_WIFI_BASIC_2G(basic_info); 
	
	//2.4G ssid
    strcpy(basic_info->wifi_detail[WIFI_2G].ssid,cjson_get_value(cj_get,"ssid",""));
	
	//wifi����״̬ 0:�� 1:��       
    basic_info->wifi_detail[WIFI_2G].enable= cjson_get_number(cj_get,"enable",1);
    SET_WIFI_BASIC_DETAIL_ENABLE(&basic_info->wifi_detail[WIFI_2G]);  

	/*wifi hide 0:������ 1:����*/
    basic_info->wifi_detail[WIFI_2G].ssid_hide = cjson_get_number(cj_get,"ssid_hide",0);
    SET_WIFI_BASIC_DETAIL_SSID_HIDE(&basic_info->wifi_detail[WIFI_2G]); 
	
	//2.4G password
    passwd = cjson_get_value(cj_get,"passwd","");	
    if(0 != strcmp("",passwd))
    {
        strcpy(basic_info->wifi_detail[WIFI_2G].passwd,passwd);
		/*���û�ȡ2g��ssid�����־λ����Ϊ�����ǿ�ѡ�� */
        SET_WIFI_BASIC_DETAIL_PASSWD(&basic_info->wifi_detail[WIFI_2G]);   
    }
	/*��ȡ���ܷ�ʽ*/
    strcpy(basic_info->wifi_detail[WIFI_2G].sec,cjson_get_value(cj_get,"sec","NONE"));
    SET_WIFI_BASIC_DETAIL_SEC(&basic_info->wifi_detail[WIFI_2G]);
	/*��֧�ּ��ܷ�ʽ*/
    n_sec_option = cjson_get_number(cj_get,"n_sec_option",1); 
    basic_info->n_sec_option = n_sec_option;
    sec_option = cJSON_GetObjectItem(cj_get,"sec_option");  
    for(i = 0; i < n_sec_option; i++)
    {
        obj = cJSON_GetArrayItem(sec_option,i);
        strcpy(basic_info->sec_option[i],cjson_get_value(obj,"sec","NONE"));
    }

	/***************5G������Ϣ*****************/
    basic_info->wifi_detail[WIFI_5G].type = WIFI_5G;
    SET_WIFI_BASIC_5G(basic_info); 
	
	//5G ssid
    strcpy(basic_info->wifi_detail[WIFI_5G].ssid,cjson_get_value(cj_get,"ssid_5g",""));
	
	//wifi����״̬ 0:�� 1:��       
    basic_info->wifi_detail[WIFI_5G].enable= cjson_get_number(cj_get,"enable_5g",1);
    SET_WIFI_BASIC_DETAIL_ENABLE(&basic_info->wifi_detail[WIFI_5G]);  

	/*wifi hide 0:������ 1:����*/
    basic_info->wifi_detail[WIFI_5G].ssid_hide = cjson_get_number(cj_get,"ssid_hide_5g",0);
    SET_WIFI_BASIC_DETAIL_SSID_HIDE(&basic_info->wifi_detail[WIFI_5G]); 
	
	//5G password
    passwd = cjson_get_value(cj_get,"passwd_5g","");	
    if(0 != strcmp("",passwd))
    {
        strcpy(basic_info->wifi_detail[WIFI_5G].passwd,passwd);
		/*���û�ȡ2g��ssid�����־λ����Ϊ�����ǿ�ѡ�� */
        SET_WIFI_BASIC_DETAIL_PASSWD(&basic_info->wifi_detail[WIFI_5G]);   
    }
	/*��ȡ���ܷ�ʽ*/
    strcpy(basic_info->wifi_detail[WIFI_5G].sec,cjson_get_value(cj_get,"sec_5g","NONE"));
    SET_WIFI_BASIC_DETAIL_SEC(&basic_info->wifi_detail[WIFI_5G]);
	/*��֧�ּ��ܷ�ʽ*/
    n_sec_option = cjson_get_number(cj_get,"n_sec_option_5g",1); 
    basic_info->n_sec_option = n_sec_option;
    sec_option = cJSON_GetObjectItem(cj_get,"sec_option_5g");  
    for(i = 0; i < n_sec_option; i++)
    {
        obj = cJSON_GetArrayItem(sec_option,i);   
        strcpy(basic_info->sec_option[i],cjson_get_value(obj,"sec_5g","NONE"));
    }
	
    cJSON_Delete(cj_query);
    cJSON_Delete(cj_get);
     
	return 0;
}

/*************************************************************************
  ����: ����wifi������Ϣ��ʹ������Ч�ĺ���
  ����: ��ʹ��
  ����ֵ: ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
#ifdef CONFIG_APP_COSTDOWN
void biz_m_wifi_basic_info_set_process(void)
#else
static void biz_m_wifi_basic_info_set_process(
											const api_cmd_t *cmd,
											int data_len,
											int ret)
#endif
{
	//���wifi������Ϊ��ʱ������app����ʧ������
#ifndef CONFIG_APP_COSTDOWN
	cyg_thread_delay(50);
#endif

	//int biz_ret = 100;
	cJSON *cj_set	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WIFI_BASIC_PROCESS));
	
	app_set_module_param(cj_query,cj_set);	//���ù�������ʹwifi������Ч
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);
}

/*************************************************************************
  ����: ʵ������wifi������Ϣ�Ļص�����
  ����: ��Ҫ��wifi_basic_t�����ݱ��浽·����
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wifi_basic_info_set_cb(const api_cmd_t *cmd, 
											  wifi_basic_t *basic_info,
											  void *privdata)
{
    int ret = 0;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    cJSON *cj_set = NULL;
	
	if(NULL == basic_info) 
	{
        printf("func:%s line:%d basic is NULL\n",__func__,__LINE__);
 		return 1;
	}
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	if(NULL == cj_query || NULL == cj_set)
	{
		printf("func:%s line:%d cj_query or cj_set created fail NULL\n",__func__,__LINE__);
 		return 1;
	}
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WIFI_BASIC));

	/**********************2.4G��������******************************/
	cJSON_AddStringToObject(cj_set,"ssid",basic_info->wifi_detail[WIFI_2G].ssid); 
	cJSON_AddNumberToObject(cj_set,"enable",basic_info->wifi_detail[WIFI_2G].enable);
	cJSON_AddNumberToObject(cj_set,"ssid_hide",basic_info->wifi_detail[WIFI_2G].ssid_hide);	
    cJSON_AddStringToObject(cj_set,"passwd",basic_info->wifi_detail[WIFI_2G].passwd);
	
	/*app������ ���ܷ�ʽ�·��ĸ�ʽ��WPA-PSK��WPA2-PSK��WPA/WPA2-PSK��NONE*/
	/*ע�⣬��������Ҫ��webҳ���·��ĸ�ʽ����һ�£�WPA/AES��WPA2/AES��WPAWPA2/AES��NONE*/
	cJSON_AddStringToObject(cj_set,"sec",basic_info->wifi_detail[WIFI_2G].sec);

	/**********************5G��������******************************/
	cJSON_AddStringToObject(cj_set,"ssid_5g",basic_info->wifi_detail[WIFI_5G].ssid); 
	cJSON_AddNumberToObject(cj_set,"enable_5g",basic_info->wifi_detail[WIFI_5G].enable);
	cJSON_AddNumberToObject(cj_set,"ssid_hide_5g",basic_info->wifi_detail[WIFI_5G].ssid_hide);	
    cJSON_AddStringToObject(cj_set,"passwd_5g",basic_info->wifi_detail[WIFI_5G].passwd);
	cJSON_AddStringToObject(cj_set,"sec_5g",basic_info->wifi_detail[WIFI_5G].sec);

	//���ù���set������������
    ret = app_set_module_param(cj_query,cj_set);	
   	
    if(0 != ret)	//�жϵײ������Ƿ�ɹ�,0:�ɹ�
    {
        printf("func:%s line:%d set wifi basic is fail \n",__func__,__LINE__);
        cJSON_Delete(cj_query);
        cJSON_Delete(cj_set);
        return 1;
    }
    
	/*3.(��δ��뱣�����ɣ�����Ҫ�޸�)��������wifi��Ϣ��
	    ��ʹ������Чʱ����ֹ�ֻ�app���·�����Ͽ����ӣ����������ֻ�app�ظ����óɹ���Ϣ����ʹ������Ч��*/
	
	// ��ʱ���صĴ�����Ϊ10
	#ifndef CONFIG_APP_COSTDOWN
	wifi_common_ack_t wifi_ack = {0};
	wifi_ack.err_code = 0;
	uc_api_lib_cmd_resp_notify( cmd, 0, sizeof(wifi_common_ack_t),
				&wifi_ack, biz_m_wifi_basic_info_set_process); /* ֪ͨapp�������óɹ� */
	#endif
    
    cJSON_Delete(cj_query);
    cJSON_Delete(cj_set);
#ifdef CONFIG_APP_COSTDOWN
	return 0;
#else
	return COMPLETE_RET; /* 10 */
#endif
 
}

/*************************************************************************
  ����: ��ȡwifi����״̬
  ����: chan ����wifi����״̬
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wifi_channel_sta_get_cb(const api_cmd_t *cmd, 
									       wifi_channel_info_t *chan,
									       void *privdata)
{
	int ret = 0;
	cJSON *cj_get	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;

	if (!chan) {		
		printf("func param is null\n");
		return 1;
	}
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_WL_CHANNEL_GRADE));

	ret = app_get_module_param(cj_query,cj_get);	
	chan->chan_2g_sta = cjson_get_number(cj_get,"channel_2g_sta",15);//Ĭ��ֵΪ15
	
	cJSON_Delete(cj_get);
	cJSON_Delete(cj_query);
	return ret; 
	
}


/*************************************************************************
  ����: ��ʼwifi����
  ����: ��ʹ��
  ����ֵ: ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
#ifdef CONFIG_APP_COSTDOWN
void biz_m_wifi_channel_sta_set_process(void)
#else
static void biz_m_wifi_channel_sta_set_process(
											const api_cmd_t *cmd,
											int data_len,
											int ret)
#endif
{
	cJSON *cj_set	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());	
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WL_CHANNEL_GRADE));

	app_set_module_param(cj_query,cj_set);
	
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);
}

#ifndef CONFIG_APP_COSTDOWN
/*************************************************************************
  ����: ��ʼwifi���ٻص�����
  ����: ��ʹ��
  ����ֵ: 10�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ����Ҫ
 ************************************************************************/
int biz_m_wifi_channel_sta_set_cb(const api_cmd_t *cmd,
									       void *privdata)
{
	//(��δ��뱣�����ɣ�����Ҫ�޸�)wifi���ٹ����У�Ϊ��ֹ�ֻ�app��·�����Ͽ����ӣ����������ֻ�app�ظ����óɹ���Ϣ����ʹ������Ч��
	// ��ʱ���صĴ�����Ϊ10
	wifi_common_ack_t wifi_ack = {0};
	wifi_ack.err_code = 0;
	uc_api_lib_cmd_resp_notify( cmd, 0, sizeof(wifi_common_ack_t),
				&wifi_ack, biz_m_wifi_channel_sta_set_process); /* ֪ͨapp�������óɹ� */
	return COMPLETE_RET; /* 10 */
}
#else
int biz_m_wifi_channel_sta_set_cb(const api_cmd_t *cmd,
									       void *privdata)
{
	return COMPLETE_RET;
}
#endif

/*************************************************************************
  ����: ��ȡ�����͵�wifi��Ϣ
  ����: info ������Ҫ���͵�wifi��Ϣ
  ����ֵ: 0�ɹ� 1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wifi_push_wifi_info_get(wifi_basic_t *info)
{
	int ret = 0;
	cJSON *cj_get	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;

	if (!info) 
	{		
		printf("[%s][%d]func param is null\n",__func__,__LINE__);
		return 1;
	}
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());	
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_WL_PUSH_INFO));

	ret = app_get_module_param(cj_query,cj_get);
	
	//ע��: ����ֻ��Ҫ����ssid��enable
	/****************2.4G*****************/
	info->wifi_detail[WIFI_2G].type = cjson_get_number(cj_get,"wifi_type",0); 
	SET_WIFI_BASIC_2G(info); 
	
	strncpy(info->wifi_detail[WIFI_2G].ssid, cjson_get_value(cj_get,"ssid",""), MAX_SSID_LENGTH); 
	
	info->wifi_detail[WIFI_2G].enable = cjson_get_number(cj_get,"wifi_enable",1); 
    SET_WIFI_BASIC_DETAIL_ENABLE(&(info->wifi_detail[WIFI_2G])); 

	/****************5G*****************/
	info->wifi_detail[WIFI_5G].type = cjson_get_number(cj_get,"wifi_type_5g",1); 
	SET_WIFI_BASIC_5G(info); 
	
	strncpy(info->wifi_detail[WIFI_5G].ssid, cjson_get_value(cj_get,"ssid_5g",""), MAX_SSID_LENGTH); 
	
	info->wifi_detail[WIFI_5G].enable = cjson_get_number(cj_get,"wifi_enable_5g",1); 
    SET_WIFI_BASIC_DETAIL_ENABLE(&(info->wifi_detail[WIFI_5G])); 

	cJSON_Delete(cj_query);
	cJSON_Delete(cj_get);
	
	return ret;
}

//#ifdef __CONFIG_GUEST__
/*�ÿ�������obj-ucloud_costdown.o�����Ƿ�ע���ģ��*/
inline int fill_options(int *outbuf, const int *inbuf, int sz)
{
	int i = 0, num = 0;
	if(NULL == outbuf)
	{
		return 0;
	}
	
	num = sz/sizeof(int);
	if(num <= 0)
	{
		return 0;	
	}
	for(i = 0; i< MAX_INT_OPTION && i < num; i++)
	{
		outbuf[i] = inbuf[i];
	}
		
	return i;
}

/*************************************************************************
  ����: ʵ�ֵõ��ÿ����������Ϣ�Ļص�����
  ����: ��Ҫ��дguest_info_t�ṹ
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wifi_guest_info_get_cb (const api_cmd_t *cmd, 
					guest_info_t *guest_info, void *privdata)
{
	char *guest_ssid = NULL;
	char *guest_sec = NULL;
	char *guest_passwd = NULL;
	guest_detail_t *guest_detail = NULL;
	int ret = 0;
	cJSON *cj_get	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;

	if (!guest_info) 
	{		
		printf("[%s][%d]func param is null\n",__func__,__LINE__);
		return 1;
	}
	
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());	
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_WL_GUEST_INFO));

	ret = app_get_module_param(cj_query,cj_get);

	//����guest ������Ϣ
	guest_detail = &(guest_info->guest_detail[WIFI_2G]);
	
	guest_detail->guest_enable = cjson_get_number(cj_get,"wifi_guest_en",0); /* wifi���� */
	guest_ssid = cjson_get_value(cj_get,"wifi_guest_ssid","Tenda_VIP");
	strncpy(guest_detail->guest_ssid,guest_ssid,sizeof(guest_detail->guest_ssid)-1);
	guest_sec = cjson_get_value(cj_get,"wifi_guest_sec","NONE");
	strncpy(guest_detail->sec,guest_sec,sizeof(guest_detail->sec)-1); 
	SET_WIFI_GUEST_DETAIL_SEC(&(guest_info->guest_detail[WIFI_2G]));
	guest_passwd = cjson_get_value(cj_get,"wifi_guest_passwd","");
	strncpy(guest_detail->guest_passwd,guest_passwd,sizeof(guest_detail->guest_passwd)-1);
	SET_WIFI_GUEST_DETAIL_PWD(guest_detail);	
	guest_detail->type = WIFI_2G;
	SET_WIFI_GUEST_2G(guest_info);
	
    guest_detail = &(guest_info->guest_detail[WIFI_5G]);
	
	guest_detail->guest_enable = cjson_get_number(cj_get,"wifi_guest_en_5g",0); /* wifi���� */
	guest_ssid = cjson_get_value(cj_get,"wifi_guest_ssid_5g","Tenda_VIP_5G");
	strncpy(guest_detail->guest_ssid,guest_ssid,sizeof(guest_detail->guest_ssid)-1);
	guest_sec = cjson_get_value(cj_get,"wifi_guest_sec_5g","NONE");
	strncpy(guest_detail->sec,guest_sec,sizeof(guest_detail->sec)-1); 
	SET_WIFI_GUEST_DETAIL_SEC(&(guest_info->guest_detail[WIFI_2G]));
	guest_passwd = cjson_get_value(cj_get,"wifi_guest_passwd_5g","");
	strncpy(guest_detail->guest_passwd,guest_passwd,sizeof(guest_detail->guest_passwd)-1);
	SET_WIFI_GUEST_DETAIL_PWD(guest_detail);	
	guest_detail->type = WIFI_5G;
	SET_WIFI_GUEST_5G(guest_info);
	
	//timeout
	guest_info->timeout = cjson_get_number(cj_get,"wifi_guest_timeout",0);
	SET_WIFI_GUEST_TIMEOUT(guest_info);
	//rate
	guest_info->rate = cjson_get_number(cj_get,"wifi_guest_rate",0);
	SET_WIFI_GUEST_RATE(guest_info);

	guest_info->rate_uplimit = 128000;
	SET_WIFI_GUEST_RATE_UPLIMIT(guest_info);

	//��ʱ�����б�
	//guest network valid time out choice
	static const int timeout_choice_list[] ={ 0, 4*60, 8*60};
	static const int rate_choice_list[] = {-1, 0, 2*128, 4*128, 8*128}; 

	guest_info->n_rate_option  = fill_options(guest_info->rate_option,rate_choice_list,sizeof(rate_choice_list));
	guest_info->n_timeout_option = fill_options(guest_info->timeout_option,timeout_choice_list,sizeof(timeout_choice_list));
	
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_get);

	return ret;
}

/*************************************************************************
  ����: ʵ�����÷ÿ����������Ϣ�Ļص�����
  ����: ��Ҫ��guest_info_t�����ݱ��浽·����
  ����ֵ: 0�ɹ���1ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_wifi_guest_info_set_cb (const api_cmd_t *cmd, 
					guest_info_t *guest_info, void *privdata)
{
	int ret = 0;
	cJSON *module 	= NULL;
	cJSON *cj_query = NULL;
    cJSON *cj_set = NULL;
	
	if(NULL == guest_info) 
	{
        printf("func:%s line:%d basic is NULL\n",__func__,__LINE__);
 		return 1;
	}
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	if(NULL == cj_query || NULL == cj_set)
	{
		printf("func:%s line:%d cj_query or cj_set created fail NULL\n",__func__,__LINE__);
 		return 1;
	}
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SET_WL_GUEST_INFO));

	//2.4G wifi guest	
	cJSON_AddNumberToObject(cj_set,"wifi_guest_en",guest_info->guest_detail[WIFI_2G].guest_enable);
	cJSON_AddStringToObject(cj_set,"wifi_guest_ssid",guest_info->guest_detail[WIFI_2G].guest_ssid);
	cJSON_AddStringToObject(cj_set,"wifi_guest_passwd",guest_info->guest_detail[WIFI_2G].guest_passwd);
	/*security mode*/
	cJSON_AddStringToObject(cj_set,"wifi_guest_sec", guest_info->guest_detail[WIFI_2G].sec);

	//5G wifi guest	
	cJSON_AddNumberToObject(cj_set,"wifi_guest_en_5g",guest_info->guest_detail[WIFI_5G].guest_enable);
	cJSON_AddStringToObject(cj_set,"wifi_guest_ssid_5g",guest_info->guest_detail[WIFI_5G].guest_ssid);
	cJSON_AddStringToObject(cj_set,"wifi_guest_passwd_5g",guest_info->guest_detail[WIFI_5G].guest_passwd);
	/*security mode*/
	cJSON_AddStringToObject(cj_set,"wifi_guest_sec_5g", guest_info->guest_detail[WIFI_5G].sec);

	/*effect  time*/ 
	cJSON_AddNumberToObject(cj_set,"wifi_guest_timeout", guest_info->timeout);
	/*down speed limit*/
	cJSON_AddNumberToObject(cj_set,"wifi_guest_rate", guest_info->rate);
	
	
	//���ù���set������������
    ret = app_set_module_param(cj_query,cj_set);	
   	
    if(0 != ret)	//�жϵײ������Ƿ�ɹ�,0:�ɹ�
    {
        printf("func:%s line:%d set wifi guest is fail \n",__func__,__LINE__);
        cJSON_Delete(cj_query);
        cJSON_Delete(cj_set);
        return 1;
    }
    
	/*3.(��δ��뱣�����ɣ�����Ҫ�޸�)��������wifi��Ϣ��
	    ��ʹ������Чʱ����ֹ�ֻ�app���·�����Ͽ����ӣ����������ֻ�app�ظ����óɹ���Ϣ����ʹ������Ч��*/
	
	// ��ʱ���صĴ�����Ϊ10
#ifndef CONFIG_APP_COSTDOWN
	wifi_common_ack_t wifi_ack = {0};
	wifi_ack.err_code = 0;
	uc_api_lib_cmd_resp_notify( cmd, 0, sizeof(wifi_common_ack_t),
				&wifi_ack, biz_m_wifi_guest_info_set_process); /* ֪ͨapp�������óɹ� */
#endif
    
    cJSON_Delete(cj_query);
    cJSON_Delete(cj_set);
	
#ifdef CONFIG_APP_COSTDOWN
	return 0;
#else
	return COMPLETE_RET; /* 10 */
#endif	
}

/*************************************************************************
  ����: ����wifi guest������Ϣ��ʹ������Ч�ĺ���
  ����: ��ʹ��
  ����ֵ: ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
#ifdef CONFIG_APP_COSTDOWN
void biz_m_wifi_guest_info_set_process(void)
#else
static void biz_m_wifi_guest_info_set_process(
											const api_cmd_t *cmd,
											int data_len,
											int ret)
#endif
{
	int biz_ret = 100;
	cJSON *cj_set	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
#ifndef CONFIG_APP_COSTDOWN
	cyg_thread_delay(50); /* ��ʱ��Ч����ֹ��������ʱ��ʾʧ������ */
#endif
	
	cj_query = cJSON_CreateObject();
	cj_set = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
	cJSON_AddItemToArray(module,cJSON_CreateString(SER_WL_GUEST_INFO_PROCESS));
	
	biz_ret = app_set_module_param(cj_query,cj_set);
	cJSON_Delete(cj_query);
	cJSON_Delete(cj_set);
}
//#endif

