


#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#include "user.h"
#include "tcpServer.h"
#include "mylib.h"
#include "fileopt.h"
#include "buf.h"
#include "linkopt.h"

//========================================配置文件接口函数
/*
配置文件格式
例如：  USER_NAEM=long
其中USER_NAEM是参数名， long是参数值
参数名在每一行的起始处；参数值的末尾是换行符
*/
#define CFG_FILE_LINE_MAX_LEN 1024//限制配置文件每一行的最大长度
#define CFG_PATH "/mnt/hgfs/share/QT/cloudfile/wkspace/cfg"  //默认cfg配置文件路径
#define CFG_TMP_FILE "tmp"  //临时文件名字


//获取文件行数
int fileLineNum(char *file)
{
    FILE *fp = fopen(file, "r");
    if(fp == NULL)
    {
        printf("fileLineNum: file open is error\n");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    if(len == 0)
    {
        return -1;
    }
    
    int i;
    char ch;
    int cnt;    
    fseek(fp, 0, SEEK_SET);
    cnt= 1;
    for(i = 0; i < len; i ++)
    {
        ch = fgetc(fp);
        if(ch == '\n')
        {
            cnt++;
        }
    }
    fclose(fp);
    
    return cnt;
}
//获得文件第n行的字符串数据
int fileGetLineStr(char *file, char *line_data, int n)
{
    if(n < 1)
    {
        printf("fileGetLineStr: n is small than 1\n");
        return -1;
    }

    FILE *fp = fopen(file, "r");
    if(fp == NULL)
    {
        printf("fileGetLineStr: file open is error\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    if(len == 0)
    {
        return -1;
    }
    
    int i, j;
    char ch;
    int cnt;
    int mark;
    fseek(fp, 0, SEEK_SET);
    cnt= 0;
    j = 0;
    mark = 0;
    for(i = 0; i < len; i ++)
    {
        ch = fgetc(fp);
        if(n - 1 == cnt)
        {
            mark = 1;   
        }
        else if(n == cnt)
        {
            break;
        }
        if(ch == '\n')
        {                  
            cnt++;
        }
        
        if(mark == 1 && ch != '\n')
        {           
            line_data[j] = ch;
            j++;            
        }
    }
    line_data[j] = '\0';
    fclose(fp);
    
    return 0;
}

//替换文件某一行数据（需要用到临时文件）
int fileReplaceLine(char *file, char *line_data, int n)
{
    int data_len = strlen(line_data);
    
    if(line_data == NULL || data_len > CFG_FILE_LINE_MAX_LEN)
    {
        printf("fileReplaceLine: para error\n");
        return -1;
    }

    //创建临时文件
    char tmp[512] = {0};
    strcpy(tmp, CFG_PATH);
    strcat(tmp, "/");
    strcat(tmp, CFG_TMP_FILE);
    
    FILE *fp_tmp = fopen(tmp, "w+");
    if(fp_tmp == NULL)
    {
        printf("fileReplaceLine: file create faild, file=%s\n", tmp);
        return -1;
    }
    
    //打开配置文件
    FILE *fp = fopen(file, "r+");
    if(fp == NULL)
    {
        printf("fileReplaceLine: file open faild\n");
        return -1;
    }
    int len;
    fseek(fp, 0 ,SEEK_END);
    len = ftell(fp);
     fseek(fp, 0, SEEK_SET);
    int i, j ;
    char ch;
    int cnt;
    int mark;
    cnt = 0;
    mark = 0;
    for(i = 0; i < len; i++)
    {
        ch = fgetc(fp);
        if(n - 1 == cnt && mark == 0)
        {
            mark = 1;
            for(j = 0; j < data_len; j++)
            {             
                fputc(line_data[j], fp_tmp);
            }                        
        }
        else if(n == cnt && mark == 1)
        {
            mark = 2;
        }
        
        if(ch == '\n')
        {
            cnt++;
        }
        
        if(mark != 1 || ch == '\n')// 似乎句尾还有 \r 这个元素
        {   
            fputc(ch, fp_tmp);
        }                  
    }    
    //将临时文件复制到配置文件
    fclose(fp);   
    fp = fopen(file, "w");
    if(fp == NULL)
    {
        printf("fileReplaceLine: file open faild, file=%s\n", file);
        return -1;
    }
    fseek(fp_tmp, 0, SEEK_END);
    len = ftell(fp_tmp);
    fseek(fp_tmp, 0, SEEK_SET);
    for(i = 0; i < len; i++)
    {
        ch = fgetc(fp_tmp);
        //putchar(ch);
        fputc(ch, fp);
    }
    
    fclose(fp);
    fclose(fp_tmp);
    remove(tmp);
   
    return 0;
}

//文件末尾新增一行
int fileEndAddLine(char *file, char *line_data)
{
    FILE *fp = fopen(file, "a");
    if(fp == NULL)
    {
        printf("fileEndAddLine: file opent faild\n");
        return -1;
    }
    int len = strlen(line_data);
    fputc('\n', fp);
    fwrite(line_data, len, 1, fp);
    fclose(fp);
    
    return 0;
}


//找到配置文件中字符串第一次出现的行数
int cfgFindParaPos(char *file, char *str)
{
    int i;
    char block[CFG_FILE_LINE_MAX_LEN] = {0};
    int line_num = fileLineNum(file);
    //printf("====>line num is %d\n", line_num);
    for(i = 1; i <= line_num; i ++)
    {
        fileGetLineStr(file, block, i);
        //printf("[i=%d]str=%s\n", i, block);
        if(strncmp(block, str, strlen(str)) == 0)
        {
            return i;
        }
    }
    
    return -1;
}



//找到配置文件中字符串第n次出现的行数
int cfgFindParaPos_nTimes(char *file, char *str, int n)
{
    int i;
    char block[CFG_FILE_LINE_MAX_LEN] = {0};
    int line_num = fileLineNum(file);
    int cnt;
    cnt = 0;
    //printf("====>line num is %d\n", line_num);
    for(i = 1; i <= line_num; i ++)
    {
        fileGetLineStr(file, block, i);    
        //printf("[i=%d]str=%s\n", i, block);
        if(strncmp(block, str, strlen(str)) == 0)
        {
              cnt++;
              if(cnt == n)
              {
                 return i;
              }          
        }
    }
    
    return -1;
}
//判断文件中，行头出现某字符串的行数
int fileGetStartstrTimes(char *file, char *str )
{
    int i;
    char block[CFG_FILE_LINE_MAX_LEN] = {0};
    int line_num = fileLineNum(file);
    int cnt;
    cnt = 0;
    //printf("====>line num is %d\n", line_num);
    for(i = 1; i <= line_num; i ++)
    {
        fileGetLineStr(file, block, i);    
        //printf("[i=%d]str=%s\n", i, block);
        if(strncmp(block, str, strlen(str)) == 0)
        {
              cnt++;                     
        }
    }
    
    return cnt;

}



//写入参数（如果存在，就覆盖;如果不存在，则末尾添加）
int cfgWritePara(char *file, char *para, char *value)
{
    int n = cfgFindParaPos(file, para);   
    char tmp[CFG_FILE_LINE_MAX_LEN] = {0};
    sprintf(tmp, "%s=%s", para, value);
    if(n > 0)
    {
        return fileReplaceLine(file, tmp, n);
    }
    else
    {
        return fileEndAddLine(file, tmp);
    }  

    return -1;
}

//取出参数
int cfgGetPara(char *file, char *para, char *value)
{
    //printf("===================>C file=%s para=%s\n", file, para);
    int n = cfgFindParaPos(file, para);
    if(n <= 0)
    {    
        printf("cfgGetPara: para not found， para=%s\n", para);
        return -1;
    } 
    char block[CFG_FILE_LINE_MAX_LEN] = {0};
    if(0 != fileGetLineStr(file, block, n))
    {
        printf("cfgGetPara: get line data faild\n");
        return -1;
    }
    int len1 = strlen(block);
    int len2 = strlen(para);
    //printf("============================>D len1=%d len2=%d\n", len1, len2);
    int i, j;
    j = 0;
    for(i = 0; i < len1; i++)
    {
        if(i > len2)
        {
            value[j] = block[i];
            j++;
        }       
    }
    value[j] = '\0';
         

    return 0;
}

//测试
void cfgTest(void)
{
    
    //printf("=========>line=%d\n", cfgFindParaPos_nTimes("file.txt", "USER_NAME", 4));

}





//===============================================================用户账户链表
/*
用链表来描述所有用户对象
用配置文件来存储链表信息
配置文件： ./wkspace/user/user.cfg
*/
//typedef struct STRUCT_USER_LINKtmp{
//    char name[128];
//    char passwd[128];
//    int level; //级别
//    struct STRUCT_USER_LINKtmp *next;
//}STRUCT_USER_LINK;

//创建链表
STRUCT_USER_LINK *userLinkCreate(void)
{
    STRUCT_USER_LINK *head = (STRUCT_USER_LINK *)malloc(sizeof(STRUCT_USER_LINK)); 
    head->next = NULL;
    return head;
}
//增加节点：尾部
STRUCT_USER_LINK *userLinkAddNode(STRUCT_USER_LINK *head)
{
    if(head == NULL)
    {
        printf("userLinkAddNode: head is NULL\n");
        return NULL;
    }
    STRUCT_USER_LINK *node = (STRUCT_USER_LINK *)malloc(sizeof(STRUCT_USER_LINK));
    STRUCT_USER_LINK *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
    }
    prob->next = node;
    node->next = NULL;
    return node;
}
//增加节点：指定索引（注：索引为节点的索引，从0开始，不包含链表头）
STRUCT_USER_LINK *userLinkInsertNode(STRUCT_USER_LINK *head, int index)
{

    if(head == NULL)
    {
        printf("userLinkInsertNode: head is NULL\n");
        return NULL;
    }

    STRUCT_USER_LINK *node = (STRUCT_USER_LINK *)malloc(sizeof(STRUCT_USER_LINK));
    STRUCT_USER_LINK *prob = head->next;
    STRUCT_USER_LINK *last = head;
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        if(cnt == index)
        {
            last->next = node;
            node->next = prob;
            return node;;
        }
        cnt++;
        last = prob;
        prob = prob->next;
    }
    free(node);
    return NULL;
}
//删除节点：尾部的节点
int userLinkDelNode(STRUCT_USER_LINK *head)
{
    if(head == NULL)
    {
        printf("userLinkDelNode: head is NULL\n");
        return -1;
    }
    STRUCT_USER_LINK *prob = head;
    STRUCT_USER_LINK *last = head;
    while(prob->next != NULL)
    {   
        last = prob;
        prob = prob->next;
    }
    if(prob != head)
    {
        last->next = NULL;
        free(prob);
    }
    return 0;
}
//删除节点： 指定索引的节点
int userlinkDelNode_byIndex(STRUCT_USER_LINK *head, int index)
{
     if(head == NULL)
    {
        printf("userlinkDelNode_byIndex: head is NULL\n");
        return -1;
    }
    int cnt;
    STRUCT_USER_LINK *prob = head->next;
    STRUCT_USER_LINK *last = head;
    while(prob != NULL)
    {
        if(cnt == index)
        {
            last->next = prob->next;
            free(prob);
            return 0;
        }
        cnt++;
        last = prob;
        prob = prob->next;
    }
    return -1;
}
//删除节点：指定节点
int userLinkDelNode_byNode(STRUCT_USER_LINK *head, STRUCT_USER_LINK *node)
{
     if(head == NULL)
    {
        printf("userLinkAddNode: head is NULL\n");
        return -1;
    }
    int cnt;
    STRUCT_USER_LINK *prob = head->next;
    STRUCT_USER_LINK *last = head;
    while(prob != NULL)
    {
        if(prob == node)
        {
            last->next = prob->next;
            free(prob);
            return 0;
        }
        cnt++;
        last = prob;
        prob = prob->next;
    }
    return -1;
}
//清空列表节点列表
int userLinkClearNode(STRUCT_USER_LINK *head)
{
     if(head == NULL)
    {
        printf("userLinkClearNode: head is NULL\n");
        return -1;
    }
    STRUCT_USER_LINK *prob = head->next;
    STRUCT_USER_LINK *last = head;
    while(prob != NULL)
    {
        if(prob->next == NULL)
        {                        
            free(prob);
            last->next = NULL;
            prob = head->next;
            last = head;
            continue;
        }
        last = prob;
        prob = prob->next;
    }
    head->next = NULL;
    return 0;

}
//摧毁链表
int userLinkDestory(STRUCT_USER_LINK **head_p)
{
    STRUCT_USER_LINK *head = *head_p;
     if(head == NULL)
    {
        printf("userLinkDestory: head is NULL\n");
        return -1;
    }
    STRUCT_USER_LINK *prob = head->next;
    STRUCT_USER_LINK *last = head;
    while(prob != NULL)
    {
        if(prob->next == NULL)
        {                        
            free(prob);
            last->next = NULL;
            prob = head->next;
            last = head;
            continue;
        }
        last = prob;
        prob = prob->next;
    }
    free(head);
    *head_p = NULL;
    return 0;
}
//获取链表节点个数 
int userLinkGetNum(STRUCT_USER_LINK *head)
{
    if(head == NULL)
    {
        printf("userLinkGetNum: head is NULL\n");
        return -1;
    }
    STRUCT_USER_LINK *prob = head->next;
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        cnt++;
        prob = prob->next;   
    }
    return cnt;
}
//获取节点的索引号
int userLinkGetIndex_byNode(STRUCT_USER_LINK *head, STRUCT_USER_LINK *node)
{
    if(head == NULL)
    {
        printf("userLinkGetIndex_byNode: head is NULL\n");
        return -1;
    }
    STRUCT_USER_LINK *prob = head->next;
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
//通过索引号获取节点
STRUCT_USER_LINK *userLinkGetNode_byIndex(STRUCT_USER_LINK *head, int index)
{
    if(head == NULL)
    {
        printf("userLinkGetIndex_byNode: head is NULL\n");
        return NULL;
    }
    STRUCT_USER_LINK *prob = head->next;
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        if(cnt == index)
        {
            return prob;
        }
        cnt++;
        prob = prob->next;   
    }
    return NULL;

}

