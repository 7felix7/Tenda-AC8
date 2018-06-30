#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bcmnvram.h>
#include "pi_common.h"
#include <shutils.h>

	     
extern void SoftNAT_OP_Mode(int count);
extern int RunSystemCmd(char *filepath, ...);
extern int rtl_vlan_support_enable;
static int rtl_vlan_support_enable_old = 0;

/*****************************************************************************
 �� �� ��  : tenda_set_port_pvid
 ��������  : port�ڵ�pvid���ã���iptv���и�����ӿ��ƺ�
 �������  : unsigned int lan_pvid��wan_pvid�� iptv_stb_pvid��������Ĭ��Ϊ1
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : �����ɺ���
*****************************************************************************/
void tenda_set_port_pvid(unsigned int wan_pvid, unsigned int iptv_stb_pvid)
{
	char name[80] = {0};
	char *next = NULL;
	char *value = NULL;
	char *iptv_portName = NULL;
	char vlan_nat_port[128] = {0};

	iptv_portName = nvram_get("iptv_portName");
	value = nvram_get("vlan_nat_port");

	if((iptv_portName == NULL) || (value == NULL))
		return ;
	
	if (iptv_stb_pvid > 3) {
	   rtl_setPortDefVlanByDevName(iptv_stb_pvid, 0, 0, iptv_portName);
	   rtl_setPortDefVlanByDevName(3, 0, 0, "wan");
	} else {
	   rtl_setPortDefVlanByDevName(3, 0, 0, iptv_portName);
	   rtl_setPortDefVlanByDevName(2, 0, 0, "wan");
	}

	strcpy(vlan_nat_port,value);
	foreach(name, vlan_nat_port, next)
	{
		rtl_setPortDefVlanByDevName(1, 0, 0, name);		
	}
}

/*****************************************************************************
 �� �� ��  : tenda_addRtlVlan
 ��������  : ���iptv���б���vlan,���iptv��,��Ӻ�д��nvram��
 �������  : unsigned int vlan_Vid
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : �����ɺ���
*****************************************************************************/
void tenda_add_iptv_Vlan(unsigned int vlan_Vid)
{
	char *iptv_portName = NULL;
	iptv_portName = nvram_get("iptv_portName");

	if(iptv_portName == NULL)
		return ;

	rtl_addRtlVlanEntry(vlan_Vid);
	rtl_setRtlVlanForwardRule(vlan_Vid, 2);
	rtl_addRtlVlanMemberPortByDevName(vlan_Vid, iptv_portName);
	rtl_addRtlVlanMemberPortByDevName(vlan_Vid, "wan");
	rtl_setRtlVlanPortTagByDevName(vlan_Vid, iptv_portName, 1);
	rtl_setRtlVlanPortTagByDevName(vlan_Vid, "wan", 1);
}

/*****************************************************************************
 �� �� ��  : Vlan_init_config
 ��������  : vlan��ʼ������
 �������  : unsigned int set_enable_iptv
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : �����ɺ���
*****************************************************************************/
void Vlan_init_config(unsigned int set_enable_iptv, unsigned int iptv_port_pvid)
{
	char name[80] = {0};
	char *next = NULL;
	char *value = NULL;
	char *iptv_portName = NULL;
	char vlan_nat_port[128] = {0};

	iptv_portName = nvram_get("iptv_portName");
	value = nvram_get("vlan_nat_port");

	if((iptv_portName == NULL) || (value == NULL))
		return ;
	rtl_vlan_support_enable_old = rtl_vlan_support_enable;
	rtl_vlan_support_enable = 0;

	//pvid����Ҫ���ڴ˴������Ĭ��iptv���ò����ϼ�����ip����
	tenda_set_port_pvid(3, iptv_port_pvid);
	rtl_addRtlVlanEntry(1);
	rtl_setRtlVlanForwardRule(1,1);

	strcpy(vlan_nat_port,value);
	foreach(name, vlan_nat_port, next)
	{
		rtl_addRtlVlanMemberPortByDevName(1,name);
	}


	 rtl_addRtlVlanEntry(2);
	 rtl_setRtlVlanForwardRule(2,1);
	 rtl_addRtlVlanMemberPortByDevName(2,"lan8");
	 rtl_addRtlVlanMemberPortByDevName(2,"wan");

	 rtl_addRtlVlanEntry(3);
	if (iptv_port_pvid > 3) {
	     rtl_setRtlVlanForwardRule(3, 1);
	} else {
	     rtl_setRtlVlanForwardRule(3, 2);
	 }
	 rtl_addRtlVlanMemberPortByDevName(3,iptv_portName);
	 rtl_addRtlVlanMemberPortByDevName(3,"wan");

#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)
    rtl_addCustomPassthroughVlanInfo();
#endif
    rtl_vlan_support_enable = 1;
}

