
/*
   文件操作链表
   （1）产生随机数
#include <ctime>
qsrand(time(NULL));
int n = qrand() % 5;    //产生5以内的随机数
*/
#include "optlink.h"
#include <QTime>
#include <QDateTime>
#include <QDebug.h>



#define OPT_LINK_MAX_NODE_NUM 1000    //最大的记录条数，超过之后，如果还要添加，则需要删除老的节点



#define CODE_SELETE_NUM 36
const char u_code_selete[CODE_SELETE_NUM]={
    '0','1','2','3','4','5','6','7','8','9',
    'a','b','c','d','e','f','g','h','i','j',
    'k','l','m','n','o','p','q','r','s','t',
    'u','v','w','x','y','z'
};

//非类方法
//生成唯一码
int generateUniqueCode(char *code)
{
    static int cnt = 0;
    cnt++;
    //char code[UNIQUE_CODE_SIZE + 1] = {0};//最后一个字符存放'\0'
    QTime time;
    quint16 n;
    int i;
    for(i = 0; i < UNIQUE_CODE_SIZE / 2; i++)
    {
        time = QTime::currentTime();
        qsrand(time.msec() + time.second()*1000 + (65536 * 2 / UNIQUE_CODE_SIZE) * i + cnt);
        n = qrand() % 65536;    //产生16位数的随机数
        code[i * 2 ] = u_code_selete[(n >> 8) % 36];
        code[i * 2 + 1] = u_code_selete[(n & 0xff) % 36];
    }
    code[UNIQUE_CODE_SIZE] = '\0';

    //qDebug()<<"随机码："<<QString(code);
    return 0;
}


//################################################################链表基本功能
//初始化
OptLink::OptLink()
{
    ;
}
//拷贝
OptLink::Optlink(OptLink *c)
{
    opt = c->opt;
    strcpy(unique, c->unique);
    obj.clear();
    obj.append(c->obj);
    content.clear();
    content.append(c->content);
    time = c->time;
    back.clear();
    back.append(c->back);
    index = c->index;
}

//方法
OptLink *OptLink::linkCreate(void)//链表创建
{
    OptLink *head = new OptLink();
    head->next = NULL;
    return head;
}


OptLink *OptLink::linkAddNode(OptLink *head)//增加节点
{
    if(head == NULL)
    {
        qDebug()<<"linkAddNode: head is NULL\n";
        return NULL;
    }
    OptLink *node = new OptLink();
    OptLink *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
    }
    prob->next = node;
    node->next = NULL;
    //生成唯一码，唯一码重复的概率还是很低的，所以只进行一次检测
    generateUniqueCode(node->unique);
    int mark = 1;//一次检测标志
    prob = head->next;
    while(prob != NULL)
    {
        if(strcmp(prob->unique, node->unique) == 0 && prob != node)
        {
            if(mark == 1)
            {
                generateUniqueCode(node->unique);
                prob = head->next;
                mark = 0;
                continue;
            }
            printf("linkAddNode: unique is repetition\n");
            return NULL;
        }
        prob = prob->next;
    }
    //时间戳
    node->time = QDateTime::currentDateTime().toSecsSinceEpoch();

    //节点个数限制
    int cnt;
    cnt = 0;
    prob = head->next;
    while(prob != NULL)
    {
        cnt++;
        prob = prob->next;
    }
    if(cnt >= OPT_LINK_MAX_NODE_NUM)
    {
        prob = head->next;
        head->next = head->next->next;
        delete &prob;
    }

    return node;
}


OptLink *OptLink::linkInsertNode(OptLink *head, int index)//插入节点
{
    if(head == NULL)
    {
        return NULL;
    }
    int  i;
    OptLink *node = new OptLink();
    OptLink *prob = head;
    i = 0;
    while(prob != NULL)
    {

        if(i == index)
        {
            node->next = prob->next;
            prob->next = node;
            break;
        }
        i++;
        prob = prob->next;
    }

    //生成唯一码，唯一码重复的概率还是很低的，所以只进行一次检测
    generateUniqueCode(node->unique);
    int mark = 1;//一次检测标志
    prob = head->next;
    while(prob != NULL)
    {
        if(strcmp(prob->unique, node->unique) == 0 && prob != node)
        {
            if(mark == 1)
            {
                generateUniqueCode(node->unique);
                mark = 0;
                continue;
            }
            printf("linkAddNode: unique is repetition\n");
            return NULL;
        }
        prob = prob->next;
    }
    //时间戳
    node->time = QDateTime::currentDateTime().toSecsSinceEpoch();

    //节点个数限制
    int cnt2;
    cnt2 = 0;
    prob = head->next;
    while(prob != NULL)
    {
        cnt2++;
        prob = prob->next;
    }
    if(cnt2 >= OPT_LINK_MAX_NODE_NUM)
    {
        prob = head->next;
        head->next = head->next->next;
        delete &prob;
    }
    return node;
}