//-------------拓展功能-----------------//
//根据用户（user_name 成员）名找到第一个匹配的节点
STRUCT_USER_LINK *userLinkGetNode_byUserName(STRUCT_USER_LINK *head, char *user_name)
{
    if(head == NULL)
    {
        printf("userLinkGetNode_byUserName: head is NULL\n");
        return NULL;
    }
    STRUCT_USER_LINK *prob = head->next;
 
    while(prob != NULL)
    {
        if(strcmp(prob->name, user_name) == 0)
        {
            return prob;
        }
        prob = prob->next;   
    }
    return NULL;
}

//为每位用户创建对应目录
int userLink_createFloderForUser(STRUCT_USER_LINK *head, char *path, char *para_Floder)
{
	int ret;
	if(head == NULL)
	{
		printf("userLinkGetNode_byUserName: head is NULL\n");
		return -1;
	}
	STRUCT_USER_LINK *prob = head->next;
 	ret = 0;
 	char tmp[512];
 	char para_table[512];
	char data_table[512];
	OptLink *opt_head = linkOptCreate();
	//userLinkShow(head);
	while(prob != NULL)
	{
	    if(prob->name == NULL || (prob->name != NULL && strlen(prob->name) <= 0))
	    {   
	        //printf("userLink_createFloderForUser: user cfg file conver to link has some error\n");
            continue;
	    }
		ret += floderAdd(path, prob->name);
		//printf("==============================>B prob->name=%s\n", prob->name);
		sprintf(tmp, "%s/%s/%s", path, prob->name, para_Floder);
		ret += floderCreate(tmp);

		//创建本地 para table 和 data table		
		sprintf(para_table, "%s/%s", tmp, USER_LOCAL_PARA_TABLE);
		sprintf(data_table, "%s/%s", tmp, USER_LOCAL_DATA_TABLE);
		//printf("===========================>A para_tabler=%s\n", para_tabler);
		if(fullPath_bufExit(para_table) == 0)
		{
            //printf("userLink_createFloderForUser: file has exist\n");
		}
		else
		{
            paraTable_create(opt_head, para_table);
		}

		if(fullPath_bufExit(data_table) == 0)
		{
            //printf("userLink_createFloderForUser: file has exist\n");
		}
		else
		{
            paraTable_create(opt_head, data_table);
		}
		
		
		prob = prob->next;	 
	}
	return ret;	
}


