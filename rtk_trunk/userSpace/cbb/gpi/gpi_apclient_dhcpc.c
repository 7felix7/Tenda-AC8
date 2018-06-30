#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "apclient_dhcpc.h"
#include "bcmnvram.h"

PI8 gpi_apclient_dhcpc_addr(PI8 *ip,PI8 *mask)
{
	if(NULL == ip || NULL == mask)
		return 0;
		
	return tpi_apclient_dhcpc_get_ip(ip,mask);	
}

PI8 gpi_apclient_dhcpc_ping_gateway()
{
	return tpi_apclient_dhcpc_ping_gateway();	
}

PI8 gpi_apclient_dhcpc_enable()
{		
	return tpi_apclient_dhcpc_get_enable();	
}

PI8 gpi_apclient_dhcpc_enable_by_mib()
{	
	SYS_WORK_MODE wl_mode = WL_ROUTE_MODE;
	wl_mode = gpi_wifi_get_mode();
	if(WL_APCLIENT_MODE == wl_mode 
		|| WL_BRIDGEAP_MODE == wl_mode)
	{
		return 1;
	}
	return 0;	
}
#ifdef __CONFIG_APCLIENT_CLIENT_INFO__
/*lq ����ӿڣ������û�����*/
RET_INFO gpi_apclient_dhcpc_client_action(void* data,int action)
{
	int ret = RET_ERR;
	if(NULL == data)
	{
		return RET_ERR;
	}
	switch(action)
	{
/*����һ��*/
		case ADD_CLIENT:
			ret = tpi_apclient_dhcpc_add_client(*((struct apclient_client_info*)data));
			break;
/*ɾ��һ��*/	
		case REMOVE_CLIENT:
			ret = tpi_apclient_dhcpc_remove_client((unsigned char*)data);
			break;
/*��������*/	
		case UPDATE_CLIENT:
			ret =tpi_apclient_dhcpc_update_client_info();
			break;
/*�������*/	
		case FLUSH_CLIENT:
			ret = tpi_apclient_dhcpc_flush_client_info();
			break;
			
		default:
			return RET_ERR;
	}
	return ret;
}
/*lq ����mac��ַ��ȡ��Ӧ�Ŀͻ��˵�ip�͵�dev_name��Ϣ*/
struct apclient_client_info* gpi_apclient_dhcpc_get_client_info(unsigned char* mac)
{
	return tpi_apclient_dhcpc_get_client_info(mac);
}
#endif