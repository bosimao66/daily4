

/*
描述：
（1）并发处理客户端请求
指的时可以同时处理不同会话的请求内容.
注意：
在一个会话之内，不能同时处理请求，例如在同一个会话之内，请求下载文件
a.txt 后又下载文件 b.txt ,那么只有在下载完a.txt后才会开始下载b.txt。
当然可以实现同时处理，但是要对流程非常清晰，考虑缓存文件命名区别，还有各种改进的方面，在加上测试，
非常耗时，所以现在不打算改了。

*/





//建立TCP服务器
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sys/types.h"//数据类型定义
#include "sys/socket.h" //提供socket函数及数据结构
#include "netinet/in.h" //定义数据结构sockaddr_in
#include "arpa/inet.h" //供IP地址转换函数
#include "unistd.h"
#include <pthread.h>
#include <time.h>

//自定义头文件
#include "tcpServer.h"
#include "linkCl.h"
#include "buf.h"
#include "request.h"


#define RECV_BLOCK_SIZE (1024)




//接受用户的连接
void *acceptUserConnect(void *arg)
{
    //printf("acceptUserConnect is comming\n");
    int cnt;
    int sockfd = *((int *)arg);
    struct sockaddr_in addr;
	int len = sizeof(struct sockaddr_in);
	int newid = -1;
    CLIENT_INFO *cf;    
    time_t t_tmp;
    int num;
    char ip_str[20] = {0};
    
    cnt = 0;
    while(1)
     {
        newid = accept(sockfd, (struct sockaddr *)&addr, &len);
        if(newid < 0)
        {   
            sleep(2);
            close(newid);
            continue;
        }    
        //有新的用户会话，存入链表
        num = linkClGetNum(global_link_cl_info_head_p) ;//范围检测
        if(num < 0 || num > MAX_CLIENT_NUM)
        {
            printf("acceptUserConnect: client num is out of range !\n");
            continue;
        }      
        cnt++;        
        cf = linkClAddNode(global_link_cl_info_head_p); 
        if(cf == NULL)
        {
            printf("acceptUserConnect: add node faild\n");
            close(newid);
            continue;
        }
        cf->ipv4 = addr.sin_addr.s_addr;        
        cf->num = cnt;
        cf->fd = newid;
        cf->del_mark = 0;  //超时销毁的标记， 0x88 CLIENT_TIMEOUT_DEL_MARK 代表销毁
        cf->dialog_mark = dialogmark_notStart;      
        cf->t = time(&t_tmp);//如果失败返回-1，如何处理这种情况？？
        
        if(cf->t  == -1)
        {
            printf("acceptUserConnect: Get time faild\n");
             close(newid);
            linkClDelNode(global_link_cl_info_head_p, cf); 
            continue;
        }
        //注：ip 解析，可能机器的大小端有关
        sprintf(ip_str,"%d.%d.%d.%d",  ((unsigned char *)(&cf->ipv4))[0], ((unsigned char *)(&cf->ipv4))[1],
              						((unsigned char *)(&cf->ipv4))[2],((unsigned char *)(&cf->ipv4))[3]);
        //printf("new dialog is commig...\n");
        printf("===========dialog %d ip:%s fd=%d  start==========\n", cnt, ip_str, cf->fd);
              
    }
}


void *timeoutDetection(void *arg)
{
     //printf("wdThreadTimeout is OK\n");
    time_t t_tmp;
    time_t t ;
    int time_out = *((int *)arg);
    CLIENT_INFO *prob;
    int ret;
    char ip_str[20] = {0};
    while(1)
    {
       // printf("超时检测...t=%d  timeout=%d\n", t, time_out);       
        //fflush(stdout);
        t = time(&t_tmp);
         if(t == -1)
         {
            printf("wdThreadTimeout get time faild\n");
            sleep(1);
            continue ;
         }
        //遍历客户信息链表，判断是否超时；注意链表头从全局变量中获取
         prob = global_link_cl_info_head_p->next;
         while(prob != NULL)
         {
           // printf("prob time %d\n", prob->t);
            if(t > (prob->t + time_out) && prob->timeout_en == 1)
             {
                //销毁线程，要在父程序中进行？？
                prob->del_mark = CLIENT_TIMEOUT_DEL_MARK;              
            }
            prob = prob->next;           
         }
         sleep(2);    //每隔2秒进行检测操作
    }    
}

