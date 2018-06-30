/*
 * Copyright (c) 2014-2015 Alibaba Group. All rights reserved.
 *
 * Alibaba Group retains all right, title and interest (including all
 * intellectual property rights) in and to this computer program, which is
 * protected by applicable intellectual property laws.  Unless you have
 * obtained a separate written license from Alibaba Group., you are not
 * authorized to utilize all or a part of this computer program for any
 * purpose (including reproduction, distribution, modification, and
 * compilation into object code), and you must immediately destroy or
 * return to Alibaba Group all copies of this computer program.  If you
 * are licensed by Alibaba Group, your rights to utilize this computer
 * program are limited by the terms of that license.  To obtain a license,
 * please contact Alibaba Group.
 *
 * This computer program contains trade secrets owned by Alibaba Group.
 * and, unless unauthorized by Alibaba Group in writing, you agree to
 * maintain the confidentiality of this computer program and related
 * information and to not disclose this computer program and related
 * information to any other person or entity.
 *
 * THIS COMPUTER PROGRAM IS PROVIDED AS IS WITHOUT ANY WARRANTIES, AND
 * Alibaba Group EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED,
 * INCLUDING THE WARRANTIES OF MERCHANTIBILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, TITLE, AND NONINFRINGEMENT.
 */

#ifndef _ALINKGW_API_H_
#define _ALINKGW_API_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************/


#define ALINKGW_TRUE                1
#define ALINKGW_FALSE               0


#define ALINKGW_DISABLE				2
#define ALINKGW_UPDATE				1
#define ALINKGW_OK                  0
#define ALINKGW_ERR                 -1
#define ALINKGW_BUFFER_INSUFFICENT  -2

#define ALINKGW_SDK_VERSION         "v2.0.1"


/*************************************************************************
�����豸�����������ƶ��壬�豸���̿����Զ����µ�����
*************************************************************************/
#define ALINKGW_ATTR_WLAN_SWITCH_STATE      "wlanSwitchState"
#define ALINKGW_ATTR_WLAN_SWITCH_SCHEDULER  "wlanSwitchScheduler"
#define ALINKGW_ATTR_ALI_SECURITY           "aliSecurity"
#define ALINKGW_ATTR_PROBE_NUMBER           "probedNum"
#define ALINKGW_ATTR_PROBE_INFO             "proberInfo"
#define ALINKGW_ATTR_ACCESS_ATTACK_NUM		"accessAttackNum"
#define ALINKGW_ATTR_ACCESS_ATTACKR_INFO	"accessAttackerInfo"
#define ALINKGW_ATTR_TPSK                   "tpsk"
#define ALINKGW_ATTR_TPSK_LIST              "tpskList"

#define ALINKGW_SUBDEV_ATTR_DLSPEED         "dlSpeed"
#define ALINKGW_SUBDEV_ATTR_ULSPEED         "ulSpeed"
#define ALINKGW_SUBDEV_ATTR_WANDLSPEED         "wanDlSpeed"
#define ALINKGW_SUBDEV_ATTR_WANULSPEED        "wanUlSpeed"
#define ALINKGW_SUBDEV_ATTR_DLBWINFO         "dlBwInfo"
#define ALINKGW_SUBDEV_ATTR_ULBWINFO         "ulBwInfo"

#define ALINKGW_ATTR_PROBED_SWITCH_STATE     			 "probedSwitchState"
#define ALINKGW_ATTR_ACCESS_ATTACK_SWITCH_STATE      "accessAttackSwitchState"

#define ALINKGW_ATTR_WLAN_SETTING_24		"wlanSetting24g"
#define ALINKGW_ATTR_WLAN_SECURITY_24		"wlanSecurity24g"
#define ALINKGW_ATTR_WLAN_CHANNEL_24		"wlanChannelCondition24g"
#define ALINKGW_ATTR_WLAN_PAMODE			"wlanPaMode"
#define ALINKGW_ATTR_QOS_SETTING			"speedupSetting"

#define ALINKGW_ATTR_SUBDEVICE_DLSPEED		"dlSpeed"
#define ALINKGW_ATTR_SUBDEVICE_ULSPEED		"ulSpeed"

