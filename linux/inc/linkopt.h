
#ifndef _LINKOPT_H_
#define _LINKOPT_H_

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "buf.h"


#define UNIQUE_CODE_SIZE 16


#define FILE_NAME_MAX_SIZE 64
#define FILE_PATH_MAX_SIZE 256
#define OPT_LINK_BUF_NAME "optlink"   //由于字节大小不定，所以存放数据的结构体
//成员应设置为“指向”buf内容的结构体
//此缓存应该在链表节点释放时候删除相应内容
#define OPT_LIN_BUF_PATH "/mnt/hgfs/share/QT/cloudfile/wkspace/link" 
#define WORK_SPACE_PATH "/mnt/hgfs/share/QT/cloudfile/wkspace" 

typedef struct OptLinktmp{
	int opt;
	char unique[UNIQUE_CODE_SIZE + 1];//唯一标识码（被创建时赋值，之后不更改;每个字节范围 0-9 a-z）
	char obj[FILE_NAME_MAX_SIZE];
	char new_name[FILE_NAME_MAX_SIZE];
	time_t time;//时间戳
	int index;
	struct OptLinktmp *next;
}OptLink;

#include "fileopt.h"  //注：fileopt.h 需要宏 UNIQUE_CODE_SIZE、需要结构体 OptLink


OptLink *gloable_opt_link_head;

//为了存储链表中未知字节大小的成员，例如 content、back需要开辟本地缓存文件，但是每个
//链表动作可能有不同之处，所以会引入很多问题
//为了获取 缓存文件名，则需要新建管理 OptLink 链表的链表？？管理链表的结构体就可以了
#define OPT_LINK_MAX_NUM 20          //同时存在的最大链表个数
//#define OPT_LINK_MAX_NODE_NUM 1000   //每个链表最大的节点数（由于要使用到数组的原因，不得不设置上限，有点无奈）
typedef struct OptLinkManagetmp{
	int num;
	OptLink *head[OPT_LINK_MAX_NUM];//OptLink 链表头
	char buf_name[OPT_LINK_MAX_NUM][FILE_NAME_MAX_SIZE];//某链表专属的缓存文件名
}OptLinkManage;
OptLinkManage gloale_opt_link_manage_head;


//数据表内容操作
typedef enum enum_contentOpttmp{
	contentOpt_new = 1,
	contentOpt_append,
	contentOpt_del,
	contentOpt_get,
	contentOpt_getInfo   //获取信息，主要为起始处start和长度len
}enum_contentOpt;



int generateUniqueCode(char *code);
int linkOptManage_getIndex(OptLink *head);
int dataTable_create(char *name);
//数据表操作接口（增、删、改、查）
int dataTable_contentOptInt(char *data_table, OptLink *head, OptLink *node, int opt, void *data, unsigned int start, unsigned int len);
int dataTable_contentOpt(OptLink *head, OptLink *node, int opt, void *data, unsigned int start, unsigned int len);
void linkOptInit(void);
OptLink *linkOptCreate(void);
//OptLink *linkOptCreate_assignBuf(char *buf_name);
OptLink *linkOptAddNode(OptLink *head);
OptLink *linkOptInsertNode(OptLink *head, int index);
void linkOptDelNode(OptLink *head);
void linkOptDelNode_byIndex(OptLink *head, int index);
int linkOptDelNode_byNode(OptLink *head, OptLink *node);
void linkOptDelNodeAll(OptLink *head);
void linkOptlinkDestory(OptLink *head);
int linkOptGetIndex(OptLink *head, OptLink *node);
int linkOptGetNum(OptLink *head);
int linkOptlinkAppend(OptLink *head, OptLink *add_head);
void OptLinkShow(OptLink *head);
void OptLinkTest(void);

OptLink *linkOptLink_getNodeByUnique(OptLink *head, char *unique, int t);
int linkOptlinkAppend_noRepeat(OptLink *head, OptLink *add_head);




#endif


