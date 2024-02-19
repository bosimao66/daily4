#ifndef FILEOPT_H
#define FILEOPT_H

#include <QString>
#include "QFile.h"
#include "QDir.h"
#include <QDebug.h>
#include <QTime>
#include <QDateTime>
#include "optlink.h"



//注意最后的 /
#define FILE_OPT_WK_PATH "D:/long/cloudeFile"
#define FILE_OPT_BACK_DIR "back"
#define FILE_OPT_STORE_DIR "store"
#define FILE_OPT_USER_DIR "user"
#define FILE_OPT_USER_TABLE_DIR ".table"
#define USER_LOCAL_PARA_TABLE "local.para.table"
#define USER_LOCAL_DATA_TABLE "local.data.table"
#define USER_SYNC_PARA_TABLE "sync.para.table"
#define USER_SYNC_DATA_TABLE "sync.data.table"
#define USER_DOWNLOAD_PARA_TABLE "download.para.table"
#define USER_DOWNLOAD_DATA_TABLE "download.data.table"


#define START_CODE_0 0x66
#define START_CODE_1 0x77
#define START_CODE_2 0x88
#define START_CODE_3 0x99



//存储参数表的结构体
#define PARA_FILE_NAME_MAX_SIZE 128
typedef struct STRUCT_PARA_TABLE_ELEMENTtmp{
				char file_name[PARA_FILE_NAME_MAX_SIZE];//文件名
				unsigned char opt;//操作码
				char unique[UNIQUE_CODE_SIZE + 1];//唯一标识码（包含字符串尾部'\0'）
				unsigned int index;//在链表中的序号
				unsigned int time;//时间戳
}STRUCT_PARA_TABLE_ELEMENT;



typedef enum fileOptCodetmp{
				fileOptCode_new = 1,
				fileOptCode_cover,
				fileOptCode_del

}fileOptCode;


class FileOpt
{
public:
    FileOpt();
    QString wk_path;//注意路径最后面带横杠
    QString back_path;//备份路径
    QString back_dir;
    QString store_path;//仓库文件路径（包含文件参数散列表、数据散列表，用于存储链表内容）
    QString store_dir;
	//当前用户
    QString current_user_path;
    QString current_user_table_path;
    QString current_para_table_sync;//包含路径
    QString current_data_table_sync;
    QString current_para_table_local;
    QString current_data_table_local;
    QString current_para_table_download;
    QString current_data_table_download;
	QString current_back_path;  //当前用户备份路径


    void test();
    void init();
    int manage(int opt, QString name, QByteArray content);
    int manageAndSave(int opt, QString name, QByteArray content);
    int linkNodeAction(OptLink *node);
    int linkBackAction(OptLink *node);
    bool fileExist(QString name);
    bool floderExist(QString name);
    int createFile(QString name, QByteArray content);
    int coverFile(QString name, QByteArray content);
    int delFile(QString name);
    void setOptLinkHead(OptLink *head, OptLink *interface);
    int exeLinkAction(OptLink *head);
    int backLinkAction(OptLink *head);
    int linkToParaFile(OptLink *head, QString file_name);
    int linkToDataFile(OptLink *head, QString file_name);
    int linkToFile(OptLink *head, QString para_file, QString data_file);
    OptLink *fileToLink(QString para_name, QString data_name);
    int userFloderInit(QString user_name);
    int setCurrentUserFloder(QString user_name);
    int paraTable_toOptLink(QString para_name, OptLink *out_head);
	int paraTableAnalyze(QString local_p_table, QString local_d_table, QString download_tab);
	int syncFile();

    //操作链表接口
    OptLink *opt_link_head;
    OptLink *opt_link_int;

    OptLink *out_diff_server_head;
    OptLink *out_diff_client_head;
    OptLink *out_conflict_head;


};

#endif // FILEOPT_H










