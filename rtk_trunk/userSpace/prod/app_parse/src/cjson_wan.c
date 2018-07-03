/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : cjson_wan.c
  �� �� ��   : ����
  ��    ��   : liusongming
  ��������   : 2016��12��19��
  ����޸�   :
  ��������   :

  ��������   : app��ȡ�������豸����

  �޸���ʷ   :
  1.��    ��   : 2016��12��19��
    ��    ��   : liusongming
    �޸�����   : �����ļ�

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "cjson_handle.h"
#include "cgi_lib_config.h"
#include "wan.h"


#define WAN_NUM 1   //WAN�ڸ���
/****************************************************************************
������  :app_get_wan_basic
����    :����app��ȡWAN�ڻ�����Ϣ
����    :
    recv_root:
         ��ʽ:{"module":["APP_GET_WAN_BASIC"]}
    send_root:���ں����ش����ݣ�
        ��ʽ:{"n_wan":1,"wan":[{"interfacenum":1,"type":1,"adsl_name":"ruanjian06",
        "adsl_pwd":"ruanjian06","ip_addr":0,"netmask":2148813625,"gateway":2160369456,
        "primary_dns":4,"backup_dns":0,"conn_time":0,"sta":0}]}

    info:��
��    ��   : 2016��12��23��
��    ��   : liusongming
�޸�����   : �½�����
****************************************************************************/

