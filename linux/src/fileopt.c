

#include <stdio.h>
#include <string.h>
#include "fileopt.h"
#include "tcpServer.h"
#include "user.h"


#define FILE_OPT_BLOCK_SIZE 1024

#define FILE_ADD_PATH(out, f, path)  do{\
	if(path == NULL)\
	{\
		sprintf(out, "%s", f);\
	}\
	else\
	{\
		sprintf(out, "%s/%s", path, f);\
	}\
}while(0);





//将链表存于散列表中
/*
 *用于同步的散列表文件存于store文件夹中（临时的则放置于 optlink 文件夹），例如：para.link  和 data.link
 参数散列表约定：
 （1）有效数据起始于起始码 0x66 0x77 0x88 0x99（在文件中找到第一个此标志）
 （2）数据头：结构体的个数，大小为4个字节（肯定够用了）
 （3）数据有多个连续的结构体组成，结构包含必要参数：opt、obj、unique（以后可以拓展，加一个MD5校验码）
 数据散列表约定：
 （1）有效数据起始于起始码 0x66 0x77 0x88 0x99
 （2）数据头：结构块的个数，大小为4个字节（肯定够用了）
 （3）连续的数据块：数据块长度（4字节）+ 序号(4字节) + 唯一标识码（16字节）+ 有效数据
 */

//根据链表创建散列表
int paraTable_create(OptLink *head, char *para_name)
{
    if(head == NULL)
    {
        return -1;
    }

    //创建文件
    if(0 != fullPath_bufCreate(para_name))
        {
        //printf("paraTable_create: file create faild, file %s maybe exist\n", para_name);
        return -1;
    }
    
    //填充头部
    int num = linkOptGetNum(head);
    char tmp[32] = {0};//注意：以后更改代码时要看大小是否超过范围
    tmp[0] = START_CODE_0;
    tmp[1] = START_CODE_1;
    tmp[2] = START_CODE_2;
    tmp[3] = START_CODE_3;
    tmp[4] = (num & 0xff000000) >> 24;   
    tmp[5] = (num & 0xff0000) >> 16;  
    tmp[6] = (num & 0xff00) >> 8;  
    tmp[7] = (num & 0xff);  
    fullPath_bufAppend(para_name, tmp, 8);

   //存储结构体元素
    int size = sizeof(STRUCT_PARA_TABLE_ELEMENT);
    STRUCT_PARA_TABLE_ELEMENT ele;    
    OptLink *prob = head->next;
    int index;
    index = 0;
    while(prob != NULL)
    {
        memset((void *)(&ele), 0 ,size);
        if(prob->obj != NULL)
        {
            strcpy(ele.file_name, prob->obj);
        }
        if(prob->new_name != NULL)
        {
            strcpy(ele.new_file_name, prob->new_name);
        }
        ele.index = index;
        ele.opt = prob->opt;
        if(prob->unique != NULL)
        {
            strcpy(ele.unique, prob->unique);
        }
        ele.time = prob->time;
        
        fullPath_bufAppend(para_name, (char *)(&ele), size);
        
        index++;
        prob = prob->next;
    }
   
}

