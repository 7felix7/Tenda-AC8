/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cgi_lib_system_status.c
  �� �� ��   : ����
  ��    ��   : liquan
  ��������   : 2016��12��12��
  ����޸�   :
  ��������   :

  ��������   : system_info����С���ܵ�Ԫ��get��set��

  �޸���ʷ   :
  1.��    ��   : 2016��12��12��
    ��    ��   : luorilin
    �޸�����   : �����ļ�

******************************************************************************/
#include "cgi_lib.h"
#include "wan.h"
#include "wifi.h"
#include <router_net.h>
#include <netinet/if_ether.h>
#include "dhcp_server.h"
#include "systools.h"



#ifdef __CONFIG_INDICATE_LED__
typedef enum
{
    INDICATE_LED_OFF = 0,
    INDICATE_LED_EXIT
} LED_OFF_TYPE;
#endif
extern int dns_redirect_disable;//������ ./userSpace/cbb/src/dnsmasq/src/dnsmasq_proto.c


/*****************************************************************************
 �� �� ��  : cgi_lib_get_system_status_info
 ��������  : ��ȡsystem_status��wan��Ϣ
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_get_system_status_info(webs_t wp, cJSON *root, void *info)
{
    char value[PI_BUFLEN_256]      = { 0 };
    P_WAN_INFO_STRUCT wan_common_info = NULL;

    wan_common_info = gpi_wan_get_info();
    //wanIP
    memset(value, 0, sizeof(value));
    snprintf(value, sizeof(value), "%s", wan_common_info->wan_cur_ipaddr);
    if (strncmp(value, "0.0.0.0", strlen("0.0.0.0")) == 0)
    {
        strcpy(value,"");
    }
    cJSON_AddStringToObject(root,LIB_STATUS_WAN_IP,value);

    //wanMask
    memset(value, 0, sizeof(value));
    snprintf(value, sizeof(value), "%s", wan_common_info->wan_cur_mask);
    if (strncmp(value, "0.0.0.0", strlen("0.0.0.0")) == 0)
    {
        strcpy(value,"");
    }
    cJSON_AddStringToObject(root,LIB_STATUS_WAN_MASK,value);

    //wan gateway
    memset(value, 0, sizeof(value));
    snprintf(value, sizeof(value), "%s", wan_common_info->wan_cur_gw);
    if (strncmp(value, "0.0.0.0", strlen("0.0.0.0")) == 0)
    {
        strcpy(value,"");
    }
    cJSON_AddStringToObject(root,LIB_STATUS_WAN_GATWAY,value);

    //wanDns1
    cJSON_AddStringToObject(root,LIB_STATUS_DNS1,wan_common_info->wan_cur_dns1);

    //wanDns2
    cJSON_AddStringToObject(root,LIB_STATUS_DNS2,wan_common_info->wan_cur_dns2);

    return RET_SUC;
}
/*****************************************************************************
 �� �i ��  : cgi_lib_get_system_status_wanMac
 ��������  : ��ȡsystem_status��wanMac
 ������i  : webs_t wp
             cJSON *root
             void *info
 ������i  : �S
 �� �� ֵ  :

 GS����ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : luorilin
    GS�����I   : G����ɺ��i

*****************************************************************************/
RET_INFO cgi_lib_get_system_status_wanMac(webs_t wp, cJSON *root, void *info)
{
    P_WAN_HWADDR_INFO_STRUCT wan_hwaddr_info = gpi_wan_get_hwaddr_info();

    /*wan mac*/
    cJSON_AddStringToObject(root, LIB_STATUS_WAN_MAC, wan_hwaddr_info->wan_hwaddr);
    return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_lib_get_software_version
 ��������  : ��ȡsoftware_version
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_get_software_version(webs_t wp, cJSON *root, void *info)
{
    char iterm_value[PI_BUFLEN_64];

    /*software version*/
    memset(iterm_value, '\0', PI_BUFLEN_64 * sizeof(char));
    snprintf(iterm_value, PI_BUFLEN_64, T("%s_%s"), W311R_ECOS_SV, NORMAL_WEB_VERSION);
    cJSON_AddStringToObject(root, LIB_SOFTWARE_VERSION, iterm_value);
    return RET_SUC;
}
/*****************************************************************************
 �� �� ��  : cgi_lib_get_wan_connectTime
 ��������  : ��ȡwan_connectTime
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_get_wan_connectTime(webs_t wp, cJSON *root, void *info)
{
    char iterm_value[PI_BUFLEN_64];

    P_WAN_INFO_STRUCT wan_common_info = NULL;
    wan_common_info = gpi_wan_get_info();

    /*run time*/
    memset(iterm_value, '\0', PI_BUFLEN_64 * sizeof(char));
    snprintf(iterm_value, PI_BUFLEN_64, "%llu", wan_common_info->connect_time);
    cJSON_AddStringToObject(root, LIB_WAN_CONNECT_TIME, iterm_value);

    return RET_SUC;
}

