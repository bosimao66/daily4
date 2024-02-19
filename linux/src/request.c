#include <stdio.h>
#include <string.h>
#include "sys/types.h"
#include "sys/socket.h"
//ssize_t send(int sockfd, const void *buf, size_t len, int flags);

#include "request.h"
#include "user.h"
#include "fileopt.h"
#include "buf.h"

#define SEND_BLOCK_SIZE (1024)


//对客户端的请求处理（请求数据放置于缓存文件中）
int requestPro(char *buf_name,  CLIENT_INFO *node)
{
    //取出数据头
	STRUCT_DATA_HEAD head;
	bufGetByPos(buf_name, 0, sizeof(STRUCT_DATA_HEAD), (void *)(&head));
	printf("==>数据头：mode=%d opt=%d dataLen=%d paraInt0=%d paraSTr0=%s\n", head.mode, head.opt, head.data_len, head.paraInt[0], head.paraStr[0]);
	char buf[64] = {0};
	bufGetByPos(buf_name, sizeof(STRUCT_DATA_HEAD), (head.data_len > 64)? 64 : head.data_len , buf);
	//printf("数据：%s\n", buf);

    //账户密码验证
    printf("==>账户信息：user_name=%s  passwd（encrypt）=%s\n", head.user_name, head.passwd);
    int ret;
    ret = accountInt_passwdVerify(head.user_name, head.passwd);     //ret = 0 代表验证成功  -1 代表用户名不存在  -2 代表密码错误               
	
    //printf("============>A %d\n", optcode_login);
    int pro_mark = -1;
	//分别处理
	if(head.mode == datamode_request   && head.opt == optcode_hellomsg)
	{
		if(ret == 0)
		{
		    char tmp[256] = {0};
		    sprintf(tmp, "tcp server: welcome you，%s", head.user_name);
            requestPro_hello(node->fd, buf_name, tmp );
		}
		else
		{
            requestPro_hello(node->fd, buf_name, "tcp server: welcome you，you can try a free account: guset  123");
		}
		pro_mark = 1;
	}
	//上载文件
	else if( head.mode == datamode_request   && (head.opt == optcode_newfile_cover || head.opt == optcode_newfile_nocover))
	{
		if(ret == 0)
		{
            requestPro_upload(node, head, buf_name);
		}
		pro_mark = 2;
	}
	//下载文件
    else if(head.mode == datamode_request   && head.opt == optcode_download_file)
	{
		if(ret == 0)
		{
            requestPro_download(node, head);
		}		
		pro_mark = 3;
	}
	//请求同步
	else if(head.mode == datamode_request && (head.opt == optcode_syncToService || head.opt == optcode_syncFromService || head.opt == optcode_syncGetParaTable))
	{
	    if(ret == 0)
	    {
            requestPro_fileSync(node, head, buf_name);
	    }
        pro_mark = 4;
	}
	//账户注册
	else if(head.mode == datamode_request && head.opt == optcode_register)
	{	
		printf("==>请求操作：账户注册\n");
		fflush(stdout);
		requestPro_register(node, head, ret);
	    pro_mark = 5; 	   
	}
	//账户登录
	else if(head.mode == datamode_request && head.opt == optcode_login)
	{
		printf("==>请求操作：账户登录\n");
        requestPro_login(node, head, ret); 
        pro_mark = 6;
	}
	//账户销毁
	else if(head.mode == datamode_request && head.opt == optcode_accountDestory)
	{
		printf("==>请求操作：账户销毁\n");
        requestPro_accountDestroy(node, head, ret); 
        pro_mark = 7;
	}
	else
	{
          printf("==>操作码有错，无对应动作。 mode=%d code=%d\n", head.mode, head.opt);
          pro_mark = 8;
	}
	
	//printf("====================>B %d\n", pro_mark);
  
    return 0;
}



//回应hello信息
int requestPro_hello( int fd, char *buf_name, char *reply)
{
	int i;
	int rem;
	STRUCT_DATA_HEAD head = {0};
	head.mode = datamode_reply;
	head.opt = optcode_hellomsg;
	head.data_len = 0;

	if(strlen(reply) > DATA_HEAD_PARA_STR_MAX_LEN)//注意回复信息不要超过字符串参数大小
		{
		reply[DATA_HEAD_PARA_STR_MAX_LEN] = '\0';
	}
	strcpy(head.paraStr[0], reply);

    printf("==>请求动作：打招呼\t返回码：无\n"); 
	//填充发送的信息（数据为空，hello信息放置于参数中）
	sendData(fd, head, NULL);
	
	return 0;
}


