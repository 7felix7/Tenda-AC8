#include <stdlib.h>
#include <string.h>

#include <bcmnvram.h>
#include "pi_wan_err_info.h"
/*
˵����
	1.֮ǰ������������ֱ����ʾ�����У���ȡ��IP֮��������ʾ��������
		��APģʽ��ȥ�������У����Ź�������ʾ���ڳ��������С���
		���ǵ��Ž�ģʽ�������ԣ�ֻ�����Ž�ģʽ�»�ȡ��IP֮��������ʾΪ��������
	2.���ǿռ��ٰ�PPPOE������֤ʱ����ʾʱ��Ϊ1-5���ӣ�û�ǿռ��ٰ���ʾΪ1-2���ӡ�
	3.WISPģʽ�²�֧��MAC��ַ��¡���ʲ���ʾ
(һ)APģʽ��
	STATIC:
		1.	��⵽WAN������δ���ӣ����鲢���Ӻ�����WAN������
		2.	δ����
		3.	���ڳ��������С�
		4.	δ����������ϵ���Ŀ����Ӫ��
		5.	������
		6.	ϵͳ��⵽����������ʽ����Ϊ��̬IP�����ֶ�ѡ�����þ�̬IP����������
	DHCP:
		1.	��⵽WAN������δ���ӣ����鲢���Ӻ�����WAN������
		2.	δ����
		3.	���ڳ��������С�
		4.	δ��������ȷ�ϲ�ʹ��·����ʱ�Ƿ���������������粻�ܣ�����ϵ���Ŀ����Ӫ��
		5.	������
		6.	IP��ͻ�����޸�LAN��IP
		7.	������Ӫ��Զ������Ӧ����ȷ�ϲ���·����ʱ�Ƿ���������������粻�ܣ�����ϵ����������Ӫ�̽��
	PPPOE:
		1.	��⵽WAN������δ���ӣ����鲢���Ӻ�����WAN������
		2.	δ����
		3.	�������������Ŀ���û����Ϳ�������Ƿ���ȷ�����Եȣ���������Լ1-5����
		4.	���ųɹ������޷�����������������ȷ�ϲ�ʹ��·����ʱ�Ƿ���������������粻�ܣ�����ϵ���Ŀ����Ӫ��
		5.	������
		6.	�û���������֤ʧ�ܣ���ȷ�����Ŀ���û����������벢��������
		7.	������Ӫ��Զ������Ӧ����ȷ�ϲ���·����ʱ�Ƿ���������������粻�ܣ�����ϵ����������Ӫ�̽��
(��)WISPģʽ��
	STATIC:
		1.	�ȵ��źŷŴ�ģʽδ�Ž�
		2.	�ȵ��źŷŴ�ģʽ�Ž���...
		3.	�ȵ��źŷŴ�ģʽ�Žӳɹ������ڳ�������...
		4.	δ����������ϵ���Ŀ����Ӫ��
		5.	������
		6.  ����������֤ʧ�ܣ������������ϼ��������룡
	DHCP:
		1.	�ȵ��źŷŴ�ģʽδ�Ž�
		2.	�ȵ��źŷŴ�ģʽ�Ž���...
		3.	�ȵ��źŷŴ�ģʽ�Žӳɹ������ڳ�������...
		4.	ϵͳ�ѻ�ȡ��IP�����޷�������������������ϵ���Ŀ����Ӫ��
		5.	������
		6.	IP��ͻ�����޸�LAN��IP
		7.	������Ӫ��Զ������Ӧ����ȷ�ϲ���·����ʱ�Ƿ���������������粻�ܣ�����ϵ����������Ӫ�̽��
		8.  ����������֤ʧ�ܣ������������ϼ��������룡
	PPPOE:
		1.	�ȵ��źŷŴ�ģʽδ�Ž�
		2.	�������������Ŀ���û����Ϳ�������Ƿ���ȷ�����Եȣ���������Լ1-5����
		3.	���ųɹ������޷�����������������ȷ�ϲ�ʹ��·����ʱ�Ƿ���������������粻�ܣ�����ϵ���Ŀ����Ӫ��
		4.	������
		5.	�û���������֤ʧ�ܣ���ȷ�����Ŀ���û����������벢��������
		6.	������Ӫ��Զ������Ӧ����ȷ�ϲ���·����ʱ�Ƿ���������������粻�ܣ�����ϵ����������Ӫ�̽��
		8.  ����������֤ʧ�ܣ������������ϼ��������룡
(��)APCLIENTģʽ��
	1.	�����źŷŴ�ģʽδ�Ž�
	2.	�����źŷŴ�ģʽ�Ž���...
	3.	�����źŷŴ�ģʽ�Žӳɹ������ڳ�������...
	4.	δ����������ϵ���Ŀ����Ӫ��
	5.	������
	6.  ����������֤ʧ�ܣ������������ϼ��������룡
*/
#ifdef __CONFIG_APCLIENT_DHCPC__
extern int gpi_get_apclient_dhcpc_addr(char *ip,char *mask);
#endif
extern WLMODE tpi_get_wl_mode();

