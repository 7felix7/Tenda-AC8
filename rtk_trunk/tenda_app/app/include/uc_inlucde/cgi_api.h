#ifndef	__CGI_API_H__
#define __CGI_Api_H__

typedef enum {
	VER_TYPE_MIN = 0,
	VER_TYPE_SUCCESS = VER_TYPE_MIN,	//��ȡ�°汾��Ϣ�ɹ�
	VER_TYPE_NO_VER,					//������û���°汾
	VER_TYPE_NO_CUR_VER,                         //��������ʱ���޷���ȡ��ǰ�汾��Ϣ
	VER_TYPE_MAX
}ENUM_VER_TYPE;

typedef enum {
	UP_TYPE_MIN = 0,
	UP_TYPE_UPGRADE_Q = UP_TYPE_MIN,	//����ѯ������������
	UP_TYPE_MEM_NOT_ENOUGH,				//�ڴ治��,�޷�����
	UP_TYPE_WAIT_SVR,					//·�������Ŷ�����
	UP_TYPE_FW_DOWNING,					//·�����������ع̼�
	UP_TYPE_WRITING_TO_FLASH,			//·����������д�̼�
	UP_TYPE_MAX
}ENUM_UP_TYPE;

typedef enum {
	OPT_TYPE_MIN = 0,
	OPT_TYPE_OL_UPGRADE = 1,
	OPT_TYPE_OL_POLICY_UPGRADE = 4,
	OPT_TYPE_MAX
}OPT_TYPE;

//ѯ���°汾ʱ�յ��Ļظ�����
typedef struct query_version_s{
	ENUM_VER_TYPE	 type;
	char data[0];
}query_version_t;

//��������ʱ�յ��Ļظ�����
typedef struct upgrade_s{
	ENUM_UP_TYPE  type;
	char data[0];
}upgrade_t;



enum {
	CL_UNKNOW = 0,									/*δ֪*/
	CL_MONITOR_SRV = 1, 							/*�������*/
	CL_DISP_SRV = 2,								/*���������*/
	CL_DEV_SRV =3 ,									/*�豸������*/
	CL_WEB_SRV = 4, 								/*Web������*/
	CL_APP_SRV = 5,									/*Ӧ�÷�����*/
	CL_ROUTE = 6,									/*route �豸*/
	CL_APP = 7,										/*app*/
	CL_UPGRADE_SRV = 8,								/* Upgrade server */
	CL_SPEEDTEST_SRV = 9,							/* Speed test server */
	CL_APNS_SRV = 10,								/*app push notification server*/
	CL_CGI = 11,										/*cgi����*/
	CL_INNER_PROCESS = 12,							/*�ڲ�ʵ�ֽ���*/
	CL_MAX,
};


typedef enum {
	ERR_MIN			= 0,
	ERR_NO_ERR		= ERR_MIN,
	ERR_UNKNOWN_ERR,				//δ֪����
	ERR_JSON_TOO_LONG,				//JSON̫��
	ERR_OUT_MEMORY,					//mallocʧ��,û�ڴ�
	ERR_CON_FAIL,					//����ucloudʧ��
	ERR_SOCKET_ERR,					//socket����
	ERR_SOCKET_TIMEOUT,				//socket select��ʱ
	ERR_COMMON_ERR,					//ͳһ ����,һ�㶼���������ڲ�����ִ��ʧ��ʱ�ظ�
	ERR_INVALID_MODULE_CMD,			//�յ�����ȷ��ģ�������
	ERR_CMD_UNREGISTER,				//������ucloud��δ��ע��
	ERR_PARSE_FAILED,				//���ݽ���ʧ��
	ERR_PACK_FAILED,				//���ݴ��ʧ��
	ERR_CHECK_FAILED,				//���ݼ��ʧ��
	ERR_LINK_FAILED, 				//ucloud�����ڲ�ʵ�ֽ���ʧ��
	ERR_SVR_CON_FAIL,				//���ӷ�����ʧ��
	ERR_SVR_AUTH_FAIL,				//�ͷ�������֤ʱʧ��
	ERR_MANAGE_DISABLE,				//δ�����ƹ���
	ERR_CMD_FAIL,					//���Ʒ���������ʱ�������
	ERR_UCLOUD_BUSY,				//ucloud��æ,�������������
	ERR_CONNECTING_SVR,				//�������ӷ�����
	ERR_ACCOUNT_BIND_TOO_MANY_ROUTE //���ʺŰ��˹����·����
}errno_t;