//将参数表解析成链表
int paraTable_toOptLink(char *para_name, OptLink *out_head)
{
    int i;
    //文件长度获取和存在性检测
    if(fullPath_bufExit(para_name) != 0)
    {
        printf("paraTable_toOptLink: file not exist\n");
        return -1;
    }
    int len = fullPath_bufgetLen(para_name);
    if(len < 8)
    {
        printf("paraTable_toOptLink: file is too short\n");
        return -1;
    }

    //起始码校验以及获取节点数量
    char tmp[32] = {0};
    fullPath_bufGetBlock(para_name, tmp, 0, 8);
    if((unsigned char)(tmp[0]) != START_CODE_0 || 
        (unsigned char)(tmp[1]) != START_CODE_1 || 
        (unsigned char)(tmp[2]) != START_CODE_2 || 
        (unsigned char)(tmp[3]) != START_CODE_3 )
     {
        printf("paraTable_toOptLink: fiel format is error\n");
        return -1;

     }
     int num = (unsigned char)(tmp[4]) >> 24 | 
                (unsigned char)(tmp[5]) >> 16 |
                (unsigned char)(tmp[6]) >> 8 |
                (unsigned char)(tmp[7]) ;
     if(num < 0 )
     {
        printf("paraTable_toOptLink: num get error\n");
        return -1;
     }

     //创建链表（请确保初始化--linkOptInit已完成）
     linkOptDelNodeAll(out_head);
     int size = sizeof(STRUCT_PARA_TABLE_ELEMENT);
     STRUCT_PARA_TABLE_ELEMENT ele;
     OptLink *node;
     int seek =8;
     if(size * num > len)
     {
        num = len / size;
        if(num < 1)//表明没有节点数据
        {
            return 0;
        }
     }
     for(i = 0; i < num; i++)
     {      
        memset((void *)(&ele), 0, size);
        fullPath_bufGetBlock(para_name, (void *)(&ele), seek, size);
        

        //printf("====>ele  name=%s  index=%d  opt=%d  unique=%s\n", ele.file_name, ele.index, ele.opt, ele.unique);
        node = linkOptAddNode(out_head);
        if(node == NULL)
        {
            continue;
        }
        node->index = ele.index;//这个成员比较无用，可以考虑删除
        strcpy(node->obj, ele.file_name);
        strcpy(node->new_name, ele.new_file_name);
        strcpy(node->unique, ele.unique);
        node->opt = ele.opt;
        node->time = ele.time;
        
        seek += size;
     }

     return 0;
}

//执行链表的动作
int fileOpt_exeLinkAction(char *wkpath, OptLink *head, char *data_table, int by_force)
{
    if(head == NULL)
    {
        printf("fileOpt_exeLinkAction: head is NULL\n");
        return -1;
    }
    OptLink *prob = head->next;
    while(prob != NULL)
    {

        fileOpt_exeNodeAction(wkpath, head, prob, data_table, by_force);
    
        prob = prob->next;
    }

    return 0;
        
}



//执行一个节点的动作（buf_name: 存储节点对应的内容）  by_force = 1 代表新建文件时，覆盖已有文件   0 则不动作
int fileOpt_exeNodeAction(char *wkpath, OptLink *head, OptLink *node, char *data_table, int by_force)
{
    char full_name[512] = {0};
    char tmp_name[512] = {0};
    
    //参数检测
    if(data_table == NULL)
    {
        printf("fileOpt_exeNodeAction: para error\n");
        return -1;
    }

	FILE_ADD_PATH(full_name, node->obj, wkpath);
	//printf("=================>C full_name=%s\n", full_name);
    char tmp[32] = {0};
    int start;
    int len;
    if(0 != dataTable_contentOptInt(data_table, head, node, contentOpt_getInfo, tmp, 0, 0))
    {
        printf("fileOpt_exeNodeAction: get info error\n");
        return -1;
    }
    start = (unsigned char)(tmp[0]) >> 24 | 
        (unsigned char)(tmp[1]) >> 16 | 
        (unsigned char)(tmp[2]) >> 8 | 
        (unsigned char)(tmp[3]) ; 
    len = (unsigned char)(tmp[4]) >> 24 | 
        (unsigned char)(tmp[5]) >> 16 | 
        (unsigned char)(tmp[6]) >> 8 | 
        (unsigned char)(tmp[7]) ; 
    //printf("===================================>B start=%d len=%d\n", start, len) ;   
    if(node->opt == fileOptCode_new)
    {
        //printf("===========================>A obj=%s\n", node->obj);
        if(0 != fullPath_bufCreateNoCover(full_name))
        {
            printf("fileOpt_exeNodeAction: file exist\n");
            return -1;
        }
        if(by_force == 1)
        {
			 if(0 > dataCopy_cover(data_table, full_name, start, len))
		    {
		        printf("fileOpt_exeNodeAction: data copy error\n");
		        return -1;
		    }
        }
       
        
    }
    else if(node->opt == fileOptCode_cover)
    {
         if(0 != fullPath_bufCreate(full_name))
         {        
            printf("fileOpt_exeNodeAction: file create error\n");
            return -1;
         }
         if(0 > dataCopy_cover(data_table, full_name, start, len))
         {
            printf("fileOpt_exeNodeAction: data copy  error\n");
            return -1;
         }
         
    }
    else if(node->opt == fileOptCode_del)
    {
        if(0 != remove(full_name))
        {
            return -1;
        }
    }
     else if(node->opt == fileOptCode_rename)
    {
    	FILE_ADD_PATH(tmp_name, node->new_name, wkpath);
        if(0 != rename(full_name, tmp_name))
        {
            return -1;
        }
    }
    return 0;
}

