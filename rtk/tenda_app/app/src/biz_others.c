
#include "process_api.h"
#include "biz_typedef.h"
#include "biz_list.h"
#include "biz_others.h"
#ifndef CONFIG_APP_COSTDOWN
extern int biz_m_api_lib_thread_fd_get(void);
#endif
extern int biz_m_wifi_push_wifi_info_get(wifi_basic_t *info);
extern int biz_m_strange_host_info_get(rub_strange_host_info_t **rub_info,int *n_host);

static char * old_ssid = 0;

/* *
 *  ����ssid��¼
 */
static void update_ssid(const char * new_ssid)
{
	if (old_ssid) {
		free(old_ssid);
	}

	int len = strlen(new_ssid) + 1;
	old_ssid = (char *)malloc(sizeof(char) * len);
	memset(old_ssid, 0, len);
	strncpy(old_ssid, new_ssid, len - 1);
}

/*************************************************************************
  ����: ���͵�wifi��Ϣ
  ����: ��
  ����ֵ: ��
 ************************************************************************/
#ifdef CONFIG_APP_COSTDOWN
int biz_m_wifi_push_wifi_info(wifi_basic_t *basic_info) 
{
	if (!basic_info)
		return 0;
	memset(basic_info, 0, sizeof(*basic_info));
	biz_m_wifi_push_wifi_info_get(basic_info);

	if (!old_ssid || strncmp(old_ssid,
		basic_info->wifi_detail[WIFI_2G].ssid, MAX_SSID_LENGTH)) {
		update_ssid(basic_info->wifi_detail[WIFI_2G].ssid);
		return 1;
	}
	return 0;
}
#else
void biz_m_wifi_push_wifi_info(void) 
{
	wifi_basic_t basic_info;
	memset(&basic_info, 0, sizeof(basic_info));
	biz_m_wifi_push_wifi_info_get(&basic_info);

	if (!old_ssid || strncmp(old_ssid,
		basic_info.wifi_detail[WIFI_2G].ssid, MAX_SSID_LENGTH)) {
		
		if (-1 == uc_api_m_wifi_push_wifi_info(&basic_info)) {
			BIZ_DEBUG("push wifi info [%s] failed !\n", 
					   basic_info.wifi_detail[WIFI_2G].ssid);
		}
		uc_api_lib_commit(biz_m_api_lib_thread_fd_get());
		update_ssid(basic_info.wifi_detail[WIFI_2G].ssid);
	}
}
#endif

typedef struct node_strange_info {
	char mac[MAC_STRING_LEN];
	int  online;
	list_node_t list;
} node_strange_info_t;

/* list ͷ��� */
static list_node_t list_node_head = INIT_LIST_NODE(list_node_head);

/* *
 *  �����豸�Ƿ�����ʷ�б���
 */
static node_strange_info_t * get_history_node_from_mac(const char * mac)
{
	node_strange_info_t * pos = NULL;
	list_for_each_entry_next(pos, &list_node_head, list) {
		if (0 == strcmp(pos->mac, mac)) {
			return pos;
		}
	}
	return NULL;
}

/* *
 *  ��İ���豸���뵽������
 */
static void add_strange_info_node_to_list(const char * mac)
{
	node_strange_info_t * new_node =
		(node_strange_info_t * )malloc(sizeof(node_strange_info_t));
	memset(new_node, 0, sizeof(node_strange_info_t));

	new_node->online = 1;
	strncpy(new_node->mac, mac, sizeof(new_node->mac) - 1);
	add_node_to_list_tail(&new_node->list, &list_node_head);
	//BIZ_DEBUG("add strange info [%s]\n", mac);
}

/* *
 *  ɾ���Ѿ����ߵ�İ���豸
 */
static void delete_off_line_strange_info_node(void)
{
	node_strange_info_t * pos = NULL;
	node_strange_info_t * n = NULL;
	
	list_for_each_entry_safe(pos, n, &list_node_head, list) {
		if (0 == pos->online) {
			//BIZ_DEBUG("delete strange info [%s]\n", pos->mac);
			detach_node_from_list(&pos->list);
			free(pos);
		} else {
			/* �������豸��Ϊ�豸���� */
			pos->online = 0;
		}
	}
}

#ifndef CONFIG_APP_COSTDOWN
/* *
 *  ����İ���豸
 */
static void push_strange_host_info(rub_strange_host_info_t *info)
{	
	if (-1 == uc_api_m_rub_net_push_strange_host_info(info)) {
		BIZ_DEBUG("push strange host [%s] unsuccessfully !\n", info->dev_name);
	}
	//BIZ_DEBUG("push strange host [%s] !\n", info->dev_name);
	uc_api_lib_commit(biz_m_api_lib_thread_fd_get());
}
#endif

/*************************************************************************
  ����: �������ѣ�����İ���豸��
  ����: ��
  ����ֵ: ��
  ����: ����ÿ���õ��Ķ������ߵ������豸��������Ҫ�Լ��ж��Ƿ����ͣ�
  	 ����ͬһ���豸��һֱ���͡�
************************************************************************/
#ifdef CONFIG_APP_COSTDOWN
void biz_m_rub_net_push_strange_host_info(push_strange_info_func *push_func)
#else
void biz_m_rub_net_push_strange_host_info(void)
#endif
{
	rub_strange_host_info_t *info = NULL;
	int n = 0;
	biz_m_strange_host_info_get(&info, &n);

	int i;
	node_strange_info_t * node;

	if (info) {
		for (i = 0; i < n; i++) {
			if (!(node = get_history_node_from_mac(info[i].mac))) {
				add_strange_info_node_to_list(info[i].mac);
				#ifndef CONFIG_APP_COSTDOWN
				push_strange_host_info(&info[i]);
				#else
				if (push_func)
					push_func(&info[i], sizeof(rub_strange_host_info_t));
				#endif
			} else {
				node->online = 1;
			}
		}

		delete_off_line_strange_info_node();
		free(info);
	}
}