/************************************************
 * Function: 		cgi_ucloud_init
 * Description: 	��ʼ��cgiͬucloud������
 * Input: 		
 * Output:		
 * Return: 		0:��ʼ���ɹ�
 * 				-1:��ʼ��ʧ��
************************************************/
int cgi_ucloud_init(void);
/************************************************
 * Function: 		cgi_ucloud_destory
 * Description: 	�Ͽ�ͬucloud����,�������
 * Input: 			
 * Output:		
 * Return: 		
************************************************/
void cgi_ucloud_destory(void);
/************************************************
 * Function: 	c	gi_ucloud_query_version
 * Description: 	cgiͨ��ucloudѯ���°汾
 * Input: 		���÷��ؽ���ĵ�ַ
 * Output:		���ؽ�������ͼ�����
 * Return: 		0:ѯ�ʰ汾����ִ�гɹ�,����
 * 				�ӽṹ�����õ���������
 * 				-1:ѯ�ʰ汾����ִ��ʧ��
************************************************/
int cgi_ucloud_query_version(query_version_t **version);
/************************************************
 * Function: 	cgi_ucloud_policy_query_version
 * Description: 	cgiͨ��ucloudѯ����������°汾
 * Input: 		���÷��ؽ���ĵ�ַ
 * Output:		���ؽ�������ͼ�����
 * Return: 		0:ѯ�ʰ汾����ִ�гɹ�,����
 * 				�ӽṹ�����õ���������
 * 				-1:ѯ�ʰ汾����ִ��ʧ��
************************************************/
int cgi_ucloud_policy_query_version(query_version_t **version);
/************************************************
 * Function: 		cgi_ucloud_upgrade_enable
 * Description: 	cgiͨ��ucloudִ����������
 * Input: 		���÷��ؽ���ĵ�ַ
 * Output:		���ؽ�������ͼ�����
 * Return: 		0:������������ִ�гɹ�,����
 * 				�ӽṹ�����õ���������
 * 				-1:������������ִ��ʧ��
************************************************/
int cgi_ucloud_upgrade_enable(upgrade_t **upgrade);
/************************************************
 * Function: 		cgi_ucloud_upgrade_enable
 * Description: 	cgiͨ��ucloudִ�����߲�������
 * Input: 		���÷��ؽ���ĵ�ַ
 * Output:		���ؽ�������ͼ�����
 * Return: 		0:������������ִ�гɹ�,����
 * 				�ӽṹ�����õ���������
 * 				-1:������������ִ��ʧ��
************************************************/
int cgi_ucloud_policy_upgrade_enable(upgrade_t **upgrade);
/************************************************
 * Function: 		cgi_ucloud_speedtest_enable
 * Description: 	cgiͨ��ucloudִ�����߲���
 * Input: 		
 * Output:		
 * Return: 		0:���߲�������ִ�гɹ�
 * 				-1:���߲�������ִ��ʧ��
************************************************/
int cgi_ucloud_speedtest_enable(void);
/************************************************
 * Function: 		get_ucloud_errno
 * Description: 	����ִ��ʧ��ʱִ�иú���
 * 				��ȡ������
 * Input: 		
 * Output:		
 * Return: 		errno_t���͵Ĵ�����
************************************************/
errno_t get_ucloud_errno(void);

#if 1
int cgi_ucloud_login(char *in_json);
int cgi_ucloud_update_pwd(char *in_json);
int cgi_ucloud_login_sta_update();
#endif

/************************************************
 * Function: 		cgi_ucloud_sys_basic_info_get
 * Description: 	��ȡϵͳ��Ϣ
 * Input: 		����Ϊϵͳģ�������cgi���ú���,in
 * Output:		*out_json ָ�����ݻ�����
 * Return: 		
 * 				
************************************************/
int cgi_ucloud_sys_basic_info_get(char **out_json, int *out_len);
int cgi_ucloud_sys_advance_info_get(char **out_json, int *out_len);
int cgi_ucloud_sys_reboot(char **out_json, int *out_len);
int cgi_ucloud_sys_reset(char **out_json, int *out_len);
int cgi_ucloud_sys_set_time_zone(char *in, char **out_json, int *out_len);
int cgi_ucloud_query_uc_account(char **out_json, int *out_len);
int cgi_ucloud_update_uc_account(char *in);
int cgi_ucloud_set_manage_en(char *in, char **out_json, int *out_len);
int cgi_ucloud_get_manage_en(char **out_json, int *out_len);
int cgi_ucloud_common_test();
#ifdef ECOS
int cgi_restart_connect_svr();
#endif

#endif

