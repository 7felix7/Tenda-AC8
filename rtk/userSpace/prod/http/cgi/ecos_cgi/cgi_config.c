/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cgi_config.c
  �� �� ��   : ����
  ��    ��   : liquan
  ��������   : 2016��12��8��
  ����޸�   :
  ��������   :

  ��������   : ����ҳ��set��get�Ľӿں���

  �޸���ʷ   :
  1.��    ��   : 2016��12��8��
    ��    ��   : liquan
    �޸�����   : �����ļ�

******************************************************************************/
#include "webs.h"
#include "cgi_common.h"
#include "cgi_lib_config.h"

/*����ҳ��set����С����ģ�鵥Ԫ*/
CGI_MODULE_INFO cgi_set_public_modules[]=
{
    {"wanBasicCfg",     	CGI_SET,    	cgi_accessParam_set},
    {"lanCfg",         	 	CGI_SET,        cgi_syamanage_lan_set},
    {"wifiBasicCfg",    	CGI_SET,        cgi_wifiBasic_set},
    {"wifiPower",       	CGI_SET,        cgi_wifiPower_set},
    {"wifiTime",        	CGI_SET,        cgi_wifiSched_set},
#ifdef __CONFIG_WPS_RTK__
    {"wifiWPS",         	CGI_SET,        cgi_wifiWps_set},
#endif
    {"wifiAdvCfg",      	CGI_SET,        cgi_wifiParam_set},
    {"wifiRelay",           CGI_SET,        cgi_wifiRelay_set},
    {"synSysTime",         	CGI_SET,        cgi_wizard_systime_set},
    {"loginAuth",       	CGI_SET,        cgi_sysmanage_loginpwd_set},
    {"wanAdvCfg",       	CGI_SET,        cgi_sysmanage_wan_set},
    {"sysOperate",      	CGI_SET,        cgi_sysmanage_operate_set},
    {"softWare",        	CGI_SET,        cgi_sysmanage_fireware_set},
    {"sysTime",     		CGI_SET,        cgi_sysmanage_systime_set},
    {"remoteWeb",   		CGI_SET,        cgi_sysmanage_remoteWeb_set},
    {"staticIPList",        CGI_SET,        cgi_nat_staticip_set},
    {"portList",        	CGI_SET,        cgi_nat_portForward_set},
    {"ddns",            	CGI_SET,        cgi_nat_ddns_set},
    {"dmz",         		CGI_SET,        cgi_nat_dmz_set},
    {"upnp",           	 	CGI_SET,        cgi_nat_upnp_set},
    {"ping",				CGI_SET,		cgi_nat_ping_set},
    {"macFilter",       	CGI_SET,    	cgi_macfilter_set},
    {"onlineList",      	CGI_SET,    	cgi_tc_set_qoslist},
    {"parentCtrlList",      CGI_SET,    	cgi_parent_set_onlineList},
    {"parentAccessCtrl",	CGI_SET,    	cgi_parent_set_parentAccessCtrl},
#ifdef __CONFIG_TENDA_APP__
	/*��������*/
    {"noUpgradePrompt", 	CGI_SET,		cgi_notNow_upgrade_set}, /*�����ݲ�����*/	
#endif
#ifdef __CONFIG_GUEST__
	{"wifiGuest",       	CGI_SET, 		cgi_wifi_guest_info_set},
#endif
#ifdef __CONFIG_WL_BEAMFORMING_EN__
	{"wifiBeamforming",		CGI_SET,		cgi_beaforming_enable_set},
#endif

#ifdef __CONFIG_LED__
	{"LEDControl",	CGI_SET,    	cgi_power_save_led_set},
#endif
#ifdef __CONFIG_IPTV__
		{"IPTV",	CGI_SET,		cgi_nat_iptv_set},
#endif
    {"elink",                    CGI_SET,        cgi_nat_elink_set}, //elink
    {"",					CGI_NONE,		NULL},//����
};
/*����ҳ��get����С����ģ�鵥Ԫ*/
CGI_MODULE_INFO cgi_get_public_modules[]=
{
    {"wanBasicCfg",     	CGI_GET,        cgi_accessParam_get},
    {"internetStatus",  	CGI_GET,        cgi_system_get_internet_status},
    {"lanCfg",          	CGI_GET,        cgi_sysmanage_lan_get},
    {"wanAdvCfg",       	CGI_GET,        cgi_sysmanage_wan_get},
    {"wifiRelay",       	CGI_GET,        cgi_wifiRelay_get},
    {"wifiEn",              CGI_GET,    	cgi_wifiEn_get},
    {"wifiBasicCfg",    	CGI_GET,        cgi_wifiBasic_get},
    {"wifiPower",       	CGI_GET,        cgi_wifiPower_get},
    {"wifiTime",        	CGI_GET,        cgi_wifiSched_get},
#ifdef __CONFIG_WPS_RTK__
    {"wifiWPS",         	CGI_GET,        cgi_wifiWps_get},
#endif
    {"wifiAdvCfg",      	CGI_GET,        cgi_wifiParam_get},
    {"wifiScan",        	CGI_GET,        cgi_wifiScanresault},
    {"wanDetection",    	CGI_GET,        cgi_wanDetection_get},
    {"isWifiClients",   	CGI_GET,        cgi_isWifiClient_get},
    {"loginAuth",       	CGI_GET,        cgi_sysmanage_loginpwd_get},
    {"softWare",        	CGI_GET,        cgi_sysmanage_fireware_get},
    {"sysTime",     		CGI_GET,        cgi_sysmanage_systime_get},
    {"remoteWeb",   		CGI_GET,        cgi_sysmanage_remoteWeb_get},
    {"systemInfo",          CGI_GET,    	cgi_system_get_system_info},
    {"staticIPList",        CGI_GET,        cgi_nat_staticip_get},
    {"portList",        	CGI_GET,        cgi_nat_portForward_get},
    {"ddns",            	CGI_GET,        cgi_nat_ddns_get},
    {"dmz",         		CGI_GET,        cgi_nat_dmz_get},
    {"upnp",            	CGI_GET,        cgi_nat_upnp_get},
    {"ping",				CGI_GET,		cgi_nat_ping_get},
    {"macFilter",       	CGI_GET,    	cgi_get_macfilter_list},
    {"onlineList",      	CGI_GET,    	cgi_tc_get_online_list},
#ifdef __CONFIG_GUEST__
    {"guestList",      	CGI_GET,    	cgi_guest_get_online_list},
#endif
#ifdef __CONFIG_TC__
    {"deviceStatistics",    CGI_GET,    	cgi_system_get_device_statistics},
#endif
    {"localhost",       	CGI_GET,    	cgi_get_localhost},
    {"parentCtrlList",  	CGI_GET,    	cgi_parent_get_online_list},
    {"parentAccessCtrl", 	CGI_GET,   		cgi_parent_get_parentAccessCtrl},
#ifdef __CONFIG_TENDA_APP__
	/*��������*/
    {"hasNewSoftVersion",	CGI_GET,		cgi_ucloud_version_get},  //�°汾��Ϣ	
    {"onlineUpgradeReady",	CGI_GET,		cgi_ucloud_upgrade_get},  //��������
#endif
#ifdef __CONFIG_AUTO_CONN_CLIENT__
	{"synchroStatus",		CGI_GET,		cgi_auto_sync_info_get},  //
#endif
#ifdef __CONFIG_GUEST__
	{"wifiGuest",       	CGI_GET, 		cgi_wifi_guest_info_get},
#endif
#ifdef __CONFIG_WL_BEAMFORMING_EN__
	{"wifiBeamforming",		CGI_GET,		cgi_beaforming_enable_get},
#endif
#ifdef __CONFIG_LED__
	{"LEDControl",	CGI_GET,		cgi_power_save_led_get},
#endif
#ifdef __CONFIG_IPTV__
		{"IPTV",	CGI_GET,		cgi_nat_iptv_get},
#endif
	{"wpsModule",       CGI_GET,      cgi_wps_hasmodule},
	    {"elink",                    CGI_GET,        cgi_nat_elink_get}, //elink

    {"",					CGI_NONE,	NULL},//����
};

/* �����ҳ���˽�й��ܣ�Ҳ����˵������ĳЩҳ���ʱ����Ҫ����һЩ����Ĳ���
   �����������������ʵ�ֺ��� */
CGI_MODULE_INFO cgi_set_private_modules[]=
{
    {"setWizard",       	CGI_SET,    	cgi_wizard_set_succeed},
    {"setWAN",      		CGI_SET,    	cgi_set_wan},
    {"",					CGI_NONE,	NULL},//����
};


