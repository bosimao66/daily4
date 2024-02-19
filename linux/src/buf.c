
//基本接口
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "buf.h"
#include "tcpServer.h"



//全局变量：文件路径。初始化时判断是否存在，不存在的话选择程序运行的路径
char gloable_buf_path[512];
char gloable_file_path[512];


void bufInit(void)
{
    if(floderExist(TCP_SERVER_WORK_PATH) != 0)
	{
        if(floderCreate(TCP_SERVER_WORK_PATH))
        {
            printf("accountInit: attension,floder %s is not exist\n", TCP_SERVER_WORK_PATH);
        }
	}

	sprintf(gloable_buf_path, "%s/%s", TCP_SERVER_WORK_PATH, TCP_SERVER_BUF_FLODER);
	sprintf(gloable_file_path, "%s/%s", TCP_SERVER_WORK_PATH, TCP_SERVER_FILE_FLODER);
	//创建文件夹
	floderCreate(gloable_buf_path);
	floderCreate(gloable_file_path);

	//假如不存在...
	DIR* d;
	d = opendir(gloable_buf_path);
	if(d == NULL)
	{
		strcpy(gloable_buf_path, "./");
	}
	else
	{
		strcat(gloable_buf_path, "/");
	}
	
	d = opendir(gloable_file_path);
	if(d == NULL)
	{
		strcpy(gloable_file_path, "./");
	}
	else
	{
		strcat(gloable_file_path, "/");
	}
	
	
	
}


//开辟新的缓存文件
int bufCreate(char *fileName)
{   
    char fullName[512] = {0};//有超过 512字节 这么长的路径吗？
    if(strlen(gloable_buf_path) + strlen(fileName) > 512)
    {
        printf("bufCreate: path or fileName is too long!\n");
        return -1;
    }
    memset(fullName, 0, sizeof(fullName));
    strcpy(fullName, gloable_buf_path);
    strcat(fullName, fileName);
    
    FILE *fp = fopen(fullName, "w");
    if(fp == NULL)
    {
        printf("bufCreate: file open is error, fileName=%s\n", fullName);
        return -1;
    }
    
    fclose(fp);
    return 0;
}

//清空
int bufClear(char *fileName)
{
    return bufCreate(fileName);
}


//数据写入缓存文件(追加内容)
int bufIn(char *fileName, void *data, unsigned int len)
{
    int ret; 
    int i;
	char ch;
	int rem;
    char fullName[512] = {0};//有超过 512字节 这么长的路径吗？
    if(strlen(gloable_buf_path) + strlen(fileName) > 512)
    {
        printf("bufIn: path or fileName is too long!\n");
        return -1;
    }
    memset(fullName, 0, sizeof(fullName));
    strcpy(fullName, gloable_buf_path);
    strcat(fullName, fileName);
    
    FILE *fp = fopen(fullName, "a");
    if(fp == NULL)
    {
        printf("bufIn: file open is error, fileName=%s\n", fullName);
        return -1;
    }

    ret = fwrite(data, BUF_BLOCK_SIZE, ( len / BUF_BLOCK_SIZE) , fp);//按块写入，比一个一个字节操作更快？？
	rem = len % BUF_BLOCK_SIZE;
	for(i = 0; i < rem; i++)
	{
		ch = *((char *)data + len - rem + i);
		fputc(ch, fp);
		//putchar(ch);
	}        	
    //printf("\nbufIn test end==================>\n");
	//fflush(stdout);
    fclose(fp);
    return 0;
}


//按块读取缓存内容（块的编号从0开始）
int bufGetBlock(char *fileName, int blockSize, int blockNum, void *out_data)
{
    int ret;
    char fullName[512] = {0};//有超过 512字节 这么长的路径吗？
    if(blockNum < 0)
    {
        printf("bufGetBlock: blockNum is out of range!\n");
        return -1;
    }
    if(strlen(gloable_buf_path) + strlen(fileName) > 512)
    {
        printf("bufGetBlock: path or fileName is too long!\n");
        return -1;
    }
    memset(fullName, 0, sizeof(fullName));
    strcpy(fullName, gloable_buf_path);
    strcat(fullName, fileName);
    
    FILE *fp = fopen(fullName, "r");
    if(fp == NULL)
    {
        printf("bufGetBlock: file open is error\n");
        return -1;
    }
    rewind(fp);
    fseek(fp, blockSize * blockNum, SEEK_SET);
    ret = fread(out_data, blockSize, 1, fp);
    fclose(fp);
    return ret;
}

