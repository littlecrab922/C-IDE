#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QtWidgets/QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDir>//获取文件路径
#include <QFile>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>//菜单
#include <QMenuBar>//菜单栏
#include <QPalette>
#include <QSplitter>
#include <QStatusBar>//状态栏
#include <QtEvents>
#include <QTextBlock>
#include <QTextBrowser>
#include <QTextEdit>
#include <QTextStream>
#include <QToolButton>//工具栏
#include <QToolBar>//工具
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QProcess>
#include <QPushButton>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>
#include <set>
#include <iostream>
#include <stack>
#include "find.h"
using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

    QTabWidget *text_tab;
    QTabWidget *information_tab;
    //项目管理
    QTreeWidget *itemtree;

    //工具栏及按钮
    QToolBar *toolbar;
    QToolButton *btn[11];

    //状态栏
    QStatusBar *stb;

    //文本
    QsciScintilla *text[20];
    QsciLexerCPP *textLexer;

    //菜单栏
    QMenu *file;
    QAction *fileAction[4];

    QMenu *edit;
    QAction *editAction[7];

    QMenu *run;
    QAction *runAction[4];

    QMenu *help;
    QAction *help_about;

    //分裂窗口
    QSplitter *all;
    QSplitter *right;
    QSplitter *debug;
    QSplitter *debugbtn;
    QSplitter *btn_top;
    QSplitter *btn_mid;
    QSplitter *btn_bottom;


    //字体大小调整
    QComboBox *sizeCbx;

    //编译及调试信息框
    QPushButton *debug_btn[9];
    QTextBrowser *compile_browser;
    QTextBrowser *debug_browser;
    QLineEdit *debug_line;

    //debug
    QProcess gdb_dialog;
    set<int>break_point[20];//断点
    int frame_position = 0;
    struct fit
    {
        QString code;
        QString annotation;
    };

    stack<fit> x[20];
    int flag_new[20];        //标记：如果新建了文件就为1，初始值为0
    int current_size = 14;
    int file_count = 0;
    int dir_count = 0;
    int current_page;
    QString file_path[20];     //保存文件的路径
    QString dir_path[20];       //保存文件夹的路径
    void text_init(QFont);
    void getAllFiles(QString _file_path,QString filename,QTreeWidgetItem *project_item);


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open();
    void on_copy();
    void on_paste();
    void on_cut();
    void on_selectAll();
    void on_find();
    void find_text(QString);
    void replace_one_text(QString);
    void replace_all_text(QString,QString);
    void on_save();
    void on_undo();
    void on_redo();
    void on_create();
    void on_compile();
    void on_run();
    void on_compile_run();
    void on_debug();
    void on_margin_clicked(int, int, Qt::KeyboardModifiers);
    void on_debug_start_clicked();
    void on_debug_step_clicked();
    void on_debug_display_clicked();
    void on_debug_frame_clicked();
    void on_debug_run_clicked();
    void on_debug_continue_clicked();
    void on_debug_print_clicked();
    void on_debug_quit_clicked();
    void on_debug_frame_show();
    void onTabCurrentChanged(int);
    void openitem();
    void on_open_dir();
    void show_annotate();
    void hide_annotate();
    void on_about();

};

#endif // MAINWINDOW_H