//显示
int userLinkShow(STRUCT_USER_LINK *head)
{
     if(head == NULL)
    {
        printf("userLinkClearNode: head is NULL\n");
        return -1;
    }
    STRUCT_USER_LINK *prob = head->next;
    printf("==>userLink node num: %d\n", userLinkGetNum(head));
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        printf("[%d] name=%s passwd=%s level=%d\n",  cnt, prob->name, prob->passwd, prob->level);
        cnt++;
        prob = prob->next;
    }

}
//测试
void userLinkTest(void)
{
    STRUCT_USER_LINK *head = userLinkCreate();
    STRUCT_USER_LINK *node1 = userLinkAddNode(head);
    STRUCT_USER_LINK *node2 = userLinkAddNode(head);
    STRUCT_USER_LINK *node3 = userLinkAddNode(head);
    STRUCT_USER_LINK *node4 = userLinkAddNode(head);
    if(node1 == NULL || node2 == NULL || node3 == NULL)
    {
        printf("add node error\n");
        return ;
    }
    strcpy(node1->name, "node1");
    strcpy(node1->passwd, "psd1");
    node1->level = 1;
    strcpy(node2->name, "node2");
    strcpy(node2->passwd, "psd2");
    node2->level = 2;
    strcpy(node3->name, "node3");
    strcpy(node3->passwd, "psd3");
    node3->level = 3;
    strcpy(node4->name, "node4");
    strcpy(node4->passwd, "psd4");
    node4->level = 4;
    printf("=======================测试1\n");
    userLinkShow(head);

    userLinkDelNode(head) ; 
    printf("=======================测试2\n");
    userLinkShow(head);
    
    STRUCT_USER_LINK *node5 = userLinkInsertNode(head, 2) ; 
    strcpy(node5->name, "node5");
    strcpy(node5->passwd, "psd5");
    node5->level = 5;
    printf("=======================测试3\n");
    userLinkShow(head);

    printf("=======================测试4 \nndoe2的节点索引：%d\n", userLinkGetIndex_byNode( head, node2));

    printf("=======================测试5 \n索引为2的节点名：%s\n", userLinkGetNode_byIndex( head, 2));
    
    userLinkDestory(&head);
    printf("=======================测试6\n");
    userLinkShow(head);

   
}