//从指定起始位置按块读取缓存内容（块的编号从0开始）
int bufGetBlockByPos(char *fileName, int pos, int blockSize, int blockNum, void *out_data)
{
    int ret;
    char fullName[512] = {0};//有超过 512字节 这么长的路径吗？
    if(blockNum < 0)
    {
        printf("bufGetBlockByPos: blockNum is out of range!\n");
        return -1;
    }
    if(strlen(gloable_buf_path) + strlen(fileName) > 512)
    {
        printf("bufGetBlockByPos: path or fileName is too long!\n");
        return -1;
    }
    memset(fullName, 0, sizeof(fullName));
    strcpy(fullName, gloable_buf_path);
    strcat(fullName, fileName);
    
    FILE *fp = fopen(fullName, "r");
    if(fp == NULL)
    {
        printf("bufGetBlockByPos: file open is error\n");
        return -1;
    }
    rewind(fp);
    fseek(fp, blockSize * blockNum + pos, SEEK_SET);
    ret = fread(out_data, blockSize, 1, fp);
    fclose(fp);
    return ret;
}



//从指定起始位置读取缓存内容（块的编号从0开始）
int bufGetByPos(char *fileName, int pos, int len, void *out_data)
{
    int ret;
    char fullName[512] = {0};//有超过 512字节 这么长的路径吗？
    if(len < 0)
    {
        printf("bufGetByPos: blockNum is out of range!\n");
        return -1;
    }
    if(strlen(gloable_buf_path) + strlen(fileName) > 512)
    {
        printf("bufGetByPos: path or fileName is too long!\n");
        return -1;
    }
    memset(fullName, 0, sizeof(fullName));
    strcpy(fullName, gloable_buf_path);
    strcat(fullName, fileName);
    
    FILE *fp = fopen(fullName, "r");
    if(fp == NULL)
    {
        printf("bufGetByPos: file open is error\n");
        return -1;
    }
    rewind(fp);
    fseek(fp, pos, SEEK_SET);
    ret = fread(out_data, len, 1, fp);
    fclose(fp);
    return ret;
}



