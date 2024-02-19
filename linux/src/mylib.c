
#include "mylib.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>  
#include <dirent.h>



int cutByte(void *from, void *to, int start, int num)
{
	//范围检测
	if(start < 0 || num < 0)
	{
		return -1;
		
	}
	//截取字节数据
	unsigned char *a = (unsigned char *)from;
	unsigned char *b = (unsigned char *)to;
	int i = 0;
	for(i = 0; i < num; i++)
	{
		*(b + i) =*(a + start + i );
				
	}
		
	return 0;
}


//文件夹创建
int floderCreate(char *path)
{
    return mkdir(path,  0);
}
//在某路径中创建文件夹
int floderAdd(char *path, char * floder_name)//注：路径尾部不包含/
{
    char tmp[512] = {0};
    sprintf(tmp, "%s/%s", path, floder_name);
    return mkdir(tmp, 0);
}
//文件夹删除
int floderDelete(char *path)
{

	return  rmdir(path);
}
//在某路径中删除文件夹
int floderSub(char *path, char * floder_name)//注：路径尾部不包含/
{
    char tmp[512] = {0};
    sprintf(tmp, "%s/%s", path, floder_name);
    return rmdir(tmp);
}

//判断文件夹是否存在
//int floderExist(char *path)
//{
//	int mode;
//	if(-1 == access(path, mode))
//	{
//		return -1;
//	}
//	return mode;	
//}
int floderExist(char *path)
{
	DIR* d;
	d = opendir(path);
	if(d == NULL)
	{
		return -1;
	}	
	//d.close();
	return 0;
}











