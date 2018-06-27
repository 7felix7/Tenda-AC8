/******************************************************************************
          ��Ȩ���� (C), 2015-2018, �����м����ڴ�Ƽ����޹�˾
 ******************************************************************************
  �� �� ��   : mem_check.c
  �� �� ��   : ����
  ��    ��   : liquan
  ��������   : 2016��8��26��
  ����޸�   :
  ��������   :

  ��������   : �����ڴ�й©���

  �޸���ʷ   :
  1.��    ��   : 2016��8��26��
    ��    ��   : liquan
    �޸�����   : �����ļ�

******************************************************************************/

#include<mem_check.h>
#include<stdlib.h>

mem_node* mem_head= NULL;

/*****************************************************************************
 �� �� ��  : mem_node_add
 ��������  : ����ڴ�������Ϣ�ڵ�
 �������  : void *ptr
             size_t block
             char* func
 �������  : ��
 �� �� ֵ  : static

 �޸���ʷ      :
  1.��    ��   : 2016��8��11��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
static void mem_node_add(void *ptr, size_t block, char* func)
{
    mem_node *node = NULL;
    ptr_node   *block_ptr = NULL;
    char *funtion_name = NULL;

    block_ptr = malloc(sizeof(ptr_node));
    if(block_ptr == NULL)
    {
        printf("fun:%s     line:%d    malloc fail\n",__FUNCTION__,__LINE__);
        return ;
    }
    block_ptr->block = block;
    block_ptr->ptr = ptr;
    block_ptr->next = NULL;

    mem_node *pn = mem_head;
    while(pn)
    {
        if(0 == strcmp(pn->fun_name,func))
        {
            block_ptr->next = pn->ptr_head;
            pn->ptr_head = block_ptr;
            return ;
        }
        pn = pn->next;
    }

    node = malloc(sizeof(mem_node));
    if(node == NULL)
    {
        free(block_ptr);
        printf("fun:%s line:%d    malloc fail\n",__FUNCTION__,__LINE__);
        return ;
    }

    funtion_name = (char*)malloc(strlen(func)+1);
    if(funtion_name == NULL)
    {
        free(node);
        free(block_ptr);
        printf("fun:%s line:%d    malloc fail\n",__FUNCTION__,__LINE__);
        return ;
    }

    strcpy(funtion_name,func);
    node->fun_name = funtion_name;
    node->ptr_head = block_ptr;

    if (mem_head)
    {
        node->next = mem_head;
    }
    else
    {
        node->next = NULL;
    }
    mem_head = node;
}

/*****************************************************************************
 �� �� ��  : mem_node_remove
 ��������  : �Ƴ����߸���һ���ڴ�ڵ�
 �������  : void *ptr
             char* func
 �������  : ��
 �� �� ֵ  : static

 �޸���ʷ      :
  1.��    ��   : 2016��8��12��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
static void mem_node_remove(void *free_ptr)
{
    ptr_node   *cur_block_ptr = NULL;
    ptr_node   *next_block_ptr = NULL;
    mem_node *next_node = NULL;
    mem_node *cur_node = mem_head;

    if (cur_node)
    {
        next_node = cur_node->next;
        cur_block_ptr = cur_node->ptr_head;
        next_block_ptr = cur_block_ptr->next;

        if(cur_block_ptr->ptr == free_ptr)
        {
            cur_node->ptr_head= next_block_ptr;
            free(cur_block_ptr);
            cur_block_ptr = NULL;
            if(next_block_ptr == NULL)
            {
                free(cur_node->fun_name);
                free(cur_node);
                mem_head = next_node;
            }
        }
        else
        {
            while(next_block_ptr)
            {

                if(next_block_ptr->ptr == free_ptr)
                {
                    cur_block_ptr->next = next_block_ptr->next;
                    free(next_block_ptr);

                    goto exit;
                }
                cur_block_ptr = next_block_ptr;
                next_block_ptr = cur_block_ptr->next;
            }


            while(next_node)
            {
                cur_block_ptr = next_node->ptr_head;
                next_block_ptr = cur_block_ptr->next;
                if(cur_block_ptr->ptr == free_ptr)
                {
                    next_node->ptr_head = next_block_ptr;
                    free(cur_block_ptr);
                    if(next_block_ptr == NULL)
                    {
                        cur_node->next = next_node->next;
                        free(next_node->fun_name);
                        free(next_node);
                    }
                    goto exit;
                }
                else
                {
                    while(next_block_ptr)
                    {
                        if(next_block_ptr->ptr == free_ptr)
                        {
                            cur_block_ptr->next = next_block_ptr->next;
                            free(next_block_ptr);
                            goto exit;
                        }
                        cur_block_ptr = next_block_ptr;
                        next_block_ptr = cur_block_ptr->next;
                    }
                }
                cur_node = next_node;
                next_node = cur_node->next;
            }
        }
    }
exit:
    return;
}

/*****************************************************************************
 �� �� ��  : mem_node_fflush
 ��������  : ����ڴ���Ϣ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : static

 �޸���ʷ      :
  1.��    ��   : 2016��8��12��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
static void mem_node_fflush()
{
    ptr_node   *cur_block_ptr = NULL;
    ptr_node   *temp_block_ptr = NULL;
    mem_node *temp_node = NULL;
    mem_node *cur_node = mem_head;

    while(cur_node)
    {
        cur_block_ptr = cur_node->ptr_head;
        while(cur_block_ptr)
        {
            temp_block_ptr = cur_block_ptr;
            cur_block_ptr = cur_block_ptr->next;
            free(temp_block_ptr);
        }
        temp_node = cur_node;
        cur_node = cur_node->next;
        free(temp_node->fun_name);
        free(temp_node);
    }
    mem_head = NULL;
}

/*****************************************************************************
 �� �� ��  : memcheck_do_time
 ��������  : ��Ҫ�Ƿ�ֹ�����ȹ�������⹦������ռ���ڴ����
 �������  : ��
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��8��12��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
void memcheck_do_time()
{
    ptr_node   *cur_block_ptr = NULL;
    int  ptr_node_size = sizeof(ptr_node);
    int  mem_node_size = sizeof(mem_node);
    mem_node *cur_node = mem_head;
    size_t total = 0;

    while(cur_node)
    {
        cur_block_ptr = cur_node->ptr_head;
        while(cur_block_ptr)
        {
            cur_block_ptr = cur_block_ptr->next;
            total += ptr_node_size;
        }
        total += mem_node_size;
        total = total + strlen(cur_node->fun_name) + 1;
        cur_node = cur_node->next;

    }

    if(total > MAX_MEM_NODE)
        mem_node_fflush();


}

/*****************************************************************************
 �� �� ��  : show_block
 ��������  : ��ʾδ�ͷŵ��ڴ�����
 �������  : int argc
             char **argv
 �������  : ��
 �� �� ֵ  :

 �޸���ʷ      :
  1.��    ��   : 2016��8��12��
    ��    ��   : liquan
    �޸�����   : �����ɺ���

*****************************************************************************/
void show_block(int argc,char **argv)
{
    int i = 0;
    mem_node *cur_node = mem_head;
    ptr_node   *block_ptr = NULL;
    size_t total = 0;
    int filter = 0;

    if(argc > 1)
    {
        if(strcmp(argv[1],"show") == 0)
        {
            if(argc > 2)
                filter = atoi(argv[2]);
            printf("\n%-30s|no_free \n","function_name");
            printf("-------------------------------------\n");
            while (cur_node)
            {
                i = 0;
                mem_node *pnext = cur_node->next;
                block_ptr = cur_node->ptr_head;


                while(block_ptr)
                {
                    i++;
                    total += block_ptr->block;
                    block_ptr = block_ptr->next;
                }
                if(i < filter)
                {
                    cur_node = pnext;
                    continue;
                }
                printf("%-30s", cur_node->fun_name);
                printf("|  %d\n",i);
                printf("-------------------------------------\n");
                cur_node = pnext;

            }
            printf("all:%dbyte\n", total);
        }
        else if(strcmp(argv[1],"fflush") == 0&&argc == 2)
        {
            mem_node_fflush();
        }
        else if(strcmp(argv[1],"fun") == 0 && (argc > 2))
        {
            while (cur_node)
            {
                mem_node *pnext = cur_node->next;
                if(0 == strcmp(cur_node->fun_name,argv[2]))
                {

                    block_ptr = cur_node->ptr_head;
                    printf("%s:\n", cur_node->fun_name);

                    while(block_ptr)
                    {
                        total += block_ptr->block;
                        printf("%d   %p\n",block_ptr->block,block_ptr->ptr);
                        block_ptr = block_ptr->next;
                    }
                    printf("-------------------------------------\n");
                    printf("all:%dbyte\n", total);
                    printf("-------------------------------------\n");
                    return;
                }
                cur_node = pnext;
            }
            printf("no find this function\n");
        }
        else
        {
            printf("tenda_mem [show/fun] [filter(times)/fun_name]\n");
        }
    }
    else
    {
        printf("tenda_mem [show/fun] [filter(times)/fun_name]\n");
    }
}


void *tenda_malloc(size_t elem_size, char *func)
{
    void *ptr = malloc(elem_size);
    if(ptr != NULL)
    {
        mem_node_add(ptr, elem_size,func);
        memset(ptr,0,elem_size);
    }
    return ptr;
}

void *tenda_calloc(size_t count, size_t elem_size, char *func)
{
    void *ptr = calloc(count, elem_size);
    if(ptr != NULL)
    {
        mem_node_add(ptr, elem_size * count, func);
    }
    return ptr;
}

void tenda_free(void *ptr)
{
    free(ptr);
    mem_node_remove(ptr);
}

