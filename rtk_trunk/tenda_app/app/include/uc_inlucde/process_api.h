#ifndef __PROCESS_API_H__
#define __PROCESS_API_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>

#include "uc_m_sys_types.h"
#include "uc_m_login_types.h"
#include "uc_m_wifi_types.h"
#include "uc_m_safe_check_types.h"
#include "uc_m_wan_types.h"
#include "uc_m_rub_net_types.h"
#include "uc_m_ol_host_types.h"
#include "uc_m_hand_qos_types.h"
#include "uc_m_cloud_info_types.h"
#include "uc_m_energy_types.h"
#include "uc_m_dev_types.h"
#include "uc_m_parent_control_types.h"
#include  "uc_ol_upgrade_types.h"

/*ͳһ����ṹ��*/
typedef struct _failure_msg {
	int code;
	unsigned char message[0];
} failure_msg_t;

/*��Ϊע���ȡ��ע���,�����汾�����䲻�ɼ�*/
enum {
	eSTAT_NORMAL,
	eSTAT_UNREGISTER
};

#define SN_MAX				18
#define MAC_MAX				18
#define VER_MAX				32
#define POLICY_VER_MAX             64
#define COMPANY_LEN			128
#define PRODUCT_LEN			129
#define PASSWD_MAX_LEN      128
#define COMPLETE_RET               10

#define MAX_UPGRADE_DIR_LEN	256

/*�˽ṹ��Ϊucloudͬ�ڲ�ʵ�ֽ��̵Ľ�������ṹ��,
�����汾�����䲻�ɼ�*/
typedef struct api_cmd_s {
	unsigned char		src;			/*�������Դ*/
	unsigned char		module;			/*ģ��*/
	unsigned char		cmd;			/*����id*/
	unsigned char		seq;			/*��������кţ��������Ϊ0*/
	unsigned char		status;			/*�ɹ�orʧ��or����*/
	unsigned char		ack;			/*��Ǹ�������ʵ�ֽ��̷��ظ��ն˵�*/
	unsigned char		pad[5];			/*����*/
	unsigned int		session_id;		/*����session��id*/
	unsigned int		rcv_time;		/*�յ������ʱ��*/
	unsigned int		len;			/*data �ĳ���*/
	char				data[0];		/*����Ľṹ��*/
} api_cmd_t;

/*�յ�����ڴ�����ʱ��ظ��ڴ��Ƿ��㹻,
���ڴ��㹻���enough_memory��1,������0*/
struct mem_state_t {
	int  enough_memory;				
};

/*���þ����·��,pathָ��̬�����·�����ַ���, len��ʾ�ַ�������*/
struct download_path_t {
	char *path;
	int   len;
};

/*���в��ٳɹ���֪ͨ�ڲ�ʵ�ֽ���,��λΪkb/s,
���ֵΪ0˵������ʧ��*/
struct speed{
	unsigned int value;
};

//ע������ʱͨ�õĻص�����
typedef int (command_cb_fn_t)(api_cmd_t *cmd, unsigned int data_len, void *data, void *privdata);
typedef int (mem_check_cb_t)(struct mem_state_t *memory_state, int img_size);


 //ע�ᷢ����ɣ�������������Ļص�����
 typedef void (complete_cb_t) (api_cmd_t *cmd, int len, int ret);

 //delay this function work after config data 
typedef void (timer_delay_cb) (void);

//����·������������
typedef struct cloud_basic_info {	
		int  enable;						//�ƹ����Ƿ���
		char sn[SN_MAX];					//����sn,��Ϊ��
		char mac[MAC_MAX];					//����mac,����
		char ver[VER_MAX]; 					//��������汾,����
		char policy_ver[POLICY_VER_MAX];                 //���ò�������汾��֧�ֲ�������ʱ������
		char company[COMPANY_LEN];			//���ù�˾����,����
		char product[PRODUCT_LEN];			//�����豸�ͺ�,����
		char ver_passwd[PASSWD_MAX_LEN];	//���ð汾����,����
		command_cb_fn_t	*sn_set_cb;			//ע������sn����Ļص�����
		void			*sn_set_priv;		//ע������sn�����˽������
} cloud_basic_info_t;