WANMODE tpi_get_wan_mode(WLMODE wl_mode)
{
    WANMODE ret = WAN_DHCP_MODE;
    int wan_mode = 0;

    switch(wl_mode)
    {
        case WL_AP_MODE:
        case WL_WISP_MODE:
            wan_mode = get_wan_type();
            if(wan_mode < WAN_NONE_MODE || wan_mode > WAN_PPPOE_MODE)
            {
                ret = WAN_NONE_MODE;
            }
            else
            {
                ret = wan_mode;
            }
            break;
        case WL_APCLIENT_MODE:
			ret = WAN_DHCP_MODE;
			break;
        case WL_WDS_MODE:
        default:
            ret = WAN_NONE_MODE;
            break;
    }

    return ret;
}

PI_INFO tpi_get_wan_status(WLMODE wl_mode,WANSTATUS * wan_status,WIFISTASTATUS * wifi_status)
{
    PI_INFO ret = PI_SUC;
    int wanLinkSta = 0;
    int conStat = 0;

	//���ﲻ��wan_status��wifi_status���п��жϣ���ΪAPģʽ����Ҫwifi_status�����жϷŵ�switch�������
	
    switch(wl_mode)
    {
        case WL_AP_MODE:
			if(NULL == wan_status)
			{
				return PI_ERR;
			}
            wan_link_check();
            wanLinkSta = get_wan_linkstatus();
            if(0 == wanLinkSta)
            {
                (*wan_status) = WAN_NO_WIRE;
            }
            else
            {
                conStat = get_wan_connstatus();
                if(2 == conStat)
                    (*wan_status) = WAN_CONNECTED;
                else if(1 == conStat)
                    (*wan_status) = WAN_CONNECTING;
                else
                    (*wan_status) = WAN_DISCONNECTED;
            }
            break;
        case WL_WISP_MODE:
        case WL_APCLIENT_MODE:
			if(NULL == wan_status || NULL == wifi_status)
			{
				return PI_ERR;
			}
            gpi_get_wifi_status_info(wan_status,wifi_status);
            break;
        case WL_WDS_MODE:
        default:
            ret = PI_ERR;
            break;
    }

    return ret;
}

static WANERRINFO tpi_get_err_result(P_WAN_ERR_INFO_STRUCT p)
{
    WANERRINFO ret = INFO_ERROR;

    ret = ((p->code)%100) + 100 * ((p->network_check)%10) + 1000 * ((p->wan_mode)%10)
			+ 10000 * ((p->wl_mode)%10) + 100000 * ((p->time_action)%10)
          	+ 1000000 * ((p->color_action)%10) + 10000000 * ((p->button_action)%10);

    return ret;
}

DHCPERRCODE tpi_get_dhcp_err_result()
{
    DHCPERRCODE ret = COMMON_NONE;
    char dhcp_check[5] = {0};

    sprintf(dhcp_check,"%s",nvram_safe_get(ERR_CHECK));

    if(0 == strcmp(dhcp_check,"11"))
    {
        ret = DHCP_IP_CONFLLICT;
    }
	else if(0 == strcmp(dhcp_check,"5"))
	{
		ret = DHCP_NO_RESPOND;
	}

    return ret;
}