void OptLink::linkDelNode(OptLink *head)//删除节点
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob->next != NULL)
    {
        last = prob;
        prob = prob->next;

    }
    last->next = NULL;
    delete prob;
}


void OptLink::linkDelNode_byIndex(OptLink *head, int index)//根据索引删除节点  注：索引为节点的一个属性，以0为起始自增
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    int i = 0;
    while(prob != NULL)
    {
        if(i == index)
        {
            last->next = prob->next;
            delete prob;
            return;
        }
        i++;
        last = prob;
        prob = prob->next;
    }
}

//通过节点本身删除节点
int OptLink::linkDelNode_byNode(OptLink *head, OptLink *node)
{
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob != NULL)
    {
        if(prob == node)
        {
            last->next = node->next;
            delete node;
            return 0;
        }
        last = prob;
        prob = prob->next;
    }

    return -1;
}

//删除链表的所有节点，但是并不删除链表头
void OptLink::linkDelNodeAll(OptLink *head)
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob != NULL)
    {
        if(prob->next == NULL)
        {
            delete prob;
            last->next = NULL;
            prob = head->next;
            last = head;
            continue;
        }
        last = prob;
        prob = prob->next;
    }
    head->next = NULL;
}

void OptLink::linkDestory(OptLink *head)//摧毁链表
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    OptLink *last = head;
    while(prob != NULL)
    {
        if(prob->next == NULL)
        {
            delete prob;
            last->next = NULL;
            prob = head->next;
            last = head;
            continue;
        }
        last = prob;
        prob = prob->next;
    }
    delete head;
}

//获取索引
int OptLink::linkGetIndex(OptLink *head, OptLink *node)
{
    OptLink *prob = head->next;
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        if(prob == node)
        {
            return cnt;
        }
        cnt++;
        prob = prob->next;
    }
    return -1;
}
//获取节点个数
int OptLink::getNum(OptLink *head)
{
    if(head == NULL)
    {
        return -1;
    }
    OptLink *prob = head->next;
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        cnt++;
        prob = prob->next;
    }

    return cnt;
}

//链表追加(没有新增)
int OptLink::linkAppend(OptLink *head, OptLink *add_head)
{
    if(head == NULL || add_head == NULL)
    {
        qDebug()<<"linkAppend: link head is NULL";
        return -1;
    }
    OptLink *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
    }

    prob->next = add_head->next;

    return 0;
}











//测试

void OptLink::show(OptLink *head)
{
    if(head == NULL)
    {
        return;
    }
    OptLink *prob = head->next;
    qDebug()<<"totle num: "<< getNum(head);
    while(prob != NULL)
    {
        qDebug()<<" opt="<<prob->opt<<" obj="<<prob->obj
            <<" time="<<prob->time<<" unique="<<prob->unique<<" content="<<prob->content;
        prob = prob->next;
    }

}

void OptLink::test(void)
{
    OptLink *head = linkCreate();
    OptLink *node1 = linkAddNode(head);
    OptLink *node2 = linkAddNode(head);
    OptLink *node3 = linkAddNode(head);
    OptLink *node4 = linkAddNode(head);
    node1->opt = 1;
    node1->obj.clear();
    node1->obj.append("name1");
    node1->content.clear();
    node1->content.append("cont1");
    node2->opt = 2;
    node2->obj.clear();
    node2->obj.append("name2");
    node2->content.clear();
    node2->content.append("cont2");
    node3->opt = 3;
    node3->obj.clear();
    node3->obj.append("name3");
    node3->content.clear();
    node3->content.append("cont3");
    node4->opt = 4;
    node4->obj.clear();
    node4->obj.append("name4");
    node4->content.clear();
    node4->content.append("cont4");
    qDebug()<<"================>链表测试1";
    show(head);

    qDebug()<<"================>链表测试2";
    linkDelNode_byNode(head, node4);
    qDebug()<<"[获取索引]node3 的索引是"<<(int)linkGetIndex(head, node3);
    show(head);

    /*qDebug()<<"================>链表测试3";
      OptLink *node5 = linkInsertNode(head, 1);
      node5->opt = 5;
      node5->obj.clear();
      node5->obj.append("name5");
      node5->content.clear();
      node5->content.append("cont5");
      show(head);

      qDebug()<<"================>链表测试4";
      linkDelNode_byIndex(head, 1);
      show(head);

      qDebug()<<"================>链表测试5";
      linkDestory(head);
      show(head);*/
}