/*************************************************************************
�����豸����service���ƶ��壬�豸���̿����Զ����µ�service
*************************************************************************/
#define ALINKGW_SERVICE_AUTHDEVICE          "authDevice"
#define ALINKGW_SERVICE_CHANGEPASSWORD      "changePassword"

#define ALINKGW_SERVICE_BWCHECK				"bwCheck"
#define ALINKGW_SERVICE_FWUPGRADE 			"fwUpgrade"

#define ALINKGW_SERVICE_WL_CHANNEL			"refineWlanChannel"
/*************************************************************************
* @desc: attribute���ͣ���Ϊ���֣�

һ������ֵ����ͨ�������ַ���/����ֵ/���ֱ�ʾ�����ԣ�������wlanSwitchState��
��ֵ���Ա�ʾΪ��

"wlanSwitchState":{
    "value"��"1",  //���߿��أ�bool���ͣ�"1"����򿪣�"0"����ر�
    "when":"1404443369"
}��

��һ������ֵ��Ҫ���<name, value>����ϱ�ʾ�����ԣ�������wlanSwitchScheduler��
��ֵΪ��
"wlanSwitchScheduler":
{
   "set":{
        "enabled":"1",  //��ʱ�����أ�bool���ͣ�"1"����򿪣�"0"����ر�
        "offTime":[  //�ر�wifi�Ķ�ʱʱ��
            "UTC+08:00 0 0 22 * * 1-5",
            "UTC+08:00 0 0 23 * * 6-7"
        ],
        "onTime":[  //��wifi�Ķ�ʱʱ��
            "UTC+08:00 0 0 8 * * 1-5",
            "UTC+08:00 0 0 10 * * 6-7"
        ]
    },
    "when":"1404443369"
}

���������Ƕ�����ϱ�ʾ�����ԣ���tpsklist:
"tpskList":
[
    {
        "set":{
            "tpsk":"12345678",   //hash(secret(model))
            "mac":"11:22:33:44:55:66",  //station��mac��ַ
            "duration":"0"  //��Ч��,0��ʾ������Ч
        },
        "when":"1404443369"
    },
    {
        "set":{
            "tpsk":"12345678",   //hash(secret(model))
            "mac":"12:34:56:78:90:12"
            "duration":"0"
        },
        "when":"1404443369"
    }
]

*************************************************************************/
typedef enum {
    ALINKGW_ATTRIBUTE_simple = 0,  //ֵ���ɵ����ַ���/����ֵ/���ֱ�ʾ������
    ALINKGW_ATTRIBUTE_complex,     //ֵ�����ɶ��<name, value>����ϱ�ʾ������
    ALINKGW_ATTRIBUTE_array,       //ֵ������һ���������������ʾ������
    ALINKGW_ATTRIBUTE_MAX          //�������ͷ������������ֵ����ʾ��������
}ALINKGW_ATTRIBUTE_TYPE_E;


typedef enum {
    ALINKGW_STATUS_INITAL = 0,  //��ʼ״̬��
    ALINKGW_STATUS_INITED,      //alinkgw��ʼ�����
    ALINKGW_STATUS_REGISTERED,  //ע��ɹ�
    ALINKGW_STATUS_LOGGED       //��¼�������ɹ�����������´��ڸ�״̬
}ALINKGW_CONN_STATUS_E;

typedef enum{
    ALINKGW_LL_NONE = 0,
    ALINKGW_LL_ERROR,
    ALINKGW_LL_WARN,
    ALINKGW_LL_INFO,
    ALINKGW_LL_DEBUG
}ALINKGW_LOGLEVEL_E;

/*************************************************************************
Function:       int (*ALINKGW_KVP_save_cb)(const char *key,
                                           const char *value)
Description:    �������ͣ���ʾ�����ṩ��alinkgw����״̬����֪ͨ����ԭ��
Input:          new_status:    ���º������״̬
Output:         ��
Return:         ��
Others:
*************************************************************************/
typedef void (*ALINKGW_STATUS_cb)(ALINKGW_CONN_STATUS_E new_status);



