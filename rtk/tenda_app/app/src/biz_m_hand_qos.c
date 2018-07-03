#include "cgi_module_interface.h"
#include "biz_m_hand_qos.h"


/*************************************************************************
  ����: ��ȡ�豸�Ĵ���������Ϣ
  ����: hand_qos_get_param_t  ָ����ȡ�����豸���������豸
         hand_qos_common_ack_t  ����ȡ����Ϣ����ýṹ
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_hand_qos_info_get_cb (const api_cmd_t *cmd,
                              hand_qos_get_param_t *param,
                              hand_qos_common_ack_t **rules_info,
                              void *privdata)
{
    int ret = 0;
    int len = 0;
    int count = 0;
    cJSON *cj_get   = NULL;
    cJSON *module   = NULL;
    cJSON *cj_query     = NULL;
    cJSON *extern_data = NULL;

    cj_query = cJSON_CreateObject();
    cj_get = cJSON_CreateObject();
    cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
    cJSON_AddItemToArray(module,cJSON_CreateString(GET_HAND_QOSINFO));

    /*��ȡ�����������������豸����Ϣ����Ҫ����opt*/
    if(HAND_QOS_OPT_GET_SPEC == param->opt)
    {
        cJSON_AddItemToObject(cj_query, "extern_data", extern_data = cJSON_CreateObject());
        cJSON_AddStringToObject(extern_data,"opt","one");
        cJSON_AddStringToObject(extern_data,"mac",param->mac);
    }
    else
    {
        cJSON_AddItemToObject(cj_query, "extern_data", extern_data = cJSON_CreateObject());
        cJSON_AddStringToObject(extern_data,"opt","all");
    }

    ret = app_get_module_param(cj_query,cj_get);




    /*��ȡ����*/
    int i = 0;

    cJSON *hand_qoInfo = NULL;
    cJSON *child_obj = NULL;
    char *rate = NULL;

    hand_qoInfo = cJSON_GetObjectItem(cj_get,"hand_qosInfo");
    /*�����豸������*/
    if(hand_qoInfo)
    {
        count = cJSON_GetArraySize(hand_qoInfo);
    }
    else
    {
        count = 0;
    }
    /*���ݻ�ȡ����count���������ڴ�*/
    len = sizeof(hand_qos_common_ack_t) + sizeof(hand_qos_rule_t) * count;
    *rules_info = (hand_qos_common_ack_t *)malloc(len);

    if(!(*rules_info))
    {
        printf("hosts_info alloc mem is fail\n");
        cJSON_Delete(cj_get);
        cJSON_Delete(cj_query);
        return 1;
    }
    memset(*rules_info, 0, len);
    hand_qos_info_t *qos_info = &((*rules_info)->info);

    for(i = 0; i < count; i++)
    {
        child_obj = cJSON_GetArrayItem(hand_qoInfo,i); /*/��ȡ����Ԫ��*/

        //��ȡһ�����ٹ���
        strcpy(qos_info->rules[i].mac_addr,cjson_get_value(child_obj, "mac",""));  /*mac*/

        qos_info->rules[i].u_rate = cjson_get_number(child_obj, "u_rate",-1);  /*�ϴ�����*/
        qos_info->rules[i].d_rate = cjson_get_number(child_obj, "d_rate",-1);  /*��������*/
        SET_HAND_QOS_DOWN_RATE(&(qos_info->rules[i]));
        SET_HAND_QOS_UP_RATE(&(qos_info->rules[i]));

        strcpy(qos_info->rules[i].dev_name,cjson_get_value(child_obj, "dev_name",""));  /*dev_name*/
        SET_HAND_QOS_DEV_NAME(&(qos_info->rules[i]));

        //printf("====>>>>:mac:%s,u_rate:%2f,d_rate:%2f,dev_name:%s\n",qos_info->rules[i].mac_addr,
        // qos_info->rules[i].u_rate,qos_info->rules[i].d_rate,qos_info->rules[i].dev_name);
    }
    //���ع�������
    qos_info->rule_count = count;

    /*�����ڴ���ܴ�С*/
    qos_info->mem_len = sizeof(hand_qos_rule_t) * (qos_info->rule_count);
    SET_HAND_QOS_ACK_INFO(*rules_info); /* ���ñ�־λ */

    cJSON_Delete(cj_get);
    cJSON_Delete(cj_query);

    return ret;
}
/*************************************************************************
  ����: �����豸�Ĵ���������Ϣ
  ����: hand_qos_set_info_t ���ô�����Ϣ����Ҫ�����ݽṹ
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_hand_qos_info_set_cb (const api_cmd_t *cmd,
                              const hand_qos_set_info_t *set_info,
                              void *privdata)
{
    int ret = 0;
    cJSON *cj_set   = NULL;
    cJSON *module   = NULL;
    cJSON *cj_query     = NULL;

    cj_query = cJSON_CreateObject();
    cj_set = cJSON_CreateObject();
    cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
    cJSON_AddItemToArray(module,cJSON_CreateString(SET_HAND_QOSINFO));

    int i = 0;
    cJSON *root = NULL;
    cJSON *obj = NULL;

    /*��Ҫ���õ����ݷ�װ��cjson��ʽ�����ýӿ�ʵ������*/
    cJSON_AddItemToObject(cj_set,"set_hand_qosInfo",root = cJSON_CreateArray());

    for(i = 0; i < set_info->rule_count; i++)
    {
        cJSON_AddItemToArray(root,obj = cJSON_CreateObject());

        cJSON_AddNumberToObject(obj,"hand_qosInfo_opt",set_info->set_rules[i].opt);

        cJSON_AddStringToObject(obj,"dev_name",set_info->set_rules[i].rule.dev_name);
        cJSON_AddStringToObject(obj,"mac",set_info->set_rules[i].rule.mac_addr);

        cJSON_AddNumberToObject(obj,"up_rate",set_info->set_rules[i].rule.u_rate);
        cJSON_AddNumberToObject(obj,"down_rate",set_info->set_rules[i].rule.d_rate);
    }

    ret = app_set_module_param(cj_query,cj_set);

    //���ñ�־λ
    for(i = 0; i < set_info->rule_count; i++)
    {
        HAS_HAND_QOS_DOWN_RATE(&set_info->set_rules[i].rule);
        HAS_HAND_QOS_UP_RATE(&set_info->set_rules[i].rule);
        HAS_HAND_QOS_DEV_NAME(&set_info->set_rules[i].rule);
    }
    cJSON_Delete(cj_set);
    cJSON_Delete(cj_query);

    return ret;
}


