
#ifndef CLOUDFILE_H
#define CLOUDFILE_H

#include <QMainWindow>
#include <QtNetwork>
#include <QFile>

#include "optlink.h"
#include "fileopt.h"

//-----------------------非类方法
int dataEncrypt_byte8(void *data, int len, char *out);
//-------------------------


#define DATA_HEAD_PARA_NUM 4 //参数个数
#define DATA_HEAD_PARA_STR_MAX_LEN 50    //字符串的最大长度，注意不要超过范围
//模式
typedef enum enum_datamodetmp{
    datamode_request = 1,
    datamode_reply,
}enum_datamode;
//前导码和结束码（弊端：如果收到的数据本身含有结束码，那么会出错）
#define MY_PRE_CODE_0 0x66
#define MY_PRE_CODE_1 0x77
#define MY_PRE_CODE_2 0x88
#define MY_PRE_CODE_3 0x99
//#define MY_END_CODE_0 0x99
//#define MY_END_CODE_1 0x88
//#define MY_END_CODE_2 0x77
//#define MY_END_CODE_3 0x66
//操作码
typedef enum enum_optcodetmp{
    optcode_nothing = 1,
    optcode_sync,
    optcode_hellomsg,
    optcode_newfile_cover,
    optcode_newfile_nocover,    //请求上载文件，不覆盖
    optcode_download_file,  //请求下载文件
    optcode_syncToService,  //请求推送到服务器进行同步
    optcode_syncFromService, //请求从服务器拉取来进行同步
    optcode_syncGetParaTable,//请求云端的本地参数表
    optcode_login,  //登录
    optcode_register,    //注册
    optcode_accountDestory,  //销毁账户
    opcode_forgetPasswd     //忘记密码
}enum_optcode;
//头部数据结构体
typedef struct STRUCT_DATA_HEAD_tmp{
    quint8 mode;
    quint16 opt;
    quint32 data_len;
                char user_name[33];
                char passwd[33];
    int paraInt[DATA_HEAD_PARA_NUM];//其他的整形参数
    char paraStr[DATA_HEAD_PARA_NUM][DATA_HEAD_PARA_STR_MAX_LEN];//字符串参数
}STRUCT_DATA_HEAD;


namespace Ui {
class cloudFile;
}

class cloudFile : public QMainWindow
{
    Q_OBJECT

public:
    cloudFile();
    cloudFile(QString wkpath);
    ~cloudFile();

private slots:
//    void on_pushButton_test_clicked();
//    void on_pushButton_connect_clicked();
//    void on_pushButton_tcpServer_clicked();
//    void on_pushButton_disconnect_clicked();
//      void on_pushButton_test_2_clicked();
    void clientDiscon();
    int recvMsg();



//private:
public:
    Ui::cloudFile *ui;
    void init(QString wkpath);
    void testFun();
    void testFun2();
    void sendData(QTcpSocket *socket, STRUCT_DATA_HEAD head, QByteArray data);
    int connectTcpServer(QString ip, int port);
    int creatTcpServer(QString ip, int port);
    int upLoadCover(QString from, QString to);
    int upLoadNoCover(QString from, QString to);
    int sayHello(QString s);
    int recvPro(QByteArray t);
    int replyHello(STRUCT_DATA_HEAD head);
    int replyUpload(STRUCT_DATA_HEAD head);
    int reqDownload(QString name, QString save_name);
    int replyDownload(STRUCT_DATA_HEAD head, QByteArray t);
    int reqSyncFile(int stage, QString para1, QString para2);
    int replySyncFile(STRUCT_DATA_HEAD head, QByteArray t);
    int reqUserRegister(QString name, QString passwd);
    int replyAccountRegister(STRUCT_DATA_HEAD head);
    int reqUserLogin(QString name, QString passwd);
    int replyAccountLogin(STRUCT_DATA_HEAD head);
    int reqUserDestory(QString name, QString passwd);
    int replyAccountDestory(STRUCT_DATA_HEAD head);
    int reqSyncFile_start(QString user_name, QString user_passwd);

    QTcpSocket *tcp_client_sock;
    int client_sock_mark;
    QByteArray recv_data;
    quint32 recv_len;
    quint32 recv_expet_len;
    int recv_pro_start;
    QString download_save_name;

    //文件操作接口类
    FileOpt *file_opt_int;
    //操作记录接口类
    OptLink *opt_link_int;
    OptLink *opt_link_head;
    //当前用户名 和密码（加密后的密码）
    QString default_user_name;
    QString default_user_passwd;
    QString current_user_name;
    QString current_user_passwd;
    int default_user_mark;    //使用用户名及其密码的标志 0-使用  1-使用默认用户   2-使用当前用户



    //####################################文件操作存于链表接口
    int fileRecord_clearLink();
    int fileRecord_addFile(QString name, QString content);
    int fileRecord_modifyFile(QString name, QString content);
    int fileRecord_delFile(QString name);
    int fileRecord_rename(QString name, QString new_name);

};

#endif // CLOUDFILE_H










