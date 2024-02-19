//#include "cloudfile.h"


#include "mainwindow.h"    //也包含了  cloudfile.h 文件
#include <QDebug>


/*备忘
  (1)
  QByteArray 转换为 char *
  1 char *ch;//不要定义成ch[n];
  2 QByteArray byte;
  3 ch = byte.data();

  char * 转换为 QByteArray
  1 char *ch;
  2 QByteArray byte;
  3 byte = QByteArray(ch);

  (2)非阻塞延时
  void Delay_MSec(unsigned int msec)
  {
  QEventLoop loop;//定义一个新的事件循环
  QTimer::singleShot(msec, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
  loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
  }

  （3）数据流的用法
  QByteArray block;
  QDataStream ds(&block, QIODevice::ReadWrite | QIODevice::Truncate);

  ds << (quint8)1;
  ds << (quint8)2;
  ds << (quint8)3;
  ds << (quint8)4;
  qDebug()<<" block0="<<(int)block[0]<<
  " block1="<<(int)block[1]<<
  " block2="<<(int)block[2]<<
  " block3="<<(int)block[3];

  （4） 数据流的用法      2
  Example (write binary data to a stream):

  QFile file("file.dat");
  file.open(QIODevice::WriteOnly);
  QDataStream out(&file);   // we will serialize the data into the file
  out << QString("the answer is");   // serialize a string
  out << (qint32)42;        // serialize an integer

  Example (read binary data from a stream):

  QFile file("file.dat");
  file.open(QIODevice::ReadOnly);
  QDataStream in(&file);    // read the data serialized from the file
  QString str;
  qint32 a;
  in >> str >> a;           // extract "the answer is" and 42



*/
//#############################声明
int serverReplyToMainWindow(int action_num, int reply_code, QString msg);