PPPOEERRCODE tpi_get_pppoe_err_result()
{
    PPPOEERRCODE ret = COMMON_NONE;
    char pppoe_check[5]= {0};

    sprintf(pppoe_check,"%s",nvram_safe_get(ERR_CHECK));

    if(!strcmp(pppoe_check,"5"))//����Ӧ
    {
        ret = PPPOE_NO_RESPOND;
    }
    else if(!strcmp(pppoe_check,"7"))//������֤
    {
        ret = COMMON_CONNECTING;
    }
    else if(!strcmp(pppoe_check,"2"))//�û������������
    {
        ret = PPPOE_CHECKED_PASSWORD_FAIL;
    }
    else if(!strcmp(pppoe_check,"3"))//�ɹ�
    {
        ret = COMMON_CONNECTED_ONLINEING;
    }

    return ret;
}

PI_INFO tpi_get_connecting_err_info(P_WAN_ERR_INFO_STRUCT p,WIFISTASTATUS wifi_status)
{
	PI_INFO ret = PI_SUC;

	if(NULL == p)
	{
		return PI_ERR;
	}
	
	p->button_action = BUTTON_DOWN;
	p->color_action = COLOR_TRY;
	p->time_action = TIME_NONE;
	p->code = COMMON_CONNECTING;

	if(WL_AP_MODE != p->wl_mode && (WIFI_AUTHENTICATED_FAIL == wifi_status || WIFI_AUTH_FAIL == wifi_status))
	{
		p->color_action = COLOR_ERR;
		switch(p->wan_mode)
		{
			case WAN_STATIC_MODE:
				p->code = STATIC_WL_CHECKED_PASSWORD_FAIL;
				break;
			case WAN_DHCP_MODE:
				p->code = DHCP_WL_CHECKED_PASSWORD_FAIL;
				break;
			case WAN_PPPOE_MODE:
				p->code = PPPOE_WL_CHECKED_PASSWORD_FAIL;
				break;
			default:
				ret = PI_ERR;
				break;
		}
		return ret;
	}
	
	switch(p->wl_mode)
	{
		case WL_AP_MODE:
		case WL_WISP_MODE:
            if(WAN_DHCP_MODE == p->wan_mode)
            {
                DHCPERRCODE dhcp_err = tpi_get_dhcp_err_result();
                if(dhcp_err == DHCP_IP_CONFLLICT)
                {
                    p->color_action = COLOR_ERR;
                    p->code = DHCP_IP_CONFLLICT;
                }
				else if(dhcp_err == DHCP_NO_RESPOND)
                {
                    p->color_action = COLOR_ERR;
                    p->code = DHCP_NO_RESPOND;
                }
            }
            else if(WAN_PPPOE_MODE == p->wan_mode)
            {
                PPPOEERRCODE pppoe_err = tpi_get_pppoe_err_result();

                if(pppoe_err == COMMON_CONNECTING)
                {
                    p->color_action = COLOR_TRY;
                    p->code = COMMON_CONNECTING;
                }
#if 0
				//��ʱ��Ȼ���ųɹ������ǻ�û�л�ȡ��IP
                else if(pppoe_err == COMMON_CONNECTED_ONLINEING)
                {
                    p->color_action = COLOR_TRY;
                    p->code = COMMON_CONNECTED_ONLINEING;
                }
#endif
                else if(pppoe_err == PPPOE_CHECKED_PASSWORD_FAIL)
                {
                    p->color_action = COLOR_ERR;
                    p->code = PPPOE_CHECKED_PASSWORD_FAIL;
                }
                else if(pppoe_err == PPPOE_NO_RESPOND)
                {
                    p->color_action = COLOR_ERR;
                    p->code = PPPOE_NO_RESPOND;
                }
            }
			break;
		case WL_APCLIENT_MODE:
            p->time_action = TIME_NONE;
			break;
		default:
			break;
	}
	return ret;
}