//请求上载文件
//这里约定 buf文件 都带有数据头信息
int requestPro_upload(CLIENT_INFO *node, STRUCT_DATA_HEAD head, char *buf_name)
{
	int ret;
    ret = 0;
	STRUCT_DATA_HEAD re_head = {0};
	if(strlen(head.paraStr[0])  <= 0)
	{
		printf("reqUpload: para error\n");
		ret = -1;
        node->reply_code = ret;//记录错误码
        goto reqUpload_end;
	}
	if(head.opt == optcode_newfile_cover)//注：bufToFile_cover 返回值是写入的字节大小
	{
		if(0 <= bufToFile_cover(buf_name, head.paraStr[0], sizeof(STRUCT_DATA_HEAD), head.data_len))
        {
            ret = 0;
        } 
        else
        {
            ret = -1;
        }
	}
	else if(head.opt == optcode_newfile_nocover)
	{		
		if(0 <= bufToFile_nocover(buf_name, head.paraStr[0], sizeof(STRUCT_DATA_HEAD), head.data_len))
        {
            ret = 0;
        }       
        else
        {
            ret = -1;
        }
	}	
	else
	{
        ret = -1;
        node->reply_code = ret;
        goto reqUpload_end;
	}
	
	if(ret < 0)
	{
		printf("reqUpload: bufToFile_nocover has error\n");
	}
    
    node->reply_code = ret;
    goto reqUpload_end;


    
reqUpload_end: 
	if(node->reply_code == 0)
	{
		strcpy(re_head.paraStr[0], "tcp server: upload successfully");
	}
	else
	{
		strcpy(re_head.paraStr[0], "tcp server: upload mabe has some problem");
	}

    if(head.opt == optcode_newfile_cover)
    {
         printf("==>请求动作：新建文件\t返回码：%d\n", ret);         
    }
    else if(head.opt == optcode_newfile_nocover)
    {
         printf("==>请求动作：无覆盖新建文件\t返回码：%d\n", ret);         
    }
   //给客户端回复信息	
	re_head.mode = datamode_reply;
	re_head.opt = optcode_newfile_cover;
	re_head.data_len = 0;
	re_head.paraInt[0] = node->reply_code;//错误码的设置有待完善
	//填充发送的信息（数据为空，hello信息放置于参数中）
	sendData(node->fd, re_head, NULL);

	return ret;
}





//请求下载
//文件复制一份到 buf 文件中，再进行发送，缺点是如果要传送大文件的话，会增加处理的时间；优点是占用源文件时间可能减少
//有待优化
int requestPro_download(CLIENT_INFO *node, STRUCT_DATA_HEAD head)
{
    
	int ret;
    ret = 0;
    char new_buf[512] = {0};
    node->reply_code = 0;
	STRUCT_DATA_HEAD re_head = {0};

    printf("==>请求动作：下载文件, 文件名：%s\n", head.paraStr[0]);
    //文件名
    char file_name[512] = {0};
    char *file_name_p = file_name;
    sprintf(file_name, "%s/%s", gloable_file_path, head.paraStr[0]);
    ret = fullPath_bufExit(file_name);
    
	if(ret == 0)
	{
		strcpy(re_head.paraStr[0], "tcp server: download successfully");
		re_head.paraInt[0] = node->reply_code = 0;
	}
	else
	{   printf("requestPro_download: no such file for download, name=%s\n", file_name);
		strcpy(re_head.paraStr[0], "tcp server: download mabe has some problem");
		re_head.paraInt[0] = node->reply_code = -1;
		file_name_p = NULL;
	}
	//给客户端回复信息	
	re_head.mode = datamode_reply;
	re_head.opt = optcode_download_file;

	//填充发送的信息（数据为空，hello信息放置于参数中）	
	sendData(node->fd, re_head, file_name_p);

	return ret;   
}