//合并数据表
//简化过程： 认为文件严格格式为 起始码（4字节）+ 节点个数（4字节） + 数据（直到末尾）
int fileOpt_combineDataTable(char *t1, char *t2)    //接到t1上去
{
	char tmp[32] = {0};
	//个数设置
    int num1, num2;
    fullPath_bufGetBlock(t1, tmp, 0, 8);
	num1 = (unsigned char)(tmp[4]) >> 24 | 
			(unsigned char)(tmp[5]) >> 16 | 
			(unsigned char)(tmp[6]) >> 8 | 
			(unsigned char)(tmp[7]) ; 
	fullPath_bufGetBlock(t2, tmp, 0, 8);
	num2 = (unsigned char)(tmp[4]) >> 24 | 
			(unsigned char)(tmp[5]) >> 16 | 
			(unsigned char)(tmp[6]) >> 8 | 
			(unsigned char)(tmp[7]) ;	
	num1 += num2;
	tmp[0] = (num1 & 0xff000000) >> 24;   
    tmp[1] = (num1 & 0xff0000) >> 16;  
    tmp[2] = (num1 & 0xff00) >> 8;  
    tmp[3] = (num1 & 0xff); 
    fullPath_bufSetBlock(t1, tmp, 4, 4);
    //拼接文件
    int len = fullPath_bufgetLen(t2);
    dataCopy_append(t2, t1, 8, len - 8);

	return 0;

}




//测试
void fileOptTest(void)
{
    
   // linkOptInit();

    OptLink *head = linkOptCreate();
    OptLink *node1 = linkOptAddNode(head);
    OptLink *node2 = linkOptAddNode(head);
    OptLink *node3 = linkOptAddNode(head);
    OptLink *node4 = linkOptAddNode(head);

    node1->opt = fileOptCode_new;
    node2->opt = fileOptCode_new;
    node3->opt = fileOptCode_new;
    node4->opt = fileOptCode_new;

//    strcpy(node1->obj, "/mnt/hgfs/share/QT/cloudfile/wkspace/link/node1");
//    strcpy(node2->obj, "/mnt/hgfs/share/QT/cloudfile/wkspace/link/node2");
//    strcpy(node3->obj, "/mnt/hgfs/share/QT/cloudfile/wkspace/link/node3");
//    strcpy(node4->obj, "/mnt/hgfs/share/QT/cloudfile/wkspace/link/node4");

    strcpy(node1->obj, "name1");
    strcpy(node2->obj, "name2");
    strcpy(node3->obj, "name3");
    strcpy(node4->obj, "name4");


//
//    dataTable_contentOpt(head, node1, contentOpt_append, "kkkkkkkk", 0, 8);
//    dataTable_contentOpt(head, node2, contentOpt_append, "jjjjjjjj", 0, 8);
//    dataTable_contentOpt(head, node3, contentOpt_append, "hhhhhhhh", 0, 8);
//    dataTable_contentOpt(head, node4, contentOpt_append, "iiiiiiii", 0, 8);
//
//    OptLinkShow(head);

    
    //fileOpt_exeNodeAction(head, node1, "/mnt/hgfs/share/QT/cloudfile/wkspace/link/optlink-0.buf");

    //fileOpt_exeLinkAction(head, "/mnt/hgfs/share/QT/cloudfile/wkspace/link/optlink-0.buf");

    
    paraTable_create(head, "/mnt/hgfs/share/QT/cloudfile/wkspace/user/guest/.table/local.para.table");
//    
//
//    OptLink *head2 = linkOptCreate();
  //  paraTable_toOptLink("/mnt/hgfs/share/QT/cloudfile/wkspace/link/para-test.buf", head2);
//    OptLinkShow(head2);

//	OptLink *headtmp = linkOptCreate();
//  	paraTable_toOptLink("/mnt/hgfs/share/QT/cloudfile/wkspace/user/guest/.table/t.para.table", headtmp);
//	OptLinkShow(headtmp);
//
//	paraTable_toOptLink("/mnt/hgfs/share/QT/cloudfile/wkspace/user/guest/.table/para.link", headtmp);
//	OptLinkShow(headtmp);



	
}



