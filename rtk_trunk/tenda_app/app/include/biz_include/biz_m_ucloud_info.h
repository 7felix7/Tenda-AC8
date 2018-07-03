#ifndef BIZ_M_UCLOUD_INFO_H
#define BIZ_M_UCLOUD_INFO_H

#include "process_api.h"


#define UCLOUD_INFO_MANAGE_ENABLE    "uc_manage_en"   /* �ƹ����� */
#define UCLOUD_INFO_SN    			 "uc_serialnum"   /* �ƹ���sn���к� */
#define UCLOUD_INFO_PWD   			 "uc_password"    /* �ƹ������� */
#define MIB_NAME_UC_NEED_CLEAR_ACC	 "ucloud_need_clear_acc" /* ���˺��Ƿ���Ҫ��� */


int biz_m_ucloud_info_sn_set_cb(
	api_cmd_t *cmd, 
	unsigned int data_len,
	void *data,
	void *privdata);

int biz_m_ucloud_info_basic_get(
	cloud_basic_info_t *data);

int biz_m_ucloud_info_manage_en_get_cb(
    const api_cmd_t *cmd,
	m_cloud_info_manage_en_t *manage_en,
	void *privdata);

int biz_m_ucloud_info_manage_en_set_cb(
	const api_cmd_t *cmd,
	m_cloud_info_manage_en_t *manage_en,
	void *privdata);

#ifdef CONFIG_APP_COSTDOWN
int biz_ucloud_info_try_clear_acc(int *need_clear);
#else
int biz_ucloud_info_try_clear_acc();
#endif

int biz_cloud_info_clear_account_ack_cb(
	const api_cmd_t *cmd, 
	void *privdata);

#endif

