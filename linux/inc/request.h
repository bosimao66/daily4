

#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "tcpServer.h"
#include "linkCL.h"
#include "buf.h"


int requestPro(char *buf_name,  CLIENT_INFO *node);
int requestPro_hello( int fd, char *buf_name, char *reply);
int requestPro_upload(CLIENT_INFO *node, STRUCT_DATA_HEAD head, char *buf_name);
int requestPro_download(CLIENT_INFO *node, STRUCT_DATA_HEAD head);
int requestPro_fileSync(CLIENT_INFO *node, STRUCT_DATA_HEAD head, char *buf_name);

int sendData(int fd, STRUCT_DATA_HEAD head, char *data_from);
int dataCapsulationForSend(STRUCT_DATA_HEAD head, char *data_from, char *to);
int sendBuf(int fd, char *buf_name);

int requestPro_register(CLIENT_INFO *node, STRUCT_DATA_HEAD head, int check);
int requestPro_login(CLIENT_INFO *node, STRUCT_DATA_HEAD head, int check);
int requestPro_accountDestroy(CLIENT_INFO *node, STRUCT_DATA_HEAD head, int check);






#endif



