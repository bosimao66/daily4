#ifndef TABLINK_H
#define TABLINK_H


#include "QString"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QFrame>




class TabLink
{
public:
    TabLink();
    ~TabLink();

    int uniqueNum;//从创建以来的唯一序号
    QString title;
    QString weather;
    QString mainBody;
    QString fileName;
    QString lastFileName;//上一次的保存名字
    bool lastFileNameExit;//是否存在
    QString dir;
    QString type;//文件后缀 带 .
    QString status;//状态栏信息
    QString tabName;//tab名
    int year;
    int month;
    int day;
    int week;
    int hour;
    int minute;
    int isNew;//是否为新文件的标志（1为新文件， 0为非新文件）
    //int fileNameExit;//文件名是否存在（1为存在， 0为不存在）
    int pathExit;//路径 变更动作（0为不存在，1代表存在）
    int isSaved;//是否有未保存内容，1代表都已经保存，0代表有未保存

    QWidget *uiTab;//指向文本编辑对象所在的widget
    QLineEdit *uiTitle;//指向标题控件
    QLineEdit *uiDateYear;//指向 日期“年” 这个控件
    QLineEdit *uiDateMonth;//指向 日期“月” 这个控件
    QLineEdit *uiDateDay;//指向 日期“日” 这个控件
    QLineEdit *uiTimeHour;//指向 时间“小时” 这个控件
    QLineEdit *uiTimeMinute;//指向 时间“分” 这个控件
    QLineEdit *uiWeather;//指向 天气 控件
    QLineEdit *uiWeek;//指向 星期 控件
    QTextEdit *uiMainBody;//指向 主体文本编辑 控件
    QFrame *uiTimeBar;
    int uiIsAvailable;//控件是否赋值的标记

    TabLink *next;//指向自己
    bool isDelete;//调用fun_destory 函数摧毁对象后，此处设置为false
    //int index;//注意：本例子中，本对象不被多个链表所使用，idnex因此唯一；未被使用时
                //index = -1;
    bool isDialy;//是否为日记文本的标记，默认为不是，放在初始化
    int isChangeCnt;//
    //int fileScanObj_currentIndex;//


    //方法
    TabLink *fun_create();//返回链表头
    int fun_getTotalNum(TabLink *head);
    int fun_getIndex(TabLink *head);
    TabLink *fun_add(TabLink *head);
    void fun_del(TabLink *head, int index);
    TabLink *fun_getTabByIndex(TabLink *head, int index);
    TabLink *fun_getTabByUniqueNum(TabLink *head, int num);
    void fun_destory(TabLink *head);

    void fun_test();
    void fun_showInfo(TabLink *head);

    //专用方法
    TabLink *fun_fileExit(TabLink *head, QString dir, QString fileName);//遍历链表，判断文件是否存在于此链表中


};

#endif // TABLINK_H
