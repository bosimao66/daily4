/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_new;
    QAction *action_save;
    QAction *action_open;
    QAction *action_saveAs;
    QAction *action_menuNew;
    QAction *action_menuSave;
    QAction *action_menuOpen;
    QAction *action_menuSaveAs;
    QAction *action_close;
    QAction *action_menuClose;
    QAction *action_closeAll;
    QAction *action_menuCloseAll;
    QAction *action_closeOther;
    QAction *action_menuCloseOther;
    QAction *action_saveAll;
    QAction *action_menuSaveAll;
    QAction *action_showLeft;
    QAction *action_menuShowLeft;
    QAction *action_fontAdd;
    QAction *action_fontSub;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_4;
    QFrame *frame_left;
    QVBoxLayout *verticalLayout;
    QFrame *frame_2;
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton_hideLeft;
    QLabel *label_systemTime;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QPushButton *btn_fileScanFileMode;
    QPushButton *btn_fileScanDialyMode;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_fileScanPath;
    QLineEdit *lineEdit_pathSet;
    QPushButton *btn_pathSet;
    QWidget *widget_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_fileScanTmp1;
    QLabel *label_fileScanTmp1;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_8;
    QPushButton *btn_fileScanToday;
    QPushButton *btn_fileScanYestoday;
    QPushButton *btn_fileScanWeek;
    QPushButton *btn_fileScanMonth;
    QPushButton *btn_fileScanAll;
    QWidget *widget_actionMore;
    QPushButton *btn_del;
    QPushButton *btn_new;
    QCalendarWidget *calendarWidget;
    QSpacerItem *verticalSpacer;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *lineEdit_title_1;
    QFrame *frame_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_time;
    QLineEdit *lineEdit_dateYear_1;
    QLabel *label_year;
    QLineEdit *lineEdit_dateMonth_1;
    QLabel *label_mounth;
    QLineEdit *lineEdit_dateDay_1;
    QLabel *label_day;
    QLineEdit *lineEdit_timeHour_1;
    QLabel *label_hour;
    QLineEdit *lineEdit_timeMinute_1;
    QLabel *label_minute;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_weather;
    QLineEdit *lineEdit_weather_1;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_week;
    QLineEdit *lineEdit_week_1;
    QTextEdit *textEdit_mainBody_1;
    QLabel *label_statusBar_1;
    QWidget *widget_6;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_7;
    QVBoxLayout *verticalLayout_5;
    QPushButton *pushButton_user_user;
    QLabel *label_user_prompt;
    QWidget *widget_user_cont;
    QLineEdit *lineEdit_user_name;
    QLineEdit *lineEdit_user_passwd;
    QPushButton *pushButton_user_login;
    QPushButton *pushButton_user_register;
    QPushButton *pushButton_user_sync;
    QPushButton *pushButton_user_logout;
    QLabel *label_user_username;
    QLabel *label_user_passwd;
    QPushButton *pushButton_user_destroy;
    QCheckBox *checkBox_passwd_rem;
    QPushButton *pushButton_user_localCreate;
    QPushButton *pushButton_user_switch;
    QPushButton *pushButton_user_connectTest;
    QPushButton *pushButton_cfg_cfg;
    QLabel *label_cfg_prompt;
    QWidget *widget_cfg_cont;
    QLabel *label_cfg_serverip;
    QLabel *label_cfg_tcpport;
    QLabel *label_cfg_autosync;
    QLineEdit *lineEdit_cfg_ip;
    QLineEdit *lineEdit_cfg_port;
    QLineEdit *lineEdit_cfg_autosync;
    QPushButton *pushButton_cfg_save;
    QPushButton *pushButton_cfg_cancel;
    QCheckBox *checkBox_cfg_autonsync;
    QCheckBox *checkBox_cfg_power_on_sync;
    QPushButton *pushButton_cfg_autosync;
    QPushButton *pushButton_debug;
    QSpacerItem *verticalSpacer_4;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_4;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1048, 783);
        action_new = new QAction(MainWindow);
        action_new->setObjectName(QStringLiteral("action_new"));
        action_save = new QAction(MainWindow);
        action_save->setObjectName(QStringLiteral("action_save"));
        action_open = new QAction(MainWindow);
        action_open->setObjectName(QStringLiteral("action_open"));
        action_saveAs = new QAction(MainWindow);
        action_saveAs->setObjectName(QStringLiteral("action_saveAs"));
        action_menuNew = new QAction(MainWindow);
        action_menuNew->setObjectName(QStringLiteral("action_menuNew"));
        action_menuSave = new QAction(MainWindow);
        action_menuSave->setObjectName(QStringLiteral("action_menuSave"));
        action_menuOpen = new QAction(MainWindow);
        action_menuOpen->setObjectName(QStringLiteral("action_menuOpen"));
        action_menuSaveAs = new QAction(MainWindow);
        action_menuSaveAs->setObjectName(QStringLiteral("action_menuSaveAs"));
        action_close = new QAction(MainWindow);
        action_close->setObjectName(QStringLiteral("action_close"));
        action_menuClose = new QAction(MainWindow);
        action_menuClose->setObjectName(QStringLiteral("action_menuClose"));
        action_closeAll = new QAction(MainWindow);
        action_closeAll->setObjectName(QStringLiteral("action_closeAll"));
        action_menuCloseAll = new QAction(MainWindow);
        action_menuCloseAll->setObjectName(QStringLiteral("action_menuCloseAll"));
        action_closeOther = new QAction(MainWindow);
        action_closeOther->setObjectName(QStringLiteral("action_closeOther"));
        action_menuCloseOther = new QAction(MainWindow);
        action_menuCloseOther->setObjectName(QStringLiteral("action_menuCloseOther"));
        action_saveAll = new QAction(MainWindow);
        action_saveAll->setObjectName(QStringLiteral("action_saveAll"));
        action_menuSaveAll = new QAction(MainWindow);
        action_menuSaveAll->setObjectName(QStringLiteral("action_menuSaveAll"));
        action_showLeft = new QAction(MainWindow);
        action_showLeft->setObjectName(QStringLiteral("action_showLeft"));
        action_menuShowLeft = new QAction(MainWindow);
        action_menuShowLeft->setObjectName(QStringLiteral("action_menuShowLeft"));
        action_fontAdd = new QAction(MainWindow);
        action_fontAdd->setObjectName(QStringLiteral("action_fontAdd"));
        action_fontSub = new QAction(MainWindow);
        action_fontSub->setObjectName(QStringLiteral("action_fontSub"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_4 = new QHBoxLayout(centralWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        frame_left = new QFrame(centralWidget);
        frame_left->setObjectName(QStringLiteral("frame_left"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame_left->sizePolicy().hasHeightForWidth());
        frame_left->setSizePolicy(sizePolicy);
        frame_left->setMinimumSize(QSize(240, 0));
        frame_left->setMaximumSize(QSize(250, 16777215));
        frame_left->setFrameShape(QFrame::StyledPanel);
        frame_left->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame_left);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame_2 = new QFrame(frame_left);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame_2->sizePolicy().hasHeightForWidth());
        frame_2->setSizePolicy(sizePolicy1);
        frame_2->setMinimumSize(QSize(0, 60));
        frame_2->setMaximumSize(QSize(240, 60));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        pushButton = new QPushButton(frame_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(40, 10, 45, 45));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy2);
        pushButton->setMinimumSize(QSize(45, 45));
        pushButton->setMaximumSize(QSize(45, 45));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(16);
        pushButton->setFont(font);
        pushButton->setStyleSheet(QStringLiteral("background-color: rgb(85, 255, 0);"));
        label = new QLabel(frame_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(100, 20, 111, 16));
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(100, 40, 111, 16));
        pushButton_hideLeft = new QPushButton(frame_2);
        pushButton_hideLeft->setObjectName(QStringLiteral("pushButton_hideLeft"));
        pushButton_hideLeft->setGeometry(QRect(210, 0, 25, 23));
        pushButton_hideLeft->setMaximumSize(QSize(25, 16777215));

        verticalLayout->addWidget(frame_2);

        label_systemTime = new QLabel(frame_left);
        label_systemTime->setObjectName(QStringLiteral("label_systemTime"));
        label_systemTime->setMaximumSize(QSize(240, 16777215));

        verticalLayout->addWidget(label_systemTime);

        widget_5 = new QWidget(frame_left);
        widget_5->setObjectName(QStringLiteral("widget_5"));
        widget_5->setMinimumSize(QSize(0, 32));
        widget_5->setMaximumSize(QSize(240, 32));
        horizontalLayout_5 = new QHBoxLayout(widget_5);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(1, 1, 1, 1);
        label_3 = new QLabel(widget_5);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(60, 16777215));

        horizontalLayout_5->addWidget(label_3);

        btn_fileScanFileMode = new QPushButton(widget_5);
        btn_fileScanFileMode->setObjectName(QStringLiteral("btn_fileScanFileMode"));
        sizePolicy2.setHeightForWidth(btn_fileScanFileMode->sizePolicy().hasHeightForWidth());
        btn_fileScanFileMode->setSizePolicy(sizePolicy2);
        btn_fileScanFileMode->setMaximumSize(QSize(70, 30));

        horizontalLayout_5->addWidget(btn_fileScanFileMode);

        btn_fileScanDialyMode = new QPushButton(widget_5);
        btn_fileScanDialyMode->setObjectName(QStringLiteral("btn_fileScanDialyMode"));
        sizePolicy2.setHeightForWidth(btn_fileScanDialyMode->sizePolicy().hasHeightForWidth());
        btn_fileScanDialyMode->setSizePolicy(sizePolicy2);
        btn_fileScanDialyMode->setMaximumSize(QSize(70, 30));

        horizontalLayout_5->addWidget(btn_fileScanDialyMode);


        verticalLayout->addWidget(widget_5);

        widget_4 = new QWidget(frame_left);
        widget_4->setObjectName(QStringLiteral("widget_4"));
        widget_4->setMaximumSize(QSize(240, 30));
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(10, 1, 10, 1);
        label_fileScanPath = new QLabel(widget_4);
        label_fileScanPath->setObjectName(QStringLiteral("label_fileScanPath"));
        label_fileScanPath->setMaximumSize(QSize(240, 25));

        horizontalLayout_3->addWidget(label_fileScanPath);

        lineEdit_pathSet = new QLineEdit(widget_4);
        lineEdit_pathSet->setObjectName(QStringLiteral("lineEdit_pathSet"));
        lineEdit_pathSet->setMaximumSize(QSize(150, 25));

        horizontalLayout_3->addWidget(lineEdit_pathSet);

        btn_pathSet = new QPushButton(widget_4);
        btn_pathSet->setObjectName(QStringLiteral("btn_pathSet"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btn_pathSet->sizePolicy().hasHeightForWidth());
        btn_pathSet->setSizePolicy(sizePolicy3);
        btn_pathSet->setMaximumSize(QSize(40, 25));

        horizontalLayout_3->addWidget(btn_pathSet);


        verticalLayout->addWidget(widget_4);

        widget_2 = new QWidget(frame_left);
        widget_2->setObjectName(QStringLiteral("widget_2"));
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        widget_2->setMinimumSize(QSize(0, 180));
        widget_2->setMaximumSize(QSize(240, 180));
        scrollArea = new QScrollArea(widget_2);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 180, 180));
        scrollArea->setMinimumSize(QSize(0, 180));
        scrollArea->setMaximumSize(QSize(180, 180));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 178, 178));
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_3->setSpacing(2);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(5, 5, 5, 5);
        widget = new QWidget(scrollAreaWidgetContents);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(120, 37));
        widget->setMaximumSize(QSize(200, 35));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(2, 2, 2, 2);
        btn_fileScanTmp1 = new QPushButton(widget);
        btn_fileScanTmp1->setObjectName(QStringLiteral("btn_fileScanTmp1"));
        sizePolicy2.setHeightForWidth(btn_fileScanTmp1->sizePolicy().hasHeightForWidth());
        btn_fileScanTmp1->setSizePolicy(sizePolicy2);
        btn_fileScanTmp1->setMinimumSize(QSize(32, 32));
        btn_fileScanTmp1->setMaximumSize(QSize(32, 32));

        horizontalLayout_2->addWidget(btn_fileScanTmp1);

        label_fileScanTmp1 = new QLabel(widget);
        label_fileScanTmp1->setObjectName(QStringLiteral("label_fileScanTmp1"));

        horizontalLayout_2->addWidget(label_fileScanTmp1);


        verticalLayout_3->addWidget(widget);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        scrollArea->setWidget(scrollAreaWidgetContents);
        widget_3 = new QWidget(widget_2);
        widget_3->setObjectName(QStringLiteral("widget_3"));
        widget_3->setGeometry(QRect(180, 0, 58, 180));
        sizePolicy2.setHeightForWidth(widget_3->sizePolicy().hasHeightForWidth());
        widget_3->setSizePolicy(sizePolicy2);
        widget_3->setMinimumSize(QSize(0, 0));
        widget_3->setMaximumSize(QSize(58, 180));
        verticalLayout_8 = new QVBoxLayout(widget_3);
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setContentsMargins(11, 11, 11, 11);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        btn_fileScanToday = new QPushButton(widget_3);
        btn_fileScanToday->setObjectName(QStringLiteral("btn_fileScanToday"));
        sizePolicy1.setHeightForWidth(btn_fileScanToday->sizePolicy().hasHeightForWidth());
        btn_fileScanToday->setSizePolicy(sizePolicy1);
        btn_fileScanToday->setMaximumSize(QSize(50, 30));

        verticalLayout_8->addWidget(btn_fileScanToday);

        btn_fileScanYestoday = new QPushButton(widget_3);
        btn_fileScanYestoday->setObjectName(QStringLiteral("btn_fileScanYestoday"));
        sizePolicy1.setHeightForWidth(btn_fileScanYestoday->sizePolicy().hasHeightForWidth());
        btn_fileScanYestoday->setSizePolicy(sizePolicy1);
        btn_fileScanYestoday->setMaximumSize(QSize(50, 30));

        verticalLayout_8->addWidget(btn_fileScanYestoday);

        btn_fileScanWeek = new QPushButton(widget_3);
        btn_fileScanWeek->setObjectName(QStringLiteral("btn_fileScanWeek"));
        sizePolicy1.setHeightForWidth(btn_fileScanWeek->sizePolicy().hasHeightForWidth());
        btn_fileScanWeek->setSizePolicy(sizePolicy1);
        btn_fileScanWeek->setMaximumSize(QSize(50, 30));

        verticalLayout_8->addWidget(btn_fileScanWeek);

        btn_fileScanMonth = new QPushButton(widget_3);
        btn_fileScanMonth->setObjectName(QStringLiteral("btn_fileScanMonth"));
        sizePolicy1.setHeightForWidth(btn_fileScanMonth->sizePolicy().hasHeightForWidth());
        btn_fileScanMonth->setSizePolicy(sizePolicy1);
        btn_fileScanMonth->setMaximumSize(QSize(50, 30));

        verticalLayout_8->addWidget(btn_fileScanMonth);

        btn_fileScanAll = new QPushButton(widget_3);
        btn_fileScanAll->setObjectName(QStringLiteral("btn_fileScanAll"));
        sizePolicy1.setHeightForWidth(btn_fileScanAll->sizePolicy().hasHeightForWidth());
        btn_fileScanAll->setSizePolicy(sizePolicy1);
        btn_fileScanAll->setMaximumSize(QSize(50, 30));

        verticalLayout_8->addWidget(btn_fileScanAll);


        verticalLayout->addWidget(widget_2);

        widget_actionMore = new QWidget(frame_left);
        widget_actionMore->setObjectName(QStringLiteral("widget_actionMore"));
        widget_actionMore->setMinimumSize(QSize(0, 37));
        widget_actionMore->setMaximumSize(QSize(250, 37));
        btn_del = new QPushButton(widget_actionMore);
        btn_del->setObjectName(QStringLiteral("btn_del"));
        btn_del->setGeometry(QRect(0, 0, 35, 35));
        sizePolicy2.setHeightForWidth(btn_del->sizePolicy().hasHeightForWidth());
        btn_del->setSizePolicy(sizePolicy2);
        btn_del->setMinimumSize(QSize(35, 35));
        btn_del->setMaximumSize(QSize(35, 35));
        btn_new = new QPushButton(widget_actionMore);
        btn_new->setObjectName(QStringLiteral("btn_new"));
        btn_new->setGeometry(QRect(50, 0, 35, 35));
        sizePolicy2.setHeightForWidth(btn_new->sizePolicy().hasHeightForWidth());
        btn_new->setSizePolicy(sizePolicy2);
        btn_new->setMinimumSize(QSize(35, 35));
        btn_new->setMaximumSize(QSize(35, 35));

        verticalLayout->addWidget(widget_actionMore);

        calendarWidget = new QCalendarWidget(frame_left);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        sizePolicy1.setHeightForWidth(calendarWidget->sizePolicy().hasHeightForWidth());
        calendarWidget->setSizePolicy(sizePolicy1);
        calendarWidget->setMaximumSize(QSize(250, 180));

        verticalLayout->addWidget(calendarWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_4->addWidget(frame_left);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMinimumSize(QSize(502, 0));
        tabWidget->setMaximumSize(QSize(802, 16777215));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        lineEdit_title_1 = new QLineEdit(tab);
        lineEdit_title_1->setObjectName(QStringLiteral("lineEdit_title_1"));
        lineEdit_title_1->setMinimumSize(QSize(500, 0));
        lineEdit_title_1->setMaximumSize(QSize(800, 16777215));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setPointSize(13);
        lineEdit_title_1->setFont(font1);

        verticalLayout_2->addWidget(lineEdit_title_1);

        frame_3 = new QFrame(tab);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setMinimumSize(QSize(500, 0));
        frame_3->setMaximumSize(QSize(800, 40));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_3);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(10, 0, 10, 0);
        label_time = new QLabel(frame_3);
        label_time->setObjectName(QStringLiteral("label_time"));
        label_time->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(label_time);

        lineEdit_dateYear_1 = new QLineEdit(frame_3);
        lineEdit_dateYear_1->setObjectName(QStringLiteral("lineEdit_dateYear_1"));
        lineEdit_dateYear_1->setMaximumSize(QSize(35, 16777215));

        horizontalLayout->addWidget(lineEdit_dateYear_1);

        label_year = new QLabel(frame_3);
        label_year->setObjectName(QStringLiteral("label_year"));
        label_year->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(label_year);

        lineEdit_dateMonth_1 = new QLineEdit(frame_3);
        lineEdit_dateMonth_1->setObjectName(QStringLiteral("lineEdit_dateMonth_1"));
        lineEdit_dateMonth_1->setMaximumSize(QSize(25, 16777215));

        horizontalLayout->addWidget(lineEdit_dateMonth_1);

        label_mounth = new QLabel(frame_3);
        label_mounth->setObjectName(QStringLiteral("label_mounth"));
        label_mounth->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(label_mounth);

        lineEdit_dateDay_1 = new QLineEdit(frame_3);
        lineEdit_dateDay_1->setObjectName(QStringLiteral("lineEdit_dateDay_1"));
        lineEdit_dateDay_1->setMaximumSize(QSize(25, 16777215));

        horizontalLayout->addWidget(lineEdit_dateDay_1);

        label_day = new QLabel(frame_3);
        label_day->setObjectName(QStringLiteral("label_day"));
        label_day->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(label_day);

        lineEdit_timeHour_1 = new QLineEdit(frame_3);
        lineEdit_timeHour_1->setObjectName(QStringLiteral("lineEdit_timeHour_1"));
        lineEdit_timeHour_1->setMaximumSize(QSize(25, 16777215));

        horizontalLayout->addWidget(lineEdit_timeHour_1);

        label_hour = new QLabel(frame_3);
        label_hour->setObjectName(QStringLiteral("label_hour"));
        label_hour->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(label_hour);

        lineEdit_timeMinute_1 = new QLineEdit(frame_3);
        lineEdit_timeMinute_1->setObjectName(QStringLiteral("lineEdit_timeMinute_1"));
        lineEdit_timeMinute_1->setMaximumSize(QSize(25, 16777215));

        horizontalLayout->addWidget(lineEdit_timeMinute_1);

        label_minute = new QLabel(frame_3);
        label_minute->setObjectName(QStringLiteral("label_minute"));
        label_minute->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(label_minute);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        label_weather = new QLabel(frame_3);
        label_weather->setObjectName(QStringLiteral("label_weather"));
        label_weather->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(label_weather);

        lineEdit_weather_1 = new QLineEdit(frame_3);
        lineEdit_weather_1->setObjectName(QStringLiteral("lineEdit_weather_1"));
        lineEdit_weather_1->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(lineEdit_weather_1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        label_week = new QLabel(frame_3);
        label_week->setObjectName(QStringLiteral("label_week"));
        label_week->setMaximumSize(QSize(40, 16777215));

        horizontalLayout->addWidget(label_week);

        lineEdit_week_1 = new QLineEdit(frame_3);
        lineEdit_week_1->setObjectName(QStringLiteral("lineEdit_week_1"));
        lineEdit_week_1->setMaximumSize(QSize(30, 16777215));

        horizontalLayout->addWidget(lineEdit_week_1);

        horizontalLayout->setStretch(0, 2);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(3, 1);
        horizontalLayout->setStretch(4, 1);
        horizontalLayout->setStretch(5, 1);
        horizontalLayout->setStretch(6, 1);
        horizontalLayout->setStretch(7, 1);
        horizontalLayout->setStretch(8, 1);
        horizontalLayout->setStretch(9, 1);
        horizontalLayout->setStretch(10, 1);
        horizontalLayout->setStretch(11, 4);
        horizontalLayout->setStretch(12, 2);
        horizontalLayout->setStretch(13, 6);
        horizontalLayout->setStretch(14, 4);
        horizontalLayout->setStretch(15, 2);
        horizontalLayout->setStretch(16, 2);

        verticalLayout_2->addWidget(frame_3);

        textEdit_mainBody_1 = new QTextEdit(tab);
        textEdit_mainBody_1->setObjectName(QStringLiteral("textEdit_mainBody_1"));
        textEdit_mainBody_1->setMinimumSize(QSize(500, 0));
        textEdit_mainBody_1->setMaximumSize(QSize(800, 16777215));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setPointSize(10);
        textEdit_mainBody_1->setFont(font2);
        textEdit_mainBody_1->setAutoFormatting(QTextEdit::AutoNone);
        textEdit_mainBody_1->setTabStopWidth(2);
        textEdit_mainBody_1->setAcceptRichText(false);

        verticalLayout_2->addWidget(textEdit_mainBody_1);

        label_statusBar_1 = new QLabel(tab);
        label_statusBar_1->setObjectName(QStringLiteral("label_statusBar_1"));
        label_statusBar_1->setMinimumSize(QSize(500, 0));
        label_statusBar_1->setMaximumSize(QSize(800, 16777215));

        verticalLayout_2->addWidget(label_statusBar_1);

        tabWidget->addTab(tab, QString());

        horizontalLayout_4->addWidget(tabWidget);

        widget_6 = new QWidget(centralWidget);
        widget_6->setObjectName(QStringLiteral("widget_6"));
        widget_6->setMaximumSize(QSize(300, 1600));
        widget_6->setStyleSheet(QStringLiteral(""));
        verticalLayout_4 = new QVBoxLayout(widget_6);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        widget_7 = new QWidget(widget_6);
        widget_7->setObjectName(QStringLiteral("widget_7"));
        sizePolicy1.setHeightForWidth(widget_7->sizePolicy().hasHeightForWidth());
        widget_7->setSizePolicy(sizePolicy1);
        widget_7->setMaximumSize(QSize(300, 8000));
        widget_7->setStyleSheet(QLatin1String("gridline-color: rgb(170, 170, 127);\n"
"selection-background-color: rgb(170, 170, 255);"));
        verticalLayout_5 = new QVBoxLayout(widget_7);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        pushButton_user_user = new QPushButton(widget_7);
        pushButton_user_user->setObjectName(QStringLiteral("pushButton_user_user"));
        pushButton_user_user->setMaximumSize(QSize(50, 16777215));

        verticalLayout_5->addWidget(pushButton_user_user);

        label_user_prompt = new QLabel(widget_7);
        label_user_prompt->setObjectName(QStringLiteral("label_user_prompt"));

        verticalLayout_5->addWidget(label_user_prompt);

        widget_user_cont = new QWidget(widget_7);
        widget_user_cont->setObjectName(QStringLiteral("widget_user_cont"));
        widget_user_cont->setMinimumSize(QSize(240, 140));
        lineEdit_user_name = new QLineEdit(widget_user_cont);
        lineEdit_user_name->setObjectName(QStringLiteral("lineEdit_user_name"));
        lineEdit_user_name->setGeometry(QRect(60, 10, 113, 20));
        lineEdit_user_passwd = new QLineEdit(widget_user_cont);
        lineEdit_user_passwd->setObjectName(QStringLiteral("lineEdit_user_passwd"));
        lineEdit_user_passwd->setGeometry(QRect(60, 40, 113, 20));
        pushButton_user_login = new QPushButton(widget_user_cont);
        pushButton_user_login->setObjectName(QStringLiteral("pushButton_user_login"));
        pushButton_user_login->setGeometry(QRect(20, 70, 31, 31));
        pushButton_user_register = new QPushButton(widget_user_cont);
        pushButton_user_register->setObjectName(QStringLiteral("pushButton_user_register"));
        pushButton_user_register->setGeometry(QRect(60, 70, 31, 31));
        pushButton_user_sync = new QPushButton(widget_user_cont);
        pushButton_user_sync->setObjectName(QStringLiteral("pushButton_user_sync"));
        pushButton_user_sync->setGeometry(QRect(100, 70, 31, 31));
        pushButton_user_logout = new QPushButton(widget_user_cont);
        pushButton_user_logout->setObjectName(QStringLiteral("pushButton_user_logout"));
        pushButton_user_logout->setGeometry(QRect(140, 70, 31, 31));
        label_user_username = new QLabel(widget_user_cont);
        label_user_username->setObjectName(QStringLiteral("label_user_username"));
        label_user_username->setGeometry(QRect(10, 10, 41, 16));
        label_user_passwd = new QLabel(widget_user_cont);
        label_user_passwd->setObjectName(QStringLiteral("label_user_passwd"));
        label_user_passwd->setGeometry(QRect(10, 40, 41, 16));
        pushButton_user_destroy = new QPushButton(widget_user_cont);
        pushButton_user_destroy->setObjectName(QStringLiteral("pushButton_user_destroy"));
        pushButton_user_destroy->setGeometry(QRect(180, 70, 31, 31));
        checkBox_passwd_rem = new QCheckBox(widget_user_cont);
        checkBox_passwd_rem->setObjectName(QStringLiteral("checkBox_passwd_rem"));
        checkBox_passwd_rem->setGeometry(QRect(180, 40, 51, 20));
        pushButton_user_localCreate = new QPushButton(widget_user_cont);
        pushButton_user_localCreate->setObjectName(QStringLiteral("pushButton_user_localCreate"));
        pushButton_user_localCreate->setGeometry(QRect(0, 110, 71, 31));
        pushButton_user_switch = new QPushButton(widget_user_cont);
        pushButton_user_switch->setObjectName(QStringLiteral("pushButton_user_switch"));
        pushButton_user_switch->setGeometry(QRect(90, 110, 71, 31));
        pushButton_user_connectTest = new QPushButton(widget_user_cont);
        pushButton_user_connectTest->setObjectName(QStringLiteral("pushButton_user_connectTest"));
        pushButton_user_connectTest->setGeometry(QRect(180, 110, 61, 31));

        verticalLayout_5->addWidget(widget_user_cont);

        pushButton_cfg_cfg = new QPushButton(widget_7);
        pushButton_cfg_cfg->setObjectName(QStringLiteral("pushButton_cfg_cfg"));
        pushButton_cfg_cfg->setMaximumSize(QSize(50, 16777215));

        verticalLayout_5->addWidget(pushButton_cfg_cfg);

        label_cfg_prompt = new QLabel(widget_7);
        label_cfg_prompt->setObjectName(QStringLiteral("label_cfg_prompt"));

        verticalLayout_5->addWidget(label_cfg_prompt);

        widget_cfg_cont = new QWidget(widget_7);
        widget_cfg_cont->setObjectName(QStringLiteral("widget_cfg_cont"));
        widget_cfg_cont->setMinimumSize(QSize(240, 210));
        label_cfg_serverip = new QLabel(widget_cfg_cont);
        label_cfg_serverip->setObjectName(QStringLiteral("label_cfg_serverip"));
        label_cfg_serverip->setGeometry(QRect(20, 10, 54, 12));
        label_cfg_tcpport = new QLabel(widget_cfg_cont);
        label_cfg_tcpport->setObjectName(QStringLiteral("label_cfg_tcpport"));
        label_cfg_tcpport->setGeometry(QRect(20, 40, 54, 12));
        label_cfg_autosync = new QLabel(widget_cfg_cont);
        label_cfg_autosync->setObjectName(QStringLiteral("label_cfg_autosync"));
        label_cfg_autosync->setGeometry(QRect(20, 70, 121, 16));
        lineEdit_cfg_ip = new QLineEdit(widget_cfg_cont);
        lineEdit_cfg_ip->setObjectName(QStringLiteral("lineEdit_cfg_ip"));
        lineEdit_cfg_ip->setGeometry(QRect(80, 10, 113, 20));
        lineEdit_cfg_port = new QLineEdit(widget_cfg_cont);
        lineEdit_cfg_port->setObjectName(QStringLiteral("lineEdit_cfg_port"));
        lineEdit_cfg_port->setGeometry(QRect(80, 40, 113, 20));
        lineEdit_cfg_autosync = new QLineEdit(widget_cfg_cont);
        lineEdit_cfg_autosync->setObjectName(QStringLiteral("lineEdit_cfg_autosync"));
        lineEdit_cfg_autosync->setGeometry(QRect(140, 70, 41, 21));
        pushButton_cfg_save = new QPushButton(widget_cfg_cont);
        pushButton_cfg_save->setObjectName(QStringLiteral("pushButton_cfg_save"));
        pushButton_cfg_save->setGeometry(QRect(20, 160, 41, 31));
        pushButton_cfg_cancel = new QPushButton(widget_cfg_cont);
        pushButton_cfg_cancel->setObjectName(QStringLiteral("pushButton_cfg_cancel"));
        pushButton_cfg_cancel->setGeometry(QRect(80, 160, 41, 31));
        checkBox_cfg_autonsync = new QCheckBox(widget_cfg_cont);
        checkBox_cfg_autonsync->setObjectName(QStringLiteral("checkBox_cfg_autonsync"));
        checkBox_cfg_autonsync->setGeometry(QRect(20, 100, 71, 16));
        checkBox_cfg_power_on_sync = new QCheckBox(widget_cfg_cont);
        checkBox_cfg_power_on_sync->setObjectName(QStringLiteral("checkBox_cfg_power_on_sync"));
        checkBox_cfg_power_on_sync->setGeometry(QRect(20, 130, 71, 16));
        pushButton_cfg_autosync = new QPushButton(widget_cfg_cont);
        pushButton_cfg_autosync->setObjectName(QStringLiteral("pushButton_cfg_autosync"));
        pushButton_cfg_autosync->setGeometry(QRect(100, 100, 75, 16));

        verticalLayout_5->addWidget(widget_cfg_cont);

        pushButton_debug = new QPushButton(widget_7);
        pushButton_debug->setObjectName(QStringLiteral("pushButton_debug"));
        pushButton_debug->setMaximumSize(QSize(50, 16777215));

        verticalLayout_5->addWidget(pushButton_debug);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_4);


        verticalLayout_4->addWidget(widget_7);


        horizontalLayout_4->addWidget(widget_6);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1048, 23));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        menu_4 = new QMenu(menuBar);
        menu_4->setObjectName(QStringLiteral("menu_4"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menuBar->addAction(menu_4->menuAction());
        menu->addAction(action_menuNew);
        menu->addAction(action_menuSave);
        menu->addAction(action_menuOpen);
        menu->addAction(action_menuSaveAs);
        menu->addAction(action_menuClose);
        menu->addAction(action_menuCloseAll);
        menu->addAction(action_menuCloseOther);
        menu->addAction(action_menuSaveAll);
        menu_3->addAction(action_menuShowLeft);
        mainToolBar->addAction(action_new);
        mainToolBar->addAction(action_save);
        mainToolBar->addAction(action_open);
        mainToolBar->addAction(action_saveAs);
        mainToolBar->addAction(action_close);
        mainToolBar->addAction(action_closeAll);
        mainToolBar->addAction(action_closeOther);
        mainToolBar->addAction(action_saveAll);
        mainToolBar->addAction(action_showLeft);
        mainToolBar->addAction(action_fontAdd);
        mainToolBar->addAction(action_fontSub);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\350\256\260\345\275\225\346\234\254", Q_NULLPTR));
        action_new->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_new->setToolTip(QApplication::translate("MainWindow", "ctr+n", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_save->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_save->setToolTip(QApplication::translate("MainWindow", "ctr+s", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_open->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200", Q_NULLPTR));
        action_saveAs->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230", Q_NULLPTR));
        action_menuNew->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272", Q_NULLPTR));
        action_menuSave->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\n"
"", Q_NULLPTR));
        action_menuOpen->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200", Q_NULLPTR));
        action_menuSaveAs->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230", Q_NULLPTR));
        action_close->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\345\275\223\345\211\215", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        action_close->setToolTip(QApplication::translate("MainWindow", "ctr+w", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        action_menuClose->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\345\275\223\345\211\215", Q_NULLPTR));
        action_closeAll->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\346\211\200\346\234\211", Q_NULLPTR));
        action_menuCloseAll->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\346\211\200\346\234\211", Q_NULLPTR));
        action_closeOther->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\345\205\266\345\256\203", Q_NULLPTR));
        action_menuCloseOther->setText(QApplication::translate("MainWindow", "\345\205\263\351\227\255\345\205\266\345\256\203", Q_NULLPTR));
        action_saveAll->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\346\211\200\346\234\211", Q_NULLPTR));
        action_menuSaveAll->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\346\211\200\346\234\211", Q_NULLPTR));
        action_showLeft->setText(QApplication::translate("MainWindow", "\345\261\225\345\274\200\345\267\246\350\276\271", Q_NULLPTR));
        action_menuShowLeft->setText(QApplication::translate("MainWindow", "\345\261\225\345\274\200\345\267\246\350\276\271", Q_NULLPTR));
        action_fontAdd->setText(QApplication::translate("MainWindow", "\345\255\227\344\275\223+", Q_NULLPTR));
        action_fontSub->setText(QApplication::translate("MainWindow", "\345\255\227\344\275\223-", Q_NULLPTR));
        pushButton->setText(QString());
        label->setText(QApplication::translate("MainWindow", "\345\247\223\345\220\215\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267ID\357\274\232", Q_NULLPTR));
        pushButton_hideLeft->setText(QApplication::translate("MainWindow", "<<", Q_NULLPTR));
        label_systemTime->setText(QApplication::translate("MainWindow", "\347\263\273\347\273\237\346\227\266\351\227\264\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", " \346\250\241\345\274\217\345\210\207\346\215\242\357\274\232", Q_NULLPTR));
        btn_fileScanFileMode->setText(QApplication::translate("MainWindow", "\346\226\207\344\273\266\346\265\217\350\247\210", Q_NULLPTR));
        btn_fileScanDialyMode->setText(QApplication::translate("MainWindow", "\346\227\245\350\256\260\346\265\217\350\247\210", Q_NULLPTR));
        label_fileScanPath->setText(QApplication::translate("MainWindow", "\350\267\257\345\276\204", Q_NULLPTR));
        btn_pathSet->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260", Q_NULLPTR));
        btn_fileScanTmp1->setText(QApplication::translate("MainWindow", "\347\251\272", Q_NULLPTR));
        label_fileScanTmp1->setText(QString());
        btn_fileScanToday->setText(QApplication::translate("MainWindow", "\344\273\212\345\244\251", Q_NULLPTR));
        btn_fileScanYestoday->setText(QApplication::translate("MainWindow", "\346\230\250\345\244\251", Q_NULLPTR));
        btn_fileScanWeek->setText(QApplication::translate("MainWindow", "\346\234\254\345\221\250", Q_NULLPTR));
        btn_fileScanMonth->setText(QApplication::translate("MainWindow", "\346\234\254\346\234\210", Q_NULLPTR));
        btn_fileScanAll->setText(QApplication::translate("MainWindow", "\345\205\250\351\203\250", Q_NULLPTR));
        btn_del->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244", Q_NULLPTR));
        btn_new->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272", Q_NULLPTR));
        label_time->setText(QApplication::translate("MainWindow", "\346\227\266\351\227\264\357\274\232", Q_NULLPTR));
        label_year->setText(QApplication::translate("MainWindow", "\345\271\264", Q_NULLPTR));
        label_mounth->setText(QApplication::translate("MainWindow", "\346\234\210", Q_NULLPTR));
        label_day->setText(QApplication::translate("MainWindow", "\346\227\245", Q_NULLPTR));
        label_hour->setText(QApplication::translate("MainWindow", "\346\227\266", Q_NULLPTR));
        label_minute->setText(QApplication::translate("MainWindow", "\345\210\206", Q_NULLPTR));
        label_weather->setText(QApplication::translate("MainWindow", "\345\244\251\346\260\224\357\274\232", Q_NULLPTR));
        label_week->setText(QApplication::translate("MainWindow", "\346\230\237\346\234\237\357\274\232", Q_NULLPTR));
        label_statusBar_1->setText(QApplication::translate("MainWindow", "\347\212\266\346\200\201\346\240\217", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", Q_NULLPTR));
        pushButton_user_user->setText(QApplication::translate("MainWindow", "\350\264\246\346\210\267>>", Q_NULLPTR));
        label_user_prompt->setText(QApplication::translate("MainWindow", "\346\217\220\347\244\272\357\274\232", Q_NULLPTR));
        pushButton_user_login->setText(QApplication::translate("MainWindow", "\347\231\273\345\275\225", Q_NULLPTR));
        pushButton_user_register->setText(QApplication::translate("MainWindow", "\346\263\250\345\206\214", Q_NULLPTR));
        pushButton_user_sync->setText(QApplication::translate("MainWindow", "\345\220\214\346\255\245", Q_NULLPTR));
        pushButton_user_logout->setText(QApplication::translate("MainWindow", "\346\263\250\351\224\200", Q_NULLPTR));
        label_user_username->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\345\220\215", Q_NULLPTR));
        label_user_passwd->setText(QApplication::translate("MainWindow", "\345\257\206\347\240\201", Q_NULLPTR));
        pushButton_user_destroy->setText(QApplication::translate("MainWindow", "\346\221\247\346\257\201", Q_NULLPTR));
        checkBox_passwd_rem->setText(QApplication::translate("MainWindow", "\350\256\260\344\275\217", Q_NULLPTR));
        pushButton_user_localCreate->setText(QApplication::translate("MainWindow", "\346\234\254\345\234\260\345\210\233\345\273\272", Q_NULLPTR));
        pushButton_user_switch->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\345\210\207\346\215\242", Q_NULLPTR));
        pushButton_user_connectTest->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245\346\265\213\350\257\225", Q_NULLPTR));
        pushButton_cfg_cfg->setText(QApplication::translate("MainWindow", "\351\205\215\347\275\256>>", Q_NULLPTR));
        label_cfg_prompt->setText(QApplication::translate("MainWindow", "\346\217\220\347\244\272\357\274\232", Q_NULLPTR));
        label_cfg_serverip->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250ip:", Q_NULLPTR));
        label_cfg_tcpport->setText(QApplication::translate("MainWindow", "tcp\347\253\257\345\217\243;", Q_NULLPTR));
        label_cfg_autosync->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\220\214\346\255\245\345\217\202\346\225\260\357\274\210\345\210\206\351\222\237\357\274\211:", Q_NULLPTR));
        pushButton_cfg_save->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", Q_NULLPTR));
        pushButton_cfg_cancel->setText(QApplication::translate("MainWindow", "\345\217\226\346\266\210", Q_NULLPTR));
        checkBox_cfg_autonsync->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\220\214\346\255\245", Q_NULLPTR));
        checkBox_cfg_power_on_sync->setText(QApplication::translate("MainWindow", "\345\274\200\346\234\272\345\220\214\346\255\245", Q_NULLPTR));
        pushButton_cfg_autosync->setText(QString());
        pushButton_debug->setText(QApplication::translate("MainWindow", "\350\260\203\350\257\225", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\347\274\226\350\276\221", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
        menu_4->setTitle(QApplication::translate("MainWindow", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
