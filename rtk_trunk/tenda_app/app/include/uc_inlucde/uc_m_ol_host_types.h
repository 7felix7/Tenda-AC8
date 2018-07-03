#ifndef __M_OL_HOST_TYPES_H__
#define __M_OL_HOST_TYPES_H__
/*
**  Copyright (C), 1998-2014, Tenda Tech. Co., Ltd.
**
**  Project:	Cloud manager Api v1.0
**  File:    	
**  Author: 	Young
**  Date:    	07/08/2015
**
**  Purpose:
**    		.
**
**  History: 
**  <author>   <time>          <version >   <desc>
*	$Id: uc_lib_m_ol_host.h 241 2015-07-08 02:51:12Z yangyabing $
*/

#define OL_HOSTS_MAC_STR_LEN				20
#define OL_HOSTS_IP_STR_LEN					16
#define OL_HOSTS_NAME_STR_LEN				64
#define OL_HOSTS_SSID_STR_LEN				64
#define VENDOR_LEN							32

#define SET_OL_HOST_X(hosts,x)	\
	((hosts)->mask |= (1 << x))
#if 1
#define HAS_OL_HOST_X(hosts,x)\
	(((hosts)->mask & (1 << x)) == (1 << x))

#endif
typedef enum {
	OL_HOSTS_MAC_BLOCK = 0,
	OL_HOSTS_BW_LIMIT,
	OL_HOSTS_ONLINE_TIME,
	OL_HOSTS_UNDER_PC_CTRL,
	OL_HOSTS_UP_LIMIT,
	OL_HOSTS_DOWN_LIMIT,
	OL_HOSTS_DEV_TYPE,
	OL_HOSTS_DEV_TRUST,
	OL_HOSTS_DEV_ALIAS,
	OL_HOSTS_DEV_RELAT_SSID,
	OL_HOSTS_DEV_MANUFACTORY,
	OL_HOSTS_DEV_ONLINE_FLAG,
	OL_HOSTS_DEV_MANUFACTORY_DESC,
}__ol_hosts_get_dev_param;

#define SET_OL_HOSTS_MAC_BLOCK(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_MAC_BLOCK)
#define SET_OL_HOSTS_BW_LIMIT(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_BW_LIMIT)
#define SET_OL_HOSTS_ONLINE_TIME(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_ONLINE_TIME)	
#define SET_OL_HOSTS_UNDER_PC_CTRL(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_UNDER_PC_CTRL)
#define SET_OL_HOSTS_UP_LIMIT(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_UP_LIMIT)
#define SET_OL_HOSTS_DOWN_LIMIT(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DOWN_LIMIT)
#define SET_OL_HOSTS_DEV_TYPE(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_TYPE)
#define SET_OL_HOSTS_DEV_TRUST(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_TRUST)
#define SET_OL_HOSTS_DEV_ALIAS(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_ALIAS)
#define SET_OL_HOSTS_DEV_RELAT_SSID(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_RELAT_SSID)
#define SET_OL_HOSTS_DEV_MANUFACTORY(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_MANUFACTORY)
#define SET_OL_HOSTS_DEV_OL(ol_hosts_dev_t) \
	SET_OL_HOST_X(ol_hosts_dev_t, OL_HOSTS_DEV_ONLINE_FLAG)
#define SET_OL_HOSTS_DEV_MANUFACTORY_DESC(ol_hosts_dev_t)	\
	SET_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_MANUFACTORY_DESC)

#if 1
#define HAS_OL_HOSTS_MAC_BLOCK(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t, OL_HOSTS_MAC_BLOCK)
#define HAS_OL_HOSTS_BW_LIMIT(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_BW_LIMIT)
#define HAS_OL_HOSTS_ONLINE_TIME(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_ONLINE_TIME)	
#define HAS_OL_HOSTS_UNDER_PC_CTRL(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_UNDER_PC_CTRL)
#define HAS_OL_HOSTS_UP_LIMIT(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_UP_LIMIT)
#define HAS_OL_HOSTS_DOWN_LIMIT(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DOWN_LIMIT)
#define HAS_OL_HOSTS_DEV_TYPE(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_TYPE)
#define HAS_OL_HOSTS_DEV_TRUST(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_TRUST)
#define HAS_OL_HOSTS_DEV_ALIAS(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_ALIAS)
#define HAS_OL_HOSTS_DEV_RELAT_SSID(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_RELAT_SSID)
#define HAS_OL_HOSTS_DEV_MANUFACTORY(ol_hosts_dev_t) \
	HAS_OL_HOST_X(ol_hosts_dev_t, OL_HOSTS_DEV_MANUFACTORY)	
#define HAS_OL_HOSTS_DEV_OL(ol_hosts_dev_t) \
	HAS_OL_HOST_X(ol_hosts_dev_t, OL_HOSTS_DEV_ONLINE_FLAG)	
#define HAS_OL_HOSTS_DEV_MANUFACTORY_DESC(ol_hosts_dev_t)	\
	HAS_OL_HOST_X(ol_hosts_dev_t,OL_HOSTS_DEV_MANUFACTORY_DESC)
#endif

typedef struct ol_hosts_dev_s {
	int		mask;					//������µ�����ѡ���Ƿ����
	int mac_blocked;				//�Ƿ��ں�����������
	int bw_limited;					//�Ƿ�������������
	int online_time;				//����������ʱ��
	int under_pc_control;			//�Ƿ��ڼҳ�������	
	int curr_up_rate;		//��ǰ������������
	int curr_down_rate;		//��ǰ������������  ��λ:KB/S	
	float up_limit;			//��������	
	float down_limit;			//��������  ��λ:5.23KB/S
	int host_type;			//��������	(iphone,andorid��)
	int host_manufactory;	//��������
	int trust;				//�Ƿ�����
	int online;
	int	 access_type;			//�豸��������	(��������)
	char host_name[OL_HOSTS_NAME_STR_LEN];	//��������ԭ��
	char host_alias[OL_HOSTS_NAME_STR_LEN];//������������
	char ip[OL_HOSTS_IP_STR_LEN];	//��������ip��ַ
	char mac[OL_HOSTS_MAC_STR_LEN];	//��������mac��ַ
	char asso_ssid[OL_HOSTS_SSID_STR_LEN];	//��������������ssid��(���������豸)
	char manufactory_desc[VENDOR_LEN];	//�豸������������Huawei��δʶ����Ϊ���ַ���
}ol_hosts_dev_t;

typedef struct ol_hosts_info_s {
	int mem_len;								//�ýṹ���ʵ�ʳ���
	int hosts_count;							//��������
	ol_hosts_dev_t hosts[0];					//������Ϣ��������
}ol_hosts_info_t;

//ol_host module common ack
enum {
	_OL_HOSTS_ACK_INFO_ = 0,
};

#define HAS_OL_HOSTS_ACK_INFO(ack)	\
	HAS_OL_HOST_X(ack, _OL_HOSTS_ACK_INFO_)
#define SET_OL_HOSTS_ACK_INFO(ack)	\
	SET_OL_HOST_X(ack,_OL_HOSTS_ACK_INFO_)
	
typedef struct ol_host_common_ack_s {
	int mask;
	int err_code;
	ol_hosts_info_t info;
}ol_host_common_ack_t;
#endif

