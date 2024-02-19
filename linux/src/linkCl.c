


//创建单向链表
//客户端与服务器建立会话时的链表
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linkCl.h"

//CLIENT_INFO global_link_cl_info_head;
CLIENT_INFO *global_link_cl_info_head_p;


CLIENT_INFO *linkClCreate(void)
{
    CLIENT_INFO *head = ( CLIENT_INFO *)malloc(sizeof( CLIENT_INFO ));
    head->next = NULL;
    return head;
}


CLIENT_INFO *linkClAddNode(CLIENT_INFO *head)
{
    CLIENT_INFO *node = ( CLIENT_INFO *)malloc(sizeof( CLIENT_INFO ));
    CLIENT_INFO *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
    }
    prob->next = node;
    node->next = NULL;
    return node;
}


//注：index:节点索引不包含链表头；以0为起始；新节点的所以即为参数inex
//可插入的最大索引值为 节点数
CLIENT_INFO *linkClInsertNode(CLIENT_INFO *head, int index)
{
    CLIENT_INFO *node = ( CLIENT_INFO *)malloc(sizeof( CLIENT_INFO ));
    CLIENT_INFO *prob = head->next;
    CLIENT_INFO *last = head;
    int i = 0;
    while(prob != NULL)
    {
        if(i == index)
        {
            last->next = node;
            node->next = prob;
            
            return node;
        }
        i++;
        last = prob;
        prob = prob->next;
    }
    
    if(prob == NULL && i == index)
    {
         last->next = node;
         node->next = NULL;
         return node;
    }

    free(node);
    return NULL;
}



//删除最后一个节点
void linkClDelLastNode(CLIENT_INFO *head)
{
    if(head->next == NULL)
    {
        return;
    }   
    CLIENT_INFO *prob = head->next;
    CLIENT_INFO *last = head;
    while(prob->next != NULL)
    {
        last = prob;
        prob = prob->next;
    }
    free(prob);
    last->next = NULL;
}

//删除节点
void linkClDelNode(CLIENT_INFO *head, CLIENT_INFO *node)
{
    if(head->next == NULL)
    {
        return;
    }   
    
    CLIENT_INFO *prob = head->next;
    CLIENT_INFO *last = head;
    while(prob != NULL)
    {
        if(prob == node)
        {
            last->next = node->next;
            free(node);
            break;
        }
        last = prob;
        prob = prob->next;
    }
    
}


//删除指定索引的的节点
void linkClDelNode_byIndex(CLIENT_INFO *head, int index)
{
    if(head->next == NULL)
    {
        return;
    }   
    CLIENT_INFO *prob = head->next;
    CLIENT_INFO *last = head;
    int i = 0;
    while(prob != NULL)
    {
        if(i == index)
        {
            last->next = prob->next;
            free(prob);
            return;
        }
        i++;
        last = prob;
        prob = prob->next;
    }
}

//获得节点个数
int linkClGetNum(CLIENT_INFO *head)
{
    if(head == NULL)
        {
        return -1;
    }

    CLIENT_INFO *prob = head->next;
    int i = 0;
    while(prob != NULL)
    {      
        i++;
        prob = prob->next;
    }    

    return i;
}

//取得节点的索引号
int linkClGetIndex(CLIENT_INFO *head, CLIENT_INFO *node)
{
    CLIENT_INFO *prob = head->next;
    int i = 0;
    while(prob != NULL)
    {      
        if(prob == node)
        {
            return i;
        }
        i++;
        prob = prob->next;
    } 
    return -1;
}

//取得索引对应的节点
CLIENT_INFO *linkClGetNode_byIndex(CLIENT_INFO *head, int index)
{
    CLIENT_INFO *prob = head->next;
    int i = 0;
    while(prob != NULL)
    {      
        if(i == index)
        {
            return prob;
        }
        i++;
        prob = prob->next;
    }
    return NULL;
}



//遍历链表，查找对应的成员 ipv4，并返回节点指针
CLIENT_INFO *linkClFind_byIpv4(CLIENT_INFO *head, int ipv4)
{
    CLIENT_INFO *prob = head->next;
    while(prob != NULL)
    {      
        if(prob->ipv4 == ipv4)
        {
            return prob;
        }
        prob = prob->next;
    } 
    return NULL;
}


//链表销毁
void  linkClDestory(CLIENT_INFO *head)
{
    CLIENT_INFO *prob = head->next;
    CLIENT_INFO *last = head;
    while(prob != NULL)
    {   
    //printf("aa...\n");
        if(prob->next == NULL)
        {
            //printf("...bb\n");
            free(prob);           
            last->next = NULL;
            prob = head->next;
            last = head;
            continue;
        }       
        last = prob;
        prob = prob->next;    
    }   
}



//测试
void linkClTest_show(CLIENT_INFO *head)
{
    static int cnt = 1;
    
    CLIENT_INFO *prob = head->next;
    printf("\r\n[%d]===================>\n", cnt);
    while(prob != NULL)
    {      
        printf("ipv4=%d  num=%d   fd=%d\n", prob->ipv4, prob->num, prob->fd);
        prob = prob->next;
    } 
    cnt++;
}

void linkClTest(void)
{
    global_link_cl_info_head_p = linkClCreate();
    CLIENT_INFO *node1 = linkClAddNode(global_link_cl_info_head_p);
    CLIENT_INFO *node2 = linkClAddNode(global_link_cl_info_head_p);
    CLIENT_INFO *node3 = linkClAddNode(global_link_cl_info_head_p);
    CLIENT_INFO *node4 = linkClAddNode(global_link_cl_info_head_p);
    node1->ipv4 = 100;
    node2->ipv4 = 8;
    node3->ipv4 = 70;
    node4->ipv4 = 65;
    node1->num = 1;
    node2->num = 2;
    node3->num = 3;
    node4->num = 4;
    linkClTest_show(global_link_cl_info_head_p);

    printf("node2 的索引：%d\n", linkClGetIndex(global_link_cl_info_head_p, node2));
    printf("节点个数：%d\n", linkClGetNum(global_link_cl_info_head_p)); 
    CLIENT_INFO *prob = linkClFind_byIpv4(global_link_cl_info_head_p, 8);
    if(NULL != prob)printf("查找ipv4为8的第一个节点索引：%d\n", linkClGetIndex(global_link_cl_info_head_p, prob));
    prob = linkClGetNode_byIndex(global_link_cl_info_head_p, 3);
    printf("索引序号为3的节点成员ipv4=%d\n", prob->ipv4);
    
    linkClDelLastNode(global_link_cl_info_head_p);
    linkClTest_show(global_link_cl_info_head_p);

    CLIENT_INFO *node5 = linkClInsertNode(global_link_cl_info_head_p, 1);
    node5->ipv4 = 88;
    node5->num = 9;
    linkClTest_show(global_link_cl_info_head_p);
    printf("节点个数：%d\n", linkClGetNum(global_link_cl_info_head_p)); 

    linkClDelNode(global_link_cl_info_head_p, node2);
    linkClTest_show(global_link_cl_info_head_p);
    

    linkClDestory(global_link_cl_info_head_p);
    linkClTest_show(global_link_cl_info_head_p);

}