/*************************************************************************
Function:       int (*ALINKGW_KVP_save_cb)(const char *key,
                                           const char *value)
Description:    �������ͣ���ʾ�����ṩ�ı���<Key,Value>�ַ���Pair������ʧ
                �Դ洢���ĺ���ԭ��
Input:          key:    valueֵ���ڵ��ַ����ؼ��֣��������ַ�'\0'
                value:  ��ű�����ַ���ֵ���������ַ�'\0'
Output:         ��
Return:         0:      ����ɹ�
                -1:     ����������
                ����:   ��������
Others:
*************************************************************************/
typedef int (*ALINKGW_KVP_save_cb)(const char *key, \
                                   const char *value);



/*************************************************************************
Function:       int (*ALINKGW_KVP_load_cb)(const char *key,
                                           char *value,
                                           unsigned int buf_sz)
Description:    �������ͣ���ʾ�����ṩ�Ĵӷ���ʧ�Դ洢���ж�ȡ<Key, Value>
                �ַ���Pair�ĺ���ԭ��
Input:          key:    valueֵ���ڵ��ַ����ؼ��֣��������ַ�'\0'
                buff_size:  value����������
Output:         value:  ��Ŷ�ȡ���ַ���ֵ���������ַ�'\0'
Return:         0:      ��ȡ�ɹ�
                -1:     ����������
                ����:   ��ȡʧ��
Others:
*************************************************************************/
typedef int (*ALINKGW_KVP_load_cb)(const char *key, \
                                   char *value, \
                                   unsigned int buf_sz);


/*************************************************************************
Function:       int (*ALINKGW_ATTRIBUTE_get_cb)(char *json_out_buf,
                                                unsigned int buf_sz)
Description:    �������ͣ���ʾ�����ṩ�Ļ�ȡָ�����Ե�ֵ�ĺ���ԭ��
Input:          ��
Output:         buf:    ����������ֵ��bufָ��
                        ����ֵ����Ϊsimple��buf�д�ŵ�һֵ�ַ���,��:
						1��false��
                        ����ֵ����Ϊcomplex��buf�д��json��ʽ�ַ�������:
						{"enabled":"1","offTime":[] "onTime":[]}
                buf_sz: ������json����buffer��С
Return:         0:      ���óɹ�
                -2:     ���ڴ�����json����buffer�ռ䲻��
                ����:   ����ʧ��
Others:
*************************************************************************/
typedef int (*ALINKGW_ATTRIBUTE_get_cb)(char *buf, \
                                        unsigned int buf_sz);



/*************************************************************************
Function:       int (*ALINKGW_ATTRIBUTE_set_cb)(const char *sz_json_in)
Description:    �������ͣ���ʾ�����ṩ������ָ�����Եĺ���ԭ��
Input:          json_in: �����ַ����������ַ�'\0'
                        ����ֵ����Ϊsimple��json_in�д�ŵ�һֵ�ַ���,��:
						1��false��
                        ����ֵ����Ϊcomplex��json_in�д��json��ʽ�ַ�������:
						{"enabled":"1","offTime":[] "onTime":[]}
Output:         ��
Return:         0:      ���óɹ�
                ����:   ����ʧ��
Others:
*************************************************************************/
typedef int (*ALINKGW_ATTRIBUTE_set_cb)(const char *json_in);



/*************************************************************************
Function:       int (*ALINKGW_SERVICE_execute_cb)(const char *json_in,
                                                  char *json_out_buf,
                                                  unsigned int buf_sz);
Description:    �������ͣ���ʾ�����ṩ��ִ��Զ�˷�������ĺ���ԭ��
Input:          json_in:       �����ַ����������ַ�'\0'
Output:         json_out_buf:  ��ŷ���ִ�н��json����buffer��û�н����
                               ����������
                buf_sz:        ��ŷ���ִ�н��json����buffer��С
Return:         0:      ������óɹ�
                -1��    ���ڴ�����json����buf�ռ䲻��
                ����:   �������ʧ��
Others:
*************************************************************************/
typedef int (*ALINKGW_SERVICE_execute_cb)(const char *json_in, \
                                          char *json_out_buf, \
                                          unsigned int buf_sz);