//利用多线程 并发处理客户端需求
//用户会话，超时检测
//在实际测试中，似乎一次会话，accept的次数大于 39 时候，会出现错误，所以限定最大次数
#define CONVERSATION_MAX_NUM 20
void *newClientPro(void *arg)
{
    int index = *((int *)arg);   
    unsigned int recvLen = -1;
    unsigned char buf[20] = {0};//使用buf时，应注意是否越界
    CLIENT_INFO *prob;
    int ret;
	int cnt = 0, cnt2 = 0;
    int i, j;
    int rem;
	int break_mark;
	time_t t_tmp;
	time_t t_tmp2;
	char process_bar[20];
	unsigned int current_len;
	int back_len;
	int lost_len;
    CLIENT_INFO *node = linkClGetNode_byIndex(global_link_cl_info_head_p, index);
    if(node == NULL)
    {
        printf("get link node error: index = %d\n", index);
        ret = -1;
        return ;
    }
    while(1)
    {       
        node->timeout_en = 1;//使能超时检测
        //printf("[fd=%d][%d]等待接收数据...\n", node->fd, cnt2);
       
        memset(buf, 0 , sizeof(buf));
        recvLen = recv(node->fd, (void *)buf, 8, 0);//阻塞接收 前导码 + 数据长度信息 （4 + 4 字节）
        //printf("buf0=%x   buf1=%x   buf2=%x   buf3=%x\n", buf[0], buf[1], buf[2], buf[3]);
        //printf("buf4=%u   buf5=%u   buf6=%u   buf7=%u\n", buf[4], buf[5], buf[6], buf[7]);
        
        if(recvLen >= 8 && buf[0] == MY_PRE_CODE_0 && buf[1] == MY_PRE_CODE_1 && buf[2] == MY_PRE_CODE_2 && buf[3] == MY_PRE_CODE_3)
        {     
        	printf("[fd=%d][%d]接收到前导码，客户请求开始\n", node->fd, cnt);
            //node->timeout_en = 0;//关闭超时检测（无等待接收数据）
            unsigned int data_len = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
			printf("[fd=%d][%d]接收数据中...", node->fd, cnt);
            char block[RECV_BLOCK_SIZE] = {0}; 
            //创建缓存文件
            char bufName[DEFAULT_BUF_NAME_MAX_LEN] = {0};
            sprintf(bufName, ".recv-%d%s",  node->fd, DEFAULT_BUF_NAME);//文件描述符是唯一的，以此作为缓存文件名的一部分
            bufCreate(bufName);
			
            //printf("len is %u\n", dataLen);
			break_mark = 0;
			current_len = 0;	
			back_len = 0;
            for(i = 0; i < data_len / RECV_BLOCK_SIZE; i++)//把 BLOCK_SIZE 字节当做一个块来写入
            {
            	//进度条
            	for(j = 0; j < back_len; j++)
            	{
					printf("\b");
				}
            	sprintf(process_bar, "[%d/%d]kb  [%.2f]", data_len / 1024, current_len / 1024,((double)current_len) * 100 / data_len );
				back_len = strlen(process_bar);
				printf("%s", process_bar);

                 memset(block, 0 , RECV_BLOCK_SIZE);
				 recvLen = 0;
                 //recvLen = recv(node->fd, (void *)block, RECV_BLOCK_SIZE, MSG_DONTWAIT);//无等待接收数据，并存入buf文件
                 recvLen = recv(node->fd, (void *)block, RECV_BLOCK_SIZE, 0);//阻塞接收数据（更不容易丢失数据，但是需要超时检测），并存入buf文件
                 if(recvLen < RECV_BLOCK_SIZE)//没接收完全，如何处理？？
                 {
                    //printf("newClientPro: receive data incompletable, lost byte %d\n", BLOCK_SIZE - recvLen);
                    printf("alarm: lost byte %d\n", RECV_BLOCK_SIZE - recvLen);
                    bufIn(bufName, block, recvLen); 
					 current_len += recvLen;
					continue;
                 }
                 bufIn(bufName, block, RECV_BLOCK_SIZE);
				 current_len += RECV_BLOCK_SIZE;

				 //超时检测刷新时间（阻塞接收数据）
				 t_tmp2 = time(&t_tmp);
				 if(t_tmp2 > 0)
				 {
					node->t = t_tmp2;
				 }
            }   
						
            rem = data_len % RECV_BLOCK_SIZE;
			memset(block, 0 , RECV_BLOCK_SIZE);
            if(rem > 0)
            {
                recvLen = recv(node->fd, (void *)block, rem, 0);//阻塞接收不足一个块的数据，并存入buf文件
                bufIn(bufName, block, rem);
				current_len += recvLen;
            }   	
			for(j = 0; j < back_len; j++)
        	{
				printf("\b");
			}
            if(data_len < 1024)
            {
                sprintf(process_bar, "[%d/%d]byte  [%.2f%%]", data_len , current_len  , ((double)current_len) * 100 / data_len );	
            }
            else
            {
                sprintf(process_bar, "[%d/%d]kb  [%.2f%%]", data_len / 1024, current_len  / 1024, ((double)current_len) * 100 / data_len );	
            }
        			
			printf("%s\n", process_bar);

			//printf("bufGetSize=%d data_len=%d current_len=%d rem=%d", bufGetSize(bufName), data_len, current_len, rem);			
            //缓存长度和数据长度不等，允许 0.2 % 的丢失率
            //printf("data_len=%d  bufGetSize:%d\n", data_len, bufGetSize(bufName));
            if(data_len > bufGetSize(bufName))
            {
            	
            	if(data_len > (data_len * 0.002 + bufGetSize(bufName)))
            	{
					printf("newClientPro: buffer size is not equal receive data length,lost more then 0.2%%, abort!\n");
                	//break;
                	continue;
				}
				printf("newClientPro: buffer size is not equal receive data length,but lost less then 0.2%%, totle %d byte\n", data_len - current_len);
                //recv(node->fd, (void *)block, data_len - current_len, MSG_DONTWAIT);//无等待接收额外的字节
            }
            //处理接收到的缓存数据 （在处理完数据之前，不应释放 fd（文件描述符），因为缓存文件与此相关）
            printf("[fd=%d][%d]接收完成，处理客户端请求...\n", node->fd, cnt);   
			node->timeout_en = 0;//关闭超时检测
            requestPro(bufName, node);		//全路径表示 缓存文件
			
			//收尾操作
			node->t = time(&t_tmp);//超时检测刷新时间
			node->timeout_en = 1;//开启超时检测
	         if(node->t == -1)
	         {
	            printf("wdThreadTimeout get time faild\n");
	            
	         }
			 printf("[fd=%d][%d]此次客户端请求结束\n", node->fd, cnt);
			 cnt++;

			 
//             if(cnt >= CONVERSATION_MAX_NUM)//超过限制   有待完善.....
//             {
//                printf("===========>超过次数，退出会话<==============\n");
//                break;
//             }
			 cnt2 = 0;
			 lost_len = data_len - current_len;
			 
			 
        }  
        else 
        {           
			//sleep(1);
			if(cnt2 >= (lost_len / 8)+ 1000)//允许接收的无用数据 n * 8 byte
			{	
				printf("recv too many unuseful data, quit\n");
				fflush(stdout);
				node->timeout_en = 0;//线程自动退出，不需要超时检测了
               
				break ;
			}
			cnt2++;
			continue;           
        }
             
    }

//退出
    linkClDelNode(global_link_cl_info_head_p, node);   
	threadEndPro(node, cnt);
    
}

