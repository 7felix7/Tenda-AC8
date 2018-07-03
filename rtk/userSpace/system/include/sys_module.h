#ifndef __SYS_MODULE_H__
#define __SYS_MODULE_H__

#ifndef __RC_MODULE_H__
#include "rc_module.h"
#endif

/*�����ʾ�ܹ�������Ϣ����ϢID����*/
typedef enum
{
    MODULE_RC = 1,
    MODULE_MAX_NUM,
} MODULE_CENTER_ID;

struct moudle_center_struct
{
	MODULE_CENTER_ID center;/*������Ϣ����ID 0-255*/
	PI8  name[PI_BUFLEN_32];/*������Ϣ���ƣ����߳����Ƹ���Ϊrc��1-32*/
    RET_INFO (*init)(); /* ������Ϣ���ĳ�ʼ��������ϵͳ������ɺ�����Ӧ�ò����*/
	RET_INFO (*msg_2_tlv_func)(PI8 *msg,PI8 *tlv_msg);/*��ģ���½�MSGת����TLV��ʽ����*/
	RET_INFO (*rcv_msg_handle)(PIU8 id,PI8 *msg);/*��ģ���´�����Ϣ�ĺ���*/
	void (*callback)(PIU8 module_id,PIU32 module_delay,PIU32 module_extra_delay);/*�ص�����*/
};

extern struct moudle_center_struct moudle_center_struct_list[];
extern PIU8 sys_get_center_num();
#endif/*__SYS_MODULE_H__*/