//==================================================================同步请求：云文件关键
/*约定：

整体推送（类似于gitlab的clone）
-客户端从云端克隆
（1）客户端请求克隆
（2）服务器回应传送某文件夹所有文件
（3）客户端接收文件并做处理
-客户端推送至云端
（1）客户端请求推送，并上载文件
（2）服务器得到上载的文件，并做处理


部分同步
-客户端内容推送至服务器
（1）客户端请求 “推送更新”
（2）服务器得到请求后，向客户端请求参数表和数据表
（3）客户端响应请求，于是上载参数表和数据表
（4）服务器分析参数表和数据表，决定同步动作

-客户端拉取服务器内容
（1）客户端请求“拉取更新”
（2）服务器得到请求后，向客户端请求参数表
（3）客户端响应请求，于是上载参数表
（4）服务器得到参数表后，与本地的参数表进行比较，以决定发送 “需要修改”的部分参数表，及其对应的数据表
（5）客户端得到部分数据表，进行分析，并同步


新的处理方案：
			步骤：
			-0 客户端请求得到云端的本地参数表，和本地对比得出结论：a：正常地推送到服务器  b：正常地从服务器拉取 c：合并冲突
			-1 对应情况a：客户端请求新增更改到云端，上传同步参数表
			-2 对应情况a：承接第1步，上传同步数据表
            -3 对应情况b：客户端请求从服务器更新到自己，下载部分数据表
			//忽略-4 对应情况b：承接第3步，下载数据表

客户端
|-----------|
|   冲突    |  ^
|-----------|  |   向上拓展
|云端新增   |  |
|-----------|
|  本地     |
|-----------|

服务器端
|-----------|
|客户端新增 |  ^
|-----------|  |   向上拓展
|  本地  -  |  |
|-----------|
注：冲突分为两部分，客户端部分保留，服务器部分存于备份文件夹，以供参考。



*/
int requestPro_fileSync(CLIENT_INFO *node, STRUCT_DATA_HEAD head, char *buf_name)
{
    STRUCT_DATA_HEAD re_head = {0};
    int size = sizeof(STRUCT_DATA_HEAD);
    char full_path_buf[512];
    sprintf(full_path_buf, "%s%s", gloable_buf_path, buf_name);  //得到buf file全路径
    //printf("==============>A full_path_buf=%s\n", full_path_buf);
    int ret;

	//获得用户路径 和 参数表、数据表名
	char tmp[512] = {0};
	char user_path[512] = {0};
	char user_table_floder[512] = {0}; 
	accountInt_getPath(head.user_name, user_path);	
	sprintf(user_table_floder, "%s/%s", user_path, USER_WORK_TABLE_FLODER);
	char sync_para_table[512];
	char sync_data_table[512];
	char local_data_table[512];
	char local_para_table[512];
	sprintf(sync_para_table, "%s/%s", user_table_floder, USER_SYNC_PARA_TABLE);
	sprintf(sync_data_table, "%s/%s", user_table_floder, USER_SYNC_DATA_TABLE);
	sprintf(local_data_table, "%s/%s", user_table_floder, USER_LOCAL_DATA_TABLE);
	sprintf(local_para_table, "%s/%s", user_table_floder, USER_LOCAL_PARA_TABLE);

    
    //请求下载服务器本地参数表
    if(head.mode == datamode_request && head.opt == optcode_syncGetParaTable && head.paraInt[0] == 0)
    {
        printf("==>请求动作：下载服务器本地参数表\n");
        //回复内容，传输文件
        node->reply_code = 0;//本地记录回复码
        memset((void *)(&re_head), 0, size);//填充数据头
        re_head.data_len = 0;
        re_head.mode = datamode_reply;
        re_head.opt = optcode_syncGetParaTable;
        //re_head.paraInt[0] = 1;
        strcpy(re_head.paraStr[0], "tcp server: syncToService stage 0.");
        //要传输的文件       
        sendData(node->fd, re_head, local_para_table);
    }
     else if(head.mode == datamode_request && head.opt == optcode_syncFromService && head.paraInt[0] == 1)
    {
        printf("==>请求动作：上传参数表，并下载服务器部分数据表\n");
        //因为这些步骤 1-4 是顺序执行的，所以这里和 1 步骤都使用到了 sync_para_tabler 文件，也不会冲突
        dataCopy_cover(full_path_buf, sync_para_table, sizeof(STRUCT_DATA_HEAD), head.data_len);
        fileOpt_getDataTableByParaTable(local_data_table, sync_data_table, sync_para_table);  //根据上传的参数表，从本地数据表中获得部分数据元
        
         //回复内容
        node->reply_code = 0;//本地记录回复码
        memset((void *)(&re_head), 0, size);//填充数据头
        re_head.data_len = 0;
        re_head.mode = datamode_reply;
        re_head.opt = optcode_syncFromService;
        re_head.paraInt[0] = 1;//指示为第3步
        strcpy(re_head.paraStr[0], "tcp server:  syncToService stage 3.");
        sendData(node->fd, re_head, sync_data_table);
    }
    else if(head.mode == datamode_request && head.opt == optcode_syncToService && head.paraInt[0] == 2)
    {
        printf("==>请求动作：上传参数表\n");
       
		dataCopy_cover(full_path_buf, sync_para_table, sizeof(STRUCT_DATA_HEAD), head.data_len);
        
         //回复内容
        node->reply_code = 0;//本地记录回复码
        memset((void *)(&re_head), 0, size);//填充数据头
        re_head.data_len = 0;
        re_head.mode = datamode_reply;
        re_head.opt = optcode_syncToService;
        re_head.paraInt[0] = 2;//指示为第1步
        strcpy(re_head.paraStr[0], "tcp server:  syncToService stage 1.");
        sendData(node->fd, re_head, NULL);
    }
    else if(head.mode == datamode_request && head.opt == optcode_syncToService && head.paraInt[0] == 3)
    {
        printf("==>请求动作：上传数据表\n");
		dataCopy_cover(full_path_buf, sync_data_table, sizeof(STRUCT_DATA_HEAD), head.data_len);

		//执行同步动作
		ret = fileOpt_syncToServer(user_path);
		
         //回复内容
        node->reply_code = ret;//本地记录回复码
        memset((void *)(&re_head), 0, size);//填充数据头
        re_head.data_len = 0;
        re_head.mode = datamode_reply;
        re_head.opt = optcode_syncToService;
        re_head.paraInt[0] = 3;//指示为第2步
        if(0 == ret)
        {
        	re_head.paraInt[1] = 1;//同步成功
        }
        else
        {
			re_head.paraInt[1] = -1;//同步失败
        }
        strcpy(re_head.paraStr[0], "tcp server: syncToService stage 2.");
        sendData(node->fd, re_head, NULL);
    }    
   

    
    return 0;
}