void app_get_wan_basic(cJSON *recv_root,cJSON *send_root, void *info)
{
    cJSON *obj;
    cJSON *wan = NULL,*wanitem = NULL;
    int n_wan = 0;
    int i = 0;
    char *tmp_var = NULL;
    char *adsl_name = NULL,*adsl_pwd = NULL;
    char *wanIp = NULL,*wanMask = NULL,*wanGateway = NULL,*wanDns1 = NULL,*wanDns2 = NULL;
    char *wanConnectTime = NULL;
    unsigned int ip_addr = 0,netmask = 0,gateway = 0,primary_dns = 0,backup_dns = 0;
    int conn_time = 0;
    int wanConnectStatus = 0;
    CGI_LIB_INFO get_info;

    if(NULL == recv_root)
    {
        printf("func:%s line:%d recv_root is NULL\n",__func__,__LINE__);
        return;
    }


    //����cgi_lib���ȡapp��Ҫ����Ϣ
    PIU8 modules[] =
    {
        MODULE_GET_WAN_DETECTION,
        MODULE_GET_WAN_TYPE,
        MODULE_GET_ADSL_INFO,
        MODULE_GET_NET_INFO,
        MODULE_GET_WAN_SPEED,
        MODULE_GET_WAN_MAC,
        MODULE_GET_WAN_CONNECT_TIME,
        MODULE_GET_INTERNET_STATUS,
    };

    obj = cJSON_CreateObject();
    if(NULL == obj)
    {
        printf("func:%s line:%d create obj fail\n",__func__,__LINE__);
        return;
    }

    get_info.wp = NULL;
    get_info.root = obj;
    get_info.modules = modules;
    get_info.module_num = ARRAY_SIZE(modules);

    cgi_lib_get(get_info, NULL);        //����lib�⹫������

    cJSON_AddNumberToObject(send_root,"n_wan",WAN_NUM);  //���wan�ڸ�������F9ֻ��һ��WAN��
    cJSON_AddItemToObject(send_root,"wan",wan = cJSON_CreateArray()); //�������������ڴ��wan�ڵĻ�����Ϣ

    //�������ݽ����Լ���װ
    for(i = 0; i < WAN_NUM; i++)
    {
        cJSON_AddItemToArray(wan,wanitem = cJSON_CreateObject());
        if(NULL == wanitem)
        {
            printf("func:%s line:%d Create wanitem fail\n",__func__,__LINE__);
            cJSON_Delete(obj);
            return ;
        }
        cJSON_AddNumberToObject(wanitem,"interfacenum",i + 1);      //��ʾ�ڼ���wan��

        tmp_var = cjson_get_value(obj,"wanType","");    //��ȡWAN�ڵĽ�������
        if(NULL == tmp_var)
        {
            printf("func:%s line:%d get wanType fail\n",__func__,__LINE__);
            cJSON_Delete(obj);
            return;
        }
        if(!strcmp("static",tmp_var))       //��̬���뷽ʽ,app�ı�ʾ��ʽΪ:pppoe:1; dhcp:2;static:3
        {
            cJSON_AddNumberToObject(wanitem,"type",STATIC);
        }

        else if(!strcmp("dhcp",tmp_var))
        {
            cJSON_AddNumberToObject(wanitem,"type",DYNAMIC);
        }
        else if(!strcmp("pppoe",tmp_var))
        {
            cJSON_AddNumberToObject(wanitem,"type",ADSL);

        }
        else
        {
            cJSON_AddNumberToObject(wanitem,"type",0);  //0����δ֪����
        }
        //��ȡpppoe���˻���������
        adsl_name = cjson_get_value(obj,LIB_PPPOE_USER,"");
        adsl_pwd = cjson_get_value(obj,LIB_PPPOE_PWD,"");
        if((adsl_name == NULL) || (adsl_pwd == NULL))
        {
            printf("get adsl name or adsl password fail\n");
            cJSON_Delete(obj);
            return;
        }
        //���˻�����������Cjson����ʽ���
        cJSON_AddStringToObject(wanitem,"adsl_name",adsl_name);
        cJSON_AddStringToObject(wanitem,"adsl_pwd",adsl_pwd);

        //��ȡWAN�ڵ�ip,mask,gateway,dns
        wanIp = cjson_get_value(obj,LIB_WAN_IP,"");
        wanMask = cjson_get_value(obj,LIB_WAN_MASK,"");
        wanGateway = cjson_get_value(obj,LIB_WAN_GATEWAY,"");
        wanDns1 = cjson_get_value(obj,LIB_WAN_DNS1,"");
        wanDns2 = cjson_get_value(obj,LIB_WAN_DNS2,"");
        wanConnectTime = cjson_get_value(obj,LIB_WAN_CONNECT_TIME,"0");
        //��wan�ڵ�ip,mask,gateway,dns��CJSON����ʽ���
        cJSON_AddStringToObject(wanitem,"ip_addr",wanIp);
        cJSON_AddStringToObject(wanitem,"netmask",wanMask);
        cJSON_AddStringToObject(wanitem,"gateway",wanGateway);
        cJSON_AddStringToObject(wanitem,"primary_dns",wanDns1);
        cJSON_AddStringToObject(wanitem,"backup_dns",wanDns2);
        cJSON_AddStringToObject(wanitem,"conn_time",wanConnectTime);

        //ת����������,ֻ����δ����������²Ż��app����err�ֶΣ�������app��ȡerr�ֶ�ʱ������Ҫ���ж��Ƿ���δ����
        tmp_var = cjson_get_value(obj,"wanConnectStatus","0");
        wanConnectStatus = atoi(tmp_var);
        wanConnectStatus = (wanConnectStatus % 1000)/10;
        switch(wanConnectStatus)
        {
            case COMMON_NO_WIRE:
                cJSON_AddNumberToObject(wanitem,"sta",0);  //δ������
                break;
            case COMMON_NOT_CONNECT:
                cJSON_AddNumberToObject(wanitem,"sta",1);  //δ����
                break;
            case COMMON_CONNECTING:
                cJSON_AddNumberToObject(wanitem,"sta",2);  //������
                break;
            case COMMON_CONNECTED_ONLINEING:
                cJSON_AddNumberToObject(wanitem,"sta",3);  //������
                break;
            case COMMON_ONLINEED:
                cJSON_AddNumberToObject(wanitem,"sta",3);  //������
                cJSON_AddNumberToObject(wanitem,"err",0);  //app�涨:0-����
                break;
            case COMMON_NOT_ONLINE:
                cJSON_AddNumberToObject(wanitem,"sta",1);      //δ����
                cJSON_AddNumberToObject(wanitem,"err",566);    //566-�޷����ӵ�������
                break;
            case (COMMON_ONLINEED + 1):
                cJSON_AddNumberToObject(wanitem,"sta",1);     //δ����
                cJSON_AddNumberToObject(wanitem,"err",549);   // 549-�û������������
                break;
            case (COMMON_ONLINEED + 2):
                cJSON_AddNumberToObject(wanitem,"sta",1);             //δ����
                cJSON_AddNumberToObject(wanitem,"err",548); //0-������ 566-�޷����ӵ���������549-�û������������548-Զ�˷���������Ӧ */
                break;
            default:
                break;
        }


    }

    cJSON_Delete(obj);

    return ;
}


