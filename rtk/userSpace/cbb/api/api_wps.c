#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wps.h"

static RET_INFO api_wps_handle(RC_MODULES_COMMON_STRUCT *var);
static RET_INFO api_wps_init();

static RET_INFO api_wps_handle(RC_MODULES_COMMON_STRUCT *var)
{
	if(NULL == var)
	{
		PI_ERROR(TPI,"var is null!\n");
		return RET_ERR;
	}
	
    return tpi_wps_action(var);
}

/*****************************************************************************
 �� �� ��  : api_wps_handle
 ��������  : ��������ʱ��������WPS
 �������  : RC_MODULES_COMMON_STRUCT *var  
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��14��
    ��    ��   : fh
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO api_wifi_wps_action(RC_MODULES_COMMON_STRUCT *var)
{
	if(NULL == var)
	{
		PI_ERROR(TPI,"var is null!\n");
		return RET_ERR;
	}
	if(OP_START == var->op || OP_STOP == var->op || OP_RESTART == var->op )
	{
		return tpi_wps_action(var);
	}
}


static struct rc_msg_ops rc_wps_ops[] =
{
    {
        .intent_module_id = RC_WPS_MODULE,
		.type = INTENT_NONE,
        .ops = api_wps_handle,
    },
};

static RET_INFO api_wps_init()
{
	tpi_wps_struct_init();
	tpi_wps_first_init();
    rc_register_module_msg_opses(rc_wps_ops,RC_WPS_MODULE,ARRAY_SIZE(rc_wps_ops));
	return RET_SUC;
}

RC_MODULE_REGISTER(RC_WPS_MODULE,"rc_wps",api_wps_init,NULL);