//==================================================================账户服务
/*
注册、登录、销毁
*/
//注册： 回复的参数 paraInt[0]  1   ：成功          -1：用户名不存在   -2：系统失败
int requestPro_register(CLIENT_INFO *node, STRUCT_DATA_HEAD head, int check)
{
	int ret;
    STRUCT_DATA_HEAD re_head = {0};
    int reply_code;
	if(check != -1)//-1 代表用户名不存在
	{
		printf("   用户==>%s<==已经存在\n", head.user_name);
		reply_code = -1;
	}
	else
	{
		 ret = accountInt_registe(head.user_name, head.passwd);
		  if(ret == 0)
		{
		    reply_code = 1;
		    printf("   用户==>%s<==注册成功\n", head.user_name);
		}
		else
		{
		   reply_code = -2;
		   printf("   用户==>%s<==注册失败\n", head.user_name);
		}
	}   
    node->reply_code = reply_code;
    
    //回复信息
    re_head.data_len = 0;
    re_head.mode = datamode_reply;
    re_head.opt = optcode_register;
    re_head.paraInt[0] = reply_code;
    
    sendData(node->fd, re_head, NULL);
     
    return ret;
}
//登录
int requestPro_login(CLIENT_INFO *node, STRUCT_DATA_HEAD head, int check)
{
    int ret;
    STRUCT_DATA_HEAD re_head = {0};
    node->reply_code = check;
    //回复信息
    re_head.data_len = 0;
    re_head.mode = datamode_reply;
    re_head.opt = optcode_login;
    if(check == 0)
    {
        re_head.paraInt[0] = 1;
    }
    else
    {
        re_head.paraInt[0] = check;
    }
    
    sendData(node->fd, re_head, NULL);
     
    return ret;
}
//销毁
int requestPro_accountDestroy(CLIENT_INFO *node, STRUCT_DATA_HEAD head, int check)
{
    int ret;
    STRUCT_DATA_HEAD re_head = {0};
    int reply_code;
	if(check != 0)//-1 代表用户名不存在
	{
		printf("   用户==>%s<==验证失败\n", head.user_name);
		reply_code = -1;
	}
	else
	{
		 ret = accountInt_destory(head.user_name);
		 if(ret == 0)
		{
		    reply_code = 1;
		    printf("   用户==>%s<==销毁成功\n", head.user_name);
		}
		else
		{
		   reply_code = -2;
		   printf("   用户==>%s<==销毁失败\n", head.user_name);
		}
	}  

    node->reply_code = reply_code;
    //回复信息
    re_head.data_len = 0;
    re_head.mode = datamode_reply;
    re_head.opt = optcode_accountDestory;
    re_head.paraInt[0] = reply_code;
    
    
    sendData(node->fd, re_head, NULL);
     
    return ret;
}



