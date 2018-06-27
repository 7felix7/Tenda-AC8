#ifndef BIZ_LIST_H
#define BIZ_LIST_H

/** ��ʼ��list_node_t�ڵ� */
#define INIT_LIST_NODE(node) {&(node), &(node)}

typedef struct list_node {
	struct list_node * next, * prev;
} list_node_t;

/** �õ�member��Ա��ƫ���� */
#define member_offset(type, member)	\
	((size_t)&(((type*)0)->member))
	
/** ��member_ptrָ������������type���͵��׵�ַ */
#define type_list_entry(member_ptr, type, member) \
	(type*)((char*)member_ptr - member_offset(type, member))

/** ѭ������head->next */
#define list_for_each_entry_next(pos, head, list) \
	for (pos = type_list_entry((head)->next, typeof(*pos), list); \
		 pos && (&pos->list != (head)); \
		 pos = type_list_entry(pos->list.next, typeof(*pos), list))

/** ѭ������head->pre */
#define list_for_each_entry_prev(pos, head, list) \
	for (pos = type_list_entry((head)->prev, typeof(*pos), list); \
		 pos && (&pos->list != (head)); \
		 pos = type_list_entry(pos->list.prev, typeof(*pos), list))

/** ѭ������head->next, ɾ���ڵ�ʱʹ�� */
#define list_for_each_entry_safe(pos, n, head, list) \
	for (pos = type_list_entry((head)->next, typeof(*pos), list), \
		 n = type_list_entry((pos->list).next, typeof(*pos), list); \
		 pos && (&pos->list != (head)); \
		 pos = n, n = type_list_entry(n->list.next, typeof(*pos), list)) 


/** ��ʼ��list_node�ڵ� */
void init_list_node(list_node_t * list);
/** ���½ڵ�new���뵽prev��next֮�� */
void add_node_to_list(list_node_t * new, list_node_t * prev, list_node_t * next);
/** ���½ڵ���뵽headǰ�ߣ���������ǰ�� */
void add_node_to_list_head(list_node_t * new, list_node_t * head);
/** ���½ڵ���뵽head��ߣ��������β */
void add_node_to_list_tail(list_node_t * new, list_node_t * head);
/** ���ڵ�������з������ */
void detach_node_from_list(list_node_t * node);
/** �����Ƿ�Ϊ�� */
int is_list_empty(list_node_t * head);

#endif
