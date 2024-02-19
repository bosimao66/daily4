#ifndef OPTLINK_H
#define OPTLINK_H

#include <QString>

#define UNIQUE_CODE_SIZE 16

class OptLink
{
public:
    OptLink();
    Optlink(OptLink *c);//拷贝？？

    OptLink *next;
    //成员
    int opt;
    char unique[UNIQUE_CODE_SIZE + 1];//唯一标识码（被创建时赋值，之后不更改;每个字节范围 0-9 a-z）
    QString obj;//对象
    QString new_name;
    QByteArray content;
    qint64 time;//时间戳
    QByteArray back;//备份内容（修改之前的内容）
    int index;
    //方法
    OptLink *linkCreate(void);//链表创建
    OptLink *linkAddNode(OptLink *head);//增加节点
    OptLink *linkInsertNode(OptLink *head, int index);//增加节点
    void linkDelNode(OptLink *head);//删除节点
    void linkDelNode_byIndex(OptLink *head, int index);//根据索引删除节点  注：索引为节点的一个属性，以0为起始自增
    void linkDestory(OptLink *head);//摧毁链表
    int linkDelNode_byNode(OptLink *head, OptLink *node);
    int linkGetIndex(OptLink *head, OptLink *node);
    void linkDelNodeAll(OptLink *head);
    int getNum(OptLink *head);
    int linkAppend(OptLink *head, OptLink *add_head);
    //测试
    void test(void);
    void show(OptLink *head);
};

#endif // OPTLINK_H