//===============================================================不可逆数据转换（用于加密密码，保证密码从不显示明码）
/*
-过程：[n字节数据]  ====加密===>   [8字节目标码]  =====规范====>  [8字节规范目标码]
-方法：
（1）确定目标码的原始的 8个字节，例如 {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}；
（2）如果n不是8的倍数，那么补齐为 n1，倍数为 b，b是大于等于1的整数；
（3）目标码的原始8个字节 分b次，与数据的字节进行位操作，位操作包括 与、或、异或 这3个，轮流进行；
（4）得到的8字节目标码，每个字节进行规范，例如 取值的池子 {'0', '1', '2', '3', '4'}，通过 %5 取余操作，得到
最终的码将会在池子范围之内。
*/
//注：这里的 data 为二进制数据
#define ENCRYPT_OPT(b1, b2, a)  \
do{\
    if(a == 0)\
    {\
        b1 &= b2;\
    }\
    else if(a == 1)\
    {\
        b1 |= b2;\
    }\
    else if(a == 2)\
    {\
        b1 ^= b2;\
    }\
}while(0);

char gloable_user_char_pool[] = {
'0','1','2','3','4','5','6','7','8','9',
'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y',
'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y'
};

int dataEncrypt_byte8(void *data, int len, char *out)
{
    //printf("=================>A \n");
    //参数检测
    if(len <= 0)
    {
        printf("dataEncrypt: data len is too short\n");
        return -1;
    }

    if(data == NULL)
    {
        printf("dataEncrypt: data is NULL\n");
        return -1;
    }

    //填充
    unsigned char code[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    int i, j, cnt;
    if(len % 8 != 0)
    {
        
        for(i = 0; i < 8 - (len % 8); i++)
        {
             ((char *)data)[i + len] = '0';//用'0'填充
        }
       ((char *)data)[i + len] = '\0';
       len += 8 - (len % 8);
    }
    //printf("=================>B \n");

    cnt = 0;
    
    //位运算
    for(i = 0; i < len / 8; i++)
    {
        for(j = 0; j < 8; j ++)
        {
            ENCRYPT_OPT(code[j], ((char *)data)[i * 8 + j], cnt % 3);
            cnt++;
        }
        
    }

    int size = strlen(gloable_user_char_pool);
    //printf("================>size =%d\n",size);
    //规范化    
    for(i = 0; i < 8; i++)
    {
        out[i] = gloable_user_char_pool[code[i]  % size]; 
    }
    out[i] = '\0';
    
    
    return 0;
}


//===============================================================用户账户业务流（提供本地接口）
/*
业务流包括注册、登录、权限验证、用户目录的
可供扩展，以完善功能

-主流：
（1）注册
（2）登录
（3）home目录
（4）配置文件读取、存入
-扩展
（1）密码安全性
（2）数据安全、数据备份

-关于密码：
（1）设置为不可查看模式，通过某种单向算法转换
（2）例如密码 123456 ，转换为 "xxxxxxx" ，存储也是 "xxxxxxx"，配置文件或明文不会显示真正密码，一旦丢失，
     几乎无法挽回，但可以通过root模式重新设置密码，但是处于数据安全性考虑，可以销毁该用户敏感数据。
（3）可以参考主流解决方法，如邮箱验证
*/




//链表写入配置文件
//注：参数 USER_NAME 一般加上索引，例如 USER_NAME_0
int accountSaveToCfg(char *cfg_name, STRUCT_USER_LINK *head)
{
    if(head == NULL)
    {
        printf("accountSaveToCfg: head is NULL\n");
        return -1;
    }

    //清空配置文件，不存在则创建
    //printf("============>file=%s\n", cfg_name);
    FILE *fp = fopen(cfg_name, "w");
    if(fp == NULL)
    {
        printf("accountSaveToCfg: cfg file has some problems.\n");
        return -1;
    }
    fclose(fp);
    
    
    STRUCT_USER_LINK *prob = head->next;
    char tmp1[USER_CFG_PARA_max_size] = {0};
    char tmp2[32] = {0};
    int cnt;
    cnt = 0;
    while(prob != NULL)
    {
        sprintf(tmp1, "%s_%d",  USER_CFG_PARA_USER_NAME_str, cnt);
        cfgWritePara(cfg_name, tmp1, prob->name);
        sprintf(tmp1, "%s_%d",  USER_CFG_PARA_PASSWORD_str, cnt);
        cfgWritePara(cfg_name, tmp1, prob->passwd);
        sprintf(tmp1, "%s_%d",  USER_CFG_PARA_LEVEL_str, cnt);
        sprintf(tmp2, "%d",  prob->level);
        cfgWritePara(cfg_name, tmp1, tmp2);
        cnt++;
        prob = prob->next;
    }

    return 0;
}
//从配置文件中读出到链表
int accountCfgToLink(char * cfg_name, STRUCT_USER_LINK *head)
{
     if(head == NULL)
    {
        printf("accountCfgToLink: head is NULL\n");
        return -1;
    }
    //清空链表
    //userLinkClearNode(head);
    

    //判断文件是否存在
    //printf("============>file=%s\n", cfg_name);
    FILE *fp = fopen(cfg_name, "r+");
    if(fp == NULL)
    {
        printf("accountCfgToLink: cfg file has some problems. cfg_name=%s\n", cfg_name);
        return -1;
    }
    fclose(fp);

    //判断节点的个数，可以判断 USER_NAME 的个数
    int num = fileGetStartstrTimes(cfg_name, USER_CFG_PARA_USER_NAME_str);
    

    
    char tmp1[USER_CFG_PARA_max_size] = {0};
    char tmp2[USER_CFG_PARA_max_size] = {0};
    char tmp3[32] = {0};
    int i;
    int ret;
    //printf("==================>A num=%d\n", num);
    //STRUCT_USER_LINK *headtest = userLinkCreate(); 
    for(i = 0; i < num; i++)
    {  
        ret = 0;
        STRUCT_USER_LINK *prob = userLinkAddNode(head);
        sprintf(tmp1, "%s_%d",  USER_CFG_PARA_USER_NAME_str, i);        
        ret += cfgGetPara(cfg_name, tmp1, prob->name);
        sprintf(tmp1, "%s_%d",  USER_CFG_PARA_PASSWORD_str, i);
        ret += cfgGetPara(cfg_name, tmp1, prob->passwd);
        sprintf(tmp1, "%s_%d",  USER_CFG_PARA_LEVEL_str, i);        
        ret += cfgGetPara(cfg_name, tmp1, tmp2);
        prob->level = atoi(tmp2);
        if(prob->level == 0 && tmp2[0] != '0')//字符串转数字失败
        {
            printf("accountCfgToLink: level get faild\n");
            prob->level = USER_CFG_PARA_LEVEL_defule_value;
        }    
        if(ret != 0)//取出参数失败
        {
            printf("accountCfgToLink: ret =%d\n", ret);
            userLinkDelNode_byNode(head, prob);
            continue;
        }
        //printf("=========================>B name=%s passwd=%s level=%d\n", prob->name, prob->passwd, prob->level);
    }

    return 0;
}
//注册
//返回值 0 代表成功  -1 表示用户名问题  -2 代表密码问题  -3 代表其他问体
int accountRegister(STRUCT_USER_LINK *head, char *cfg_name, char *user_name, char *passwd, int level)
{
    //参数检测
    if(head == NULL || user_name == NULL || passwd == NULL || level < 0)
    {
        printf("accountRegister: para error\n");
        return -1;
    }
    //用户名规范检测（可用字符 0-9 a-z A-Z ；不限短，限长 32 字符；不可与原先的用户重名）
    int len = strlen(user_name);
    int i;
    if(len <= 0 || len > ACCOUNT_USER_NAME_max_size)
    {
        printf("accountRegister: user name size is out of range\n");
        return -1;
    }
    for(i = 0; i < len; i++)
    {
        if((user_name[i] >= '0' && user_name[i] <= '9') || 
            (user_name[i] >= 'a' && user_name[i] <= 'z') ||
            (user_name[i] >= 'A' && user_name[i] <= 'Z') )
        {
            ;
        }
        else
        {
            printf("accountRegister: user name formt is error, len=%d name=%s\n", len, user_name);
            return -1;
        }

    }
    if(NULL != userLinkGetNode_byUserName(head, user_name))
    {
        printf("accountRegister: user name has exist\n");
        return -1;
    }
    
    
    //密码规范 （可用字符 0-9 a-z A-Z ；不限短，限长 32 字符；存储时需要加密，加密后的密码与原先密码出了长度固定，其他规范相同，
    //          这里默认密码已经被加密）
    len = strlen(passwd);
     if(len <= 0 || len > ACCOUNT_USER_NAME_max_size)
    {
        printf("accountRegister: user password size is out of range\n");
        return -2;
    }
    for(i = 0; i < len; i++)
    {
        if((passwd[i] >= '0' && passwd[i] <= '9') || 
            (passwd[i] >= 'a' && passwd[i] <= 'z') ||
            (passwd[i] >= 'A' && passwd[i] <= 'Z') )
        {
            ;
        }
        else
        {
            printf("accountRegister: user password formt is error， len =%d passwd=%s\n",len, passwd);
            return -2;
        }
    } 

    //用户级别规范

    
    //添加节点并存入配置文件
    STRUCT_USER_LINK *node = userLinkAddNode(head);
    strcpy(node->name, user_name);
    strcpy(node->passwd, passwd);
    node->level = level;
    if(0 != accountSaveToCfg(cfg_name, head))
    {
        printf("accountRegister: save faild\n");
        return -3;
    }

    return 0;
}
//注销
int accountDestory(STRUCT_USER_LINK *head, char *cfg_name, char *user_name)
{
    STRUCT_USER_LINK *prob = userLinkGetNode_byUserName(head, user_name);
    if(prob != NULL)
    {
        if(0 != userLinkDelNode_byNode(head, prob))
        {
            printf("accountDestory: ndoe delete faild\n");
            return -1;
        }
        if(0 != accountSaveToCfg(cfg_name, head))
        {
            printf("accountDestory: save to config file faild\n");
            return -1;
        }
		//强制删除目录：调用slinux shell命令
		char tmp[512] = {0};
		sprintf(tmp, "rm -rf %s/%s/%s", TCP_SERVER_WORK_PATH, USER_WORK_FLODER, user_name);
		system(tmp);
		
        
        return 0;
    }
    else
    {
         printf("accountDestory: account is not foud\n");
         return 0;
    }
    return 0;

}
//登录验证（0代表通过、-1 表示无此用户名  -2 表示密码错误）
int accountLogin(STRUCT_USER_LINK *head, char *user_name, char *passwd)
{
    STRUCT_USER_LINK *prob = userLinkGetNode_byUserName(head, user_name);
    if(prob == NULL)
    {
        //printf("accountLogin: faild because of no such account\n");
        return -1;
    }
    if(strcmp(prob->passwd, passwd) != 0)
    {
        //printf("accountLogin: faild because of passwd error\n");
        return -2;
    }

    return 0;
}
//忘记密码处理（邮箱）
int accountForgetPassword(char *user_name)
{

    ;

}
//对外接口：账户密码验证
int accountInt_passwdVerify(char *user_name, char *passwd)
{
    return accountLogin(gloable_user_link_head, user_name, passwd);
}
//对外接口：注册（普通用户, 其他类型用户以后拓展，现在没有考虑）
int accountInt_registe(char *user_name, char *passwd)
{
    char name[512];
    USER_CFG_FILE_FULL_NAME(name);

	//创建目录
    char user_floder[512];
    char user_table_floder[512];
    sprintf(user_floder, "%s/%s/%s", TCP_SERVER_WORK_PATH, USER_WORK_FLODER, user_name);
    sprintf(user_table_floder, "%s/%s/%s/%s", TCP_SERVER_WORK_PATH, USER_WORK_FLODER, user_name, USER_WORK_TABLE_FLODER);
	floderCreate(user_floder);
	floderCreate(user_table_floder);
	//创建本地 para table 和 data table
	char para_tabler[512];
	char data_tabler[512];
	OptLink *head = linkOptCreate();
	sprintf(para_tabler, "%s/%s", user_table_floder, USER_LOCAL_PARA_TABLE);
	sprintf(data_tabler, "%s/%s", user_table_floder, USER_LOCAL_DATA_TABLE);
	//首先判断文件是否存在
	//printf("============================>A para_tabler=%s", para_tabler);
	if(fullPath_bufExit(para_tabler) == -1)
	{
		paraTable_create(head, para_tabler);
	}
	if(fullPath_bufExit(data_tabler) == -1)
	{
		paraTable_create(head, data_tabler);
	}

    return accountRegister(gloable_user_link_head, name, user_name, passwd, USER_CFG_PARA_LEVEL_defule_value);
}
//对外接口：销毁
int accountInt_destory(char *user_name)
{
    char name[512];
    USER_CFG_FILE_FULL_NAME(name);

	
	//删除目录
	char user_floder[512];
    char user_table_floder[512];
    sprintf(user_floder, "%s/%s/%s", TCP_SERVER_WORK_PATH, USER_WORK_FLODER, user_name);
    sprintf(user_table_floder, "%s/%s/%s/%s", TCP_SERVER_WORK_PATH, USER_WORK_FLODER, user_name, USER_WORK_TABLE_FLODER);
	floderDelete(user_floder);
	floderDelete(user_table_floder);
    
    return accountDestory(gloable_user_link_head, name, user_name);    
}
//对外接口：用户路径，例如 guest 用户的路径是 xx/wkspace/user/guest
int accountInt_getPath(char *user_name, char *path)
{
    sprintf(path, "%s/%s/%s", TCP_SERVER_WORK_PATH, USER_WORK_FLODER, user_name);
    return 0;    
}

//开始运行该服务
int accountServerStart(void)
{
    char name[512];
    USER_CFG_FILE_FULL_NAME(name);
    char floder[512];
    sprintf(floder, "%s/%s", TCP_SERVER_WORK_PATH, USER_WORK_FLODER);
    if(gloable_account_server_mark == 0)//服务未开始
    {
       gloable_account_server_mark = 1; 
       if(0 != accountCfgToLink(name, gloable_user_link_head))
       {
            printf("accountServerStart: account server start faild\n");
            return -1;
            gloable_account_server_mark = 0;
       }
       //userLinkShow(gloable_user_link_head);
       //为每个用户创建对应目录 和 para table、data table
       //printf("===========================>Z \n");
       userLink_createFloderForUser(gloable_user_link_head, floder, USER_WORK_TABLE_FLODER);
       
    }
    return 0;
}
//重新开始此服务
int accountServerRestart(void)
{
    char name[512];
    USER_CFG_FILE_FULL_NAME(name);
    if(gloable_account_server_mark == 1 && gloable_user_link_head != NULL)
    {
        userLinkDestory(&gloable_user_link_head);
    }
   
   gloable_account_server_mark = 1; 
   gloable_user_link_head = userLinkCreate();
   if(0 != accountCfgToLink(name, gloable_user_link_head))
   {
        printf("accountServerStart: account server start faild\n");
        return -1;
        gloable_account_server_mark = 0;
   }   
    return 0;
} 
//关闭此服务
int accountServerStop(void)
{
     char name[512];
    USER_CFG_FILE_FULL_NAME(name);
    gloable_account_server_mark = 0;
    if(0 != accountSaveToCfg(name, gloable_user_link_head))
    {
        printf("accountServerStop: save to config error\n");
        gloable_account_server_mark = 1;
        return -1;
    }
    userLinkDestory(&gloable_user_link_head);    
    return 0;
}
//初始化用户服务
int accountInit(void)
{
    int ret = 0;
	//创建目录
	char floder[512];
	if(floderExist(TCP_SERVER_WORK_PATH) != 0)
	{
        if(floderCreate(TCP_SERVER_WORK_PATH))
        {
            printf("accountInit: attension,floder %s is not exist\n", TCP_SERVER_WORK_PATH);
        }
	}
    sprintf(floder, "%s/%s", TCP_SERVER_WORK_PATH, USER_WORK_FLODER);
	floderCreate(floder);
	 sprintf(floder, "%s/%s", TCP_SERVER_WORK_PATH, USER_CFG_FLODER);
  	floderCreate(floder);
     
	//运行服务
	gloable_user_link_head = userLinkCreate();
	ret = accountServerStart();
    //假如不存在默认账户，则添加默认账户( guest   123(明码))
    if(userLinkGetNode_byUserName(gloable_user_link_head, "guest") != NULL)
    {
		return ret;
    }
    
    char tmp1[32] = "123";
    char tmp2[32] = {0};
    ret += dataEncrypt_byte8(tmp1, 3, tmp2);   //注意： dataEncrypt_byte8 第一个参数不能是 "xxxx" （const char *） 类型，暂时没找到原因
    ret += accountInt_registe("guest", tmp2);
    
    return ret;   
}


//测试
void accountTest(void)
{
     char name[512];
    USER_CFG_FILE_FULL_NAME(name);
    
    gloable_user_link_head = userLinkCreate();
    STRUCT_USER_LINK *node1 = userLinkAddNode(gloable_user_link_head);
    STRUCT_USER_LINK *node2 = userLinkAddNode(gloable_user_link_head);
    STRUCT_USER_LINK *node3 = userLinkAddNode(gloable_user_link_head);
    STRUCT_USER_LINK *node4 = userLinkAddNode(gloable_user_link_head);
    if(node1 == NULL || node2 == NULL || node3 == NULL)
    {
        printf("add node error\n");
        return ;
    }
    strcpy(node1->name, "node1");
    strcpy(node1->passwd, "psd1");
    node1->level = 1;
    strcpy(node2->name, "node2");
    strcpy(node2->passwd, "psd2");
    node2->level = 2;
    strcpy(node3->name, "node3");
    strcpy(node3->passwd, "psd3");
    node3->level = 3;
    strcpy(node4->name, "node4");
    strcpy(node4->passwd, "psd4");
    node4->level = 4;
   
    accountSaveToCfg(name, gloable_user_link_head);   

    STRUCT_USER_LINK * head = userLinkCreate();
    accountCfgToLink(name, head);
    userLinkShow(head);

    
}





//===============================================================测试
void user_c_test(void)
{   
//    userLinkTest();
//    accountTest();
//    char tmp1[] = "dfasdf fsafsdf123 dfasd";
//    char tmp2[32] = {0};
//    dataEncrypt_byte8(tmp1, strlen(tmp1), tmp2);
//    printf("============>tmp2=%s\n", tmp2);
   
}


/*int main(void)
{
    user_c_test();
    return 0;

}*/






