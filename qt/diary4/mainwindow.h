#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QTWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>

#include<QKeyEvent>
#include "tablink.h"
#include "cloudfile.h"


//#####################################文件浏览界面
//文件widget个数
#define VAR_FILE_SCAN_MAX_ENTRY 1000
//文件浏览界面 日记的范围
//#define FILE_SCAN_RANGE_ALL 1
//#define FILE_SCAN_RANGE_TODAY 2
//#define FILE_SCAN_RANGE_YESTODAY 3
//#define FILE_SCAN_RANGE_WEEK 4
//#define FILE_SCAN_RANGE_MONTH 5
#define EDIT_TAB_ADD_MODE_OPEN_AS_DIALY 6
//文件浏览模式
#define FILE_SCAN_FILE_MODE 1
#define FILE_SCAN_DIALY_MODE 2
//文件类型
#define FILE_SCAN_FILETYPE_FILE 1
#define FILE_SCAN_FILETYPE_FLODER 2
//默认工作路径（注：需要完善，但是现在不想做，以后再来搞；可考虑允许用户自己设置，而非固定）
#define FILE_SCAN_DEFAULT_WORK_PATH "D:/long/dialy"
//样式背景颜色 赤red橙orange黄yellow绿green青cyan蓝blue紫purple
#define STYLE_SHHET_BACK_COLOR_RED "background-color: rgb(255, 0, 0);"
#define STYLE_SHHET_BACK_COLOR_ORANGE "background-color: rgb(255, 170, 0);"
#define STYLE_SHHET_BACK_COLOR_YELLOW "background-color: rgb(255, 255, 0);"
#define STYLE_SHHET_BACK_COLOR_GREEN "background-color: rgb(0, 255, 0);"
#define STYLE_SHHET_BACK_COLOR_CYAN "background-color: rgb(0, 255, 255);"
#define STYLE_SHHET_BACK_COLOR_BLUE "background-color: rgb(0, 170, 255);"
#define STYLE_SHHET_BACK_COLOR_PURPLE "background-color: rgb(85, 0, 255);"
#define STYLE_SHHET_BACK_COLOR_BLACK "background-color: rgb(0, 0, 0);"
#define STYLE_SHHET_BACK_COLOR_WHITE "background-color: rgb(255, 255, 255);"
//结构体：表示一条日记文件的属性
typedef struct STRUCT_DATE_INFOtmp{
    int month;
    int day;
    int num;//从0开始
}STRUCT_DATE_INFO;

//var_fileScan_scopMode
#define FILE_SCAN_SCOP_TODAY 1
#define FILE_SCAN_SCOP_YESTODAY 2
#define FILE_SCAN_SCOP_THIS_WEEK 3
#define FILE_SCAN_SCOP_THIS_MOTH 4
#define FILE_SCAN_SCOP_ALL 5
#define FILE_SCAN_SCOP_ASSIGN_DAY 6


//#####################################文本编辑模块
//tab添加模式
#define EDIT_TAB_ADD_MODE_NEW 1
#define EDIT_TAB_ADD_MODE_OPEN 2
#define EDIT_TAB_ADD_MODE_OPEN_ASDIALY 3
//tab关闭模式
#define EDIT_TAB_DEL_MODE_CURRENT 1
#define EDIT_TAB_DEL_MODE_BYINDEX 2
#define EDIT_TAB_DEL_MODE_ALL 3
#define EDIT_TAB_DEL_MODE_OTHER 4
//文件打开模式
#define EDIT_FILE_OPEN_GENERAL 1
#define EDIT_FILE_OPEN_ANALYSE 2


//#####################################用户信息
#define USER_INFO_USER_DEFAULT_NAME "long"


//#####################################################################云端同步功能
//配置内容结构体（注：存于额配置文件中的参数名和结构体成员名一一对应，只是参数名的字母都是大写）
typedef struct strcut_CLOUD_FILE_CFGtmp{
    qint8 rem_passwd;  //记住密码（0代表不记住密码  1代表记住密码）
    unsigned char server_ip[4];  //服务器ip
    qint32 port;     //端口
    qint32 auto_sync_para;   //自动同步时间间隔参数（单位分钟）
    qint8 auto_sync_en;     //使能自动同步（0 代表非使能  1代表使能， 默认值 0）
    qint8 startup_sync_en;     //打开应用，就尝试连接服务器，并进行同步（0 否  1 是  默认值 0）
    char user_name[64];     //记住的用户名
    char user_passwd[64];    //记住的密码（已加密）
}strcut_CLOUD_FILE_CFG;