/*************************************************************************
Description:    ���豸�����ϱ��ṹ�嶨�壬��ʾ���豸�Ľṹ��ʵ��ռ�ڴ�ռ䳤�ȷǹ̶���
                 �����Ը�������
Mmenber:    mac[18]:        ���豸mac��ַ����ʽΪСд��ĸ��ʾ��17Byte��ð�Ÿ������ַ���,
                                eg: "00:11:22:cc:bb:dd"
            attr_name[]:    ���������ַ������飬��NULLָ���β
Others:
*************************************************************************/
typedef struct subdevice_attr_
{
    char mac[18];
    const char *attr_name[];
}subdevice_attr_t;


/*************************************************************************
Function:       int (*ALINKGW_ATTRIBUTE_subdevice_get_cb)(const char *subdev_mac, \
                                                  char *buf, \
                                                  unsigned int buf_sz);
Description:    �������ͣ���ʾ�����ṩ�Ļ�ȡ���豸ָ�����Ե�ֵ�ĺ���ԭ��
Input:          subdev_mac:���豸mac��ַ����ʽ:xx:xx:xx:xx:xx:xx
Output:         buf:    ����������ֵ��bufָ��
                        ����ֵ����Ϊsimple��buf�д�ŵ�һֵ�ַ���,��:
						1��false��
                        ����ֵ����Ϊcomplex��buf�д��json��ʽ�ַ�������:
						{"enabled":"1","offTime":[] "onTime":[]}
                buf_sz: ������json����buffer��С
Return:         0:      ���óɹ�
                -2:     ���ڴ�����json����buffer�ռ䲻��
                ����:   ����ʧ��
Others:
*************************************************************************/
typedef int (*ALINKGW_ATTRIBUTE_subdevice_get_cb)(const char *subdev_mac, \
                                                  char *buf, \
                                                  unsigned int buf_sz);


/*************************************************************************
Function:       int (*ALINKGW_ATTRIBUTE_subdevice_set_cb)(const char *subdev_mac, \
                                                  const char *json_in);
Description:    �������ͣ���ʾ�����ṩ���������豸ָ�����Եĺ���ԭ��
Input:          subdev_mac:���豸mac��ַ���ַ�������17Byte�������ַ�'\0'
                        ��ʽ:xx:xx:xx:xx:xx:xx
                json_in: �����ַ����������ַ�'\0'
                        ����ֵ����Ϊsimple��json_in�д�ŵ�һֵ�ַ���,��:
						1��false��
                        ����ֵ����Ϊcomplex��json_in�д��json��ʽ�ַ�������:
						{"enabled":"1","offTime":[] "onTime":[]}
Output:         ��
Return:         0:      ���óɹ�
                ����:   ����ʧ��
Others:
*************************************************************************/
typedef int (*ALINKGW_ATTRIBUTE_subdevice_set_cb)(const char *subdev_mac, \
                                                  const char *json_in);


/*************************************************************************
Function:       int ALINKGW_report_attr_subdevices(subdevice_attr_t *subdev_attrs[]);
Description:    ������豸�����Լ���ϱ��ӿڣ�֧�ֶ������ϱ�����Ҫע�⣺
                - �ú�����Ҫ��ALINKGW_start()�����
Input:          subdev_attrs:  �ϱ������豸�����Խṹ��ָ�����飬��NULL����
Output:         ��
Return:         0:      �ϱ��ɹ�;
                ����:   �ϱ�ʧ��
Others:
*************************************************************************/
int ALINKGW_report_attr_subdevices(subdevice_attr_t *subdev_attrs[]);



/*************************************************************************
Function:       int ALINKGW_register_attribute_subdevice(const char *attr_name,\
                                       ALINKGW_ATTRIBUTE_TYPE_E type,\
                                       ALINKGW_ATTRIBUTE_subdevice_get_cb get_cb,\
                                       ALINKGW_ATTRIBUTE_subdevice_set_cb set_cb);
Description:    ע�����豸����
Input:          name:   ��������
                type:   ����ֵ����
                get_cb: ��ȡ����ֵ�Ļص�����
                set_cb: ��������ֵ�Ļص�����
Output:         ��
Return:         0:      ע��ɹ�;
                ����:   ע��ʧ��
Others:
*************************************************************************/
int ALINKGW_register_attribute_subdevice(const char *attr_name,\
                                           ALINKGW_ATTRIBUTE_TYPE_E type,\
                                           ALINKGW_ATTRIBUTE_subdevice_get_cb get_cb,\
                                           ALINKGW_ATTRIBUTE_subdevice_set_cb set_cb);



