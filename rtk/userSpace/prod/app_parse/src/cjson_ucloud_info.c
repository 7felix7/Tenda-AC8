#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgi_lib.h"
#include "systools.h"

/*svn�汾��ͷ�ļ��ɽű����ɣ�������Ŀ¼�£�Makefile�Ѱ��� by lxp 2017.11.28*/
#include "svn_version.h"

#define BIZ_COMPANY_NAME "Tenda"

extern char *cjson_get_value(cJSON *root, char *name, char *defaultGetValue);
extern int cjson_get_number(cJSON *root, char *name, int defaultGetValue);

extern int __envram_set(const char *name, const char *value);
extern void biz_parse_fmt_mac_to_fmt1_mac(const char *in_mac, char *out_mac, int size);


/*************************************************************************
  ����: �����ƹ���sn���к�
  ����: data  sn���к�ָ��
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
void app_set_ucloud_info_sn(cJSON *recv_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	char sn[PI_BUFLEN_32] = {0} ;
    if(NULL == recv_root)
    {
		printf("[%s][%d]recv_root is null!\n",__func__,__LINE__);
		return;
    }
	strcpy(sn,cjson_get_value(recv_root,"uc_serialnum",""));

	__envram_set("uc_serialnum", sn); /* sn���Ʒ���������, ��Ҫ���浽falsh�� */
	envram_commit(0,NULL);
	nvram_set("uc_serialnum", sn);
	//nvram_commit();    /*nvram_commitͳһ�ڹ���������commit*/
	
	return;
}
/*************************************************************************
  ����: ��ȡ�ƹ��������Ϣ
  ����: data  �����ƹ��������Ϣ
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
void app_get_ucloud_basic_info(cJSON *recv_root,cJSON *send_root,void *info)
{
	char mac_temp[PI_MAC_STRING_LEN] = {0};
	char version[PI_BUFLEN_32] = {0};
	char product_ver[PI_BUFLEN_32] = {0};
	char *mac = nvram_safe_get("et0macaddr");
	int uc_enable = 1;

	if(nvram_match("uc_manage_en","0"))
	{
		uc_enable = 0;  
	}
	if(NULL != mac)
	{
		memset(mac_temp,0x0,sizeof(mac_temp));
		biz_parse_fmt_mac_to_fmt1_mac((const char *)mac, mac_temp, sizeof(mac_temp));
	}
	else
	{
		printf("[%s][%d]Read %s failed!\n",__func__,__LINE__,"et0macaddr");
		return 1;
	}

	/* �ƹ����Ƿ��� 1-����0-�� */ 
	cJSON_AddNumberToObject(send_root,"uc_manage_en",uc_enable);
	/* �ƹ���sn���к� */
	cJSON_AddStringToObject(send_root,"uc_serialnum",nvram_safe_get("uc_serialnum"));
	/*mac*/
    cJSON_AddStringToObject(send_root,"host_mac",mac_temp);

	snprintf(version, sizeof(version), "%s_%s(%s)", W311R_ECOS_SV, NORMAL_WEB_VERSION,SVN_VERSION);  
	cJSON_AddStringToObject(send_root,"version",version);
	//printf("*****************SVN_VERSION = %s\n",SVN_VERSION);
    /*��˾����*/
	cJSON_AddStringToObject(send_root,"company",COMPANY_NAME);
	
	//��ȡӲ���汾��
	//sscanf(nvram_safe_get("BOARD_NAME"),"%[^_]",hard_var);
	strcpy(product_ver,nvram_safe_get("BOARD_NAME"));
	cJSON_AddStringToObject(send_root,"product" ,product_ver);

	cJSON_AddStringToObject(send_root,"update_random",UPDATE_RANDOM);//�������� ����ʱ ͨ���ű������������
	//printf("*****************UPDATE_RANDOM = %s\n",UPDATE_RANDOM);
	
	//cJSON_AddStringToObject(send_root,"update_random",UPDATE_RANDOM222);//�������� ����ʱ ͨ���ű������������

	//cJSON_AddStringToObject(send_root,"update_random","cpoej5");/*/�������� ����ʱ ͨ���ű������������*/

	return;
}

/*************************************************************************
  ����: ��ȡ�ƹ�����
  ����: manage_en �����ƹ�����״̬
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
void app_get_ucloud_info_manage_en(cJSON *recv_root,cJSON *send_root,void *info)
{
	char *uc_enable = NULL;//�ƹ���Ĭ�Ͽ���

    uc_enable =  nvram_safe_get("uc_manage_en");

    cJSON_AddNumberToObject(send_root, "uc_manage_en", atoi(uc_enable));
	
	return;
}

/*************************************************************************
  ����: �����ƹ�����
  ����: manage_en ���������ƹ�����״̬
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
void app_set_ucloud_info_manage_en(cJSON *recv_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	char manage_en[PI_BUFLEN_8]  = {0};
	if(NULL == recv_root)
    {
		printf("[%s][%d]recv_root is null!\n",__func__,__LINE__);
		return;
    }
	strcpy(manage_en,cjson_get_value(recv_root,"uc_manage_en",""));

	nvram_set("uc_manage_en", manage_en); /* �����ƹ�����, ��Ҫ���浽falsh�� */
	//nvram_commit(); /*nvram_commitͳһ�ڹ���������commit*/
	return;
}
/*************************************************************************
  ����: �����ж��Ƿ���Ҫ������˺�
  ����: ����
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
void app_get_ucloud_info_try_clear_acc(cJSON *recv_root,cJSON *send_root,void *info)
{
	char value[PI_BUFLEN_32] = {0};
	
	strncpy(value, nvram_safe_get("ucloud_need_clear_acc"), sizeof(value));

	cJSON_AddStringToObject(send_root, "ucloud_need_clear_acc", value);

	return;
}

/*************************************************************************
  ����: ������˺ź��������˺�����ձ�־
  ����: ����
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
void app_set_ucloud_info_clear_account(cJSON *recv_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
	char account_en[PI_BUFLEN_8] = {0};
	if(NULL == recv_root)
    {
		printf("[%s][%d]recv_root is null!\n",__func__,__LINE__);
		return;
    }
	strcpy(account_en,cjson_get_value(recv_root, "ucloud_need_clear_acc",""));
	nvram_set( "ucloud_need_clear_acc", account_en);
	//nvram_commit(); /*nvram_commitͳһ�ڹ���������commit*/
	
	return;
}