//#############################非类中的方法
void myDelay(int d)
{
    QEventLoop loop;//定义一个新的事件循环
    QTimer::singleShot(d, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

//（注意和linux 的tcp 服务器端同步）不可逆数据转换（用于加密密码，保证密码从不显示明码）
/*
   -过程：[n字节数据]  ====加密===>   [8字节目标码]  =====规范====>  [8字节规范目标码]
   -方法：
   （1）确定目标码的原始的 8个字节，例如 {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88}；
   （2）如果n不是8的倍数，那么补齐为 n1，倍数为 b，b是大于等于1的整数；
   （3）目标码的原始8个字节 分b次，与数据的字节进行位操作，位操作包括 与、或、异或 这3个，轮流进行；
   （4）得到的8字节目标码，每个字节进行规范，例如 取值的池子 {'0', '1', '2', '3', '4'}，通过 %5 取余操作，得到
   最终的码将会在池子范围之内。
   */
//注：这里的 data 为二进制数据
#define ENCRYPT_OPT(b1, b2, a)  \
    do{\
        if(a == 0)\
        {\
            b1 &= b2;\
        }\
        else if(a == 1)\
        {\
            b1 |= b2;\
        }\
        else if(a == 2)\
        {\
            b1 ^= b2;\
        }\
    }while(0);

char gloable_user_char_pool[] = {
    '0','1','2','3','4','5','6','7','8','9',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y',
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y'
};

int dataEncrypt_byte8(void *data, int len, char *out)
{
    //printf("=================>A \n");
    //参数检测
    if(len <= 0)
    {
        printf("dataEncrypt: data len is too short\n");
        return -1;
    }
    if(data == NULL)
    {
        printf("dataEncrypt: data is NULL\n");
        return -1;
    }
    //填充
    unsigned char code[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    int i, j, cnt;
    if(len % 8 != 0)
    {
        for(i = 0; i < 8 - (len % 8); i++)
        {
            ((char *)data)[i + len] = '0';//用'0'填充
        }
        ((char *)data)[i + len] = '\0';
        len += 8 - (len % 8);
    }
    //printf("=================>B \n");
    cnt = 0;
    //位运算
    for(i = 0; i < len / 8; i++)
    {
        for(j = 0; j < 8; j ++)
        {
            ENCRYPT_OPT(code[j], ((char *)data)[i * 8 + j], cnt % 3);
            cnt++;
        }

    }
    int size = strlen(gloable_user_char_pool);
    //printf("================>size =%d\n",size);
    //规范化
    for(i = 0; i < 8; i++)
    {
        out[i] = gloable_user_char_pool[code[i]  % size];
    }
    out[i] = '\0';
    return 0;
}



//##############################初始化
cloudFile::cloudFile()
{
    //init();
}

cloudFile::cloudFile(QString wkpath)
{
    init(wkpath);
}

cloudFile::~cloudFile()
{
    delete ui;
}


void cloudFile::init(QString wkpath)
{
    client_sock_mark = 0;

    //操作记录接口初始化
    opt_link_int = new OptLink();
    opt_link_head = opt_link_int->linkCreate();
    //文件操作接口初始化
    file_opt_int = new FileOpt(wkpath);
    file_opt_int->setOptLinkHead(opt_link_head, opt_link_int);

    file_opt_int->setCurrentUserFloder(wkpath, "guest");
    file_opt_int->userFloderInit(wkpath, "guest");
    //默认用户名
    default_user_name.clear();
    default_user_name.append("guest");
    default_user_passwd.clear();
    char tmp1[8] = "123";
    char tmp2[32];
    dataEncrypt_byte8(tmp1, 3, tmp2);
    default_user_passwd.append(tmp2);
    default_user_mark = 0;

    //开始文件操作记录
    fileRecord_clearLink();

}



//##############################按键动作
//void cloudFile::on_pushButton_test_clicked()
//{
//    testFun();
//}


//void cloudFile::on_pushButton_test_2_clicked()
//{
//    testFun2();
//}


//void cloudFile::on_pushButton_connect_clicked()
//{
//    connectTcpServer("192.168.137.20", 10086);
//}

//void cloudFile::on_pushButton_tcpServer_clicked()
//{
//    creatTcpServer("192.168.137.1", 10086);
//}

//void cloudFile::on_pushButton_disconnect_clicked()
//{
//    if(client_sock_mark == 1)
//    {
//        tcp_client_sock->close();
//    }

//}


//####################################################槽函数
//接收信息
int cloudFile::recvMsg()
{
    int i;
    //qDebug()<<"recvMsg is comming";
    if(client_sock_mark != 1)
    {
        qDebug()<<"recvMsg: tcp_client_sock has something wrong\n";
        return -1;
    }
    QByteArray t = tcp_client_sock->readAll();

    if(recv_pro_start == 0 && (int)t.length() >= 8)
    {
        for(i = 0; i < (quint32)t.length() - 7; i++)
        {
            if(t[i] == MY_PRE_CODE_0 && t[i + 1] == MY_PRE_CODE_1 && t[i + 2] == MY_PRE_CODE_2 && t[i + 3] == MY_PRE_CODE_3)
            {

                recv_data.clear();
                recv_data.append(t);
                recv_len = t.length();
                //qDebug()<<"t======>"<<(int)t[i + 4]<<" "<<(int)t[i + 5]<<" "<<(int)t[i + 6]<<" "<<(int)t[i + 7];
                recv_expet_len = ((quint8)t[i + 4] << 24) | ((quint8)t[i + 5] << 16) | ((quint8)t[i + 6] << 8) | (quint8)t[i + 7];
                recv_pro_start = 1;
                //qDebug()<<"start to recv useful data , expect length "<<recv_expet_len;
                if(recv_len >= recv_expet_len)
                {
                    recvPro(recv_data);
                    recv_pro_start = 0;
                    recv_data.clear();
                    recv_expet_len = 0;
                }
                break;
            }

        }
    }
    else if(recv_pro_start == 1)
    {
        //qDebug()<<"My userful recv is comming";
        recv_data.append(t);
        recv_len += t.length();
        if(recv_len >= recv_expet_len)
        {
            recvPro(recv_data);
            recv_pro_start = 0;
            recv_data.clear();
            recv_expet_len = 0;
        }

    }
}

void cloudFile::clientDiscon()
{
    tcp_client_sock->abort();
    tcp_client_sock->close();
    tcp_client_sock->deleteLater();
    client_sock_mark = 0;


}

//####################################################功能函数
void cloudFile::testFun()
{

    file_opt_int->userFloderInit( "D:/long/cloudeFile", "guest");

    file_opt_int->test();

    //upLoadCover("D:/long/cloudeFile/aa.exe", "bb.exe");

}

void cloudFile::testFun2()
{
    //reqUserDestory("along", "456");
    file_opt_int->setCurrentUserFloder(file_opt_int->wk_path, "guest");
    reqSyncFile(0, file_opt_int->current_para_table_download, file_opt_int->current_data_table_download);

}


//向服务器打招呼
int cloudFile::sayHello(QString s)
{
    //serverReplyToMainWindow(0, 0, "aa");

    STRUCT_DATA_HEAD  head;
    head.mode = datamode_request;
    head.opt = optcode_hellomsg;
    //head.paraInt[0] = 10;
    strcpy(head.paraStr[0], s.toLatin1().data());
    QByteArray t;
    if(client_sock_mark == 1)
    {
        t.clear();
        sendData(tcp_client_sock, head, t);
    }
    else
    {
        qDebug()<<"upLoadCover: Has not connet to tcp server";
        return -1;
    }
    return 0;
}

//覆盖上载
//from：是本地文件名，包括路径; to 是云端文件名，不包括路径
int cloudFile::upLoadCover(QString from, QString to)
{
    STRUCT_DATA_HEAD  head;
    head.mode = datamode_request;
    head.opt = optcode_newfile_cover;
    //head.paraInt[0] = 10;
    strcpy(head.paraStr[0], to.toLatin1().data());
    if(client_sock_mark == 1)
    {
        QFile file(from);
        if(false == file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"upLoadCover: open file faild\n";
            return -1;
        }
        QByteArray t = file.readAll();
        file.close();
        sendData(tcp_client_sock, head, t);
    }
    else
    {
        qDebug()<<"upLoadCover: Has not connet to tcp server\n";
        return -1;
    }
    return 0;
}


//不覆盖上载
//也就是说文件存在的话，服务器不会保存上载的文件
int cloudFile::upLoadNoCover(QString from, QString to)
{
    STRUCT_DATA_HEAD  head;
    head.mode = datamode_request;
    head.opt = optcode_newfile_nocover;
    //head.paraInt[0] = 10;
    strcpy(head.paraStr[0], to.toLatin1().data());
    if(client_sock_mark == 1)
    {
        QFile file(from);
        if(false == file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"upLoadCover: open file faild\n";
            return -1;
        }
        QByteArray t = file.readAll();
        file.close();
        sendData(tcp_client_sock, head, t);
    }
    else
    {
        qDebug()<<"upLoadCover: Has not connet to tcp server\n";
        return -1;
    }
    return 0;
}

//请求下载文件 from：请求文件名  保存文件名放在类成员变量（作用有点像全局变量）
int cloudFile::reqDownload(QString name, QString save_name)
{
    STRUCT_DATA_HEAD  head;
    head.mode = datamode_request;
    head.opt = optcode_download_file;
    //head.paraInt[0] = 10;
    download_save_name.clear();
    download_save_name.append(save_name);
    strcpy(head.paraStr[0], name.toLatin1().data());
    if(client_sock_mark == 1)
    {
        QByteArray t ;
        t.clear();
        sendData(tcp_client_sock, head, t);
    }
    else
    {
        qDebug()<<"reqDownload: no connection to tcp server\n";
        return -1;
    }
    return 0;
}


//请求同步
/*
   具体处理
   （1）客户端 ==> 服务器  数据头
   模式 datamode_request 操作码 optcode_syncToService ；整形参数 0-请求提示  1-请求上载参数表   2-请求上载数据表
   模式 datamode_request 操作码 optcode_syncFromService ：整形参数 0-请求提示  1-请求下载参数表  2-请求下载数据表
   （2）服务器端 ==> 客户端 数据头
   模式 datamode_reply 操作码 optcode_syncToService : 整形参数 0-回复提示  1-接收参数表的情况  2-接收数据表的情况   3-拒绝回复
   模式 datamode_reply 操作码 optcode_syncFromService: 整形参数 0-回复提示  1-提供参数表  2-提供数据表   3-拒绝服务
   */
//多个函数切换，para_table 参数丢失，所以想到用类成员来存储，用标志位来作提示；因此同一时刻只能处理一次同步流程
//如果流程中断，标志位（进行的阶段0、1、2）可能切不回来；或许可以用另一种方法：静态变量
/*
新的处理方案：
            步骤：
            -0 客户端请求得到云端的本地参数表，和本地对比得出结论：a：正常地推送到服务器  b：正常地从服务器拉取 c：合并冲突
            -1 对应情况a：客户端请求新增更改到云端，上传同步参数表
            -2 对应情况a：承接第1步，上传同步数据表
           -3 对应情况b：客户端请求从服务器更新到自己，下载部分数据表
            //忽略-4 对应情况b：承接第3步，下载数据表

客户端
|---------|
|   冲突   |
|---------|
|云端新增   |
|---------|
|  本地    |
|---------|

服务器端
|---------|
|客户端新增 |
|---------|
|  本地    |
|---------|
注：冲突分为两部分，客户端部分保留，服务器部分存于备份文件夹，以供参考。
*/
int cloudFile::reqSyncFile_start(QString user_name, QString user_passwd)
{
    current_user_name.clear();
    current_user_passwd.clear();
    current_user_name.append(user_name);
    current_user_passwd.append(user_passwd);
    QString tmp;
    reqSyncFile(0, tmp, tmp);
    return 0;
}


int cloudFile::reqSyncFile(int stage, QString para1, QString para2)
{
    STRUCT_DATA_HEAD  head;
    QByteArray t ;
    default_user_mark = 2;

    if(stage == 0)
    {
        head.data_len = 0;
        head.mode = datamode_request;
        head.opt = optcode_syncGetParaTable;
        head.paraInt[0] = 0;
        t.clear();
    }
    else if(stage == 1)//上载服务器领先的部分参数表，并下载对应的数据表
    {
        head.mode = datamode_request;
        head.opt = optcode_syncFromService;
        head.paraInt[0] = 1;
        t.clear();
        QFile file(para1);
        //qDebug()<<"reqSyncToService: para_naem="<<para_name;
        if(false == file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"reqSyncToService: open file error";
            return -1;
        }
        t = file.readAll();
        file.close();
    }
    else if(stage == 2)//上载参数表
    {
        head.mode = datamode_request;
        head.opt = optcode_syncToService;
        head.paraInt[0] = 2;
        t.clear();
        QFile file(para1);
        //qDebug()<<"reqSyncToService: para_naem="<<para_name;
        if(false == file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"reqSyncToService: open file error";
            return -1;
        }
        t = file.readAll();
        file.close();
    }
    else if(stage == 3)//上传数据表
    {
        head.mode = datamode_request;
        head.opt = optcode_syncToService;
        head.paraInt[0] = 3;
        t.clear();
        QFile file(para1);
        //qDebug()<<"reqSyncToService: para_naem="<<para_name;
        if(false == file.open(QIODevice::ReadOnly))
        {
            qDebug()<<"reqSyncToService: open file error";
            return -1;
        }
        t = file.readAll();
        file.close();
    }



    if(client_sock_mark == 1)
    {

        sendData(tcp_client_sock, head, t);
    }
    else
    {
        qDebug()<<"reqDownload: no connection to tcp server\n";
        return -1;
    }
    return 0;
}

//请求用户注册（注：参数 passwd 是原始密码）
int cloudFile::reqUserRegister(QString name, QString passwd)
{
    QByteArray t;
    STRUCT_DATA_HEAD  head;
    head.mode = datamode_request;
    head.opt = optcode_register;
    default_user_mark = 0;    //关闭默认发送的用户名
    strcpy(head.user_name, name.toLatin1().data());
    char tmp[32] = {0};
    //dataEncrypt_byte8(passwd.toLatin1().data(), passwd.length(), tmp);
    strcpy(head.passwd, passwd.toLatin1().data());

    if(client_sock_mark == 1)
    {

        t.clear();
        sendData(tcp_client_sock, head, t);
    }
    else
    {
        qDebug()<<"reqDownload: no connection to tcp server";
        return -1;
    }
    return 0;
}
//请求用户登录（这个功能比较鸡肋）
int cloudFile::reqUserLogin(QString name, QString passwd)
{
    QByteArray t;
    STRUCT_DATA_HEAD  head;
    head.mode = datamode_request;
    head.opt = optcode_login;
    default_user_mark = 0;    //关闭默认发送的用户名
    strcpy(head.user_name, name.toLatin1().data());
    char tmp[32] = {0};
    //dataEncrypt_byte8(passwd.toLatin1().data(), passwd.length(), tmp);
    strcpy(head.passwd, passwd.toLatin1().data());

    if(client_sock_mark == 1)
    {

        t.clear();
        sendData(tcp_client_sock, head, t);
    }
    else
    {
        qDebug()<<"reqDownload: no connection to tcp server";
        return -1;
    }
    return 0;
}
//请求用户销毁
int cloudFile::reqUserDestory(QString name, QString passwd)
{
    QByteArray t;
    STRUCT_DATA_HEAD  head;
    head.mode = datamode_request;
    head.opt = optcode_accountDestory;
    default_user_mark = 0;    //关闭默认发送的用户名
    strcpy(head.user_name, name.toLatin1().data());
    char tmp[32] = {0};
   // dataEncrypt_byte8(passwd.toLatin1().data(), passwd.length(), tmp);
    strcpy(head.passwd, passwd.toLatin1().data());

    if(client_sock_mark == 1)
    {

        t.clear();
        sendData(tcp_client_sock, head, t);
    }
    else
    {
        qDebug()<<"reqDownload: no connection to tcp server";
        return -1;
    }
    return 0;
}



//处理接收到的信息
int cloudFile::recvPro(QByteArray t)
{
    qDebug()<<"已接收到数据，处理数据中......";
    quint32 a;
    quint32 b;
    a = sizeof(STRUCT_DATA_HEAD);
    b = t.length();
    //有效性检测
    if((int)t.length() < (8 + a))
    {
        qDebug()<<"recvPro: recv data is incomplete";
        return -1;
    }
    if(t[0] != MY_PRE_CODE_0 || t[1] != MY_PRE_CODE_1 || t[2] != MY_PRE_CODE_2 || t[3] != MY_PRE_CODE_3)
    {
        qDebug()<<"recvPro:  pre code has something wrong";
        return -1;
    }
    quint32 len = ((quint8)t[4] << 24) | ((quint8)t[5] << 16) | ((quint8)t[6] << 8) | (quint8)t[7] ;

    qDebug()<<"[recvPro]"<<"recv len="<<t.length()<< " data len="<<len;
    if(len > (len * 0.005) + b - 8)
    {
        qDebug()<<"recvPro:  recv data lost more than 0.5%%, abort process";
        return -1;
    }
    //提取数据头
    STRUCT_DATA_HEAD head;
    QByteArray t1, t2;
    QString str;
    t1 = t.mid(8, a);
    memcpy((void *)(&head), t1, a);
    str.sprintf("data head: mode=%d opt=%d paraInt0=%d paraStr0=>%s<=", head.mode, head.opt, head.paraInt[0], head.paraStr[0]);
    qDebug()<<"[recvPro]"<<str;
    //提取数据
    t2 = t.mid(8 + a, head.data_len);

    //分析数据头，做相应的处理
    if(head.mode == datamode_reply && head.opt == optcode_hellomsg)
    {
        replyHello(head);
    }
    else if(head.mode == datamode_reply && head.opt == optcode_newfile_cover)
    {
        replyUpload(head);
    }
    else if(head.mode == datamode_reply && head.opt == optcode_download_file)
    {
        replyDownload(head, t2);
    }
    else if(head.mode == datamode_reply &&
            (head.opt == optcode_syncToService || head.opt == optcode_syncGetParaTable || head.opt == optcode_syncFromService))//同步到服务器的处理
    {
        replySyncFile(head, t2);
    }
    else if(head.mode == datamode_reply && (head.opt == optcode_register))//同步到服务器的处理
    {
        replyAccountRegister(head);
    }
    else if(head.mode == datamode_reply && ( head.opt == optcode_login ))//同步到服务器的处理
    {
        replyAccountLogin(head);
    }
    else if(head.mode == datamode_reply && (head.opt == optcode_accountDestory ))//同步到服务器的处理
    {
        replyAccountDestory(head);
    }


}

//####################################################接口函数
//服务器回复hello的处理
int cloudFile::replyHello(STRUCT_DATA_HEAD head)
{
    qDebug()<<"[replyHello]"<<"服务器回复的信息："<<head.paraStr[0];

    //信息回复给上层到的调用
    serverReplyToMainWindow(1, 1, QString(head.paraStr[0]));

}

//服务器回复上传文件的处理
int cloudFile::replyUpload(STRUCT_DATA_HEAD head)
{
    qDebug()<<"[replyUpload]"<<"服务器返回的错误码："<<head.paraInt[0];
    qDebug()<<"[replyUpload]"<<"服务器回复的信息："<<head.paraStr[0];

}

//从服务器中下载文件
int cloudFile::replyDownload(STRUCT_DATA_HEAD head, QByteArray t)//默认覆盖保存
{
    qDebug()<<"[replyUpload]"<<"服务器返回的错误码："<<head.paraInt[0];
    qDebug()<<"[replyUpload]"<<"服务器回复的信息："<<head.paraStr[0];
    if(head.paraInt[0] == 0 && download_save_name.isEmpty() == false)
    {
        qDebug()<<"[replyDownload]保存文件 "<<download_save_name<<" 保存成功";
        QFile file(download_save_name);
        file.open(QIODevice::ReadWrite | QIODevice::Truncate);
        file.write(t);
    }
    else
    {
        return -1;
    }
    return 0;
}
//回复处理：同步文件
int cloudFile::replySyncFile(STRUCT_DATA_HEAD head, QByteArray t)
{
    QString tmp;

    if(head.opt == optcode_syncGetParaTable)
    {
        qDebug()<<"================>replySyncFile 0";
        qDebug()<<"================>current_para_table_download = "<<file_opt_int->current_para_table_download;
        QFile file(file_opt_int->current_para_table_download);
        if(false == file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            qDebug()<<"replySyncFile: file open faild, name="<<file_opt_int->current_para_table_download;
            return -1;
        }
        file.write(t);
        file.close();
        file_opt_int->paraTableAnalyze(file_opt_int->current_para_table_local, file_opt_int->current_data_table_local, file_opt_int->current_para_table_download);

        //把out_diff_server_head 对应的参数表 存到 current_para_table_download 文件，以供最后一步  syncFile() 的操作
        //开启第 1 步
        file_opt_int->linkToParaFile(file_opt_int->out_diff_server_head, file_opt_int->current_para_table_sync);
        file_opt_int->linkToParaFile(file_opt_int->out_diff_server_head, file_opt_int->current_para_table_download);
        reqSyncFile(1, file_opt_int->current_para_table_sync, tmp);

        //信息回复给上层到的调用
        serverReplyToMainWindow(4, 0, QString(" "));

    }
    else if(head.opt == optcode_syncFromService && head.paraInt[0] == 1)
    {
        qDebug()<<"================>replySyncFile 1";

        QFile file(file_opt_int->current_data_table_download);
        if(false == file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        {
            qDebug()<<"replySyncFile: file open faild, name="<<file_opt_int->current_data_table_download;
            return -1;
        }
        file.write(t);
        file.close();


        file_opt_int->linkToFile(file_opt_int->out_diff_client_head, file_opt_int->current_para_table_sync, file_opt_int->current_data_table_sync);
        reqSyncFile(2, file_opt_int->current_para_table_sync, tmp);

        //信息回复给上层到的调用
        serverReplyToMainWindow(4, 1, QString(" "));
    }
    else if(head.opt == optcode_syncToService && head.paraInt[0] == 2)//第 2 步收尾
    {
        qDebug()<<"================>replySyncFile 2";
        reqSyncFile(3, file_opt_int->current_data_table_sync, tmp);

        //信息回复给上层到的调用
        serverReplyToMainWindow(4, 2, QString(" "));
    }
    else if(head.opt == optcode_syncToService && head.paraInt[0] == 3)
    {
        qDebug()<<"================>replySyncFile 3";
        //执行同步动作

        file_opt_int->syncFile();

        //信息回复给上层到的调用
        serverReplyToMainWindow(4, 3, QString(" "));
    }


    return 0;
}




//回复处理：用户注册
int cloudFile::replyAccountRegister(STRUCT_DATA_HEAD head)
{
    if(head.paraInt[0] == 1)
    {
        qDebug()<<"replyAccountRegister: register  successfully.";
    }
    else if(head.paraInt[0] == -1)
    {
        qDebug()<<"replyAccountRegister: register is faild, user name is exist.";
    }
    else if(head.paraInt[0] == -2)
    {
        qDebug()<<"replyAccountRegister: register is faild, system error";
    }


      //信息回复给上层到的调用
    serverReplyToMainWindow(3, head.paraInt[0], QString(" "));

     return 0;
}
//回复处理：用户登录
int cloudFile::replyAccountLogin(STRUCT_DATA_HEAD head)
{
    if(head.paraInt[0] == 1)
    {
        qDebug()<<"replyAccountLogin: login successfully.";
    }
    else if(head.paraInt[0] == -1)
    {
        qDebug()<<"replyAccountLogin: login is faild, user name is not exist.";
    }
    else if(head.paraInt[0] == -2)
    {
        qDebug()<<"replyAccountLogin: login is faild, password error.";
    }

     //信息回复给上层到的调用
    serverReplyToMainWindow(2, head.paraInt[0], QString(" "));

    return 0;
}
//回复处理：用户销毁
int cloudFile::replyAccountDestory(STRUCT_DATA_HEAD head)
{
    if(head.paraInt[0] == 1)
    {
        qDebug()<<"replyAccountRegister: destory  successfully.";
    }
    else if(head.paraInt[0] == -1)
    {
        qDebug()<<"replyAccountRegister: destory is faild, check has not been passed.";
    }
    else if(head.paraInt[0] == -2)
    {
        qDebug()<<"replyAccountRegister: destory is faild, system error";
    }

     //信息回复给上层到的调用
    serverReplyToMainWindow(6, head.paraInt[0], QString(" "));
    return 0;
}


//发送 长度 + 数据头 + 数据
void cloudFile::sendData(QTcpSocket *socket, STRUCT_DATA_HEAD head, QByteArray data)
{
    QByteArray block;
    int i;

    //前导码
    block[0] = MY_PRE_CODE_0;
    block[1] = MY_PRE_CODE_1;
    block[2] = MY_PRE_CODE_2;
    block[3] = MY_PRE_CODE_3;
    //数据总长度（不包含前导码）
    quint32 len = sizeof(STRUCT_DATA_HEAD) + data.length();
    block[4] = (len & 0xff000000) >> 24;
    block[5] = (len & 0xff0000) >> 16;
    block[6] = (len & 0xff00) >> 8;
    block[7] = (len & 0xff);
    //数据头 + 数据
    //填充用户名及其密码
    if(default_user_mark == 1)//默认用户名
    {
        strcpy(head.user_name, default_user_name.toLatin1().data());
        strcpy(head.passwd, default_user_passwd.toLatin1().data());
    }
    else if(default_user_mark == 2)//当前用户名
    {
        strcpy(head.user_name, current_user_name.toLatin1().data());
        strcpy(head.passwd, current_user_passwd.toLatin1().data());
    }




    head.data_len = data.length();
    for(i = 0; i < sizeof(STRUCT_DATA_HEAD); i++)
    {
        block[8 + i] = ((char *)(&head))[i];
    }
    block.append(data);

    qDebug()<<"[sendData]"<<"send data: size="<<block.length()
        <<" mode"<<head.mode<<" opt"<<head.opt;
    socket->write(block);
    default_user_mark = 0;
}


//连接服务器
int cloudFile::connectTcpServer(QString ip, int port)
{
    if(client_sock_mark == 1)
    {
        clientDiscon();
    }
    client_sock_mark = 0;
    tcp_client_sock = new QTcpSocket();
    tcp_client_sock->open(QIODevice::WriteOnly);
    tcp_client_sock->connectToHost(ip, port);

    if (tcp_client_sock->waitForConnected(200))
    {
        qDebug()<<"[connectTcpServer]"<<"connect is successful";
        connect(tcp_client_sock, SIGNAL(disconnected()), this, SLOT(clientDiscon()));
        recv_pro_start = 0;
        connect(tcp_client_sock, SIGNAL(readyRead()), this, SLOT(recvMsg()));
        //connect(tcp_client_sock, SIGNAL(finish()), this, SLOT(recvMsg()));
        client_sock_mark = 1;
        return 0;
    }
    else
    {
        qDebug()<<"[connectTcpServer]"<<"connect is faild";
        tcp_client_sock->close();
        return -1;
    }


    return -1;
}


//建立tcp服务器（测试用）
int cloudFile::creatTcpServer(QString ip, int port)
{
    QTcpServer *service = new  QTcpServer(this);
    QHostAddress addr(ip);
    if(!service->listen(addr, port))
    {
        service->deleteLater();
        qDebug()<<" listen error";
        return -1;
    }

    QTcpSocket *sock = service->nextPendingConnection();
    if(sock != Q_NULLPTR)
    {
        connect(sock,SIGNAL(disconnected()),sock,SLOT(deleteLater()));
    }

    return 0;
}



//##########################################################获得类 MainWindow 的接口
MainWindow *gloable_MainWindow_int;
int setMainWindowIntf(MainWindow *m)
{
    gloable_MainWindow_int = m;
}
//传递服务器回复的信息
int serverReplyToMainWindow(int action_num, int reply_code, QString msg)
{
    if(gloable_MainWindow_int != NULL)
    {
        gloable_MainWindow_int->cloudFile_serverReply(action_num, reply_code, msg);
        qDebug()<<"================> serverReplyToMainWindow is commmit, action_num =%d"<<action_num;
    }

    return 0;
}




//####################################基于用户的文件操作存
//清空链表
int cloudFile::fileRecord_clearLink()
{
    opt_link_int->linkDelNodeAll(opt_link_head);
    return 0;
}
//新增文件
int cloudFile::fileRecord_addFile(QString name, QString content)
{
    OptLink *head = file_opt_int->fileToLink(file_opt_int->current_para_table_local, file_opt_int->current_data_table_local);
    OptLink *node = opt_link_int->linkAddNode(head);
    node->opt = fileOptCode_new;
    node->obj.clear();
    node->obj.append(name);
    node->content.clear();
    node->content.append(content);

    file_opt_int->linkToFile(head, file_opt_int->current_para_table_local, file_opt_int->current_data_table_local);
    delete head;//释放链表？？
    return 0;
}
//修改文件内容
int cloudFile::fileRecord_modifyFile(QString name, QString content)
{
    OptLink *head = file_opt_int->fileToLink(file_opt_int->current_para_table_local, file_opt_int->current_data_table_local);

    OptLink *prob = head;
    while(prob->next != NULL)
    {
        prob = prob->next;
    }
    //qDebug()<<"==================>fileRecord_modifyFile: prob->obj"<<prob->obj<<" name="<<name;
//    if(prob->obj.compare(name) == 0)
//    {
//        prob->content.clear();
//        prob->content.append(content);
//    }
//    else
    {
        OptLink *node = opt_link_int->linkAddNode(head);
        node->opt = fileOptCode_cover;
        node->obj.clear();
        node->obj.append(name);
        node->content.clear();
        node->content.append(content);
    }
   file_opt_int->linkToFile(head, file_opt_int->current_para_table_local, file_opt_int->current_data_table_local);
   delete head;//释放链表？？

    return 0;
}
//删除文件
int cloudFile::fileRecord_delFile(QString name)
{
    OptLink *head = file_opt_int->fileToLink(file_opt_int->current_para_table_local, file_opt_int->current_data_table_local);
    OptLink *node = opt_link_int->linkAddNode(head);
    node->opt = fileOptCode_del;
    node->obj.clear();
    node->obj.append(name);
    file_opt_int->linkToFile(head, file_opt_int->current_para_table_local, file_opt_int->current_data_table_local);
    delete head;//释放链表？？
    return 0;
}

//重命名
int cloudFile::fileRecord_rename(QString name, QString new_name)
{
    OptLink *head = file_opt_int->fileToLink(file_opt_int->current_para_table_local, file_opt_int->current_data_table_local);
    OptLink *node = opt_link_int->linkAddNode(head);
    node->opt = fileOptCode_new;
    node->new_name.clear();
    node->new_name.append(name);

    file_opt_int->linkToFile(head, file_opt_int->current_para_table_local, file_opt_int->current_data_table_local);
    delete head;//释放链表？？
    return 0;
}





















