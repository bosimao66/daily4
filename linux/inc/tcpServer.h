
#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_


#define MAX_CLIENT_NUM 15           //最大的用户数，同时作为CLIENT_INFO 里节点的个数的限制
#define CLIENT_TIMEOUT_DEL_MARK 0x88            //节点超时销毁标记
typedef enum enum_dialogmark_tmp{
    dialogmark_notStart = 0,
    dialogmark_start
}enum_dialogmark;
#define TRAN_DATA_MAX_SIZE  (1024*1024) //会话中一次传输数据的最大字节 : 1M byte，如果要上传大文件，则可以
                                        //分多次传输，逻辑又更复杂了一层
#define DEFAULT_BUF_NAME_MAX_LEN 64  //注意字符串越界的话，可能会造成致命错误



#define DATA_HEAD_PARA_NUM 4 //参数个数
#define DATA_HEAD_PARA_STR_MAX_LEN 50    //字符串的最大长度，注意不要超过范围
#define quint32 unsigned int
#define quint16 unsigned short
#define quint8 unsigned char
//前导码和结束码
#define MY_PRE_CODE_0 0x66
#define MY_PRE_CODE_1 0x77
#define MY_PRE_CODE_2 0x88
#define MY_PRE_CODE_3 0x99
//#define MY_END_CODE_0 0x99
//#define MY_END_CODE_1 0x88
//#define MY_END_CODE_2 0x77
//#define MY_END_CODE_3 0x66


//模式
typedef enum enum_datamodetmp{
    datamode_request = 1,
    datamode_reply,
}enum_datamode;

//操作码
typedef enum enum_optcodetmp{
    optcode_nothing = 1,
    optcode_sync,
    optcode_hellomsg,
    optcode_newfile_cover,
    optcode_newfile_nocover,    //请求上载文件，不覆盖
    optcode_download_file,  //请求下载文件
    optcode_syncToService,  //请求推送到服务器进行同步
    optcode_syncFromService, //请求从服务器拉取来进行同步
    optcode_syncGetParaTable,//请求云端的本地参数表
    optcode_login,  //登录
    optcode_register,    //注册
    optcode_accountDestory,  //销毁账户
    opcode_forgetPasswd     //忘记密码
}enum_optcode;
//头部数据结构体
typedef struct STRUCT_DATA_HEAD_tmp{
    quint8 mode;
    quint16 opt;
	quint32 data_len;
	char user_name[33];
	char passwd[33];
    int paraInt[DATA_HEAD_PARA_NUM];//其他的整形参数
    char paraStr[DATA_HEAD_PARA_NUM][DATA_HEAD_PARA_STR_MAX_LEN];//字符串参数
}STRUCT_DATA_HEAD;



#include "main.h"
//工作目录
#define TCP_SERVER_WORK_PATH MY_TCP_SERVER_WK_PATH
//#define TCP_SERVER_WORK_PATH  "/mnt/hgfs/share/QT/dialy4/wkspace"

//缓存文件夹
#define TCP_SERVER_BUF_FLODER "buf"
#define TCP_SERVER_FILE_FLODER "file"



void *acceptUserConnect(void *arg);
void *timeoutDetection(void *arg);
void *newClientPro(void *arg);
int tcpServerStart(char *ip, int port);

#endif