PI_INFO tpi_get_connected_err_info(P_WAN_ERR_INFO_STRUCT p)
{
	PI_INFO ret = PI_SUC;
	int internetStat = 0;
	
	if(NULL == p)
	{
		return PI_ERR;
	}
	
	p->button_action = BUTTON_DOWN;
	p->color_action = COLOR_TRY;
	p->time_action = TIME_SHOW;
	p->code = COMMON_CONNECTED_ONLINEING;
	
	switch(p->wl_mode)
	{
		case WL_AP_MODE:
		case WL_WISP_MODE:
			internetStat = get_wan_onln_connstatus();

			if(0 == internetStat)
			{
				p->color_action = COLOR_ERR;
				p->code = COMMON_NOT_ONLINE;
			}
#if 0
			else if(1 == internetStat)
			{
				p->color_action = COLOR_TRY;
				p->code = COMMON_CONNECTED_ONLINEING;
			}
#endif
			else
			{
				p->color_action = COLOR_SUC;
				p->code = COMMON_ONLINEED;
			}
		break;
		case WL_APCLIENT_MODE:
            p->time_action = TIME_NONE;
            p->color_action = COLOR_SUC;
#ifdef __CONFIG_APCLIENT_DHCPC__
			char ip[17] = {0},mask[17] = {0};
			gpi_get_apclient_dhcpc_addr(ip,mask);
			if(0 == strcmp(ip,"") || 0 == strcmp(ip,""))
			{
				p->color_action = COLOR_TRY;
				p->code = COMMON_CONNECTING;
			}
#endif		
			break;
		default:
			ret = PI_ERR;
			break;
	}

	return ret;
}

WANERRINFO tpi_get_err_info(WLMODE wl_mode)
{
	WANERRINFO ret = INFO_ERROR;
    WANSTATUS wan_status = WAN_NO_WIRE;
    int internetStat = 0;
	WIFISTASTATUS wifi_status = WIFI_INIT_FAIL;
    WAN_ERR_INFO_STRUCT p_info;
    P_WAN_ERR_INFO_STRUCT p;

    p = &p_info;

	memset(p,0x0,sizeof(WAN_ERR_INFO_STRUCT));
	
    p->wl_mode = wl_mode;
    p->wan_mode = tpi_get_wan_mode(wl_mode);

    if(WAN_NONE_MODE == p->wan_mode || PI_ERR == tpi_get_wan_status(p->wl_mode,&wan_status,&wifi_status))
    {
    	goto err_return;
    }
	
	if(WL_AP_MODE == p->wl_mode && WAN_NO_WIRE != wan_status && WAN_DISCONNECTED != wan_status)
	{
		if(0 == network_tpye)
		{
			p->network_check = NETWORK_CHECK_STATIC_MODE;
		}
		else if(1 == network_tpye)
		{
			p->network_check = NETWORK_CHECK_DHCP_MODE;
		}
		else if(2 == network_tpye)
		{
			p->network_check = NETWORK_CHECK_PPPOE_MODE;
		}
		else
		{
			p->network_check = NETWORK_CHECK_NONE_MODE;
		}
	}
	
	switch(wan_status)
	{
		case WAN_NO_WIRE:
			if(WL_AP_MODE != wl_mode)
			{
				goto err_return;
			}
			p->button_action = BUTTON_CONNECTED;
			p->color_action = COLOR_ERR;
			p->time_action = TIME_NONE;
			p->code = COMMON_NO_WIRE;
			break;
		case WAN_DISCONNECTED:
			p->button_action = BUTTON_CONNECTED;
			p->color_action = COLOR_ERR;
			p->time_action = TIME_NONE;
			p->code = COMMON_NOT_CONNECT;
			break;
		case WAN_CONNECTING:
			if(PI_ERR == tpi_get_connecting_err_info(p,wifi_status))
			{
				goto err_return;
			}
			break;
		case WAN_CONNECTED:
			if(PI_ERR == tpi_get_connected_err_info(p))
			{
				goto err_return;
			}
			break;
		default:
			goto err_return;
	}
	
	ret = tpi_get_err_result(p);
	
	return ret;	

err_return:
	return INFO_ERROR;
}

WANERRINFO tpi_get_wan_err_info()
{
    WANERRINFO ret = INFO_ERROR;
    WLMODE wl_mode = WL_AP_MODE;

    wl_mode = tpi_get_wl_mode();

    switch(wl_mode)
    {
        case WL_AP_MODE:
        case WL_WISP_MODE:
        case WL_APCLIENT_MODE:
			ret = tpi_get_err_info(wl_mode);
            break;
		case WL_WDS_MODE:
        default:
            break;
    }

    return ret;
}