//������������ʱ��Ҫע�������
typedef struct upgrade_info{
		char upgrade_dir[MAX_UPGRADE_DIR_LEN];		//�������������ַ,����
		mem_check_cb_t		*memory_check_cb;	//ע���ڴ�������Ļص�����,����
		void				      *memory_check_priv;	//˽������
		command_cb_fn_t		*begin_upgrade_cb;	//ע�Ὺʼ��������Ļص�����,����
		void				  *begin_upgrade_priv;	        //˽������
		command_cb_fn_t      *begin_policy_upgrade_cb;       //ע�������������Ļص�������֧�����߲�������ʱ������
		void				   *begin_policy_upgrade_priv;
}upgrade_info_t;

//�������ٹ���ʱ��Ҫע�������
typedef struct speed_test_info{
		command_cb_fn_t		*up_speed_set_cb;		//ע�����������ٶ�����Ļص�����,����
		void				*up_speed_set_priv;		//˽������
}speed_test_info_t;


/******************************************************************
 * Function: 		uc_api_lib_init
 * Description:	��ʼ������,�������߳�����ucloud,
 *				��ʼ��������ݽṹ,�˺����ڽ���
 *				��ʼ������һ�μ���
 *		
 * Input:
 * Output:
 * Return: 		0:��ʼ���ɹ�
 *  				-1:��ʼ��ʧ�� 
 ******************************************************************/
int uc_api_lib_init(void);
/******************************************************************
 * Function: 		uc_api_lib_destroy
 * Description:	�������,�ݻ��½��߳�,�˺����ڽ���
 *				��������Ҫucloud����ʱ����
 *		
 * Input:
 * Output:
 * Return: 		0:�����ݳɹ�
 * 		   		-1:������ʧ��
 ******************************************************************/
int uc_api_lib_connect(void);
int uc_api_lib_commit(int fd);
int uc_api_lib_destroy(void);
/******************************************************************
 * Function: 		uc_api_set_basic_info
 * Description:	���û������ݺ���
 *		
 * Input:			 �ƹ�����,mac,����汾��
 * Output:		
 * Return: 		0:���û������ݳɹ�
 * 		   		-1:���û�������ʧ��
 ******************************************************************/
int uc_api_set_basic_info(cloud_basic_info_t *data);
/******************************************************************
 * Function: 		uc_api_enable_upgrade
 * Description:	����������������
 *		
 * Input:			�������λ��,
 				�ڴ��������ִ�к���ָ��,
 				����������Ͽ�ʼ������ִ�к���ָ��
 * Output:		
 * Return: 		0:���������������ܳɹ�
 * 		   		-1:����������������ʧ��
 ******************************************************************/
int uc_api_enable_upgrade(upgrade_info_t *data);
/******************************************************************
 * Function: 		uc_api_enable_speed_test
 * Description:	����������ٹ���
 *		
 * Input:			�����ٶȲ��Գɹ�������ú���
 * Output:		
 * Return: 		0:����������ٹ��ܳɹ�
 * 		   		-1:����������ٹ���ʧ��
 ******************************************************************/
int uc_api_enable_speed_test(speed_test_info_t *data);


#if 1/*M_SYSģ�鶨������ݽṹ������*/
typedef int (basic_info_fn_t) (const api_cmd_t *cmd, 
			sys_basic_info_t *basic, void *privdata);
typedef int (advance_info_fn_t) (const api_cmd_t *cmd, 
					sys_advance_info_t *advance, void *privdata);
typedef int (sys_reboot_fn_t) (const api_cmd_t *cmd, void *privdata);
typedef int (sys_reset_fn_t) (const api_cmd_t *cmd, void *privdata);
typedef int (set_time_zone_fn_t) (const api_cmd_t *cmd, 
					sys_time_zone_t *time, void *privdata);

typedef struct m_sys_regist_s{
	basic_info_fn_t		*m_sys_basic_info_get_cb;
	void				*basic_info_privdata;
	advance_info_fn_t	*m_sys_advance_info_get_cb;
	void				*advance_info_privdata;
	sys_reset_fn_t		*m_sys_reset_cb;
	void				*reset_privdata;
	sys_reboot_fn_t		*m_sys_reboot_cb;
	void				*reboot_privdata;
	set_time_zone_fn_t		*m_sys_set_time_zone_cb;
	void				*time_zone_privdata;
}m_sys_regist_t;
#endif


#if 1/*M_LOGINģ�鶨������ݽṹ������*/
typedef int (login_fn_t) (const api_cmd_t *cmd,
					m_login_t *login_info, void *privdata);
typedef int (up_pwd_fn_t) (const api_cmd_t *cmd, 
					m_login_update_pwd_t *update_info, void *privdata);