/*��������������ģ�飬��Ҫ����cgi_api.h�ӿڣ�֮�����app�����ʹ����Ӧ�Ľӿڣ���������Ӻ����*/
#ifdef __CONFIG_TENDA_APP__

/*���������У����������صĴ�����*/
typedef enum
{
    UP_TYPE_MIN = 0,
    UP_TYPE_UPGRADE_Q = UP_TYPE_MIN,    //����ѯ������������
    UP_TYPE_MEM_NOT_ENOUGH,             //�ڴ治��,�޷�����
    UP_TYPE_WAIT_SVR,                   //·�������Ŷ�����
    UP_TYPE_FW_DOWNING,                 //·�����������ع̼�
    UP_TYPE_WRITING_TO_FLASH,           //·����������д�̼�
    UP_TYPE_MAX
} ENUM_UP_TYPE;

#define ERR_UCLOUD_BUSY 19  /*����������æ*/

static unsigned int fw_dowing_flage = 0; /*�Ƿ��������״̬�ı�־*/

/*****************************************************************************
 �� �� ��  : strCut_from_mid
 ��������  : ���ַ������м��ȡn���ַ�
 �������  : char *dst,char *src, int n,int m
             n:Ҫ��ȡn���ַ�
             m:�ӵ�m���ַ���ʼ��ȡ
 �������  : ��
 �� �� ֵ  : char *dst

 �޸���ʷ      :
  1.��    ��   : 2017��2��10��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
static char *strCut_from_mid(char *dst,const char *src, int n,int m) /*nΪ���ȣ�mΪλ��*/
{
    char *p = src;
    char *q = dst;
    int len = 0;
    if(NULL == src)
    {
        printf("[%s][%d] str is null!!\n", __FUNCTION__, __LINE__);
        return NULL;
    }
    len = strlen(src);

    if(n > len || n < 0 || m > len || m < 0)
    {
        printf("[%s][%d] input n/m error!!\n", __FUNCTION__, __LINE__);
        return NULL;
    }
    p += m;
    while(n--)
        *(q++) = *(p++);
    *(q++)='\0';

    return dst;
}
/*****************************************************************************
 �� �� ��  : is_wan_status_connect_ok
 ��������  : �ж��Ƿ�������
 �������  : ��
 �������  : ��
 �� �� ֵ  : ������:0  δ����:1

 �޸���ʷ      :
  1.��    ��   : 2017��2��10��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
int is_wan_status_connect_ok()
{
    char wan_connstatus[9] = {0};
    char temp[2] = {0};
    int wlmode = 0;   //wan�ڽ���ģʽ
    int ret = -1;
    /*��ȡ����״̬��*/
    sprintf(wan_connstatus,"%d",tpi_wan_get_err_result_info());

    /*��
    ����״̬��:��:13102060 (����λ�����ж��Ƿ�������;6:������; 4:wan���ѻ�ȡ��ip)
    */
    strCut_from_mid(temp,wan_connstatus,1,6);  /*��״̬���н�ȡ������λ*/

    wlmode = gpi_wifi_get_mode();   /*��ȡWAN�ڽ���ģʽ*/

    /*WL_APCLIENT_MODE �� WL_BRIDGEAP_MODE ģʽ��,���еĹ��涨:ֻҪDUT wan�ڻ�ȡ��ip,����Ϊ������*/
    /*������ڹ���޸ģ����Զ�λ��tpi_wan_get_connected_err_info�鿴,���������޸�*/
    if(WL_APCLIENT_MODE == wlmode || WL_BRIDGEAP_MODE == wlmode)
    {
        ret = strncmp(temp,"4",1);
    }
    else
    {
        ret = strncmp(temp,"6",1);
    }

    return ret;
}