/*************************************************************************
Function:       ALINKGW_register_attribute(const char *name,
                                           ALINKGW_ATTRRIBUTE_TYPE_E type,
                                           ALINKGW_ATTRIBUTE_get_cb get_cb,
ALINKGW_ATTRIBUTE_set_cb set_cb);
Description:    ע�᳧��ָ�����豸����
Input:          name:   ��������
                type:   ����ֵ����
                get_cb: ��ȡ����ֵ�Ļص�����
                set_cb: ��������ֵ�Ļص�����
Output:         ��
Return:         0:      ע��ɹ�;
                ����:   ע��ʧ��
Others:
*************************************************************************/
int ALINKGW_register_attribute(const char *name,
                               ALINKGW_ATTRIBUTE_TYPE_E type,
                               ALINKGW_ATTRIBUTE_get_cb get_cb,
                               ALINKGW_ATTRIBUTE_set_cb set_cb);



/*************************************************************************
Function:       ALINKGW_register_service(const char *name,
                                         ALINKGW_SERVICE_EXEC_cb exec_cb)
Description:    ע�᳧��ָ���Ŀɱ�Զ������ƽ̨���õ�RPC����
Input:          name:      ��������
                exec_bc:   Զ�̵��÷���ص�����
Output:         ��
Return:         0:      ע��ɹ�;
                ����:   ע��ʧ��
Others:
*************************************************************************/
int ALINKGW_register_service(const char *name,
                             ALINKGW_SERVICE_execute_cb exec_cb);


/*************************************************************************
Function:       ALINKGW_unregister_service(const char *name)
Description:    ע������ָ���Ŀɱ�Զ������ƽ̨���õ�RPC����
Input:          name:      ��������
Output:         ��
Return:         0:      ע���ɹ�;
                ����:   ע��ʧ��
Others:
*************************************************************************/
int ALINKGW_unregister_service(const char *name);


/*************************************************************************
Function:       ALINKGW_set_kvp_cb(ALINKGW_KVP_save_cb save_cb,
                                   ALINKGW_KVP_load_cb load_cb)
Description:    ���ó����ṩ��<Key,Value>�ַ���Pair��save/load�Ļص�����
Input:          save_cb:    ����<Key,Value>�ַ���Pair�Ļص���������ǿ�
                load_cb:    �ָ�<Key,Value>�ַ���Pair�Ļص���������ǿ�
Output:         ��
Return:         ��
Others:
*************************************************************************/
void ALINKGW_set_kvp_cb(ALINKGW_KVP_save_cb save_cb,
                        ALINKGW_KVP_load_cb load_cb);


/*************************************************************************
Function:       ALINKGW_set_conn_status_cb(int *new_status)
Description:    ���ó����ṩ��alinkgw����server��״̬�仯�ص�����
Input:          ��
Output:         new_status: ����״̬���»ص�����������ǿ�
Return:         ��
Others:
*************************************************************************/
void ALINKGW_set_conn_status_cb(ALINKGW_STATUS_cb status_cb);


/*************************************************************************
Function:       int ALINKGW_report_attr(const char *attr_name)
Description:    �豸���Բ���������ϱ��ӿ�,��Ҫע�⣺
            - �ú�����Ҫ��ALINKGW_start()�����
Input:          attr_name:  �ϱ�����������
Output:         ��
Return:         0:      �ϱ��ɹ�;
                ����:   �ϱ�ʧ��
Others:
*************************************************************************/
int ALINKGW_report_attr(const char *attr_name);


/*************************************************************************
Function:       int ALINKGW_report_attr(const char *attr_name)
Description:    �豸������ԣ�����ϱ��ӿ�,��Ҫע�⣺
            - �ú�����Ҫ��ALINKGW_start()�����
Input:          attr_name:  �ϱ�����������ָ�����飬��NULL����
Output:         ��
Return:         0:      �ϱ��ɹ�;
                ����:   �ϱ�ʧ��
Others:
*************************************************************************/
int ALINKGW_report_attrs(const char *attr_name[]);



