

#ifndef _USER_H_
#define _USER_H_



typedef struct STRUCT_USER_LINKtmp{
    char name[128];
    char passwd[128];
    int level; //级别
    struct STRUCT_USER_LINKtmp *next;
}STRUCT_USER_LINK;




//===============================================================用户账户业务流（提供本地接口）

#define USER_CFG_FLODER "account"   //父目录在 wkpath， 即工作路径  TCP_SERVER_WORK_PATH
#define USER_CFG_FILE "user.cfg"
#define USER_WORK_FLODER "user"	//父目录在 wkpath， 即工作路径  TCP_SERVER_WORK_PATH
#define USER_WORK_TABLE_FLODER ".table"  //放置参数表的目录
#define USER_CFG_FILE_FULL_NAME(full)	do{\
	strcpy(full, TCP_SERVER_WORK_PATH);\
	strcat(full, "/");\
	strcat(full, USER_CFG_FLODER);\
	strcat(full, "/");\
	strcat(full, USER_CFG_FILE);\
}while(0);

STRUCT_USER_LINK *gloable_user_link_head;
static int  gloable_account_server_mark = 0;

#define USER_CFG_PARA_USER_NAME_str "USER_NAME"
#define USER_CFG_PARA_PASSWORD_str "PASSWORD"
#define USER_CFG_PARA_LEVEL_str "LEVEL"
#define USER_CFG_PARA_LEVEL_defule_value 0  //默认为普通用户

#define USER_CFG_PARA_max_size 256    //参数最大长度
#define ACCOUNT_USER_NAME_max_size 32   //用户名最大长度
#define ACCOUNT_PASSWORD_max_size 32    //用户密码最大长度
//===============================================================





int accountSaveToCfg(char *cfg_name, STRUCT_USER_LINK *head);
int accountCfgToLink(char * cfg_name, STRUCT_USER_LINK *head);
int accountRegister(STRUCT_USER_LINK *head, char *cfg_name, char *user_name, char *passwd, int level);
int accountDestory(STRUCT_USER_LINK *head, char *cfg_name, char *user_name);
int accountLogin(STRUCT_USER_LINK *head, char *user_name, char *passwd);
int accountServerStart(void);
int accountServerRestart(void);
int accountServerStop(void);
//对外接口
int accountInt_passwdVerify(char *user_name, char *passwd);
int accountInt_registe(char *user_name, char *passwd);
int accountInt_destory(char *user_name);
int accountInit(void);
int accountInt_getPath(char *user_name, char *path);


#endif
