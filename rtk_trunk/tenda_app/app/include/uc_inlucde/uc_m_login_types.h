#ifndef __UC_M_LOGIN_TYPES_H__
#define __UC_M_LOGIN_TYPES_H__

#define	NAME_LEN				64
#define	PWD_LEN					64

typedef enum {
	LOGIN_NAME = 0,
	LOGIN_PASSWORD
}login_param;

typedef enum {
	UPDATE_NAME = 0,
	UPDATE_OLD_PWD,
	UPDATE_NEW_PWD
}up_pwd_param;

typedef struct _m_login_t {
	int		mask;
	char	username[NAME_LEN];
	char	password[PWD_LEN];
} m_login_t;

typedef struct _m_login_update_pwd_t {
	int		mask;
	char	username[NAME_LEN];
	char	old_password[PWD_LEN];
	char	new_password[PWD_LEN];
} m_login_update_pwd_t;

typedef struct _pwd_sta_t{
	int is_none;                        /*�����Ƿ�Ϊ�գ�1-����Ϊ�գ�0-���벻Ϊ��*/
}pwd_sta_t;
//ͳһ�ظ�������
typedef struct _login_common_ack_t {
	int err_code;
	pwd_sta_t sta;
}login_common_ack_t;

/*��¼�����жϲ�����*/
#define SET_LOGIN_USER_NAME(login) \
	((login)->mask |= (1 << LOGIN_NAME))
	
#define SET_LOGIN_PASSWORD(login) \
	((login)->mask |= (1 << LOGIN_PASSWORD))

#define HAS_LOGIN_USER_NAME(login) \
	(((login)->mask & (1 << LOGIN_NAME)) == (1 << LOGIN_NAME))
	
#define HAS_LOGIN_PASSWORD(login) \
	(((login)->mask & (1 << LOGIN_PASSWORD)) == (1 << LOGIN_PASSWORD))


/*�޸����������жϲ�����*/
#define SET_UPDATE_USER_NAME(update) \
	((update)->mask |= (1 << UPDATE_NAME))
	
#define SET_UPDATE_OLD_PASSWORD(update) \
	((update)->mask |= (1 << UPDATE_OLD_PWD))
	
#define SET_UPDATE_NEW_PASSWORD(update) \
	((update)->mask |= (1 << UPDATE_NEW_PWD))

#define HAS_UPDATE_USER_NAME(update) \
	(((update)->mask & (1 << UPDATE_NAME)) == (1 << UPDATE_NAME))
	
#define HAS_UPDATE_OLD_PASSWORD(update) \
	(((update)->mask & (1 << UPDATE_OLD_PWD)) == (1 << UPDATE_OLD_PWD))
	
#define HAS_UPDATE_NEW_PASSWORD(update) \
	(((update)->mask & (1 << UPDATE_NEW_PWD)) == (1 << UPDATE_NEW_PWD))


#endif