#ifdef CONFIG_APP_COSTDOWN
typedef int (get_pwd_sta_fn_t) (const api_cmd_t *cmd, 
					pwd_sta_t *sta, void *privdata);
#else
typedef int (get_pwd_sta_fn_t) (const api_cmd_t *cmd, 
					login_common_ack_t **ack_info, void *privdata);
#endif
typedef struct m_login_regist_s{
	login_fn_t			*m_login_login_info_cb;
	void				*login_info_privdata;
	up_pwd_fn_t			*m_login_update_info_cb;
	void				*update_info_privdata;
	get_pwd_sta_fn_t    *m_login_get_pwd_sta_cb;
	void                          *get_pwd_sta_privdata;
}m_login_regist_t;
#endif


#if 1/*M_WIFIģ�鶨������ݽṹ������*/
typedef int (wifi_basic_info_get_fn_t) (const api_cmd_t *cmd, 
					wifi_basic_t *basic_info, void *privdata);
typedef int (wifi_basic_info_set_fn_t) (const api_cmd_t *cmd, 
					wifi_basic_t *basic_info, void *privdata);
typedef int (wifi_guest_info_get_fn_t) (const api_cmd_t *cmd, 
					guest_info_t *guest_info, void *privdata);
typedef int (wifi_guest_info_set_fn_t) (const api_cmd_t *cmd, 
					guest_info_t *guest_info, void *privdata);
typedef int (wifi_channel_get_fn_t) (const api_cmd_t *cmd, 
					wifi_channel_info_t *chan, void *privdata);
typedef int (wifi_channel_set_fn_t) (const api_cmd_t *cmd, void *privdata);
typedef int (wifi_power_get_fn_t) (const api_cmd_t *cmd, 
					wifi_power_t *power, void *privdata);
typedef int (wifi_power_set_fn_t) (const api_cmd_t *cmd, 
					wifi_power_t *power, void *privdata);

typedef struct m_wifi_regist_s{
	wifi_basic_info_get_fn_t	*m_wifi_basic_info_get_cb;
	void				*wifi_basic_info_get_privdata;
	wifi_basic_info_set_fn_t	*m_wifi_basic_info_set_cb;
	void				*wifi_basic_info_set_privdata;
	wifi_guest_info_get_fn_t	*m_wifi_guest_info_get_cb;
	void				*wifi_guest_info_get_privdata;
	wifi_guest_info_set_fn_t	*m_wifi_guest_info_set_cb;
	void				*wifi_guest_info_set_privdata;
	wifi_channel_get_fn_t		*m_wifi_channel_get_cb;
	void				*wifi_channel_get_privdata;
	wifi_channel_set_fn_t		*m_wifi_channel_set_cb;
	void				*wifi_channel_set_privdata;
	wifi_power_get_fn_t			*m_wifi_power_get_cb;
	void				*wifi_power_get_privdata;
	wifi_power_set_fn_t			*m_wifi_power_set_cb;
	void				*wifi_power_set_privdata;
}m_wifi_regist_t;
#endif

#if 1
typedef int (guard_info_get_fn_t)(const api_cmd_t *cmd, guard_info_t *info, void *private);
typedef int (guard_info_set_fn_t)(const api_cmd_t *cmd,const guard_info_t *info, void *private);

typedef int (safe_check_fn_t) (const api_cmd_t *cmd, 
                                       safe_check_info_t *info, void *privdata);
typedef int (dns_optimize_fn_t) (const api_cmd_t *cmd, void *privdata);


typedef struct m_safe_check_regist_s{
       safe_check_fn_t         *m_safe_check_get;
       void            *m_safe_check_get_privdata;
       dns_optimize_fn_t               *m_dns_opt_set;
       void            *m_dns_opt_set_privdata;
	   guard_info_get_fn_t	*m_guard_info_get;
	   void *m_guard_info_get_private;
	   guard_info_set_fn_t	*m_guard_info_set;
	   void *m_guard_info_set_private;
}m_safe_check_regist_t;
#endif

#if 1/*M_WANģ�鶨������ݽṹ������*/
typedef int (get_rate_cb_fn_t) (const api_cmd_t *cmd, 
					wan_rate_info_t *rate_info, void *privdata);
typedef int (basic_set_cb_fn_t) (const api_cmd_t *cmd, 
					wan_basic_info_t *basic_info, void *privdata);
typedef int (basic_get_cb_fn_t) (const api_cmd_t *cmd, 
					wan_basic_info_t *basic_info, void *privdata);
