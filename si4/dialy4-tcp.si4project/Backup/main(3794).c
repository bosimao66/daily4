
#include <stdio.h>
#include <unistd.h>

#include "tcpServer.h"
#include <unistd.h>
//测试
#include "linkopt.h"
#include "fileopt.h"
#include "user.h"

//#########################################//根据需求修改
//工作路径
//#define MY_TCP_SERVER_WK_PATH  "/mnt/hgfs/share/QT/dialy4/wkspace"

//监听的IP及其端口
#define PORT 10086
#define IP  "192.168.137.20" 
//########################################//


int main(void)
{
    printf("=============TCP server for cloud files=============\n");
    //初始化
    bufInit();
    accountInit();
    linkOptInit();
    
    //fileOptTest();
    //开启服务
	tcpServerStart(IP, PORT);     //tcp服务器  
    return 0;
}