#define PARA_rem_passwd   "REM_PASSWD"
#define PARA_server_ip   "SERVER_IP"
#define PARA_port   "PORT"
#define PARA_auto_sync_para   "AUTO_SYNC_PARA"
#define PARA_auto_sync_en   "AUTO_SYNC_EN"
#define PARA_startup_sync_en   "START_UP_SYNC"
#define PARA_user_name   "USER_NAME"
#define PARA_user_passwd   "USER_PASSWD"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //##############################################################文件浏览界面模块（因为前期没有考虑用链表，所以这部分比较混乱，重新修改也麻烦）
    //函数
    void fileScanCreateEntry(QString btnColor, QString btnText, QString labelColor, QString labelText);//创建一个文件 widget
    void fileScanClear();//浏览页面的清除
    void fileSacnUpdate(int mode, QString dir);//浏览页面更新
    void fileSacnUpdate_fileMode(QString dir);//浏览页面更新-文件模式
    void fileSacnUpdate_dialyMode(QString dir);//浏览页面更新-日记模式

    //变量
    //int flag_fileScanRange;//文件浏览的范围
    QString var_fileScanDir;//工作目录(不带斜杠)
    QString var_userDefaultDir;//用户自定义工作路径（可能要结合保存文件，才能在下一次启动应用时调用）
    int var_fileScanMode;//文件浏览模式（FILE_SCAN_FILE_MODE、FILE_SCAN_DIALY_MODE）
    QString var_sheetColor[9];

    QWidget *ui_fileBack[VAR_FILE_SCAN_MAX_ENTRY];//幕布元素
    QPushButton *ui_fileIcon[VAR_FILE_SCAN_MAX_ENTRY];//图标元素
    QLabel *ui_fileName[VAR_FILE_SCAN_MAX_ENTRY];//文件名元素
    int type_fileType[VAR_FILE_SCAN_MAX_ENTRY];//文件类型（1--文件 FILE_SCAN_FILETYPE_FILE   2--文件夹 FILE_SCAN_FILETYPE_FLODER）
    int var_fileObjNum;//文件对象个数
    QString varFileScan_fileName[VAR_FILE_SCAN_MAX_ENTRY];
    //相关控件指针及其标志
     QSpacerItem *var_qspacerPoint;//文件浏览布局下的垂直间隔器
     QVBoxLayout *var_fileScanLayoutPoint;//文件浏览 scrollArea 控件幕布下的布局
     QWidget *var_scrollAreaWidgetContents;//文件浏览 scrollArea 控件的一个幕布
    int flag_qspacerExit;//垂直间隔器 存在标志（1代表存在， 0代表不存在）
    QLineEdit *var_fileScanLineEditDir;//工作目录输入控件
    QLabel *var_fileScanLabelDir;//工作目录输入提示控件

    //用于日记浏览模式
    STRUCT_DATE_INFO var_fileScan_dateInfo[VAR_FILE_SCAN_MAX_ENTRY];
    int var_fileScan_dayTotalNum;
    int var_fileScanCurrentIndex;
    QString var_fileScanCurrentFileName;
    //int var_fileScan_dateMinMonth;
    //int var_fileScan_dateMinDay;
    //int var_fileScan_dateMaxMonth;
    //int var_fileScan_dateMaxDay;
    int var_fileScan_scopMode;
    //##############################################################文本编辑模块 Edit
    //注：(1)命名规则又有了些改变，但是之前的命名也不想改了，太麻烦 (2)可以改进为多个tab显示，而非固定的最多5个tab，比较麻烦
    //（3）现在改用类链表了，大刀阔斧，修改了很多，效果也很明显：逻辑清晰，测验简单；可以多个tab显示，用时分配，不用时销毁。
    QString varEdit_weekStr[8];
    void funEdit_init();
    void funEdit_tabUpdate(TabLink *node);
    void funEdit_tabUpdateAll(TabLink *head);
    void funEdit_saveToLink(TabLink *node);
    void funEdit_saveToLinkAll(TabLink *node);
    void funEdit_tabAdd(int mode, QString dir, QString fileName);
    void funEdit_tabDel(int mode, int index);
    void funEdit_tabWidgetCreate(TabLink *node);//tab界面创建
    void funEdit_fileOpen(int mode, QString dir, QString fileName);//暂时设定为只支持.txt文件
    int funEdit_fileSave(int tabIndex);
    void funEdit_contentChangedAction(int num, int uiNum);//内容更改 槽函数


    QTabWidget *ui_tabWidget;
    TabLink link_method;//链表，用于描述tabWidget，提供方法
    TabLink *link_head;//链表头
    //##############################################################其他
    void keyPressEvent(QKeyEvent *event);
    QDialog *findDlg;
    QLineEdit *findLineEdit;
    int varCalendar_dayOfYear;
    int varFont_size;

    //##############################################################用户信息
    QString varUser_name;
    void funUser_init();
    QDialog *userDlg;
     QLineEdit *userNameEdit ;
     void saveCfgFile(QString dir, QString fileName, QString entry, QString para);
     void saveCfgFile_byName(QString name, QString entry, QString para);
     void getParaFromCfgFile(QString dir, QString entry, QString fileName, QString *para);
     int getParaFromCfgFile_byName(QString name, QString entry, QString *para);
     QLineEdit *workDirEdit;
     int flag_leftHide;
     void funUser_setPara(QString prompt);

     //#############################################################云端同步功能
    int cloudInit();
    //通过接口使用cloudFile 类
    cloudFile *cloud_file_int;
    strcut_CLOUD_FILE_CFG cloud_file_cfg;
    int cloudCfgFileGetInfo(QString file_name, strcut_CLOUD_FILE_CFG *s);
    int cloudCfgSaveToFile(QString file_name, strcut_CLOUD_FILE_CFG s);
    int scloudCfgInfoShow(strcut_CLOUD_FILE_CFG s);
    int cloudCfgExe(strcut_CLOUD_FILE_CFG s);
    int cloudCfgGetBaordCfgInfo(strcut_CLOUD_FILE_CFG *s);
    QString app_work_path;      //应用工作目录，和 var_fileScanDir不同，一个是wkpath 另一个是 wkpath/user/<user_name>

    int cloudFile_serverReply(int action_num, int reply_code, QString msg);
    int cloud_server_time_out_cnt;  //超时计数
    int cloud_server_time_out_num;  //超时触发点
    int cloud_server_time_out_en;   //使能
    int cloud_server_time_count_disp_en;  //计时显示
    QString cloud_server_time_count_str;   //字符串
    int cloudFile_timeoutPro();
    int cloudFile_timeCountDisp();
    //密码确认对话框
    QDialog *user_destory_diag;
    void UserDestory_diag_int();
    QLineEdit *edit_passwd_1;
    QLineEdit *edit_passwd_2;
    QLabel *user_destory_label_2;
    int user_destory_cancel_mark;
    //自动同步处理
    int time_updata_count;
    int cloud_file_auto_sync_en;
    int cloud_file_auto_sync_para;
    int cloud_file_sync_type;    //同步类型  1-按键同步  2-自动同步
    int cloudFile_autoSync(int en, int para);
    int cloudFile_oneShootSync(void);
    //密码修改使能 标志
    int cfg_passwd_change_mark;
    int cfg_passwd_change_en;

    //#############################################################槽
