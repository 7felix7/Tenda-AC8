#ifndef __BCMDDNS_H__
#define __BCMDDNS_H__

#ifndef __PI_COMMON_H__
#include "pi_common.h"
#endif

#ifndef __RC_MODULE_H__
#include "rc_module.h"
#endif

/* ddns ���ò��� */
typedef struct ddns_info_struct
{
    PIU8            enable;                    	 	/*����*/
    PI32          	 isp;                        	  	/*������*/
    PI8_P           ddns_set;				/*���ò���*/
    PI8_P		 status;		/*����״̬*/
	
} DDNS_INFO_STRUCT,*P_DDNS_INFO_STRUCT;

/*API*/
RET_INFO api_wan_ddns_handle(RC_MODULES_COMMON_STRUCT *var);
/*GPI*/

/*TPI*/
extern RET_INFO tpi_ddns_update_info();
extern RET_INFO tpi_ddns_struct_init();
extern RET_INFO tpi_ddns_first_init();
extern RET_INFO tpi_ddns_action(RC_MODULES_COMMON_STRUCT *var);

extern P_DDNS_INFO_STRUCT tpi_ddns_get_info();

extern int ddns_stop(void);
extern int ddns_start(void);
#endif/*__BCMDDNS_H__*/