//====================================================用户级别的文件同步操作
/*用户文件同步
（1）例子：
用户 guest , 同步文件目录放置于 xx/wkspace/user/guest
参数表 para.table 和数据表 data.table 放置于 xx/wkspace/user/guest/.table 
（2）参数表
local.para.table  本地维护的参数表
local.data.table  本地维护的数据表
sync.para.table		请求同步的参数表
sync.data.table		请求同步的数据表
*/


int fileOpt_syncToServer(char *user_path)
{
	char user_table_floder[512] = {0};
	sprintf(user_table_floder, "%s/%s", user_path, USER_WORK_TABLE_FLODER);
	if(floderExist(user_table_floder) == -1)
	{
		printf("fileOpt_userFileSync: user_floder is not exist, floder=%s\n", user_table_floder);
		return -1;
	}
	//将参数表转换为链表
	char local_para_tabler[512];
	char local_data_tabler[512];
	sprintf(local_para_tabler, "%s/%s", user_table_floder, USER_LOCAL_PARA_TABLE);
	sprintf(local_data_tabler, "%s/%s", user_table_floder, USER_LOCAL_DATA_TABLE);
	char sync_para_tabler[512];
	char sync_data_tabler[512];
	sprintf(sync_para_tabler, "%s/%s", user_table_floder, USER_SYNC_PARA_TABLE);
	sprintf(sync_data_tabler, "%s/%s", user_table_floder, USER_SYNC_DATA_TABLE);

	OptLink *local_head = linkOptCreate();
	OptLink *sync_head = linkOptCreate();
	
	if(0 == fullPath_bufExit(local_para_tabler))
	{
		//printf("===================>A  local_para_tabler=%s\n", local_para_tabler);
		paraTable_toOptLink(local_para_tabler, local_head);		
	}
	if(0 == fullPath_bufExit(sync_para_tabler))
	{
		//printf("===================>b  sync_para_tabler=%s\n", sync_para_tabler);
		paraTable_toOptLink(sync_para_tabler, sync_head); 	
	}

    printf("=============>fileOpt_syncToServer: local_head\n");
	OptLinkShow(local_head);
	printf("=============>fileOpt_syncToServer: sync_head\n");
	OptLinkShow(sync_head);
	
	//执行链表动作（为了保证和客户端同步，应实现强制执行）
	//思考一个问题：当发生冲突时，要不要完全将客户端的内容推送到云端
	//printf("================>A sync_para_tabler=%s", sync_para_tabler);
	fileOpt_exeLinkAction(user_path, sync_head, sync_data_tabler, 1);

	//将同步链表 追加到本地参数表（确保追加的节点不与之前的重复，通过唯一码和时间戳来判断）
	linkOptlinkAppend_noRepeat(local_head, sync_head);	
	paraTable_create(local_head, local_para_tabler);

	//将同步数据表追 加到本地数据包
	fileOpt_combineDataTable(local_data_tabler, sync_data_tabler);

    //摧毁链表
    linkOptlinkDestory(local_head);
    
    
	return 0;

}