//获得缓存文件大小
long bufGetSize(char *bufName)
{
     char fullName[512] = {0};//有超过 512字节 这么长的路径吗？
    if(strlen(gloable_buf_path) + strlen(bufName) > 512)
    {
        printf("bufGetSize: path or fileName is too long!\n");
        return -1;
    }
    memset(fullName, 0, sizeof(fullName));
    strcpy(fullName, gloable_buf_path);
    strcat(fullName, bufName);
    
    FILE *fp = fopen(fullName, "r");
    if(fp == NULL)
    {
        printf("bufGetSize: file open is error\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    
    fclose(fp);
    return len;

}


//全路径
long fileGetSize(char *full_name)
{
     
    FILE *fp = fopen(full_name, "r");
    if(fp == NULL)
    {
        printf("fileGetSize: file open is error, name\n", full_name);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    
    fclose(fp);
    return len;
}


//buf 追加拷贝，文件名包含路径
long dataCopy_append(char *from, char *to, unsigned int start, unsigned int len)
{
	int ret;
	char block[1024] = {0};
	int i;
	char ch;
	unsigned int rem;
    unsigned int buf_len;

    buf_len = fileGetSize(from);
    if(buf_len < start && start >= 0 && len >= 0 )
    {
        printf("bufCopy_append: get opt_len error, buf_len=%d  start=%d  len=%d\n", buf_len, start, len);
        return -1;
    }
    if(len > (buf_len - start))
    {
        printf("bufCopy_append: size of frombuf is less than expect\n ");
        len = (buf_len - start);
    }
    
    if(from == NULL || to == NULL)
    {
        printf("bufCopy_append: file name is NULL\n");
        return -1;
    }
	
    FILE *fp = fopen(from, "r");
    if(fp == NULL)
    {
        printf("bufCopy_append: file open is error\n");
        return -1;
    }
	
	FILE *fp_to = fopen(to, "a");
	if(fp_to == NULL)
    {
    	fclose(fp);
        printf("bufCopy_append: file open is error\n");
        return -1;
    }
	
	rewind(fp);
	fseek(fp, start, SEEK_SET);
    //需要移至末尾吗？可以不用
    //rewind(fp_to);
	//fseek(fp_to, 0, SEEK_END);   
	//printf("totle=%d  start=%d\n", bufGetSize(bufName), start);
	for(i = 0; i < ( len / 1024); i++)
	{
		memset(block, 0, 1024);	
		ret = 0;
		ret = fread(block, 1024, 1, fp);
		if(ret != 1)
		{
			printf("bufToFile_cover: fread error\n");
			return -1;
		}
		
		ret = fwrite(block, 1024, 1, fp_to);

		if(ret != 1)
		{
			printf("bufToFile_cover: fwrite error\n");
			return -1;
		}
		
	}	
	rem = len % 1024;
	for(i = 0; i < rem; i++)
	{
		ch = fgetc(fp);
		fputc(ch, fp_to);
		
	} 

   
    fclose(fp);
	fclose(fp_to);
    return len;	

}

//buf 覆盖拷贝，文件名包含路径
long dataCopy_cover(char *from, char *to, unsigned int start, unsigned int len)
{

	int ret;
	char block[1024] = {0};
	int i;
	char ch;
	unsigned int rem;
   
    if(from == NULL || to == NULL)
    {
        printf("bufCopy: file name is NULL\n");
        return -1;
    }
	
    FILE *fp = fopen(from, "r");
    if(fp == NULL)
    {
        printf("bufCopy: file open is error, name=%s\n", from);
        return -1;
    }
	
	FILE *fp_to = fopen(to, "w");
	if(fp_to == NULL)
    {
    	fclose(fp);
        printf("bufCopy: file open is error, name=%s\n", to);
        return -1;
    }

	
	rewind(fp);
	fseek(fp, start, SEEK_SET);
	//printf("totle=%d  start=%d\n", bufGetSize(bufName), start);
	for(i = 0; i < ( len / 1024); i++)
	{
		memset(block, 0, 1024);	
		ret = 0;
		ret = fread(block, 1024, 1, fp);
		if(ret != 1)
		{
			printf("bufToFile_cover: fread error\n");
			return -1;
		}
		
		ret = fwrite(block, 1024, 1, fp_to);

		if(ret != 1)
		{
			printf("bufToFile_cover: fwrite error\n");
			return -1;
		}
		
	}	
	rem = len % 1024;
	for(i = 0; i < rem; i++)
	{
		ch = fgetc(fp);
		fputc(ch, fp_to);
		
	} 

   
    fclose(fp);
	fclose(fp_to);
    return len;

}




long bufTobuf_cover(char *bufName, char *to, unsigned int start, unsigned int len)
{
    char fullName1[512] = {0};//有超过 512字节 这么长的路径吗？
    char fullName2[512] = {0};//有超过 512字节 这么长的路径吗？
    if(strlen(gloable_buf_path) + strlen(bufName) > 512 || strlen(gloable_buf_path) + strlen(to) > 512)
    {
        printf("bufTobuf_cover: path or fileName is too long!\n");
        return -1;
    }
		
    memset(fullName1, 0, sizeof(fullName1));
    strcpy(fullName1, gloable_buf_path);
    strcat(fullName1, bufName); 
    

	memset(fullName2, 0, sizeof(fullName2));
    strcpy(fullName2, gloable_buf_path);
    strcat(fullName2, to);

	return dataCopy_cover(fullName1, fullName2, start, len);
}


long bufTobuf_append(char *bufName, char *to, unsigned int start, unsigned int len)
{
    char fullName1[512] = {0};//有超过 512字节 这么长的路径吗？
    char fullName2[512] = {0};//有超过 512字节 这么长的路径吗？
    if(strlen(gloable_buf_path) + strlen(bufName) > 512 || strlen(gloable_buf_path) + strlen(to) > 512)
    {
        printf("bufTobuf_cover: path or fileName is too long!\n");
        return -1;
    }
		
    memset(fullName1, 0, sizeof(fullName1));
    strcpy(fullName1, gloable_buf_path);
    strcat(fullName1, bufName); 
    

	memset(fullName2, 0, sizeof(fullName2));
    strcpy(fullName2, gloable_buf_path);
    strcat(fullName2, to);

	return dataCopy_append(fullName1, fullName2, start, len);
}




//把buf中的数据转移到文件，如果指定文件存在，则覆盖
//注： buf 文件和其他文件的一个很大不同是，存放路径不同
long bufToFile_cover(char *bufName, char *to, unsigned int start, unsigned int len)
{
	
    char fullName1[512] = {0};//有超过 512字节 这么长的路径吗？
    char fullName2[512] = {0};//有超过 512字节 这么长的路径吗？
    if(strlen(gloable_buf_path) + strlen(bufName) > 512 || strlen(gloable_file_path) + strlen(to) > 512)
    {
        printf("bufToFile_cover: path or fileName is too long!\n");
        return -1;
    }
		
    memset(fullName1, 0, sizeof(fullName1));
    strcpy(fullName1, gloable_buf_path);
    strcat(fullName1, bufName); 
    

	memset(fullName2, 0, sizeof(fullName2));
    strcpy(fullName2, gloable_file_path);
    strcat(fullName2, to);

	return dataCopy_cover(fullName1, fullName2, start, len);

}


//把buf中的数据转移到文件，如果指定文件存在，则不动作
long bufToFile_nocover(char *bufName, char *to, unsigned int start, unsigned int len)
{
	char fullName[512] = {0};//有超过 512字节 这么长的路径吗？
	if(strlen(gloable_file_path) + strlen(to) > 512)
	{
		printf("bufToFile_nocover: path or fileName is too long!\n");
		return -1;
	}
	
	memset(fullName, 0, sizeof(fullName));
	strcpy(fullName, gloable_file_path);
	strcat(fullName, to);
	FILE *fp_to = fopen(fullName, "r");
	if(fp_to != NULL)
	{
		printf("bufToFile_nocover: file exist, do not cover\n");
        fflush(stdout);
		return -1;
	}

	return dataCopy_cover(bufName, to, start, len);	
}


//把文件中的数据转移到buf
//注： buf 文件和其他文件的一个很大不同是，存放路径不同
long FileToBuf(char *file_name, char *to)
{
    unsigned int len;
    char fullName1[512] = {0};//有超过 512字节 这么长的路径吗？
    char fullName2[512] = {0};//有超过 512字节 这么长的路径吗？
    if(strlen(gloable_buf_path) + strlen(file_name) > 512 || strlen(gloable_file_path) + strlen(to) > 512)
    {
        printf("bufToFile_cover: path or fileName is too long!\n");
        return -1;
    }
	
	
    memset(fullName1, 0, sizeof(fullName1));
    strcpy(fullName1, gloable_file_path);
    strcat(fullName1, file_name); 
    

	memset(fullName2, 0, sizeof(fullName2));
    strcpy(fullName2, gloable_buf_path);
    strcat(fullName2, to);

    FILE *fp = fopen(fullName1, "r");
	if(fp == NULL)
		{
		printf("FileToBuf: fopen is error\n");
		return -1;
	}
    fseek(fp, 0 ,SEEK_END);
    len = ftell(fp);
	if(0 > dataCopy_cover(fullName1, fullName2, 0, len))
    {
        return -1;
    }   
    return 0;

}




//简单测试
void bufTest(void)
{
    char data[100] = {0};
    bufCreate(".bufTest");
    bufIn(".bufTest", "\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00\n\
        ", 500);
     bufIn(".bufTest", "\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00\n\
        ", 500);
     bufIn(".bufTest", "\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00d\
fasdfa1234 89980980999999999999993423423423423-00\n\
        ", 500);   
    bufGetBlock(".bufTest", 100, 10, (void *)data);
    printf("\nlen: %d\n", bufGetSize(".bufTest"));
    printf("data: %s\n", data);
    bufClear(".bufTest");
}


//############################################################拓展
//鉴于 buf 文件默认放置于某个文件夹，造成不便调用，但是又不想重新修改，所以
//这里做一个拓展，以适用于全路径操作

//获取大小
int fullPath_bufGetSize(char *name)
{
    FILE *fp = fopen(name, "r");
    if(fp == NULL)
    {
        printf("fullPath_bufGetSize:file open error, name=%s\n", name);
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    fclose(fp);
    return len;

}


//判断文件是否存在，或者是否能够打开
int fullPath_bufExit(char *name)
{   
    FILE *fp = fopen(name, "r");
    if(fp == NULL)
        {
        return -1;
    }
    fclose(fp);
    return 0;
}

//长度获取
int fullPath_bufgetLen(char *name)
{
    FILE *fp = fopen(name, "r");
    if(fp == NULL)
        {

        printf("fullPath_bufGetPath: get len error\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    int len = ftell(fp);
    
    return len;
}



//开辟buf（覆盖创建）
int fullPath_bufCreate(char *name)
{
    FILE *fp = fopen(name, "w");
    if(fp == NULL)
    {
        //printf("fullPath_bufCreate: file open is error, fileName=%s\n", name);
        return -1;
    }
    
    fclose(fp);
    return 0;   
}

//开辟buf（非覆盖创建）
int fullPath_bufCreateNoCover(char *name)
{
    FILE *fp = fopen(name, "r");
    if(fp != NULL)
    {
        //printf("fullPath_bufCreateNoCvoer: file is exist, fileName=%s\n", name);
        return -1;
    }    
     
    fp = fopen(name, "w");
    if(fp == NULL)
    {
        printf("fullPath_bufCreateNoCover: file open is error, fileName=%s\n", name);
        return -1;
    }    
    fclose(fp);
    
    return 0;   
}

//buf追加内容
int fullPath_bufAppend(char *name, void *t, unsigned int len)
{
    int ret;
    FILE *fp = fopen(name, "a");
    if(fp == NULL)
    {
        printf("fullPath_bufAppend: file open is error, fileName=%s\n", name);
        return -1;
    }

    if(len < 0 )
    {
        printf("fullPath_bufAppend: len has problem\n");
        return -1;
    }
    ret = -1;
    if(1 == fwrite(t, len, 1, fp))
    {   
        ret = 0;
    }
    
    fclose(fp);
    return ret;   

}
//buf获取长度
unsigned int fullPath_bufGetLen(char *name)
{
    FILE *fp = fopen(name, "r");
    if(fp == NULL)
    {
        printf("bufGetSize: file open is error\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    unsigned int len = ftell(fp);
    
    fclose(fp);
    return len;
}
//buf读取内容
int fullPath_bufGetBlock(char *name, char *out, unsigned int start, unsigned int len)
{ 
    int ret;
    if(out == NULL || len <= 0)
    {
        printf("fullPath_bufGetBlock: no content to do\n");
        return -1;
    }
    
    FILE *fp = fopen(name, "r");
    if(fp == NULL)
    {
        printf("bufGetByPos: file open is error\n");
        return -1;
    }
    rewind(fp);
    fseek(fp, start, SEEK_SET);
    ret = -1;
    if( 1 == fread(out, len, 1, fp))
    {
        ret = 0;
    }
    
    fclose(fp);
    return ret;    
}

//buf读某个序号块 序号从0开始
int fullPath_bufGetBlockByIndex(char *name, char *out, int size, int index)
{ 
    int ret;
    if(out == NULL || index < 0)
    {
        printf("fullPath_bufGetBlock: no content to do\n");
        return -1;
    }
    
    FILE *fp = fopen(name, "r");
    if(fp == NULL)
    {
        printf("bufGetByPos: file open is error\n");
        return -1;
    }
    rewind(fp);
    fseek(fp, size * index, SEEK_SET);
    ret = -1;
    if( 1 == fread(out, size, 1, fp))
    {
        ret = 0;
    }
    
    fclose(fp);
    return ret;    
}


//buf写入内容
int fullPath_bufSetBlock(char *name, char *out, unsigned int start, unsigned int len)
{ 
    int ret;
    if(out == NULL || len <= 0)
    {
        printf("fullPath_bufGetBlock: no content to do\n");
        return -1;
    }
    
    FILE *fp = fopen(name, "r+");
    if(fp == NULL)
    {
        printf("bufGetByPos: file open is error\n");
        return -1;
    }
    rewind(fp);
    fseek(fp, start, SEEK_SET);
    ret = -1;
    if( 1 == fwrite(out, len, 1, fp))
    {
        ret = 0;
    }
    
    fclose(fp);
    return ret;    
}

//buf删除内容
int fullPath_bufDelBlock(char *name, unsigned int start, unsigned int len)
{
    unsigned int totle_len;
    unsigned int i;
    char ch;

    FILE *fp = fopen(name, "r+");
    if(fp == NULL)
    {
        printf("bufGetByPos: file open is error\n");
        return -1;
    }
    rewind(fp);
    fseek(fp, 0, SEEK_END);
    totle_len = ftell(fp);

    if(start < 0 || len <= 0)
    {
        printf("fullPath_bufDelBlock: start or len is error\n");
        return -1;
    }
    if((start + len) > totle_len)
    {
        len = totle_len - start;
    }
        
    //创建临时文件？？
    FILE *fp_tmp = fopen(".tmp", "w+");
    if(fp_tmp ==NULL)
    {   
        fclose(fp);
        printf("fullPath_bufDelBlock: fopen error\n");
        return -1;
    }
   
    fseek(fp, 0, SEEK_SET);
    fseek(fp_tmp, 0, SEEK_SET);
    for(i = 0; i < totle_len; i++)
     {
        ch = fgetc(fp);
        if(i >= start && i < (start + len))
        {          
            ;
        }
        else
        {
            fputc(ch, fp_tmp);
        }
    }
    //转移内容
    fseek(fp_tmp, 0 ,SEEK_END);
    unsigned int tmp_len = ftell(fp_tmp);
    fclose(fp);
    fp = fopen(name, "w");
    if(fp == NULL)
     {
        fclose(fp_tmp);
        printf("fullPath_bufDelBlock: file open error\n");
        return -1;
    }  
    fseek(fp, 0, SEEK_SET);
    fseek(fp_tmp, 0, SEEK_SET);
    for(i = 0; i < tmp_len; i++)
    {
        ch = fgetc(fp_tmp);
        fputc(ch, fp);
    }
    //删除临时文件
    fclose(fp_tmp);
    remove(".tmp");
    fclose(fp);
    return 0;

}

//插入内容
int fullPath_bufInsertBlock(char *name, void *in, unsigned int start, unsigned int len)
{
//printf("=============================>C\n");
    unsigned int totle_len;
    unsigned int i, j;
    char ch;

    FILE *fp = fopen(name, "r");
    if(fp == NULL)
    {
        printf("bufGetByPos: file open is error\n");
        return -1;
    }
    rewind(fp);
    fseek(fp, 0, SEEK_END);
    totle_len = ftell(fp);

    
    //printf("==========>C  totle_len=%d in=%s\n", totle_len, in);
    if(in == NULL )
        {
        return 0;
    }
    
    if(start < 0 || len <= 0)
    {
        printf("fullPath_bufDelBlock: start or len is error\n");
        return -1;
    }
    //printf("=============================>C totle_len=%d start=%d\n", totle_len, start);  
    if(start  > totle_len + 1)
    {
        start = totle_len;
    }
      
    //创建临时文件？？
    FILE *fp_tmp = fopen(".tmp", "w+");
    if(fp_tmp == NULL)
    {   
        fclose(fp);
        printf("fullPath_bufDelBlock: fopen error\n");
        return -1;
    }
   
    fseek(fp, 0, SEEK_SET);
    fseek(fp_tmp, 0, SEEK_SET);
    if(totle_len == 0)
    {
        for(j = 0; j < len; j++)
        {
            fputc(((char *)in)[j], fp_tmp);
        }
    }
    else if(start == totle_len)//相当于追加
    {
        for(i = 0; i < totle_len; i++)
        {          
            ch = fgetc(fp);
            fputc(ch, fp_tmp);                   
        }
        for(i = 0; i < len; i++)
        {
            fputc(((char *)in)[i], fp_tmp);
        }
    }
    else
    {
         for(i = 0; i < totle_len; i++)
         {
           
            if(i == start)
            {                          
                for(j = 0; j < len; j++)
                {
                    fputc(((char *)in)[j], fp_tmp);
                }
            } 
            ch = fgetc(fp);
            fputc(ch, fp_tmp);
                   
        }
    }
   
    //转移内容
    fseek(fp_tmp, 0 ,SEEK_END);
    unsigned int tmp_len = ftell(fp_tmp);
    fclose(fp);
    fp = fopen(name, "w");
    if(fp == NULL)
     {
        fclose(fp_tmp);
        printf("fullPath_bufDelBlock: file open error\n");
        return -1;
    }  
    fseek(fp, 0, SEEK_SET);
    fseek(fp_tmp, 0, SEEK_SET);
    for(i = 0; i < tmp_len; i++)
    {
        ch = fgetc(fp_tmp);
        fputc(ch, fp);
    }
    //删除临时文件
    fclose(fp_tmp);
    remove(".tmp");
    fclose(fp);
    return 0;

}



/*int main(void)
{
    bufTest();
    return 0;
}
*/


