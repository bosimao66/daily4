
#ifdef _MYLIB_H_
#define _MYLIB_H_


int cutByte(void *from, void *to, int start, int num);
int floderCreate(char *path);
int floderAdd(char *path, char * floder_name);
int floderDelete(char *path);
int floderSub(char *path, char * floder_name);
int floderExist(char *path);





#endif