/*************************************************************************
Function:       int ALINKGW_report_attr_direct(const char *name, )
Description:    �豸����ֱ���ϱ��ӿ�,��Ҫע�⣺
            - �ú�����Ҫ��ALINKGW_start()�����
Input:          attr_name:  �ϱ�����������
                type:       �ϱ����Ե�����
                data:       �ϱ����Ե�ֵ
Output:         ��
Return:         0:      �ϱ��ɹ�;
                ����:   �ϱ�ʧ��
Others:
*************************************************************************/
int ALINKGW_report_attr_direct(const char *attr_name,\
                               ALINKGW_ATTRIBUTE_TYPE_E type,\
                               const char *data);


/*************************************************************************
Function:       ALINKGW_attach_sub_device(const char *name,
                                          const char *type,
                                          const char *category,
                                          const char *manufacturer,
                                          const char *mac)
Description:    �������ص����豸�����ϱ�
Input:          name:           �豸����
                type:           �豸����
                category:       �豸����
                manufacturer:   �豸���̣�δ֪����ͳһΪ:unknown
                mac:            �豸mac��ַ������ǿմ�����ʽ:xx:xx:xx:xx:xx:xx
Output:         ��
Return:         0:     �ϱ��ɹ�;
                ����:  �ϱ�ʧ��
Others:
*************************************************************************/
int ALINKGW_attach_sub_device(const char *name,
                              const char *type,
                              const char *category,
                              const char *manufacturer,
                              const char *mac);


/*************************************************************************
Function:       ALINKGW_detach_sub_device(const char *dev_mac)
Description:    �������ص����豸�����ϱ�
Input:          dev_mac:    ���豸mac��ַ����ʽ:xx:xx:xx:xx:xx:xx
Output:         ��
Return:         0:     �ϱ��ɹ�;
                ����:  �ϱ�ʧ��
Others:
*************************************************************************/
int ALINKGW_detach_sub_device(const char *dev_mac);



/*************************************************************************
Function:       int ALINKGW_wait_connect(int timeout)
Description:    �ȴ�alinkgw���ӷ�������ֱ����¼�ɹ���ʱ����
Input:          timeout:    �ȴ����س�ʱʱ�䣬��λ�룬-1��ʾһֱ�ȴ�
Output:         ��
Return:         0:      ��¼�ɹ�
                ����:   δ��¼�ɹ�
Others:
*************************************************************************/
int ALINKGW_wait_connect(int timeout);



/*************************************************************************
Function:       int ALINKGW_set_sandbox_mode()
Description:    ����ɳ��ģʽ�������øú�����·���������ڲ������õ�����ƽ̨
                ����������·����������ʽ���ⷢ���汾������ƽ̨����������Ҫע�⣺
            - �ú�����Ҫ��ALINKGW_start()ǰ����
Input:
Output:         ��
Return:         0:      ���óɹ�
                ����:   ����ʧ��
Others:
*************************************************************************/
int ALINKGW_set_sandbox_mode();

/***********************************************************************
Function:       int ALINKGW_enable_asec(unsigned int bEnabled)
Description:    ��������رգ������ﰲȫ��������ܣ���Ҫע�⣺
            - �����ﰲȫ������Ĭ�Ͽ���
            - ���û�ͨ��UI����ر�/���������ﰲȫ��ʱ�����ñ��ӿ�
Input:          bEnabled�� 1 - ������ 0 - �ر�
Output:         ��
Return:         0:      ���óɹ�
                ����:   ����ʧ��
Others:
*************************************************************************/
int ALINKGW_enable_asec(unsigned int bEnabled);

/*************************************************************************
Function:       int ALINKGW_set_loglevel(ALINKGW_LOGLEVEL_E e)
Description:    ������־��Ϣ����Ĭ�ϼ���ΪALINKGW_LL_ERROR��
Input:
Output:         ��
Return:         ��
Others:
*************************************************************************/
void ALINKGW_set_loglevel(ALINKGW_LOGLEVEL_E e);

