

#ifndef _FILEOPT_H_
#define _FILEOPT_H_

#include "buf.h"
#include "linkopt.h"





//==============================================================应该和QT tcp客户端同步
#define START_CODE_0 0x66
#define START_CODE_1 0x77
#define START_CODE_2 0x88
#define START_CODE_3 0x99
//存储参数表的结构体
#define PARA_FILE_NAME_MAX_SIZE 128
typedef struct STRUCT_PARA_TABLE_ELEMENTtmp{
	char file_name[PARA_FILE_NAME_MAX_SIZE];//文件名
	unsigned char opt;//操作码
	char unique[UNIQUE_CODE_SIZE + 1];//唯一标识码（包含字符串尾部'\0'）
	unsigned int index;//在链表中的序号
	unsigned int time;
}STRUCT_PARA_TABLE_ELEMENT;

typedef enum fileOptCodetmp{
				fileOptCode_new = 1,
				fileOptCode_cover,
				fileOptCode_del

}fileOptCode;

//==================================================================



//====================================================用户级别的文件同步操作

#define USER_LOCAL_PARA_TABLE "local.para.table"
#define USER_LOCAL_DATA_TABLE "local.data.table"
#define USER_SYNC_PARA_TABLE "sync.para.table"
#define USER_SYNC_DATA_TABLE "sync.data.table"




int paraTable_create(OptLink *head, char *para_name);
int paraTable_toOptLink(char *para_name, OptLink *out_head);
int fileOpt_exeLinkAction(char *wkpath, OptLink *head, char *data_table, int by_force);
int fileOpt_exeNodeAction(char *wkpath, OptLink *head, OptLink *node, char *data_table, int by_force);
void fileOptTest(void);
int fileOpt_combineDataTable(char *t1, char *t2);
int fileOpt_syncToServer(char *user_path);


int dataTable_getEleByUnique(char *data_table, char *unique, int t, int *start, int *len);



#endif