/*�����̬ȫ�ֱ��� ���ں����°汾ʱ���Ա���*/
static char s_cur_version[PI_BUFLEN_64] = {0};

/*�����ṩ�Ľӿڣ�ͨ����������ӿڣ���ȡ������汾�Լ����������������е���Ϣ*/
extern int uc_cgi_get_module_param(cJSON *cj_query, cJSON *cj_get);


/*****************************************************************************
 �� �� ��  : cgi_lib_get_ucloud_version
 ��������  : cgi��ȡ�°汾
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_get_ucloud_version(webs_t wp, cJSON *root, void *info)
{
    char new_version[PI_BUFLEN_64] = {0};
    char cur_version[PI_BUFLEN_64] = {0};
    char *temp = NULL;
    char *description_save = NULL;
	int len = 0;

    cJSON *cj_get   = NULL;
    cJSON *module   = NULL;
    cJSON *cj_query = NULL;

	cj_query = cJSON_CreateObject();
    cj_get = cJSON_CreateObject();

	if(NULL == cj_query)
	{
		printf("[%s][%d] create cj_query err!\n",__func__,__LINE__);
		return RET_ERR; 
	}
	if(NULL == cj_get)
	{
		cJSON_Delete(cj_query);
		printf("[%s][%d] create cj_get err!\n",__func__,__LINE__);
		return RET_ERR;
	}
    //�ж�����״̬
    if ( 0 != is_wan_status_connect_ok())
    {
        /*δ���������*/
        cJSON_AddStringToObject(root, LIB_HANSNEWSOFTVERSION,"false");
    }
    else/*������������¿�ʼ��ѯ�汾����Ϣ*/
    {
        cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
        cJSON_AddItemToArray(module,cJSON_CreateString("uc_get_version"));
        uc_cgi_get_module_param(cj_query,cj_get);
        /*������汾��*/
        strcpy(new_version,cgi_lib_get_var(NULL,cj_get,"newest_ver",""));
    }
    //���°汾
    if(0 == strcmp(new_version,""))
    {
        cJSON_AddStringToObject(root, LIB_HANSNEWSOFTVERSION,"false");
    }
    else
    {
        memset(cur_version,0x0,sizeof(cur_version));
        strcpy(cur_version,nvram_safe_get("noprompt_version"));
        /*flashû�б��汣����ǰ*/
        if(0 == strcmp(cur_version,""))
        {
            sprintf(cur_version,"%s_%s", W311R_ECOS_SV, NORMAL_WEB_VERSION);
        }
        //�뱣����nvram�еĲο��汾�Ա�
        if(0 != strcmp(cur_version,"") && 0 != strcmp(new_version,cur_version))
        {
            strcpy(s_cur_version,new_version); /*���浱ǰ���°汾��/���ں��԰汾ʱ���Ա���*/

            cJSON_AddStringToObject(root, LIB_HANSNEWSOFTVERSION,"true");

            cJSON_AddStringToObject(root, LIB_UPDATEVERSION,new_version);  /*����汾��*/

            /*����°汾��Ϣ:�������޸��Ĺ����б�*/
            cJSON *description = NULL;

            description = cJSON_GetObjectItem(cj_get,"description"); /*���ļ�������*/

            temp = cJSON_Print(description);
			len = strlen(temp)*sizeof(char) + 1;
            description_save = malloc(len);
            memset(description_save,0x0,len);
            strcpy(description_save,temp);

            cJSON_AddItemToObject(root,LIB_NEWVERISONOPTIMIZE,cJSON_Parse(description_save));

            /*�������ʱ��*/
            cJSON_AddStringToObject(root,LIB_UPDATEDATE,cgi_lib_get_var(NULL,cj_get,"update_date",""));

			free(temp);
			free(description_save);
        }
        /*������ݲ�������,��ѵ�ǰ���°汾������flash��*/
        else if(0 != strcmp(cur_version,"") && 0 == strcmp(new_version,cur_version))
        {
            cJSON_AddStringToObject(root, LIB_HANSNEWSOFTVERSION,"false");
        }
    }
    /*�ͷ��ڴ�*/
    cJSON_Delete(cj_query);
    cJSON_Delete(cj_get);

    return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_lib_get_ucloud_upgrade
 ��������  : ��������
 �������  : webs_t wp
             cJSON *root
             void *info
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_get_ucloud_upgrade(webs_t wp, cJSON *root, void *info)
{
    cJSON *cj_get   = NULL;
    cJSON *module   = NULL;
    cJSON *cj_query = NULL;

    int fw_size = 0;    //�����ļ��ܴ�С
    int fw_recved = 0;     //��ǰ������
    int fw_down_sec_left = 0;  //����ʣ��ʱ��

    int error_code = -1;
    char *upgrade_btn = NULL;   //����������ť;upgrade=iimmediately

    upgrade_btn =  cgi_lib_get_var(wp,root, T("upgrade"), T(""));  //�������������ť

    /*�Ƿ�������������ť*/
    if(NULL != strstr(upgrade_btn, "immediately"))
    {
        cj_query = cJSON_CreateObject();
        cj_get = cJSON_CreateObject();

		if(NULL == cj_query)
		{
			printf("[%s][%d] create cj_query err!\n",__func__,__LINE__);
			return RET_ERR; 
		}
		if(NULL == cj_get)
		{
			cJSON_Delete(cj_query);
			printf("[%s][%d] create cj_get err!\n",__func__,__LINE__);
			return RET_ERR;
		}
		
        cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
        cJSON_AddItemToArray(module,cJSON_CreateString("uc_get_download_progress"));

        uc_cgi_get_module_param(cj_query,cj_get);  /*����������ȡ�����е���Ϣ*/

        /*���ó����ṩ�Ľӿڣ�ʵ��������������ȡ�����е���Ϣ*/
        error_code = cjson_get_number(cj_get,"error_code",0); /*��ȡ�����еĸ���״̬*/

        /*���뵽�����ؾ���״̬,����յ� UP_TYPE_UPGRADE_Q �� ERR_UCLOUD_BUSY,��������,
         ��ͣ��������ҳ��, һֱ������ʧ�ܻ����سɹ����Ƴ�����״̬ҳ��*/
        if(((UP_TYPE_UPGRADE_Q == error_code) || (ERR_UCLOUD_BUSY == error_code)) && (1 == fw_dowing_flage))
        {
            error_code = UP_TYPE_FW_DOWNING;
        }

        switch(error_code)
        {
            case UP_TYPE_UPGRADE_Q:/*����ѯ������������*/
            case UP_TYPE_WAIT_SVR:/*�����Ŷ���*/
            case ERR_UCLOUD_BUSY: /*��������æ*/
                //fw_dowing_flage = 0;
                cJSON_AddStringToObject(root,LIB_UPGRADEREADY,"wait");
                cJSON_AddStringToObject(root,LIB_WRITING_TO_FLASH,"false");
                /*������*/
                cJSON_AddNumberToObject(root, LIB_UPGRADEERORCODE,0);
                /*��������*/
                cJSON_AddNumberToObject(root, LIB_TOTALSIZE,0);
                /*�Ѿ�������*/
                cJSON_AddNumberToObject(root, LIB_DOWNLOADSIZE,0);
                /*ʣ������ʱ��*/
                cJSON_AddNumberToObject(root, LIB_RESTTIME,0);
                break;
            case UP_TYPE_FW_DOWNING:/*�������ع̼�*/
                /*��ȡ���������ļ��ܴ�С����ǰ���������Լ�����ʣ��ʱ��*/
                fw_size = cjson_get_number(cj_get,"fw_size",0);
                fw_recved = cjson_get_number(cj_get,"recved",0);
                fw_down_sec_left = cjson_get_number(cj_get,"sec_left",0);

                if(0 == fw_size || 0 == fw_recved)
                {
                    fw_dowing_flage = 0;
                    cJSON_AddStringToObject(root, LIB_UPGRADEREADY,"wait"); /*��ȡ�������ļ���СΪ0ʱ����ҳ��������ڵȴ�����״̬*/
                }
                else
                {
                    /*�����û������,����ʣ��ʱ���Ϊ0ʱ,����ʣ��ʱ��Ϊ1s*/
                    if((fw_size > fw_recved) && (0 == fw_down_sec_left))
                        fw_down_sec_left = 1;

                    fw_dowing_flage = 1;
                    cJSON_AddStringToObject(root, LIB_UPGRADEREADY,"true");
                }
                cJSON_AddStringToObject(root,LIB_WRITING_TO_FLASH,"false");
                /*������*/
                cJSON_AddNumberToObject(root, LIB_UPGRADEERORCODE,0);
                /*��������*/
                cJSON_AddNumberToObject(root, LIB_TOTALSIZE,fw_size);
                /*�Ѿ�������*/
                cJSON_AddNumberToObject(root, LIB_DOWNLOADSIZE,fw_recved);
                /*ʣ������ʱ��*/
                cJSON_AddNumberToObject(root, LIB_RESTTIME,fw_down_sec_left);
                break;
            case UP_TYPE_WRITING_TO_FLASH: /*������¼�̼�*/
                fw_dowing_flage = 0;
                cJSON_AddStringToObject(root, LIB_UPGRADEREADY,"true");
                cJSON_AddStringToObject(root,LIB_WRITING_TO_FLASH,"true");/*������¼*/
                /*������*/
                cJSON_AddNumberToObject(root, LIB_UPGRADEERORCODE,0);
                /*��������*/
                cJSON_AddNumberToObject(root, LIB_TOTALSIZE,1040384);
                /*�Ѿ�������*/
                cJSON_AddNumberToObject(root, LIB_DOWNLOADSIZE,1040384);
                /*ʣ������ʱ��*/
                cJSON_AddNumberToObject(root, LIB_RESTTIME,0);
                break;
            default:
                fw_dowing_flage = 0;
                cJSON_AddStringToObject(root,LIB_UPGRADEREADY,"fail");
                cJSON_AddStringToObject(root,LIB_WRITING_TO_FLASH,"false");
                /*������*/
                cJSON_AddNumberToObject(root, LIB_UPGRADEERORCODE,error_code);  /*���������뷵��*/
                /*��������*/
                cJSON_AddNumberToObject(root, LIB_TOTALSIZE,0);
                /*�Ѿ�������*/
                cJSON_AddNumberToObject(root, LIB_DOWNLOADSIZE,0);
                /*ʣ������ʱ��*/
                cJSON_AddNumberToObject(root, LIB_RESTTIME,0);
                break;
        }

        /*�ͷ��ڴ�*/
        cJSON_Delete(cj_query);
        cJSON_Delete(cj_get);

    }

    return RET_SUC;
}

