#ifndef _TENDA_STA_STEERING_H_
#define _TENDA_STA_STEERING_H_
#include "tenda_wlan_common.h"

#define DEFAULT_STA_STEER_ENABLE    0

#define MAX_STA_STEER_NUM           128
#define STA_MAC_LEN                 6
#define STA_STEER_AGETIME           30      //30s
#define STA_SUPPORT_RSSI_MIN        -80
#define MAX_BSS_NUM                 10
#define STA_NUM_LIMIT_2G_5G         3
#define STA_AUTH_REJECT_LIMIT       2

#define PROBE_STA_BANDTYPE_2G   0x01
#define PROBE_STA_BANDTYPE_5G   0x02

#define DEF_RSSI_ROAMING_2G     -47
#define DEF_RSSI_ROAMING_5G     -75

#define STEER_ROAMING_TO_2G          1
#define STEER_ROAMING_TO_5G          2

typedef struct probe_sta_info {
    unsigned char               use;
    unsigned char               auth_cnt;
    unsigned char               mac[STA_MAC_LEN];
    unsigned long               probe_jiffies;
    unsigned char               bandtype;
    unsigned char               roaming_status;
    int                         rssi;
    int                         is_assoced;
    int                         debug;
    void                        *priv;
} probe_sta_info_t;

struct sta_steer {
    int                 enable;
    int                 debug;
    struct timer_list   age_timer;
    unsigned int        age_timeout;
    unsigned int        sta_num;
    steer_bss_info_t    bss[MAX_BSS_NUM];
    void                *priv_5g;

    /* Enhancement function */
    int                 sta_balance_enable; // 2.4G AND 5G STA NUM BALANCE
    int                 rssi_lmt_5g;        //  5G�û������,�л���2G��RSSI����,eg:�û�Զ����Ҫ��2G
    int                 rssi_lmt_2g;        //  2G�û������,�л���5G��RSSI����,eg:�û�����������5G
    int                 auth_reject_limit;       // �ܾ�sta���Ӵ������ޣ��������ޣ������������
    int                 auth_status;             //auth����״̬������
	int                 assoc_status;            //assoc_status����״̬������
    
};

void sta_steer_proc_show();

#endif
