#ifndef _LINKCL_H_
#define _LINKCL_H_


typedef struct CLIENT_INFOtmp{
    unsigned int ipv4;           //用户ip
    int dialog_mark;             //会话状态
    int num;                     //会话编号
    int fd;                      //文件描述符
    unsigned long th;            //线程号
    time_t t;                    //时间标记
    unsigned char  del_mark;      //需要被销毁的标记
    char timeout_en;             //超时检测使能   0-非使能  1-使能
    int reply_code;				 //给客户端回复的错误码，一般0代表成功，其他只代表失败
    struct CLIENT_INFOtmp *next;
}CLIENT_INFO;


extern CLIENT_INFO *global_link_cl_info_head_p;


CLIENT_INFO *linkClCreate(void);
CLIENT_INFO *linkClAddNode(CLIENT_INFO *head);
CLIENT_INFO *linkClInsertNode(CLIENT_INFO *head, int index);
void linkClDelLastNode(CLIENT_INFO *head);
void linkClDelNode(CLIENT_INFO *head, CLIENT_INFO *node);
void linkClDelNode_byIndex(CLIENT_INFO *head, int index);
int linkClGetNum(CLIENT_INFO *head);
int linkClGetIndex(CLIENT_INFO *head, CLIENT_INFO *node);
CLIENT_INFO *linkClGetNode_byIndex(CLIENT_INFO *head, int index);
CLIENT_INFO *linkClFind_byIpv4(CLIENT_INFO *head, int ipv4);
void  linkClDestory(CLIENT_INFO *head);
void linkClTest_show(CLIENT_INFO *head);
void linkClTest(void);




#endif

