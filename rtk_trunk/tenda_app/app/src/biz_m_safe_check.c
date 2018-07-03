#include "cgi_module_interface.h"

#include "biz_m_safe_check.h"

/*************************************************************************
  ����:  wifi����ǿ�ȼ��
  ����: ��Ҫ����string
  ����ֵ: 0-û�����룬1-����ȫ��2-��ȫ
  �Ƿ���Ҫ�û�ʵ��: ��
************************************************************************/
static int biz_check_wifi_password_strength(char *string)
{
	
	int i;
	char *simple_wifi_passwd[16] = {
		"123456", "12345678", "abc123", "password", "123456789", 
		"12345",  "88888888", "654321", "87654321", "666666",
		"666666","admin","root","987654321","11223344","0123456789"};
	
	if (string == NULL || strlen(string) == 0) {
		return 0;
	}
	
	
	for (i = 0; i < 16; i ++) {
		if (strcmp(simple_wifi_passwd[i], string) == 0) {
			return 1;
		}
	}
	
	return 2;
	
}

/*************************************************************************
  ����:  web��¼���밲ȫ��� 
  ����: ��Ҫ����string
  ����ֵ: 0-û�����룬1-����ȫ��2-��ȫ
  �Ƿ���Ҫ�û�ʵ��: ��
************************************************************************/
static char wifi_password[65]={0};
static int biz_check_web_login_password_strength(char *string)
{
	int i;
	char *simple_login_passwd[16] = {
		"123456", "12345678", "abc123", "password", "123456789", 
		"12345",  "88888888", "654321", "87654321", "666666",
		"666666","admin","root","987654321","11223344","0123456789"};

		
	if (string == NULL || strlen(string) == 0) {
		return 0;
	}

	if(strcmp(string,wifi_password) == 0){
		return 0x110;
	}
	
	for (i = 0; i < 16; i ++) {
		if (strcmp(simple_login_passwd[i], string) == 0) {
			return 1;
		}
	}
	
	return 2;
}

/*************************************************************************
  ����: ʵ�ְ�ȫ���Ļص�����
  ����: ��Ҫ����safe_check_info_t
  ����ֵ: �ɹ�����0��ʧ�ܷ���1
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_safe_check_get_cb(const api_cmd_t *cmd, 
					safe_check_info_t *info, void *privdata)
{

	char login_depw[64] = {0},wl_password[65] = {0};
	int ret = 0;
	cJSON *cj_get	= NULL;
	cJSON *module	= NULL;
	cJSON *cj_query 	= NULL;
	cj_query = cJSON_CreateObject();
	cj_get = cJSON_CreateObject();
	cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());	
	cJSON_AddItemToArray(module,cJSON_CreateString(GET_SAFE_GRADE));

	ret = app_get_module_param(cj_query,cj_get);	
	strcpy(wl_password,cjson_get_value(cj_get, "wifi_password", ""));
	strcpy(login_depw,cjson_get_value(cj_get, "decode_login_password", ""));
	strcpy(wifi_password,wl_password);


	/* ��� web��¼�������� 0-û�����룬1-����ȫ��2-��ȫ 0x110 -��wifi����һ��*/
	if(biz_check_web_login_password_strength(login_depw) == 0)
	{
		info->auth_pwd_sta = 0; 
	}
	else if(biz_check_web_login_password_strength(login_depw) == 1)
	{
		info->auth_pwd_sta = 1;
	}		
	else if(biz_check_web_login_password_strength(login_depw) == 0x110)
	{
		info->auth_pwd_sta = 0x110;
	}
	else
	{
		info->auth_pwd_sta = 2;
	}

	
	/* ��� wifi 2.4g����ļ����  0-û�����룬1-����ȫ��2-��ȫ  */	
	if(biz_check_wifi_password_strength(wl_password) == 0)
	{
		info->wifi_24_pwd_sta = 0;
	}
	else if(biz_check_wifi_password_strength(wl_password) == 1)
	{
		info->wifi_24_pwd_sta = 1;
	}		
	else
	{
		info->wifi_24_pwd_sta = 2;
	}		

	/* ��� �Ƿ���dns�ٳ� 0-��ȫ, 1����ȫ  */ 
	info->dns_hijack_sta = 0;   

/*�����Ǳ����ӿ�*/
#if 0	
	info->wifi_50_pwd_sta = 1;  /* wifi 5g����ļ���� 0-û�����룻1-����2-�У�3-ǿ��4-�� */
	SET_SAFE_CHECK_INFO_WIFI_50_PWD(info); /* ������5g, �������øñ�־λ, ��Ϊwifi_50_pwd_sta�ֶ��ǿ�ѡ�� */
	
	info->ddos_attack_sta = 1;  /* �Ƿ���ddos���� 0-��ȫ Ĭ����0 */
	SET_SAFE_CHECK_INFO_DDOS_ATTACK(info); /* ������ddos����, �������øñ�־λ����Ϊddos_attack_sta�ֶ��ǿ�ѡ�� */
#endif

    cJSON_Delete(cj_query);
	cJSON_Delete(cj_get);

	return ret;
}

/*************************************************************************
  ����:  dns�Ż�
  ����:  ����Ҫ
  ����ֵ: 0-�ɹ� 1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
************************************************************************/
int biz_m_dns_optimize(const api_cmd_t *cmd, 
							   void *privdata)
{
 	printf("\n-----biz_m_dns_optimize-----\n");
	printf("\n");
	return 0;
}