//通过参数表映射的节点来获取数据表内容，并存于新的数据表文件中
int fileOpt_getDataTableByParaTable(char *source_data_table, char *new_data_table, char *para_table)
{
   
    OptLink *head = linkOptCreate();
    paraTable_toOptLink(para_table, head);
    OptLink *prob = head->next; 
    int start, len;

    OptLinkShow(head);
    //创建参数表
    dataTable_create(new_data_table);
    
    //搜索数据元
    int cnt;
    cnt = 0;
    int num = 1;
    char tmp[32] = {0};
    while(prob != NULL)
    {
        if(0 == dataTable_getEleByUnique(source_data_table, prob->unique, prob->time, &start, &len))
        {			 
            dataCopy_append(source_data_table, new_data_table, start, len);
            num ++;
        }
        else
        {
            cnt++;
        }
        prob = prob->next;
    }

    tmp[0] = (num & 0xff000000) >> 24;   
    tmp[1] = (num & 0xff0000) >> 16;  
    tmp[2] = (num & 0xff00) >> 8;  
    tmp[3] = (num & 0xff);
    fullPath_bufSetBlock(new_data_table, tmp, 4, 4);
	printf("======================>A num=%d\n", num);

    
    if(cnt > 0)
    {
        printf("fileOpt_getDataTableByParaTable: lost ele num &d\n", cnt);
    }

    

    
    return 0;

}


//通过唯一码和时间戳来搜索数据表数据元，输出 数据元的起始位置和长度
int dataTable_getEleByUnique(char *data_table, char *unique, int t, int *start, int *len)
{
      //分析此文件中的内容   
        //--1 取得文件长度
    //printf("=============================>A data_table=%s\n", data_table);
    int num;
    int totle_len = fullPath_bufGetLen(data_table);
    if(totle_len < 8)
        {
        printf("dataTable_getEleByUnique: file is too short\n");
        return 0;
    }    
    //--2 判断起始码 取得个数
    char tmp[32 + UNIQUE_CODE_SIZE +1] = {0};
    fullPath_bufGetBlock(data_table, tmp, 0, 8);
    //printf("=======================>C   tmp[0]=0x%x tmp[1]=0x%x tmp[2]=0x%x tmp[3]=0x%x\n", tmp[0], tmp[1], tmp[2], tmp[3]);
    if(START_CODE_0 != (unsigned char)tmp[0] || 
        START_CODE_1 != (unsigned char)tmp[1] ||
        START_CODE_2 != (unsigned char)tmp[2] ||
        START_CODE_3 != (unsigned char)tmp[3])
        {
        printf("dataTable_contentOptInt: file format is error\n");
        return -1;
    }
    num = (unsigned char)(tmp[4]) << 24 | (unsigned char)(tmp[5]) << 16 | (unsigned char)(tmp[6]) << 8 | (unsigned char)(tmp[7]);            
    if(num  < 0)
        {
        printf("dataTable_getEleByUnique: num is error\n");
        return -1;
    }
    //--3 查找
    int seek;
    seek = 8;
    int node_len;
    int node_time;
    int i;
    char node_unique[UNIQUE_CODE_SIZE + 1];

    //printf("============================>B num = %d\n", num);
    for(i = 0; i < num; i++)
    {
        memset(tmp, 0 , 32 + UNIQUE_CODE_SIZE +1);
        memset(node_unique, 0, UNIQUE_CODE_SIZE + 1);
        fullPath_bufGetBlock(data_table, tmp, seek, 8);
        fullPath_bufGetBlock(data_table, node_unique, seek + 8, UNIQUE_CODE_SIZE);
        node_unique[UNIQUE_CODE_SIZE] = '\0';
        node_len = (unsigned char)(tmp[0]) << 24 | (unsigned char)(tmp[1]) << 16 | (unsigned char)(tmp[2]) << 8 | (unsigned char)(tmp[3]);
        node_time = (unsigned char)(tmp[4]) << 24 | (unsigned char)(tmp[5]) << 16 | (unsigned char)(tmp[6]) << 8 | (unsigned char)(tmp[7]); 

        //printf("===============>[%d] t=%d unique=%s\n", i, node_time, node_unique);
        if(node_time == t && strcmp(node_unique, unique) == 0)
        {
            *start = seek;
            *len = node_len;
            return 0;
        }           
        seek+= node_len;

    } 
    
    return -1;
}