typedef int (detect_type_cb_fn_t) (const api_cmd_t *cmd, 
					wan_detecttype_info_t *type_info, void *privdata);

typedef struct m_wan_regist_s{
	get_rate_cb_fn_t	*m_wan_rate_get_cb;
	void				*m_wan_rate_get_privdata;
	basic_set_cb_fn_t	*m_wan_basic_set_cb;
	void				*m_wan_basic_set_privdata;
	basic_get_cb_fn_t	*m_wan_basic_get_cb;
	void				*m_wan_basic_get_privdata;
	detect_type_cb_fn_t	*m_wan_detect_type_cb;
	void				*m_wan_detect_type_privdata;
}m_wan_regist_t;


#endif

#if 1
typedef int (access_user_fn_t) (const api_cmd_t *cmd, \
					access_user_t *user_info, void *privdata);
typedef int (rub_net_get_fn_t) (const api_cmd_t *cmd, \
					rub_net_t *rub_net_info, void *privdata);
typedef int (rub_net_set_fn_t) (const api_cmd_t *cmd, \
					rub_net_t *rub_net_info, void *privdata);
typedef int (history_list_get_fn_t) (const api_cmd_t *cmd,\
					rub_network_history_t *list_info, void *privdata);
typedef int (history_list_clear_fn_t) (const api_cmd_t *cmd, void *privdata);
#ifdef CONFIG_APP_COSTDOWN
typedef int (black_list_get_fn_t) (const api_cmd_t *cmd, \
					black_list_t **black_list, int len, void *privdata);
#else
typedef int (black_list_get_fn_t) (const api_cmd_t *cmd, \
					black_list_t **black_list,void *privdata);
#endif
typedef int (black_list_flush_fn_t)(void*);
typedef int (mf_mode_get_fn_t)(mac_filter_mode_t *, void *);
typedef int (mf_mode_set_fn_t)(mac_filter_mode_t *, void *);
typedef int (white_list_add_fn_t)(access_list_set_t *, void *);
typedef int (white_list_del_fn_t)(access_list_set_t *, void *);
typedef int (white_list_get_fn_t)(white_list_t *, void *);
typedef int (white_list_flush_fn_t)(void*);
typedef int (black_list_add_fn_t)(access_list_set_t *, void *);
typedef int (black_list_del_fn_t)(access_list_set_t *, void *);

typedef struct m_rub_net_regist_s {
	access_user_fn_t  	*m_access_user_set;
	void		*m_access_user_set_privdata;
	rub_net_get_fn_t  		*m_rub_net_get;
	void		*m_rub_net_get_privdata;
	rub_net_set_fn_t  	*m_rub_net_set;
	void		*m_rub_net_set_privdata;
	history_list_get_fn_t  		*m_history_list_get;
	void		*m_history_list_get_privdata;
	history_list_clear_fn_t  	*m_history_list_clear;
	void		*m_history_list_clear_privdata;
	black_list_get_fn_t *m_black_list_get;
	void		*m_black_list_get_privdata;
	black_list_flush_fn_t *m_black_list_flush;
	void 		*m_black_list_flush_privdata;
	mf_mode_get_fn_t *m_macfilter_mode_get;
	void 		*m_macfilter_mode_get_privdata;
	mf_mode_set_fn_t *m_macfilter_mode_set;
	void		*m_macfilter_mode_set_privdata;
	white_list_add_fn_t	*m_white_list_add;
	void		*m_white_list_add_privdata;
	white_list_del_fn_t	*m_white_list_del;
	void 		*m_white_list_del_privdata;
	white_list_get_fn_t	*m_white_list_get;
	void		*m_white_list_get_privdata;
	white_list_flush_fn_t	*m_white_list_flush;
	void		*m_white_list_flush_privdata;
	black_list_add_fn_t	*m_black_list_add;
	void		*m_black_list_add_privdata;
	black_list_del_fn_t	*m_black_list_del;
	void		*m_black_list_del_privdata;
}m_rub_net_regist_t;
#endif

#if 1/*M_OL_HOST?��?��?����?��?��y?Y?��11o��?����?*/
typedef int (ol_hosts_get_cb_fn_t)(const api_cmd_t *cmd,
				 ol_host_common_ack_t **ol_hosts_get_info, void *privdata);


typedef struct m_ol_host_regist_s{
	ol_hosts_get_cb_fn_t      *m_ol_host_get_cb;
	void				      *m_ol_host_get_privdata;

}m_ol_host_regist_t;