//==================================================================发送数据接口
//发送数据
int sendData(int fd, STRUCT_DATA_HEAD head, char *data_from)
{
    int ret;
    char to[512] = {0};
    //创建新的buf文件
    sprintf(to, "%s/%s/.send-%d%s", TCP_SERVER_WORK_PATH, TCP_SERVER_BUF_FLODER, fd, DEFAULT_BUF_NAME);
    fullPath_bufCreate(to);
       
    ret = dataCapsulationForSend(head, data_from, to);
    if(ret != 0)
    {
        return ret;
    }
    
    return sendBuf(fd, to);  
}

//封装前导码+数据头+数据到缓存文件；data_from 不包含数据头；head.data_len由data_from确定
int dataCapsulationForSend(STRUCT_DATA_HEAD head, char *data_from, char *to)
{
    long len; 
    long a, b;
    char buf[32] = {0};
    if(to == NULL)
    {
        printf("dataCapsulationForSend: destination buf file name is NULL\n");
        return -1;
    }

    //添加路径
   
    
    //获取长度
	a = sizeof(STRUCT_DATA_HEAD) ;
    if(data_from != NULL)
    {
        b = fullPath_bufGetSize(data_from);
    }   
    else
    {
        b = 0;
    }	
    //printf("===========>b=%d\n", b);
    head.data_len = b;    //重新指定数据长度
				
    //填充前导码和数据头部、数据
//	if(0 != bufCreate(to))
//	{
//		printf("sendData: bufCreate error, abort!");
//		return -1;
//	}	
	len = a + b;
	buf[0] = MY_PRE_CODE_0;
	buf[1] = MY_PRE_CODE_1;
	buf[2] = MY_PRE_CODE_2;
	buf[3] = MY_PRE_CODE_3;
	buf[4] = (len & 0xff000000) >> 24;
    buf[5] = (len & 0xff0000) >> 16;
    buf[6] = (len & 0xff00) >> 8;
    buf[7] = (len & 0xff);


	fullPath_bufAppend(to, (void *)buf, 8);
	fullPath_bufAppend(to, (void *)(&head) , a);
    if(data_from != NULL)dataCopy_append(data_from, to, 0, b);     
    return 0;
}


//发送缓存文件中的内容
int sendBuf(int fd, char *buf_name)
{
    long except_len, send_len;
    int i;
    char block[SEND_BLOCK_SIZE] = {0};
    int rem;
    int ret;

    if(buf_name == NULL)
    {
        printf("sendBuf: buf_name is NULL\n");
        return -1;
    }
    printf("====================> send file size=%d\n", fullPath_bufGetSize(buf_name));

    send_len = 0;
    //分块发送buf中的全部内容
    except_len = fullPath_bufGetSize(buf_name);
	for(i = 0; i < except_len / SEND_BLOCK_SIZE; i++)
	{
		memset(block, 0 ,SEND_BLOCK_SIZE);
		if(0 > fullPath_bufGetBlock(buf_name,  (void *)block, SEND_BLOCK_SIZE * i, SEND_BLOCK_SIZE))
			{
			printf("sendBuf: bufGetBlock error\n");
			continue;
		}

		ret = send(fd, (void *)block, SEND_BLOCK_SIZE, 0);
		if(ret < SEND_BLOCK_SIZE)
		{
			printf("sendData: send  incompletely, lose %d byte\n", SEND_BLOCK_SIZE - ret);
			send_len += ret;
		}
		send_len += SEND_BLOCK_SIZE;

	}
	rem = except_len % SEND_BLOCK_SIZE;
	memset(block, 0 ,SEND_BLOCK_SIZE);	
	if(0 == fullPath_bufGetBlock(buf_name, (void *)block, except_len - (except_len % SEND_BLOCK_SIZE), (except_len % SEND_BLOCK_SIZE)))
	{
		ret = send(fd, (void *)block, rem, 0);
		if(ret < rem)
		{
			printf("sendData: send  incompletely, lose %d byte\n", rem - ret);
		send_len += ret;
		}
		send_len += rem;
	}
	//长度检测
	printf("==>发送：except send: %d  totle send:%d  totle lose:%d \n", except_len, send_len, except_len - send_len);
	if(((float)(except_len - send_len)) / except_len > 0.005)
	{
		printf("sendData: totle lose more than 0.5%%,abort!\n");
		return -1;
	}

	return send_len;
}