/****************************************************************************
������  :app_set_wan_basic
����    :����app����WAN�ڻ�����Ϣ
����    :
    send_root:���ں����ش����ݣ�
        ��ʽ:{"n_wan":1,"wan":[{"adsl_name":"ruanjian06","adsl_pwd":"ruanjian06"}]}

        msg:
   err_code:���ڴ�Ŵ�����
    info:��
��    ��   : 2016��12��23��
��    ��   : liusongming
�޸�����   : �½�����
****************************************************************************/
void app_set_wan_basic(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
    CGI_LIB_INFO set_info;
    cJSON *obj = NULL;
    PI8 err_code[PI_BUFLEN_32] = {0};
    cJSON *wan = NULL,*wanitem = NULL;
    int type = 0;
    CGI_MSG_MODULE remov_msg;
    char *wan_ip = NULL, *gw = NULL,*wan_msk = NULL,*dns1 = NULL,*dns2 = NULL;
    char *wanPPPoEUser = NULL, *wanPPPoEPwd = NULL;

    if((NULL == send_root) || (NULL == result_code))
    {
        printf("recv_root is null\n");
        return ;
    }

    wan = cJSON_GetObjectItem(send_root,"wan");
    if(NULL == wan)
    {
        printf("func:%s line:%d get wan obj fail\n",__func__,__LINE__);
        return ;
    }
    //��app�����������װ��CGIҪ������ݸ�ʽ
    wanitem = cJSON_GetArrayItem(wan,0);  //Ĭ��ȡwan�����еĵ�һ��wan����Ϣ��˫wan�����Ҫѭ��ȡ
    obj = cJSON_CreateObject();
    if(NULL == obj)
    {
        printf("create obj fail\n");
        return;
    }
    //��ȡ��������
    type = cjson_get_number(wanitem,"type",0);
    if(type == STATIC)      //��̬����
    {
        cJSON_AddStringToObject(obj, LIB_WAN_TYPE, "static");
        //��ȡIP
        wan_ip = cjson_get_value(wanitem,"ip_addr","0.0.0.0");
        cJSON_AddStringToObject(obj, LIB_WAN_IP, wan_ip);
        //��ȡ��������
        wan_msk = cjson_get_value(wanitem,"netmask","0.0.0.0");
        cJSON_AddStringToObject(obj, LIB_WAN_MASK, wan_msk);
        //��ȡ����
        gw = cjson_get_value(wanitem,"gateway","0.0.0.0");
        cJSON_AddStringToObject(obj, LIB_WAN_GATEWAY, gw);
        //��ȡDNS
        dns1 = cjson_get_value(wanitem,"primary_dns","");
        cJSON_AddStringToObject(obj, LIB_WAN_DNS1, dns1);

        dns2 = cjson_get_value(wanitem,"backup_dns","");
		dns2 = strncmp(dns2,"0.0.0.0",strlen("0.0.0.0")) ? dns2 : "";
        cJSON_AddStringToObject(obj, LIB_WAN_DNS2, dns2);

    }
    else if(DYNAMIC == type)    //��̬����
    {
        cJSON_AddStringToObject(obj, LIB_WAN_TYPE, "dhcp");
    }
    else if(ADSL == type)       //pppoe����
    {
        //��ȡ�˻���������
        wanPPPoEUser = cjson_get_value(wanitem,"adsl_name","");
        wanPPPoEPwd = cjson_get_value(wanitem,"adsl_pwd","");

        cJSON_AddStringToObject(obj, LIB_WAN_TYPE, "pppoe");
        cJSON_AddStringToObject(obj, LIB_PPPOE_USER, wanPPPoEUser);
        cJSON_AddStringToObject(obj, LIB_PPPOE_PWD, wanPPPoEPwd);
    }
    else        //δ֪����
    {
        printf("unknow type\n");
        cJSON_Delete(obj);
        return ;
    }

    //����lib���ȡ������Ϣ
    PIU8 modules[] =
    {
        MODULE_SET_WAN_ACCESS,
    };
    set_info.wp = NULL;
    set_info.root = obj;
    set_info.modules = modules;
    set_info.module_num = ARRAY_SIZE(modules);
    cgi_lib_set(set_info, msg, err_code, NULL);  //���ù���set��������

    memset(remov_msg.msg,0x0,MAX_MODULE_MSG_MAX_LEN);
    remov_msg.id = RC_WAN_MODULE;
    remove_msg_to_list(msg,remov_msg);  //�Ƴ�WANģ����ص���Ϣ

    if(strcmp(err_code,"0"))
        *result_code = 1;

    cJSON_Delete(obj);
    return ;
}
/****************************************************************************
������  :app_set_wan_basic_process
����    :����app����wan��֮��ʹ������Ч
����    :
    send_root:

    info:��
��    ��   : 2016��12��23��
��    ��   : liusongming
�޸�����   : �½�����
****************************************************************************/
void app_set_wan_basic_process(cJSON *send_root,CGI_MSG_MODULE *msg,int *result_code,void *info)
{
    CGI_MSG_MODULE msg_tmp;
    msg_tmp.id = RC_WAN_MODULE;
    sprintf(msg_tmp.msg, "op=%d", OP_RESTART);
    add_msg_to_list(msg, &msg_tmp);
}