private slots:
    void on_pushButton_clicked();//【自动槽】测试按钮（暂时是头像按钮）
    void timerUpdate();//系统时间更新
    void on_calendarWidget_clicked(const QDate &date);//【自动槽】日历点击事件
    void on_btn_fileScanAll_clicked();//【自动槽】显示范围
    void on_btn_pathSet_clicked();//【自动槽】工作路径
    void fileButtonOnclick(int index);//文件对象按钮点击响应
    void funFileScan_ModeChange(int mode);
    void fileScanFileDel();
    void fileScanFileAdd();
    void fileScanScop_today();
    void fileScanScop_yestoday();
    void fileScanScop_this_week();
    void fileScanScop_this_month();

    //新建、打开、保存和另存动作
    void funEditAction_new();
    void funEditAction_open();
    void funEditAction_save();
    void funEditAction_saveAll();
    void funEditAction_saveAs();
    void funEditAction_close();
    void funEditAction_closeAll();
    void funEditAction_closeOther();
    //快捷键响应
    void showFindText();

    void userSetOk();
    void userSetCancel();

    void on_pushButton_hideLeft_clicked();

    void on_action_showLeft_triggered();
    //其他
    void funOther_setFontSize(int mode);
    //==========================云同步功能相关槽
    int bthUserUserClicked();
    int bthCfgCfgClicked();
    int mydebug();
    int cfgCfgNotSavePrompt();      //更改未保存提示
    int bthCfgSaveClicked();
    int bthCfgCancelClicked();
    int cfgUserPasswdChanged();
    int bthUserLoginClicked();//登录
    int bthUserRegisterClicked();//注册
    int bthUserLogoutClicked();//注销
    int bthUserSyncClicked();//同步
    int bthUserDestoryClicked();//销毁
    int bthUserlocalCreateClicked();//本地创建
    int bthUserSwitchClicked();//用户切换
    int bthUserConnectTestClicked();//连接
    int UserDestory_passwdConfirm();//密码确认
    int UserDestory_passwdCancel();



private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
