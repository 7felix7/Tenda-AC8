#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bcmnvram.h>

#include "debug.h"
#include "systools.h"

extern void do_reset(int );
extern void nvram_default(void);

/*���ļ�����ʹ��*/
static RET_INFO tpi_reboot_action()
{
	cyg_thread_delay(2*RC_MODULE_1S_TIME);
	do_reset(1);
	return RET_SUC;
}

static RET_INFO tpi_restore_action()
{
	cyg_thread_delay(2*RC_MODULE_1S_TIME);
	nvram_default();
	do_reset(1);
	return RET_SUC;
}

RET_INFO tpi_systools_action(RC_MODULES_COMMON_STRUCT *var)
{
	RET_INFO ret = RET_SUC;

	//����coverity��������޸ģ�ԭ��Ϊ��Ч���ж�:NULL == var->string_info  2017/1/11 F9��Ŀ�޸�
	if(NULL == var || 0 == strcmp("",var->string_info))
	{
		return RET_ERR;
	}
	
	if(0 == memcmp("reboot",var->string_info,strlen("reboot")))
	{
		ret = tpi_reboot_action();
	}
	else if(0 == memcmp("restore",var->string_info,strlen("restore")))
	{
		ret = tpi_restore_action();
	}
	else if(0 == memcmp("commit",var->string_info,strlen("commit")))
	{
	/*���commit����������һЩ��Ҫ��ʱ���صĲ����������ȷ�commit��Ϣ,
	Ȼ���������أ�ǰ��������������Ϣ���߳����ȼ���Ҫ���ڵ���main�̵߳����ȼ�*/
		nvram_commit();
	}

	return ret;
}