/*****************************************************************************
 �� �� ��  : cgi_notNow_upgrade_set
 ��������  : �ݲ�����
 �������  : struct upgrade_message *mess
             char *data_save
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��12��13��
    ��    ��   : luorilin
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_notNow_upgrade_set(webs_t wp,cJSON *root,CGI_MSG_MODULE *msg, char *err_code,void *info)
{
    char    *no_prompt = NULL;  //�ο��汾;

    no_prompt =  cgi_lib_get_var(wp,root, T(LIB_NOPROMPT), T("off"));//�ݲ�������ť

    if(0 == strcmp(no_prompt, "on"))
    {
        nvram_set("noprompt_version", s_cur_version);   //�����°汾���浽nvram�� �´����ο�
    }

    sprintf(err_code, "%s", "0");

    return RET_SUC;
}
#endif //__CONFIG_TENDA_APP__


/*****************************************************************************
 �� �� ��  : cgi_lib_set_wizard_succeed
 ��������  : app��ɿ�������
 �������  :
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2017��1��5��
    ��    ��   : liusongming
    �޸�����   : �����ɺ���

*****************************************************************************/
RET_INFO cgi_lib_set_wizard_succeed(webs_t wp,cJSON *root,CGI_MSG_MODULE *msg,char *err_code,void *info)
{
    //add by z10312 dns�ض���ֻ�ڿ�������ǰ��Ч
    //dns_redirect_disable = 1;
    _SET_VALUE("mode_need_switch", "no");//��־���������ͼ���������ǰ����
    _SET_VALUE(SYSCONFIG_QUICK_SET_ENABLE, "0"); //��־�ſ������ý���
#ifdef __CONFIG_PPPOE_SERVER__
    set_synchro_type(MANUAL_SYNCHRO);
#endif
#ifdef __CONFIG_INDICATE_LED__
    set_indicate_led_off(INDICATE_LED_EXIT);
#endif

    return RET_SUC;
}


