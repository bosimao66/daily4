

#include "tablink.h"
#include "QDebug"



static int global_uniqueNum = 0;//节点序号自增，每个节点的 num 数字唯一


TabLink::TabLink()
{
    this->next = NULL;
    this->isDelete = false;
    this->uiIsAvailable = 0;
    this->isDialy = false;
    this->isSaved = 1;
    this->isChangeCnt = 0;
    this->lastFileNameExit = false;
    this->isNew = 1;
    //this->fileScanObj_currentIndex = -1;
}

TabLink::~TabLink()
{
    ;
}

//创建链表
TabLink *TabLink::fun_create()
{
    TabLink *head = new TabLink();
    return head;
}
//获取链表节点个数
int TabLink::fun_getTotalNum(TabLink *head)
{
    if(head == NULL || head->isDelete == true)
    {
        qDebug()<<"fun_getTotalNum error: head is NULL";
        return -1;
    }
    int cnt = 0;
    TabLink *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
        cnt++;
    }
    return cnt;

}
//添加链表节点
TabLink * TabLink::fun_add(TabLink *head)
{
    if(head == NULL || head->isDelete == true)
    {
        qDebug()<<"fun_add error: head is NULL";
        return NULL;
    }

    TabLink *node = new TabLink();

    TabLink *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
    }
    prob->next = node;

    node->uniqueNum = global_uniqueNum;
    global_uniqueNum++;

    return node;
}
//删除链表节点（链表头不计，节点索引index 从0开始）
void TabLink::fun_del(TabLink *head, int index)
{
    if(head == NULL || head->isDelete == true)
    {
        qDebug()<<"fun_del error: head is NULL";
        return;
    }
    int cnt = 0;
    TabLink *prob = head->next;
    TabLink *last = head;
    while(prob != NULL)
    {
        if(cnt == index)
        {
            prob->isDelete = true;
            last->next = prob->next;
            delete prob;

            return;
        }
        last = prob;
        prob = prob->next;
        cnt++;
    }

}

//获取本节点的索引值
int TabLink::fun_getIndex(TabLink *head)
{
    int cnt = 0;
    TabLink *prob = head->next;
    while(prob != NULL)
    {
        if(this->uniqueNum == prob->uniqueNum)
        {
           return cnt;
        }
        cnt++;
        prob = prob->next;
    }

    return -1;
}


//通过索引来获取链表节点
TabLink * TabLink::fun_getTabByIndex(TabLink *head, int index)
{
    if(head == NULL || head->isDelete == true)
    {
        qDebug()<<"fun_getTabByIndex error: head is NULL";
        return NULL;
    }
    int cnt = 0;
    TabLink *prob = head;
    while(prob->next != NULL)
    {
        if(cnt == index)
        {
            return prob->next;
        }
        prob = prob->next;
        cnt++;
    }

    return NULL;
}

//通过唯一序号来获取链表节点
TabLink *TabLink::fun_getTabByUniqueNum(TabLink *head, int num)
{
    if(head == NULL || head->isDelete == true)
    {
        qDebug()<<"fun_getTabByIndex error: head is NULL";
        return NULL;
    }

    TabLink *prob = head;
    while(prob->next != NULL)
    {
        if(prob->next->uniqueNum == num)
        {
            return prob->next;
        }
        prob = prob->next;

    }

    return NULL;

}


void TabLink::fun_destory(TabLink *head)
{
    if(head == NULL || head->isDelete == true)
    {
        qDebug()<<"fun_destory error: head is NULL";
        return;
    }
    while(head->next != NULL)
    {
        TabLink *prob = head;
        TabLink *last = prob;
        while(prob->next != NULL)
        {
            last = prob;
            prob = prob->next;
        }
        prob->isDelete = true;
        delete prob;
        last->next = NULL;
        qDebug()<<"del +";

    }
    head->isDelete = true;
    delete head;
}

void TabLink::fun_test()
{
    TabLink *head = this->fun_create();
    TabLink *node1 = this->fun_add(head);
    TabLink *node2 = this->fun_add(head);
    TabLink *node3 = this->fun_add(head);

    qDebug()<<"=======================测试1";
    node1->fileName.append("aa");
    node2->fileName.append("bb");
    node3->fileName.append("cc");
    node1->isSaved = 0;
    node2->isSaved = 1;
    node3->isSaved = 0;
    this->fun_showInfo(head);
    qDebug()<<"=======================测试2";
    this->fun_del(head, 2);
    this->fun_showInfo(head);
    qDebug()<<"=======================测试3";
    this->fun_destory(head);
    this->fun_showInfo(head);

}
void TabLink::fun_showInfo(TabLink *head)
{
    if(head == NULL || head->isDelete == true)
    {
        qDebug()<<"fun_showInfo error: head is NULL";
        return;
    }

    qDebug()<<"============tablink info================";
    qDebug()<<"num: "<<this->fun_getTotalNum(head);
    TabLink *prob = head;
    while(prob->next != NULL)
    {
        qDebug()<<"fileName："<<prob->next->fileName<<" isSaved"<<prob->next->isSaved;
        prob = prob->next;
    }



}


TabLink *TabLink::fun_fileExit(TabLink *head, QString dir, QString fileName)
{
    TabLink *prob = head->next;
    while(prob != NULL)
    {
        if(dir.compare(prob->dir) == 0 &&
           fileName.compare(prob->fileName) == 0)
        {

            return prob;
        }
        prob = prob->next;
    }

    return NULL;
}