#endif

#if 1/*M_HAND_QOS?��?��?����?��?��y?Y?��11o��?����?*/
typedef int (hand_qos_get_fn_t) (const api_cmd_t *cmd,hand_qos_get_param_t *param,  \
					 hand_qos_common_ack_t **info, void *privdata);

typedef int (hand_qos_set_fn_t) (const api_cmd_t *cmd, \
					const hand_qos_set_info_t *set_info, void *privdata);

typedef int (hand_qos_get_genable_fn_t) (const api_cmd_t *cmd, \
					hand_qos_common_ack_t *info, void *privdata);

typedef int (hand_qos_set_genable_fn_t) (const api_cmd_t *cmd, \
					hand_qos_global_en_t *info, void *privdata);
					
typedef int (hand_qos_max_uplimit_fn_t) (const api_cmd_t *cmd, \
					hand_qos_max_uplimit_t *info, void *privdata);

typedef struct m_hand_qos_regist_s{
	hand_qos_get_fn_t              *m_hand_qos_get_cb;
	void				          *m_hand_qos_get_privdata;
	hand_qos_set_fn_t             *m_hand_qos_set_cb;
	void				          *m_hand_qos_set_privdata;
	hand_qos_get_genable_fn_t   *m_hand_qos_get_genable_cb;
	void                                   *m_hand_qos_get_genable_privdata; 
	hand_qos_set_genable_fn_t   *m_hand_qos_set_genable_cb;
	void                                   *m_hand_qos_set_genable_privdata; 
	hand_qos_max_uplimit_fn_t   *m_hand_qos_max_uplimit_cb;
	void						*m_hand_qos_max_uplimit_privdata; 

}m_hand_qos_regist_t;

#endif
#if 1
typedef int (cloud_info_manage_en_get_fn_t) (const api_cmd_t *cmd,  \
					m_cloud_info_manage_en_t *manage_en, void *privdata);
typedef int (cloud_info_manage_en_set_fn_t) (const api_cmd_t *cmd,  \
					m_cloud_info_manage_en_t *manage_en, void *privdata);
typedef int (cloud_info_clear_account_ack_fn_t) (const api_cmd_t *cmd,  \
					void *privdata);
typedef struct m_cloud_info_regist_s{
	cloud_info_manage_en_get_fn_t              *cloud_info_manage_en_get_cb;
	void				          *cloud_info_manage_en_get_privdata;
	cloud_info_manage_en_set_fn_t             *cloud_info_manage_en_set_cb;
	void				          *cloud_info_manage_en_set_privdata;
	cloud_info_clear_account_ack_fn_t             *cloud_info_clear_account_ack_cb;
	void				          *cloud_info_clear_account_ack_privdata;

}m_cloud_info_regist_t;
#endif

#if 1 /*M_ENERGY*/
typedef int (led_get_fn_t) (const api_cmd_t *cmd, \
					m_energy_led_t *led_info, void *privdata);
typedef int (led_set_fn_t) (const api_cmd_t *cmd,  \
					m_energy_led_t *led_info, void *privdata);
typedef int (wl_timer_get_fn_t) (const api_cmd_t *cmd,\
					m_energy_wireless_timer_t *timer_info, void *privdata);
typedef int (wl_timer_set_fn_t) (const api_cmd_t *cmd,  \
					m_energy_wireless_timer_t *timer_info, void *privdata);
typedef int (energy_mode_get_fn_t) (const api_cmd_t *cmd, \
					m_energy_mode_t *led_info, void *privdata);
typedef int (energy_mode_set_fn_t) (const api_cmd_t *cmd,  \
					m_energy_mode_t *led_info, void *privdata);
typedef struct m_energy_regist_s{
	led_get_fn_t                 *m_led_get_cb;
	void				    *m_led_get_privdata;
	led_set_fn_t                 *m_led_set_cb;
	void				    *m_led_set_privdata;
	wl_timer_get_fn_t   	    *m_wl_timer_get_cb;
	void                              *m_wl_timer_get_privdata; 
	wl_timer_set_fn_t   	    *m_wl_timer_set_cb;
	void                              *m_wl_timer_set_privdata; 
	energy_mode_get_fn_t   	    *m_energy_mode_get_cb;
	void                              *m_energy_mode_get_privdata; 
	energy_mode_set_fn_t   	    *m_energy_mode_set_cb;
	void                              *m_energy_mode_set_privdata; 

}m_energy_regist_t;
#endif