/*************************************************************************
Function:       int ALINKGW_reset_binding()
Description:    ���·�����豸���ֻ�APP�û��İ󶨹�ϵ����Ҫע�⣺
                - �ú����������ڻָ�·�����������ó��������������������µ���
				- �ú���ֻ�����豸�����������ܳɹ�
                - �ú�����������ALINKGW_start()֮��ALINKGW_end()֮ǰ����
Input:
Output:         ��
Return:         0:      ���ɹ�
                ����:   ���ʧ��
Others:
*************************************************************************/
int ALINKGW_reset_binding();

/*************************************************************************
Function:       ALINKGW_start(const char *sn, ....)
Description:    ����alink���񣬵��øýӿ�ǰ��Ҫ����ƽ̨��Ϣ��ע�����Ժͷ���
Input:          sn:        �豸���к�, ���64�ֽ�
                name:      �豸���ƣ����32�ֽ�
                brand:     �豸Ʒ�ƣ����32�ֽ�
                type:      �豸���ͣ����32�ֽ�
                category:  �豸���࣬���32�ֽ�
                manufacturer:   �豸������, ���32�ֽ�
                version:   �̼��汾��,���32�ֽ�
                mac:       �豸mac��ַ����ʽ11:22:33:44:55:66������ʹ��lan
				           �ӿ�mac��ַ,����mac��¡�����豸mac��ַ���
                model:     �豸�ͺţ�������ƽ̨��Ȩ�����80�ֽ�
                cid:       �豸оƬID��ͬ���Ͳ�ͬ�豸��cid���벻ͬ�����64�ֽ�
                key:       �豸�����ƶ�key��������ƽ̨�䷢
                secret:    �豸�����ƶ���Կ��������ƽ̨�䷢
Output:         ��
Return:         0:     �����ɹ�;
                ����:  ����ʧ��
Others:
*************************************************************************/
int ALINKGW_start(
    const char *sn,
    const char *name,
    const char *brand,
    const char *type,
    const char *category,
    const char *manufacturer,
    const char *version,
    const char *mac,
    const char *model,
    const char *cid,
    const char *key,
    const char *secret);

/*************************************************************************
Function:       ALINKGW_end()
Description:    ֹͣalink���񣬲��ͷ���Դ
Input:          ��
Output:         ��
Return:         0:     ֹͣ�ɹ�;
                ����:  ֹͣʧ��
Others:
*************************************************************************/
int ALINKGW_end();


/*************************************************************************
Function:       ALINKGW_cloud_save()
Description:    �ƶ˴洢����
Input:          name: ���������ַ�������'\0'��β��
                val_buf: ����ֵbuffer
                val_len: ����ֵbuffer length
Output:         ��
Return:         0:     ����ɹ�;
                ����:  ����ʧ��
Others:
*************************************************************************/
int ALINKGW_cloud_save(
    const char *name,
    unsigned char *val_buf,
    unsigned int val_len);

/*************************************************************************
Function:       ALINKGW_cloud_restore()
Description:    ���ƶ˻ָ�����
Input:          name: ���������ַ�������'\0'��β��
Output:         val_buf: ��Ŵ��ƶ˻�ȡ������ֵ��buffer
                val_len: value-result����������valueʱ�����buffer��size������resultʱ�����value length
                        ����resultʱ�����value length
Return:         0:     �ָ��ɹ�;
                ����:  �ָ�ʧ��
Others:
*************************************************************************/
int ALINKGW_cloud_restore(
    const char *name,
    unsigned char *val_buf,
    unsigned int *val_len);


/*************************************************************************
Function:       ALINKGW_get_cloud_attrs()
Description:    ���ƶ˻�ȡ���һ���ϱ�������ֵ�ӿ�
Input:          attr_name:  ��ȡ�������������飬��NULL����
Output:         val_buf: ��Ŵ��ƶ˻�ȡ������ֵ��buffer
                val_len: value-result����������valueʱ�����buffer��size��
                        ����resultʱ�����value length
Return:         0:     �ָ��ɹ�;
                ����:  �ָ�ʧ��
Others:
*************************************************************************/
int ALINKGW_get_cloud_attrs(
    const char *attr_name[],
    unsigned char *val_buf,
    unsigned int *val_len);



/************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _ALINKGW_API_H_ */

