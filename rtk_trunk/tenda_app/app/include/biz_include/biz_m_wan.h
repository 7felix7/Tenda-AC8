#ifndef BIZ_M_WAN_H
#define BIZ_M_WAN_H

#include "process_api.h"

enum{
	REMOTE_SERVER_NO_RESPONSE=548,       //Զ�˷���������Ӧ
	USERNAME_PASSWD_ERROR=549,           //�û������������
	NOT_CONNECT_NETWORK=566,             //�޷����ӵ�������
};

enum{
	NOT_LINK= 0,                         //δ������
	NOT_NETWORK,                         //δ����
	NETWORKING,                          //������
	NETWORKED,                           //������
};

int biz_m_wan_basic_get_cb(
	const api_cmd_t *cmd, 
	wan_basic_info_t *basic_info,
	void *privdata);

int biz_m_wan_basic_set_cb(
	const api_cmd_t *cmd, 
	wan_basic_info_t *basic_info,
	void *privdata);

int biz_m_wan_rate_get_cb(
	const api_cmd_t *cmd,
	wan_rate_info_t *rate_info,
	void *privdata);

int biz_m_wan_detect_type_cb (
	const api_cmd_t *cmd, 
	wan_detecttype_info_t *type_info,
	void *privdata);

#endif