/****************************************************************************
������  :app_get_wan_rate
����    :����app��ȡWAN������
����    :
    send_root:���ں����ش����ݣ�

    info:��
��    ��   : 2016��12��23��
��    ��   : liusongming
�޸�����   : �½�����
****************************************************************************/

void app_get_wan_rate(cJSON *recv_root,cJSON *send_root, void *info)
{
    cJSON *obj;
    CGI_LIB_INFO get_info;
    int interfacenum = 0,cur_uplink = 0,cur_downlink = 0,max_uplink = 0,max_downlink =0;
    char *statusUpSpeed = NULL,*statusDownSpeed = NULL;

    if(NULL == recv_root)
    {
        printf("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
        return ;
    }

    //���ÿ⺯����ȡ������Ϣ
    PIU8 modules[] =
    {
        MODULE_GET_STREAM_STATISTIC,
    };

    obj = cJSON_CreateObject();

    get_info.wp = NULL;
    get_info.root = obj;
    get_info.modules = modules;
    get_info.module_num = ARRAY_SIZE(modules);
    cgi_lib_get(get_info, NULL);    //����WAN�ڹ���get������ȡ��Ϣ

    //��������ת��
    statusUpSpeed = cjson_get_value(obj,"statusUpSpeed","0");
    cur_uplink = atoi(statusUpSpeed) * 1000;
    statusDownSpeed = cjson_get_value(obj,"statusDownSpeed","0");
    cur_downlink = atoi(statusDownSpeed) * 1000;

    cJSON_AddNumberToObject(send_root,"interfacenum",1);
    cJSON_AddNumberToObject(send_root,"cur_uplink",cur_uplink);
    cJSON_AddNumberToObject(send_root,"cur_downlink",cur_downlink);
    cJSON_AddNumberToObject(send_root,"max_uplink",0);  //Ĭ��Ϊ0���ɣ�app�ò�������������
    cJSON_AddNumberToObject(send_root,"max_downlink",0);

    cJSON_Delete(obj);

    return ;
}


/****************************************************************************
������  :app_get_wan_rate
����    :����WAN�ڼ�⣬ֻ�ڿ�������ҳ�����
����    :
    send_root:���ں����ش����ݣ�

    info:��
��    ��   : 2016��12��23��
��    ��   : liusongming
�޸�����   : �½�����
****************************************************************************/
void app_get_wan_detect(cJSON *recv_root,cJSON *send_root, void *info)
{
    cJSON *obj;
    char *wanDetection = NULL;
    CGI_LIB_INFO get_info;

    if(NULL == recv_root)
    {
        printf("file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
        return ;
    }

    //����lib���ȡ����
    PIU8 modules[] =
    {
        MODULE_GET_WAN_DETECTION,
    };

    obj = cJSON_CreateObject();

    get_info.wp = NULL;
    get_info.root = obj;
    get_info.modules = modules;
    get_info.module_num = ARRAY_SIZE(modules);
    cgi_lib_get(get_info, NULL);

    //�����ݽ��н���
    wanDetection = cjson_get_value(obj,"wanDetection","dhcp");
    if(NULL == wanDetection)
    {
        printf("get wanDetection fail\n");
        cJSON_Delete(obj);
        return ;
    }
    if(!strcmp(wanDetection,"disabled"))    //δ������
    {
        cJSON_AddNumberToObject(send_root,"detect_type",NO_LINE);
    }
    else
    {
        if(!strcmp(wanDetection,"static"))  //��̬
        {
            cJSON_AddNumberToObject(send_root,"detect_type",DET_STATIC);

        }
        else if(!strcmp(wanDetection,"dhcp")) //��̬
        {
            cJSON_AddNumberToObject(send_root,"detect_type",DET_DHCP);

        }
        else if(!strcmp(wanDetection,"pppoe"))  //pppoe
        {
            cJSON_AddNumberToObject(send_root,"detect_type",DET_PPPOE);

        }
        else if(!strcmp(wanDetection,"detecting"))  //�����
        {
            cJSON_AddNumberToObject(send_root,"detect_type",DETECTING);

        }
    }


    cJSON_Delete(obj);


    return ;
}



