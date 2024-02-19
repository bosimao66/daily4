#ifndef _BUF_H_
#define _BUF_H_


//#define BUF_PATH "./buf/"
//#define FILE_PATH "./file/"
#define BUF_BLOCK_SIZE (10*1024)
#define DEFAULT_BUF_NAME  ".buf"

extern char gloable_buf_path[512];
extern char gloable_file_path[512];



int bufCreate(char *fileName);
int bufClear(char *fileName);
int bufIn(char *fileName, void *data, unsigned int len);
int bufGetBlock(char *fileName, int blockSize, int blockNum, void *out_data);
int bufGetBlockByPos(char *fileName, int pos, int blockSize, int blockNum, void *out_data);
int bufGetByPos(char *fileName, int pos, int len, void *out_data);
long bufGetSize(char *bufName);
long fileGetSize(char *full_name);
void bufTest(void);
long dataCopy_append(char *from, char *to, unsigned int start, unsigned int len);
long dataCopy_cover(char *from, char *to, unsigned int start, unsigned int len);
long bufTobuf_cover(char *bufName, char *to, unsigned int start, unsigned int len);
long bufTobuf_append(char *bufName, char *to, unsigned int start, unsigned int len);

long bufToFile_cover(char *bufName, char *to, unsigned int start, unsigned int len);
long bufToFile_nocover(char *bufName, char *to, unsigned int start, unsigned int len);
long FileToBuf(char *file_name, char *to);


//############################################################拓展
//鉴于 buf 文件默认放置于某个文件夹，造成不便调用，但是又不想重新修改，所以
//这里做一个拓展，以适用于全路径操作
//判断文件是否存在，或者是否能够打开
int fullPath_bufExit(char *name);
int fullPath_bufgetLen(char *name);
int fullPath_bufCreate(char *name);
int fullPath_bufCreateNoCover(char *name);
int fullPath_bufAppend(char *name, void *t, unsigned int len);
unsigned int fullPath_bufGetLen(char *name);
int fullPath_bufGetBlock(char *name, char *out, unsigned int start, unsigned int len);
int fullPath_bufGetBlockByIndex(char *name, char *out, int size, int index);
int fullPath_bufSetBlock(char *name, char *out, unsigned int start, unsigned int len);
int fullPath_bufDelBlock(char *name, unsigned int start, unsigned int len);
int fullPath_bufInsertBlock(char *name, void *in, unsigned int start, unsigned int len);
int fullPath_bufGetSize(char *name);



#endif


