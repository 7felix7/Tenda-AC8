#ifndef _GPI_WAN_ERR_INFO_
#define _GPI_WAN_ERR_INFO_
#include "pi_common.h"
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
#ifndef WANERRINFO
#define WANERRINFO unsigned int
#define INFO_ERROR 0
#endif

/*
�ɵ͵��ߵĿ�ʼ����
��1λ�͵�2λ��ʾ���������
��3Ϊ��ʾWAN�ڻ��������(0��ʾ�����,1��ʾstatic IP,2��ʾDHCP,3��ʾPPPOE)
��4λ��ʾWAN������(1��ʾstatic IP,2��ʾDHCP,3��ʾPPPOE)
��5λ��ʾ����ģʽ(0��ʾAP,1��ʾWISP,2��ʾAPClient)
��6λ����ҳ����ʾ����ʱ��/IP��Ϣ��(0.����ʾ1.��ʾ)
��7λ����ҳ����ʾ��ɫ(1��ʾ������ɫ��2��ʾ������ɫ��3��ʾ�ɹ���ɫ)
��8λ����ҳ���ж��Ƿ��жϿ�����(1,�Ͽ�2����)(Ϊ�˼�����Ҫ�Ͽ���ť���������)
�������Ǿ������г����е�״̬������������ȥ�����������ϲ�ӿ���ʵ��
*/
typedef struct wan_err_info_struct
{
	unsigned char code;/*0-99��ʾ�������*/
	unsigned char network_check;/*0-3��ʾWAN�ڻ��������,0�޽�����߲����,1STATIC,2DHCP,3PPPOE*/
	unsigned char wan_mode;/*1-3��ʾWAN������,1STATIC,2DHCP,3PPPOE*/
	unsigned char wl_mode;/*0-2��ʾ����ģʽ,0AP,1WISP,2APCLIENT*/
	unsigned char time_action;/*0-1��ʾ��ʾIP��Ϣ,0����ʾ,1��ʾ*/
	unsigned char color_action;/*1-3��ʾҳ����ɫ,1����,2����,3�ɹ�*/
	unsigned char button_action;/*1-2��ʾ��ť����,1�Ͽ�,2����*/
} WAN_ERR_INFO_STRUCT,*P_WAN_ERR_INFO_STRUCT;

typedef enum
{
    COMMON_NONE                     = 0,
    COMMON_NO_WIRE                  = 1,
    COMMON_NOT_CONNECT              = 2,
    COMMON_CONNECTING               = 3,
    COMMON_CONNECTED_ONLINEING      = 4,
    COMMON_NOT_ONLINE               = 5,
    COMMON_ONLINEED                 = 6,
} COMMONERRCODE;

typedef enum
{
    STATIC_WL_CHECKED_PASSWORD_FAIL = (COMMON_ONLINEED + 1),
	STATIC_NETWORK_CHECK			= (COMMON_ONLINEED + 2),
} STATICERRCODE;

typedef enum
{
    DHCP_IP_CONFLLICT               = (COMMON_ONLINEED + 1),
	DHCP_NO_RESPOND               	= (COMMON_ONLINEED + 2),
	DHCP_WL_CHECKED_PASSWORD_FAIL 	= (COMMON_ONLINEED + 3),
	DHCP_NETWORK_CHECK				= (COMMON_ONLINEED + 4),
} DHCPERRCODE;

typedef enum
{
    PPPOE_CHECKED_PASSWORD_FAIL     = (COMMON_ONLINEED + 1),
    PPPOE_NO_RESPOND                = (COMMON_ONLINEED + 2),
	PPPOE_WL_CHECKED_PASSWORD_FAIL	= (COMMON_ONLINEED + 3),
	PPPOE_NETWORK_CHECK				= (COMMON_ONLINEED + 4),
} PPPOEERRCODE;

#define ERR_CHECK       "err_check"
/*
ERR_CHECK:
0:Ĭ��ֵ
7:������֤�û���������
2:�û������������
3:���ųɹ�
5:Զ�̿ͻ�������Ӧ
11:DHCP��ͻ
*/
#define WAN_ISONLN      "wan_isonln"
/*
WAN_ISONLN:
0:δ����
1:������
*/
#define WAN_CHECK       "wan_check"
/*
WAN_CHECK:
0:�����
1:������
*/
extern int network_tpye;
#endif