#if 1/*M_DEV*/
typedef int (dev_nickname_get_fn_t) (const api_cmd_t *cmd,dev_name_t *param,  \
					 nickname_info_t *info, void *privdata);

typedef int (dev_nickname_set_fn_t) (const api_cmd_t *cmd, \
					nickname_info_t *set_info, void *privdata);

typedef struct m_dev_nickname_regist_s{
	dev_nickname_get_fn_t              *m_dev_nickname_get_cb;
	void				          *m_dev_nickname_get_privdata;
	dev_nickname_set_fn_t             *m_dev_nickname_set_cb;
	void				          *m_dev_nickname_set_privdata;
}m_dev_nickname_regist_t;

#endif
#if 1/*M_PARENT_CONTROL*/
typedef int (parent_control_get_fn_t) (const api_cmd_t *cmd, \
		pc_get_param_t* param, parent_control_common_ack_t **info, void *privdata);
typedef int (parent_control_set_fn_t) (const api_cmd_t *cmd, \
					const pc_set_param_t *param, void *privdata);
typedef int (parent_control_get_type_fn_t) (const api_cmd_t *cmd, \
					parent_control_common_ack_t *param, void *privdata);

typedef struct m_parent_control_regist_s{
	parent_control_get_fn_t             *m_parent_control_get_cb;
	void				          *m_parent_control_get_privdata;
	parent_control_set_fn_t             *m_parent_control_set_cb;
	void				          *m_parent_control_set_privdata;
	parent_control_get_type_fn_t  		*m_parent_control_get_type_cb;
	void				          *m_parent_control_get_type_privdata;
}m_parent_control_regist_t;

#endif
int uc_api_enable_m_sys(m_sys_regist_t *data);
int uc_api_enable_m_login(m_login_regist_t *data);
int uc_api_enable_m_wifi(m_wifi_regist_t *data);
int uc_api_enable_m_safe_check(m_safe_check_regist_t *data);
int uc_api_enable_m_wan(m_wan_regist_t * data);
int uc_api_enable_m_rub_net(m_rub_net_regist_t *data);
int uc_api_enable_m_ol_host(m_ol_host_regist_t * data);
int uc_api_enable_m_hand_qos(m_hand_qos_regist_t * data);
int uc_api_enable_m_energy(m_energy_regist_t *data);
int uc_api_enable_m_dev_nickname(m_dev_nickname_regist_t *data);

/******************************************************************
 * Function: 		uc_api_m_rub_net_push_strange_host_info
 * Description:	����İ���������ߵ���Ϣ
 *		
 * Input:			rub_strange_host_info_t
 * Output:		
 * Return: 		0:�������ݳɹ�
 * 		   		-1:��������ʧ��
 ******************************************************************/
int uc_api_m_rub_net_push_strange_host_info(rub_strange_host_info_t *info);
int uc_api_enable_m_cloud_info(m_cloud_info_regist_t * data); 
/******************************************************************
 * Function: 		uc_api_m_cloud_info_send_clear_account
 * Description:	�������·���������˺���Ϣ���ýӿڷ�����Ϣ����
 *			����ִ�гɹ���ucloud��ص�ע��ʵ�ֺ���cloud_info_clear_account_ack_fn_t
 *		
 * Input:			��
 * Output:		
 * Return: 		0:�������ݳɹ�
 * 		   		-1:��������ʧ��
 ******************************************************************/
int uc_api_m_cloud_info_send_clear_account(); 

/******************************************************************
 * Function: 		uc_api_m_wifi_push_wifi_info
 * Description:	�ϱ�wifi������Ϣ�޸ĵ������������
 *		
 * Input:			wifi_basic_t
 * Output:		
 * Return: 		0:�������ݳɹ�
 * 		   		-1:��������ʧ��
 ******************************************************************/
int uc_api_m_wifi_push_wifi_info(wifi_basic_t *info);
/* Macro api */
int uc_api_enable_m_parent_control(m_parent_control_regist_t * data); 

#ifdef ECOS
/******************************************************************
 * Function: 		uc_api_m_cloud_info_restart_connect_svr
 * Description:	���������Ʒ�����
 *		
 * Input:			��
 * Output:		
 * Return: 		0:�������ݳɹ�
 * 		   		-1:��������ʧ��
 ******************************************************************/

int uc_api_m_cloud_info_restart_connect_svr(void);
#endif
#endif
