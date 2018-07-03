#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auto_conn_client.h"

static RET_INFO api_auto_conn_client_handle(RC_MODULES_COMMON_STRUCT *var);
static RET_INFO api_auto_conn_client_init();
extern RET_INFO api_auto_connn_wifi_handle(RC_MODULES_COMMON_STRUCT *var);

static RET_INFO api_auto_conn_client_handle(RC_MODULES_COMMON_STRUCT *var)
{
	if (NULL == var)
	{
		PI_ERROR(TPI, "var is null!\n");
		return RET_ERR;
	}

	return tpi_auto_conn_client_action(var);
}

static struct rc_msg_ops rc_auto_conn_clinet_ops[] =
{
	{
		.intent_module_id = RC_AUTO_CONN_CLIENT_MODULE,
		.type = INTENT_NONE,
		.ops = api_auto_conn_client_handle,
	},
	/*��Ϊ֮ǰ�ڴ������Զ��Žӵ���Ϣ��ʱ��������߷�����Ϣ���˷���Դ�����ｫ���Ƹ���Ϊ��Ϣ���󣩹�������������Զ��Žӷ�����Ϣ���������Զ��������������߷�����Ϣ*/
	{
		.intent_module_id = RC_WIFI_MODULE,
		.type = INTENT_NEXT,
		.ops = api_auto_connn_wifi_handle,
	},
};

static RET_INFO api_auto_conn_client_init()
{
	tpi_auto_conn_client_struct_init();
	tpi_auto_conn_client_first_init();
	rc_register_module_msg_opses(rc_auto_conn_clinet_ops, RC_AUTO_CONN_CLIENT_MODULE, ARRAY_SIZE(rc_auto_conn_clinet_ops));
	return RET_SUC;
}

RC_MODULE_REGISTER(RC_AUTO_CONN_CLIENT_MODULE, "rc_auto_conn_client", api_auto_conn_client_init, NULL);
