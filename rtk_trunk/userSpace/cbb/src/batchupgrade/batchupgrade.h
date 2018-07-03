
#define UPGRADE_DEBUG(fmt, args...) 		do{		/* show debug*/	\
	printf("upgrade debug->%s: %s(%d)--"fmt, __FILE__, __func__, __LINE__, ## args);	\
}while(0);

#define MAGIC_HTTPD_HEAD	"\r\n\r\n"
#define MAGIC_IMAGE_HEAD	"HDR0"

typedef enum tpi_upgrade_ret{	
	BAD_HTTPD_FORMAT,			/* �޷�������HTTPD������ */
	BAD_IMAGE_FORMAT,			/* ����������ļ���ʽ*/
	READ_FLASH_FAIL,				/* �޷���ȡFLASH���� */
	OUT_OF_SIZE,					/* �����ļ�̫�� */
	UPGRADE_FAIL,					/* ����ʧ�� */	
	UPGRADE_OK					/* �����ɹ� */
}TPI_UPGRADE_RET;

typedef enum{
	TPI_FALSE,				/* �� */
	TPI_TRUE				/* �� */
}TPI_BOOL;

/* �ӿں�������ֵ���� */
typedef enum tpi_ret{
	TPI_RET_OK = 0,						/* �ɹ� */
	TPI_RET_APP_RUNNING = 1,			/* ģ���������� */
	TPI_RET_APP_DEAD = 2,				/* ģ���Ѿ��˳� */
	TPI_RET_NULL_POINTER = 1001,		/* ��ָ����� */
	TPI_RET_INVALID_PARAM = 1002,		/* �Ƿ����� */
	TPI_RET_ERROR = 0xff				/* ʧ�� */
}TPI_RET;

extern int get_flash_offset(int *offset);
extern int tenda_upload_fw1(char *stream, int offset_in,int flash_offset);
extern TPI_BOOL validate_image_format(char *stream, int httpd_offset);
extern void ifupgrade_deinit(void);
extern int batch_upgrade_start(void);
extern void cluster_free(char* data);
extern void set_all_led_blink(void);
extern void ate_set_all_led_on(void);
extern void restore_defaults(void);