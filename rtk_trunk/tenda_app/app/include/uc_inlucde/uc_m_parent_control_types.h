/*
**  Copyright (C), 1998-2014, Tenda Tech. Co., Ltd.
**
**  Project:	Cloud manager Api v1.0
**  File:    	
**  Author: 	yangyabing
**  Date:    	07/09/2015
**
**  Purpose:
**    		.
**
**  History: 
**  <author>   <time>          <version >   <desc>
*	$Id: 
*/

#ifndef __UC_M_PARENT_CONTROL_TYPES_H__
#define __UC_M_PARENT_CONTROL_TYPES_H__

#define PC_MAX_URL_NUM 			10
#define PC_MAX_URL_LENT 			64
#define PC_MAX_DEV_NAME_LEN 			64
#define PC_MAC_STR_LEN 			20
#define PC_DAYS_STR_LEN 			16
#define PC_TIME_LEN 				16

#define SET_PARENT_CONTROL_X(pc,x)	\
	((pc)->mask |= (1 << x))
#define HAS_PARENT_CONTROL_X(pc,x) \
	(((pc)->mask & (1 << x)) == (1 << x))
#define SET_PARENT_CONTROL_TYPE(pc, x) \
	((pc)->list_limit_type |= (1 << x))
	
typedef enum {
	PC_PARAM_OPT_GET_ALL = 0,
	PC_PARAM_OPT_GET_SPEC,
}pc_param_opt_get;

typedef enum {
	PC_PARAM_OPT_SET_UPDATE = 0,
	PC_PARAM_OPT_SET_DELETE,
}pc_param_opt_set;

typedef enum {
	PC_LIST_LIMIT_TYPE_BLACK = 0, //������
	PC_LIST_LIMIT_TYPE_WHITE,     //������
}pc_list_limit_type;

#define SET_PARENT_CONTROL_TYPE_BLACK(pc) \
	SET_PARENT_CONTROL_TYPE(pc, PC_LIST_LIMIT_TYPE_BLACK)
#define SET_PARENT_CONTROL_TYPE_WHITE(pc) \
	SET_PARENT_CONTROL_TYPE(pc, PC_LIST_LIMIT_TYPE_WHITE)

enum {
	_PC_RULE_URL_LIMIT_TYPE_ = 0,
	_PC_RULE_DEV_NAME_ = 1,
};
#define SET_PC_RULE_URL_LIMIT_TYPE(pc)	\
	SET_PARENT_CONTROL_X(pc,_PC_RULE_URL_LIMIT_TYPE_)
#define SET_PC_RULE_DEV_NAME(pc)	\
	SET_PARENT_CONTROL_X(pc,_PC_RULE_DEV_NAME_)
#define HAS_PC_RULE_URL_LIMIT_TYPE(pc) \
	HAS_PARENT_CONTROL_X(pc,_PC_RULE_URL_LIMIT_TYPE_)
#define HAS_PC_RULE_DEV_NAME(pc) \
	HAS_PARENT_CONTROL_X(pc,_PC_RULE_DEV_NAME_)
typedef struct pc_rule_s{
	int mask;
	int pc_flag;						//�ҳ������Ƿ���Ч
	char mac[PC_MAC_STR_LEN];		//���ù����mac��ַ
	char days[PC_DAYS_STR_LEN];		//�û����õ���������(1#2#3#4#5#6#7)
	char time_desc[PC_TIME_LEN];	//�û����õ�ʱ��(ʱ���)
	int restricted;						//�������Ƿ���
	char urls[PC_MAX_URL_NUM * PC_MAX_URL_LENT];		//url�ľ�������
	int url_limit_type;				//ʹ�ð��������ߺ�����
    char dev_name[PC_MAX_DEV_NAME_LEN];
}pc_rule_t;

typedef struct pc_info_s{
	int rule_count;							//���������Ŀ
	pc_rule_t rules[0];
}pc_info_t;

enum {
	_PC_GET_PARAM_MAC_ = 0,
};
#define SET_PC_GET_PARAM_MAC(pc)	\
	SET_PARENT_CONTROL_X(pc,_PC_GET_PARAM_MAC_)
#define HAS_PC_GET_PARAM_MAC(pc) \
	HAS_PARENT_CONTROL_X(pc,_PC_GET_PARAM_MAC_)
typedef struct pc_get_param_s {
	int mask;
	pc_param_opt_get	opt;		//0 - GET_ALL, 1 - GET_SPEC
	char mac[PC_MAC_STR_LEN];	//ָ��mac�ļҳ����ƹ���
}pc_get_param_t;

typedef struct pc_set_param_s {
	pc_param_opt_set	opt;
	pc_rule_t spec_rule;
}pc_set_param_t;

typedef struct pc_type_s {
	int list_limit_type;//����ʹ�ù������ͣ���������������...,��λ����ʾ
}pc_type_t;
//parent control common ack
enum {
	_PARENT_CONTROL_ACK_INFO_ = 0,
	_PARENT_CONTROL_ACK_LIST_LIMIT_TYPE,
};

#define SET_PARENT_CONTROL_ACK_INFO(ack)	\
	SET_PARENT_CONTROL_X(ack,_PARENT_CONTROL_ACK_INFO_)
#define HAS_PARENT_CONTROL_ACK_INFO(ack) \
	HAS_PARENT_CONTROL_X(ack,_PARENT_CONTROL_ACK_INFO_)
#define SET_PARENT_CONTROL_ACK_LIST_LIMIT_TYPE(ack)	\
	SET_PARENT_CONTROL_X(ack,_PARENT_CONTROL_ACK_LIST_LIMIT_TYPE)
#define HAS_PARENT_CONTROL_ACK_LIST_LIMIT_TYPE(ack) \
	HAS_PARENT_CONTROL_X(ack,_PARENT_CONTROL_ACK_LIST_LIMIT_TYPE)
	
typedef struct parent_control_common_ack_s {
	int mask;
	int err_code;
	pc_type_t pc_type;  
	pc_info_t pc_info;
}parent_control_common_ack_t;

#endif