/*****************************************************************************
 �� �� ��  : tenda_set_vlanInfo
 ��������  : iptv����ʱ��ز�������
 �������  : unsigned int set_enable_iptv, iptv_port_pvid, vlan_id
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : �����ɺ���
*****************************************************************************/
void tenda_set_vlanInfo(unsigned int set_enable_vlan, unsigned int set_enable_iptv, unsigned int iptv_port_pvid, unsigned int vlan_id)
{
    char *value;
    char vlan_tmp[1024];
    char *tmp;   
    char *tok;
    rtl_vlan_support_enable_old = rtl_vlan_support_enable;
    rtl_vlan_support_enable = 0;

    if(set_enable_vlan == 0)
        return;

    rtl_flushRtlVlanEntry();
    Vlan_init_config(set_enable_iptv, iptv_port_pvid);
    value = nvram_get(ADVANCE_IPTV_VLAN_ID);
    stpcpy(vlan_tmp, value);
    tmp = vlan_tmp;
    while(tmp != NULL) {
        tok = strsep(&tmp,",");
        tenda_add_iptv_Vlan(atoi(tok)); //��������vlan
    }
    #if defined(CONFIG_RTL_CUSTOM_PASSTHRU)
    rtl_addCustomPassthroughVlanInfo();
    #endif
    rtl_vlan_support_enable = 1;
}
/*****************************************************************************
 �� �� ��  : config_tenda_vlan
 ��������  : vlanʹ�ܣ�iptvʹ�ܿ���
 �������  : Ŀǰ:��
 �������  : ��
 �� �� ֵ  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��11��7��
    ��    ��   : cz
    �޸�����   : �����ɺ���
*****************************************************************************/

void config_tenda_vlan(void)
{
    unsigned int set_enable_vlan;
    unsigned int set_enable_iptv;
    unsigned int vlan_id;
    unsigned int iptv_port_pvid;
    char *value = NULL;

    if(nvram_match(ADVANCE_IPTV_ENABLE,"1") 
		&& nvram_match(SYSCONFIG_WORKMODE,"route"))
    {
            set_enable_iptv = 1;
            set_enable_vlan = 1;
     }
    else
    {
            return;
    }

    value = nvram_get(ADVANCE_IPTV_VLAN_ID_CUR);
    if(value == NULL)
    {
         vlan_id = 3;
         iptv_port_pvid = 3;
    }
    else
    {
        vlan_id = atoi(value);        
        iptv_port_pvid = vlan_id;
    }

    printf("set_enable_vlan : %u  set_enable_iptv:%u iptv_port_pvid:%u\n",set_enable_vlan,set_enable_iptv,iptv_port_pvid);
    tenda_set_vlanInfo(set_enable_vlan, set_enable_iptv, iptv_port_pvid, vlan_id);
}


/* ��װvlan �ӿ�, RTL819X ��ʵ��, ��δ��װbcm, add by z10312 20160104 */
void tapf_vlan_config()
{

#ifdef CONFIG_RTL_HARDWARE_NAT
#ifndef HAVE_NOETH
    extern int rtl_setWanNetifMtu(int mtu);
    rtl_setWanNetifMtu(1500);
#endif
#endif
                        

#ifdef  RTL819X

    if(nvram_match(SYSCONFIG_WORKMODE, "route") 
        || nvram_match(SYSCONFIG_WORKMODE, "bridge"))
    {
        /* AP��ģʽ�����wan�ӿ�����Ϊeth0��ôwan��lan��ͬһ��vlan */
        if(nvram_match(SYSCONFIG_WORKMODE, "bridge")
        && nvram_match("wan0_ifname","eth0"))
            SoftNAT_OP_Mode(1);
        else
            SoftNAT_OP_Mode(2);
    }
    else if(nvram_match(SYSCONFIG_WORKMODE, "wisp") 
        || nvram_match(SYSCONFIG_WORKMODE, "client+ap") )
/*�޸������м�ģʽ��WAN�ڲ�����ΪLAN��*/
        SoftNAT_OP_Mode(2);
     else
            SoftNAT_OP_Mode(2);
    
#endif
    PI_PRINTF(MAIN,"vlan config ok!\n");
    return;
}
