#ifndef __WIFI_SWITCH_SCHED_CHECK_H_
#define __WIFI_SWITCH_SCHED_CHECK_H_

#ifndef __PI_COMMON_H__
#include "pi_common.h"
#endif

#ifndef __RC_MODULE_H__
#include "rc_module.h"
#endif


//LED�Ŀ�������,����,����,��ʱ�ر�
typedef enum
{
	LED_CTL_TYPE_ON = 0,		//����
	LED_CTL_TYPE_OFF,	//����
	LED_CTL_TYPE_DURATION,	//��ʱ�ر�
}LED_CTL_TYPE;

//LED��״̬
typedef enum
{
	LED_STA_OFF = 0,
	LED_STA_ON,
	LED_STA_MAX,
}LED_STATUS;

struct timer_period
{
	PI32 is_time_zone_east;
	PI32 time_zone;
	PIU32 timer;     //��ʼʱ�䣨��һ���0ʱ���㣬��λΪs��  
};

typedef struct led_timer_info_struct
{
	PIU8 enable;	
	PI8 sel_week[7];                  //ѡ������ڣ�since sunday ,if workday selected, set {0,1,1,1,1,1,0}
	PIU8 ctl_type;			//LED�Ŀ�������,0:����,1:���أ�2:��ʱ�ر�
	PIU8 time_flag;			//���ڱ����ʼʱ����ڽ���ʱ��,0:str_time < end_tim,1:str_time > end_time
	struct timer_period led_timer_strtime;           
	struct timer_period led_timer_endtime; 

}LED_TIMER_CHECK_INFO_STRUCT,*P_LED_TIMER_CHECK_INFO_STRUCT;

/*API*/

/*GPI*/

/*TPI*/
extern RET_INFO tpi_led_update_info();
extern RET_INFO tpi_led_struct_init();
extern RET_INFO tpi_led_first_init();
extern RET_INFO tpi_led_action(RC_MODULES_COMMON_STRUCT *var);

extern RET_INFO tpi_wifi_switch_sched_web_info(PI8 *enable,PI8 *times,PI8 *weeks);

extern void tpi_wifi_switch_sched_debug_level(PI32 level);
#endif/*__WIFI_SWITCH_SCHED_CHECK_H_*/