/*************************************************************************
  ����: ��ȡ���ô������ٵ��������
  ����: info ���������õ������е����ֵ
  ����ֵ: 0-�ɹ���1-ʧ��
  �Ƿ���Ҫ�û�ʵ��: ��
 ************************************************************************/
int biz_m_hand_qos_max_uplimit_cb(
    const api_cmd_t *cmd,
    hand_qos_max_uplimit_t *info,
    void *privdata)
{
    int ret = 0;
    cJSON *cj_get   = NULL;
    cJSON *module   = NULL;
    cJSON *cj_query     = NULL;

    if (!info)
    {
        return 1;
    }

    cj_query = cJSON_CreateObject();
    cj_get = cJSON_CreateObject();
    cJSON_AddItemToObject(cj_query, "module", module = cJSON_CreateArray());
    cJSON_AddItemToArray(module,cJSON_CreateString(GET_HAND_QOS_MAX_UPLIMIT));

    ret = app_get_module_param(cj_query,cj_get);

    info->up_val = cjson_get_number(cj_get, "up_val",301);     /* �������ٵ����ֵ38528KB/s, Mbps = 128KB/s */
    info->down_val = cjson_get_number(cj_get, "down_val",301); /* �������ٵ����ֵ38528KB/s, Mbps = 128KB/s */

    cJSON_Delete(cj_get);
    cJSON_Delete(cj_query);

    return ret;
}