//线程退出处理
int threadEndPro(CLIENT_INFO *node)
{
    //清除缓存文件
    char recv_buf[512] = {0};
    char send_buf[512] = {0};
    sprintf(recv_buf, "%s/%s/.recv-%d%s", TCP_SERVER_WORK_PATH, TCP_SERVER_BUF_FLODER, node->fd, DEFAULT_BUF_NAME);
    sprintf(send_buf, "%s/%s/.send-%d%s", TCP_SERVER_WORK_PATH, TCP_SERVER_BUF_FLODER, node->fd, DEFAULT_BUF_NAME);

    remove(recv_buf);
    remove(send_buf);

    return 0;
}

int tcpServerStart(char *ip, int port)
{
	int ret;

	//创建套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		printf("socket is error\r\n");
		return -1;
	}
	struct sockaddr_in  addr;
	addr.sin_family = AF_INET;
//	addr.sin_port = htons(10086);  
//	addr.sin_addr.s_addr = inet_addr("192.168.137.20");	
    addr.sin_port = htons(port);  
    addr.sin_addr.s_addr = inet_addr(ip);

	
	//绑定ip和端口号
	ret = bind(sockfd, (struct sockaddr *)(&addr), sizeof(struct sockaddr_in));
	if(ret != 0)
	{
		printf("bind is error\r\n");
		return -1;
	}
	
	//监听（15是同时在线最多客户数）
	ret = listen(sockfd, MAX_CLIENT_NUM);
	if(ret < 0)
	{
		printf("lsiten is error\r\n");
		return -1;
	}
	printf("listen...\n");
	
	 
    //创建链表
    global_link_cl_info_head_p = linkClCreate();
    //linkClTest();

	//调用到某些接口需要的初始化
	//bufInit();

    //接收用户的连接（线程）
    pthread_t  ac_th; 
    int sockfd_tmp;
    sockfd_tmp = sockfd;
    ret = pthread_create(&ac_th, NULL, acceptUserConnect, (void *)(&sockfd_tmp));
    if(ret < 0)
	{
		printf("pthread create is error\r\n");
		return -1;
	} 
    
    //超时检测（线程）
    pthread_t  timeout_th;  
    int time_out = 10;  
    ret = pthread_create(&timeout_th, NULL, timeoutDetection, (void *)(&time_out));
    if(ret < 0)
	{
		printf("pthread create is error\r\n");
		return -1;
	}
    
    CLIENT_INFO *prob;
    pthread_t th;
    int index;
    char ip_str[20] = {0};
	while(1)//主循环里不能调用阻塞函数
	{
        //遍历链表，查看是否有新的会话
        prob = global_link_cl_info_head_p->next;
         while(prob != NULL)
         {
            if(prob->del_mark == dialogmark_notStart)//需要开启线程处理新的会话
             {      
                index = linkClGetIndex(global_link_cl_info_head_p, prob);
                if(index == -1)
                {
                    printf("tcpServerStart: linkClGetIndex is error\r\n");
            		continue;
                }
                ret = pthread_create(&th, NULL, newClientPro, (void *)(&index));
                if(ret < 0)
            	{
            		printf("tcpServerStart: pthread create is error\r\n");
            		continue;
            	}
                prob->th = th;   
                prob->del_mark = dialogmark_start;
                         	               
            }
            prob = prob->next;           
         }
     
    
        //遍历链表，查看是否有因为超时而需要释放的线程
        prob = global_link_cl_info_head_p->next;
         while(prob != NULL)
         {
            if(prob->del_mark == CLIENT_TIMEOUT_DEL_MARK)
             {      
                //printf("pthread fd=%d will be canceled\n", prob->fd);
                sprintf(ip_str,"%d.%d.%d.%d",  ((unsigned char *)(&prob->ipv4))[0], ((unsigned char *)(&prob->ipv4))[1],
              						((unsigned char *)(&prob->ipv4))[2],((unsigned char *)(&prob->ipv4))[3]);
                printf("==========dialog %d ip=%s fd=%d  timeout end========\n", prob->num, ip_str, prob->fd);
                close(prob->fd);  
                ret = pthread_cancel(prob->th);
                if(ret != 0)
                 {
                    printf("tcpServerStart: pthread_cancel is error\n");
                    //fflush(stdout);
                }
                threadEndPro(prob);
            	linkClDelNode(global_link_cl_info_head_p, prob);                
            }
            prob = prob->next;           
         }

        //睡眠1秒
        sleep(1);

         
	}	
	return 0;
}



