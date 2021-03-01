#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //中心窗口容器
    {
        all = new QSplitter(Qt::Horizontal,0);
        setCentralWidget(all);
        itemtree = new QTreeWidget(all);
        right = new QSplitter(Qt::Vertical,all);
        all->setStretchFactor(0,1);
        all->setStretchFactor(1,3);
        //文本
        text_tab = new QTabWidget(right);
        connect(text_tab, SIGNAL(currentChanged(int)), this, SLOT(onTabCurrentChanged(int)));
        right->setStretchFactor(0,3);
        right->setStretchFactor(1,1);
        //信息显示框
        information_tab = new QTabWidget(right);
        compile_browser = new QTextBrowser(information_tab);
        debug = new QSplitter(information_tab);
        information_tab->addTab(compile_browser,"编译结果");
        information_tab->addTab(debug,"调试信息");
    }
    //debug框
    {
        debugbtn = new QSplitter(Qt::Vertical,debug);
        debug_browser = new QTextBrowser(debug);
        debug_browser->setMinimumHeight(120);
        btn_top = new QSplitter(Qt::Horizontal,debugbtn);
        btn_mid = new QSplitter(Qt::Horizontal,debugbtn);
        btn_bottom = new QSplitter(Qt::Horizontal,debugbtn);
        debug_line = new QLineEdit(btn_bottom);
        debug_line->setFont(QFont("Consolas",16));
        QStringList debugbtn_namelist;
        debugbtn_namelist<<tr("开始执行")<<tr("单步执行")<<tr("跟踪变量")<<tr("查看栈帧")
                        <<tr("开始运行")<<tr("继续运行")<<tr("打印变量")<<tr("结束调试");
        for(int i=0; i<8; i++)
        {
            if(i<4)
            {
                debug_btn[i] = new QPushButton(btn_top);
                debug_btn[i]->setFixedSize(QSize(60, 55));
                debug_btn[i]->setText(debugbtn_namelist[i]);
            }
            else
            {
                debug_btn[i] = new QPushButton(btn_mid);
                debug_btn[i]->setFixedSize(QSize(60, 55));
                debug_btn[i]->setText(debugbtn_namelist[i]);
            }
        }
        connect(debug_btn[0], &QToolButton::clicked,  this, &MainWindow::on_debug_start_clicked);
        connect(debug_btn[1], &QToolButton::clicked,  this, &MainWindow::on_debug_step_clicked);
        connect(debug_btn[2], &QToolButton::clicked,  this, &MainWindow::on_debug_display_clicked);
        connect(debug_btn[3], &QToolButton::clicked,  this, &MainWindow::on_debug_frame_clicked);
        connect(debug_btn[4], &QToolButton::clicked,  this, &MainWindow::on_debug_run_clicked);
        connect(debug_btn[5], &QToolButton::clicked,  this, &MainWindow::on_debug_continue_clicked);
        connect(debug_btn[6], &QToolButton::clicked,  this, &MainWindow::on_debug_print_clicked);
        connect(debug_btn[7], &QToolButton::clicked,  this, &MainWindow::on_debug_quit_clicked);
    }

    //文本框
    {
        all->setHidden(true);
    }

    //工具栏
    {
        toolbar = addToolBar(tr("工具栏"));
        toolbar->setMovable(false);//设置不可拖动
        toolbar->setAllowedAreas(Qt::TopToolBarArea);//放置窗口顶部
    }

    //工具栏按钮
    {
        QStringList tb_namelist;
        tb_namelist<<tr("新建")<<tr("打开")<<tr("保存")<<tr("撤销")<<tr("前进")<<tr("编译")<<tr("运行")<<tr("编译并运行")<<tr("调试")<<tr("打开注释")<<tr("隐藏注释");
        QStringList tb_iconlist;
        tb_iconlist<<tr("create")<<tr("open")<<tr("save")<<tr("undo")<<tr("redo")<<tr("com")<<tr("run")<<tr("cr")<<tr("debug")<<tr("show")<<tr("hide");
        for(int i=0; i<11; i++)
        {
            btn[i] = new QToolButton(this);
            btn[i]->setToolTip(tb_namelist[i]);
            btn[i]->setFixedSize(QSize(40, 40));
            QString iconname = QString(":/img/%1").arg(tb_iconlist[i]);//arg()方法用于填充字符串中的%1,%2...为给定的参数
            btn[i]->setIcon(QPixmap(iconname));//QPixmap是为了在屏幕上显示图像而设计和优化的 setIcon添加图标
            btn[i]->setAutoRaise(true);
            btn[i]->setStatusTip(tb_namelist[i]);//设置该动作的提示文本
            toolbar->addWidget(btn[i]);//toolbar工具栏添加这个按钮
            if(i==2||i==4||i==8)
                toolbar->addSeparator();//设置分隔竖线
        }
        connect(btn[0], &QToolButton::clicked,  this, &MainWindow::on_create);
        connect(btn[1], &QToolButton::clicked,  this, &MainWindow::on_open);
        connect(btn[2], &QToolButton::clicked,  this, &MainWindow::on_save);
        connect(btn[3], &QToolButton::clicked,  this, &MainWindow::on_undo);
        connect(btn[4], &QToolButton::clicked,  this, &MainWindow::on_redo);
        connect(btn[5], &QToolButton::clicked,  this, &MainWindow::on_compile);
        connect(btn[6], &QToolButton::clicked,  this, &MainWindow::on_run);
        connect(btn[7], &QToolButton::clicked,  this, &MainWindow::on_compile_run);
        connect(btn[8], &QToolButton::clicked,  this, &MainWindow::on_debug);
        connect(btn[9], &QToolButton::clicked,  this, &MainWindow::show_annotate);
        connect(btn[10], &QToolButton::clicked,  this, &MainWindow::hide_annotate);
    }

    //菜单栏-文件
    {
        file = this->menuBar()->addMenu(tr("文件"));//在菜单栏中加入一个菜单项
        QStringList file_pathlist;
        file_pathlist<<tr("新建")<<tr("打开")<<tr("保存")<<tr("打开文件夹");
        QStringList file_iconlist;
        file_iconlist<<tr("create")<<tr("open")<<tr("save")<<tr("open_2");
        QStringList file_sclist;
        file_sclist<<tr("Ctrl+N")<<tr("Ctrl+O")<<tr("Ctrl+S")<<tr("Ctrl+D");
        for(int i=0; i<4; i++)
        {
            fileAction[i] = new QAction(QIcon(QString(":/img/%1").arg(file_iconlist[i])),file_pathlist[i], this);
            fileAction[i]->setShortcut(file_sclist[i]);//设快捷方式
            file->addAction(fileAction[i]);//添加下滑栏中项目
            file->addSeparator();//加入一个分隔符
        }
        connect(fileAction[0], SIGNAL(triggered()), this, SLOT(on_create()));
        connect(fileAction[1], SIGNAL(triggered()), this, SLOT(on_open()));
        connect(fileAction[2], SIGNAL(triggered()), this, SLOT(on_save()));
        connect(fileAction[3], SIGNAL(triggered()), this, SLOT(on_open_dir()));
    }

    //菜单栏-编辑
    {
        edit = this->menuBar()->addMenu(tr("编辑"));
        QStringList edit_namelist;
        edit_namelist<<tr("撤销")<<tr("前进")<<tr("复制")<<tr("粘贴")<<tr("剪切")<<tr("全选")<<tr("查找替换");
        QStringList edit_iconlist;
        edit_iconlist<<tr("undo")<<tr("redo");
        QStringList edit_sclist;
        edit_sclist<<tr("Ctrl+Z")<<tr("Ctrl+Y")<<tr("Ctrl+C")<<tr("Ctrl+V")<<tr("Ctrl+X")<<tr("Ctrl+A")<<tr("Ctrl+F");
        for(int i=0; i<7; i++)
        {
            if(i<2)
                editAction[i] = new QAction(QIcon(QString(":/img/%1").arg(edit_iconlist[i])),edit_namelist[i], this);
            else
                editAction[i] = new QAction(edit_namelist[i], this);
            editAction[i]->setShortcut(edit_sclist[i]);
            edit->addAction(editAction[i]);//添加下滑栏中项目
            edit->addSeparator();//加入一个分隔符
        }
        connect(editAction[0], SIGNAL(triggered()), this, SLOT(on_undo()));
        connect(editAction[1], SIGNAL(triggered()), this, SLOT(on_redo()));
        connect(editAction[2], SIGNAL(triggered()), this, SLOT(on_copy()));
        connect(editAction[3], SIGNAL(triggered()), this, SLOT(on_paste()));
        connect(editAction[4], SIGNAL(triggered()), this, SLOT(on_cut()));
        connect(editAction[5], SIGNAL(triggered()), this, SLOT(on_selectAll()));
        connect(editAction[6], SIGNAL(triggered()), this, SLOT(on_find()));
    }

    //菜单栏-运行
    {
        run = this->menuBar()->addMenu(tr("运行"));
        QStringList run_namelist;
        run_namelist<<tr("编译")<<tr("运行")<<tr("编译并运行")<<tr("调试");
        QStringList run_iconlist;
        run_iconlist<<tr("com")<<tr("run")<<tr("cr")<<tr("debug");
        QStringList run_sclist;
        run_sclist<<tr("F9")<<tr("F10")<<tr("F11")<<tr("F5");
        for(int i=0; i<4; i++)
        {
            runAction[i] = new QAction(QIcon(QString(":/img/%1").arg(run_iconlist[i])),run_namelist[i], this);
            runAction[i]->setShortcut(run_sclist[i]);
            run->addAction(runAction[i]);//添加下滑栏中项目
            run->addSeparator();//加入一个分隔符
        }
        connect(runAction[0], SIGNAL(triggered()), this, SLOT(on_compile()));
        connect(runAction[1], SIGNAL(triggered()), this, SLOT(on_run()));
        connect(runAction[2], SIGNAL(triggered()), this, SLOT(on_compile_run()));
        connect(runAction[3], SIGNAL(triggered()), this, SLOT(on_debug()));
    }

    //菜单栏-帮助
    {
        help = this->menuBar()->addMenu(tr("帮助"));
        help_about = new QAction(tr("关于我们"),this);
        help->addAction(help_about);
        connect(help_about, SIGNAL(triggered()), this, SLOT(on_about()));
    }

    //项目管理栏
    {
        itemtree->setColumnCount(2);//设置列数
        itemtree->setHeaderLabels(QStringList()<<tr("项目管理")<<tr("地址"));//设置标题
        //打开选中文件
        connect(itemtree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(openitem()));
    }

    //状态栏
    {
        stb = statusBar();
        QLabel *lable = new QLabel("Made by 铁Five",this);
        stb->addWidget(lable);//状态栏添加组件
        lable->setLineWidth(3);
        lable->setFrameShape(QFrame::Box); // 设置标签形状
        lable->setFrameShadow(QFrame::Raised); // 设置标签阴影
    }


    //调整字体大小
    {
        sizeCbx = new QComboBox();
        QFontDatabase FontDb;
        foreach (int size, FontDb.standardSizes())
        {
            sizeCbx->addItem(QString::number(size));//添加数值
        }
        sizeCbx->setCurrentIndex(7);//设置默认值
        void (QComboBox:: * cbxSingal)(const QString &text) = &QComboBox::currentIndexChanged;//设置信号
        connect(sizeCbx, cbxSingal, this, [=](const QString &size){
            for(int i=0; i<file_count; i++)
            {
                QString content = text[i]->text();
                text[i]->clear();
                current_size = size.toInt();
                QFont font("Consolas",size.toInt());
                text_init(font);
                text[i]->setText(content);

            }


        });
        toolbar->addWidget(sizeCbx);
    }

    for(int i = 0; i<20; i++){
        flag_new[i] = 0;
        break_point[i].clear();
    }

}

MainWindow::~MainWindow()
{

}

//如果是文件的话就先打开，再添加进itemtree
//如果是目录的话就先添加进itemtree，再打开
void MainWindow::on_open()
{
    bool f = false;
    //获得打开的文件的文件绝对路径
    QString filepath = QFileDialog::getOpenFileName(this,tr("打开文件"),QString(),0/*tr("文本文件(*.txt) ;; C++文件(*.h *.cpp *.hpp);;")*/);
    //getOpenFileName,打开对话框获取用户文件名，仅返回打开文件的名称
    //参数 1.指定父组件 2.对话框标题 3.对话框打开时默认显示目录 4.对话框的后缀名过滤器
    if(!filepath.isEmpty())//如果文件路径不为空
    {
        QFile file(filepath);//构造一个以 filepath 为名的Qfile对象
        if(!file.open(QIODevice::ReadOnly)||!file.isReadable())
        {
            //判断文件是否可打开
            QMessageBox::warning(this,tr("错误"),tr("不能打开文件"));
            return;
        }
        for(int i = 0; i < file_count ;i++)
        {
            if(filepath == file_path[i])
            {
                f = true;
                break;
            }
        }
        if(f)
        {
            QMessageBox::warning(this,tr("提示"),tr("已经打开文件"));
            return;
        }
        all->setHidden(false);//使得编辑区页面可见
        text[file_count] = new QsciScintilla(text_tab);
        QString file_text = QFileInfo(filepath).fileName();
        text_tab->addTab(text[file_count],file_text);
        text_tab->setCurrentIndex(file_count);
        text[file_count]->clear();//清空编辑区页面

        QTextStream ins(&file);//QTextStream会自动将 Unicode 编码同操作系统的编码进行转换
        QFont font("Consolas",current_size);
        //text[file_count]->setFont(font);
        text_init(font);
        text[file_count]->setText(ins.readAll());//讲ins中的内容放进编辑区
        file_path[file_count] = filepath;
        file.close();
        file_count += 1;
    }

}

void MainWindow::on_copy()
{
    text[current_page]->copy();
}

void MainWindow::on_paste()
{
    text[current_page]->paste();
}

void MainWindow::on_cut()
{
    text[current_page]->cut();
}

void MainWindow::on_selectAll()
{
    text[current_page]->selectAll();
}

void MainWindow::on_find()
{
    Find *find = new Find;
    find->show();
    connect(find, SIGNAL(find_text(QString)), this, SLOT(find_text(QString)));
    connect(find, SIGNAL(replace_text(QString)), this, SLOT(replace_one_text(QString)));
    connect(find, SIGNAL(replace_all(QString,QString)), this, SLOT(replace_all_text(QString,QString)));
}
void MainWindow::find_text(QString str)
{
    text[current_page]->findFirst(str,true,true,true,true,true,-1,-1,true,false,false);

}

void MainWindow::replace_one_text(QString str2)
{
    text[current_page]->replace(str2);

}

void MainWindow::replace_all_text(QString str, QString str2)
{
    QString content  = text[current_page]->text();
    int x=0;
    int len = str.length();
    while(x>=0)
    {
        x = content.indexOf(str);
        content = content.replace(x,len,str2);
        x = content.indexOf(str);
    }
    text[current_page]->setText(content);
}

void MainWindow::on_save()
{
    if(text[current_page]->text() == "")
    {
        QMessageBox::warning(this,tr("提示"),tr("内容不能为空"));
        return;
    }
    if(flag_new[current_page])//如果是新文件则弹出文件保存框
    {
        QString filepath = QFileDialog::getSaveFileName(this,tr("保存文件"),QString(),tr("C文件(*.c) ;; C++文件(*.cpp) ;; 头文件(*.h) ;; 文本文件(*.txt)"));
        if(!filepath.isEmpty())
        {
            QFile file(filepath);
            if(!file.open(QIODevice::WriteOnly))
            {
                QMessageBox::warning(this,tr("错误"),tr("不能打开文件"));
                return;
            }
            QTextStream outs(&file);
            file_path[current_page] = filepath;
            QString file_text = QFileInfo(filepath).fileName();
            outs<<text[current_page]->text();
            outs.flush();
            file.close();
            flag_new[current_page] = 0;
            text_tab->setTabText(current_page, file_text);
        }

    }

    else //旧文件直接保存
    {
        QFile file(file_path[current_page]);
        if(!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this,tr("错误"),tr("不能打开文件"));
            return;
        }
        QTextStream outs(&file);
        outs<<text[current_page]->text();
        outs.flush();
        file.close();
    }
}

void MainWindow::on_undo()
{
    text[current_page]->undo();
}

void MainWindow::on_redo()
{
    text[current_page]->redo();
}

void MainWindow::on_create()
{
    all->setHidden(false);     //显示文本框
    text[file_count] = new QsciScintilla(text_tab);
    text_tab->addTab(text[file_count],"未命名");
    text_tab->setCurrentIndex(file_count);
    flag_new[file_count] = 1;                 //新文件标记位设为1
    QFont font("Consolas",current_size);
    text[file_count]->setFont(font);
    file_count += 1;
    text_init(font);
}


void MainWindow::on_compile()
{
    on_save();
    QString current_file_name = file_path[current_page];
    QString produce_file_name = current_file_name;
    produce_file_name.replace(".cpp", ".exe");
    produce_file_name.replace(".c", ".exe");
    produce_file_name.replace(".h", ".exe");
    produce_file_name.replace(".txt", ".exe");
    QString command = "gcc -o " + produce_file_name + " " + current_file_name;

    QProcess p(0);
    p.start(command);
    p.waitForStarted();
    p.waitForFinished();
    QString std_error = QString::fromLocal8Bit(p.readAllStandardError());
    QString std_output = QString::fromLocal8Bit(p.readAllStandardOutput());
    compile_browser->setText("编译结果:");
    if(std_error == NULL){
        compile_browser->append("成功编译");
    }
    else{
        compile_browser->append(std_error);
    }
}

void MainWindow::on_run()
{
    on_save();
    QString current_file_name = file_path[current_page];
    QString command = current_file_name;
    command.replace(".cpp", ".exe");
    command.replace(".c", ".exe");
    command.replace(".h", ".exe");
    command.replace(".txt", ".exe");

    system(command.toLatin1().data());
//    QProcess p(0);
//    p.start(command);
//    p.waitForStarted();
//    p.waitForFinished();
//    QString std_output = QString::fromLocal8Bit(p.readAllStandardOutput());
//    compile_browser->setText("运行结果:");
//    compile_browser->append(std_output);
}

void MainWindow::on_compile_run()
{
    on_save();
    QString current_file_name = file_path[current_page];
    QString produce_file_name = current_file_name;
    produce_file_name.replace(".cpp", ".exe");
    produce_file_name.replace(".c", ".exe");
    produce_file_name.replace(".h", ".exe");
    produce_file_name.replace(".txt", ".exe");
    QString command = "gcc -o " + produce_file_name + " " + current_file_name;
    QProcess p(0);
    p.start(command);
    p.waitForStarted();
    p.waitForFinished();
    QString std_error = QString::fromLocal8Bit(p.readAllStandardError());
    compile_browser->setText("编译结果:");
    if(std_error == NULL){
        compile_browser->append("成功编译");
        command = produce_file_name;
        system(command.toLatin1().data());
//        compile_browser->append("运行结果:");
//        p.start(command);
//        p.waitForStarted();
//        p.waitForFinished();
//        QString std_output = QString::fromLocal8Bit(p.readAllStandardOutput());
//        compile_browser->append(std_output);
    }
    else{
        compile_browser->append(std_error);
    }
}

void MainWindow::on_debug()
{
    on_save();
    QString current_file_name = file_path[current_page];
    QString produce_file_name = current_file_name;
    produce_file_name.replace(".cpp", "");
    produce_file_name.replace(".c", "");
    produce_file_name.replace(".h", "");
    produce_file_name.replace(".txt", "");
    QString command = "gcc -g " + current_file_name + " -o " + produce_file_name;

    QProcess p(0);
    p.start(command);
    p.waitForStarted();
    p.waitForFinished();
    debug_browser->setText("调试结果:");
    QString std_error = QString::fromLocal8Bit(p.readAllStandardError());

    if(std_error == NULL){
        //启动调试器
        command = "gdb " + produce_file_name;
        debug_browser->append(command);
        gdb_dialog.start(command);
        gdb_dialog.waitForStarted();
        QString output;
        if(gdb_dialog.waitForReadyRead()){
            output = gdb_dialog.readAll();
        }
        debug_browser->append(output);

        //设置断点
        set<int>::iterator it;
        for(it = break_point[current_page].begin(); it != break_point[current_page].end(); it++){
            command = "break " + QString::number(*it+1) +"\r\n";
            debug_browser->append(command);
            gdb_dialog.write(command.toUtf8().data());
            QString output;
            if(gdb_dialog.waitForReadyRead()){
                output = gdb_dialog.readAll();
            }
            debug_browser->append(output);
        }
    }
    else{
        debug_browser->append(std_error);
    }
}

void MainWindow::text_init(QFont font)
{
    textLexer = new QsciLexerCPP;
    textLexer->setFont(font);
    text[current_page]->setLexer(textLexer);

    //设置自动补全的字符串和补全方式
    QsciAPIs *apis = new QsciAPIs(textLexer);
    apis->add(QString("#include"));
    apis->add(QString("stdio.h"));
    apis->add(QString("using"));
    apis->add(QString("namespace"));
    apis->add(QString("struct"));
    apis->add(QString("iostream"));
    apis->add(QString("vector"));
    apis->add(QString("algorithm"));
    apis->add(QString("math.h"));
    apis->add(QString("stdlib.h"));
    apis->add(QString("string"));
    apis->add(QString("string.h"));
    apis->add(QString("queue"));
    apis->add(QString("stack"));
    apis->add(QString("map"));
    apis->add(QString("set"));
    apis->add(QString("complex"));
    apis->add(QString("long"));
    apis->add(QString("main"));
    apis->add(QString("int"));
    apis->add(QString("char"));
    apis->add(QString("string"));
    apis->add(QString("if"));
    apis->add(QString("else"));
    apis->add(QString("switch"));
    apis->add(QString("default"));
    apis->add(QString("break"));
    apis->add(QString("double"));
    apis->add(QString("enum"));
    apis->add(QString("float"));
    apis->add(QString("long"));
    apis->add(QString("short"));
    apis->add(QString("void"));
    apis->add(QString("for"));
    apis->add(QString("while"));
    apis->add(QString("continue"));
    apis->add(QString("return"));
    apis->add(QString("static"));
    apis->add(QString("sizeof"));
    apis->add(QString("typedef"));
    apis->add(QString("const"));
    apis->prepare();
    text[current_page]->setAutoCompletionSource(QsciScintilla::AcsAll);   //自动补全所以地方出现的
    text[current_page]->setAutoCompletionCaseSensitivity(true);   //设置自动补全大小写敏感
    text[current_page]->setAutoCompletionThreshold(2);    //输入一个字符就会出现自动补全的提示

    //行号显示区域
    text[current_page]->setMarginType(0, QsciScintilla::NumberMargin);
    text[current_page]->setMarginLineNumbers(0, true);
    text[current_page]->setMarginWidth(0,30);
    text[current_page]->setMarginsFont(font);

    //断点设置区域
    text[current_page]->setMarginType(1, QsciScintilla::SymbolMargin);
    text[current_page]->setMarginLineNumbers(1, false);
    text[current_page]->setMarginWidth(1,20);
    text[current_page]->setMarginSensitivity(1, true);

    text[current_page]->setMarginMarkerMask(1, 0x02);
    text[current_page]->markerDefine(QsciScintilla::Circle, 1);
    text[current_page]->setMarkerBackgroundColor(QColor("#ee1111"), 1);
    connect(text[current_page], SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)),this,
            SLOT(on_margin_clicked(int, int, Qt::KeyboardModifiers)));

    //单步执行显示区域
    text[current_page]->setMarginType(2, QsciScintilla::SymbolMargin);
    text[current_page]->setMarginLineNumbers(2, false);
    text[current_page]->setMarginWidth(2, 20);
    text[current_page]->setMarginSensitivity(2, false);
    text[current_page]->setMarginMarkerMask(2, 0x04);
    text[current_page]->markerDefine(QsciScintilla::RightArrow, 2);
    text[current_page]->setMarkerBackgroundColor(QColor("#eaf593"), 2);



    text[current_page]->setIndentationGuides(QsciScintilla::SC_IV_LOOKBOTH);
    text[current_page]->setCaretLineVisible(true);
    text[current_page]->setCaretLineBackgroundColor(QColor(200,255,255));
    //text[current_page]->setMarginsBackgroundColor(Qt::lightGray);
    text[current_page]->setBraceMatching(QsciScintilla::SloppyBraceMatch);

    //代码折叠
    text[current_page]->setAutoIndent(true);
    text[current_page]->setMarginType(3,QsciScintilla::SymbolMargin);
    text[current_page]->setMarginLineNumbers(3,false);
    text[current_page]->setMarginWidth(3,15);
    text[current_page]->setMarginSensitivity(3,true);
    text[current_page]->setFolding(QsciScintilla::BoxedFoldStyle,3);
    //text[current_page]->setMarginsBackgroundColor(QColor("#bbfaae"));


}

void MainWindow::on_margin_clicked(int margin, int line, Qt::KeyboardModifiers state)
{
    Q_UNUSED(state);
    int mark = text[current_page]->markersAtLine(line);
    if (margin == 1) {//断点
        if(mark & (1<<1)){
            //去掉断点
            text[current_page]->markerDelete(line, 1);
            set<int>::iterator it;
            it = break_point[current_page].find(line);
            break_point[current_page].erase(it);
            if(gdb_dialog.state() == QProcess::Running){
                QString command = "clear " + QString::number(line+1) +"\r\n";
                debug_browser->append(command);
                gdb_dialog.write(command.toUtf8().data());
                QString output;
                if(gdb_dialog.waitForReadyRead()){
                    output = gdb_dialog.readAll();
                }
                debug_browser->append(output);
            }
        }
        else{
            //增加断点
            text[current_page]->markerAdd(line, 1);
            break_point[current_page].insert(line);
            if(gdb_dialog.state() == QProcess::Running){
                QString command = "break " + QString::number(line+1) +"\r\n";
                debug_browser->append(command);
                gdb_dialog.write(command.toUtf8().data());
                QString output;
                if(gdb_dialog.waitForReadyRead()){
                    output = gdb_dialog.readAll();
                }
                debug_browser->append(output);
            }
        }
    }
}

//开始执行
void MainWindow::on_debug_start_clicked()
{
    if(gdb_dialog.state() == QProcess::NotRunning){
        on_debug();
    }

    QString command = "start\r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    //1
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    debug_browser->append(output);
    //2
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    debug_browser->append(output);

    QString response = "(y or n)";
    if(output.contains(response)){
        command = "y\r\n";
        gdb_dialog.write(command.toUtf8().data());
        if(gdb_dialog.waitForReadyRead()){
            output = gdb_dialog.readAll();
        }
        debug_browser->append(output);
    }
    on_debug_frame_show();
}

//单步执行
void MainWindow::on_debug_step_clicked()
{
    QString command = "step\r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    debug_browser->append(output);
    on_debug_frame_show();
}

//跟踪变量
void MainWindow::on_debug_display_clicked()
{
    QString x = debug_line->text();
    QString command = "display " + x +" \r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    debug_browser->append(output);
}

//查看栈帧
void MainWindow::on_debug_frame_clicked()
{
    QString command = "frame\r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    debug_browser->append(output);
}

//开始运行
void MainWindow::on_debug_run_clicked()
{
    QString command = "run\r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    debug_browser->append(output);

    QString response = "(y or n)";
    if(output.contains(response)){
        command = "y\r\n";
        gdb_dialog.write(command.toUtf8().data());
        if(gdb_dialog.waitForReadyRead()){
            output = gdb_dialog.readAll();
        }
        debug_browser->append(output);
    }
    on_debug_frame_show();
}

//继续运行
void MainWindow::on_debug_continue_clicked()
{
    QString command = "continue\r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    debug_browser->append(output);
    on_debug_frame_show();
}

//打印变量
void MainWindow::on_debug_print_clicked()
{
    QString x = debug_line->text();
    QString command = "print " + x +" \r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    debug_browser->append(output);
}

//结束调试
void MainWindow::on_debug_quit_clicked()
{
    QString command = "quit\r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }
    command = "y\r\n";
    gdb_dialog.write(command.toUtf8().data());
    gdb_dialog.waitForFinished();
    debug_browser->append("调试结束");
    text[current_page]->markerDelete(frame_position, 2);
    frame_position = 0;
}

//栈帧显示
void MainWindow::on_debug_frame_show()
{
    QString command = "frame\r\n";
    gdb_dialog.write(command.toUtf8().data());
    QString output;
    if(gdb_dialog.waitForReadyRead()){
        output = gdb_dialog.readAll();
    }

    text[current_page]->markerDelete(frame_position, 2);
    frame_position = 0;
    for(int i=0; i<output.length(); i++){
        if(output[i] == '\n' && i+1 < output.length() && output[i+1] >= '0' && output[i+1] <= '9'){
            for(int j=i+1; j<output.length(); j++){
                if(output[j] == '\t')
                    break;
                frame_position = frame_position*10 + output[j].toLatin1()-'0';
            }
            break;
        }
    }
    //gdb下标从1开始，QT中从0开始
    if(frame_position)
        frame_position--;
    text[current_page]->markerAdd(frame_position, 2);
}


void MainWindow::onTabCurrentChanged(int index)
{
    current_page = index;
}

void MainWindow::hide_annotate()
{
    QString code = text[current_page]->text();
    fit temp;
    int j=0;
    int flag;
    QString str1;
    QString str2;
    for(int i = 0 ;code.length()!=0 && i < code.length(); i++)
    {
        flag=0;
        if(i >= 0 && i < code.length()-1 && code.at(i) == '/' && code.at(i+1) == '/')
        {
            j=i+2;
            //存储注释
            while(j < code.length() && code.at(j) != '\n')
                j++;
            temp.annotation = code.mid(i,j-i+1);
            if(i!=0)
                str1 = code.left(i);
            else
                str1 = "";
            if(j<code.length()-1)
                str2 = code.mid(j+1);
            else
                str2 = "";
            code = str1 + str2;
            j=i;
            //存储注释后的两行
            while(code.length() !=0 && j < code.length())
            {
                if(code.at(j)=='\n')
                    flag++;
                if(flag==2)
                    break;
                j++;

            }
            if(code.length() != 0)
                temp.code = code.mid(i,j-i+1);
            else
                temp.code = "";
            qDebug()<<"temp.annotation="+temp.annotation;
            qDebug()<<"temp.code="+temp.code;
            x[current_page].push(temp);
            i=-1;
        }

        if(i >= 0 && i < code.length()-1 && code.at(i) == '/' && code.at(i+1) == '*')
        {
            j=i+2;
            //存储注释
            while(j < code.length() && ((code.at(j-1) != '*' || code.at(j) != '/')))
                j++;
            temp.annotation = code.mid(i,j-i+1);
            if(i!=0)
                str1 = code.left(i);
            else
                str1 = "";
            if(j<code.length()-1)
                str2 = code.mid(j+1);
            else
                str2 = "";
            code = str1 + str2;
            j=i;
            //存储注释后的两行
            while(code.length() !=0 && j < code.length())
            {
                if(code.at(j)=='\n')
                    flag++;
                if(flag==2)
                    break;
                j++;

            }
            if(code.length() != 0)
                temp.code = code.mid(i,j-i+1);
            else
                temp.code = "";
            qDebug()<<"temp.annotation="+temp.annotation;
            qDebug()<<"temp.code="+temp.code;
            x[current_page].push(temp);
            i=-1;
        }
    }
    text[current_page]->setText(code);
}

void MainWindow::on_about()
{
    QMessageBox::information(this,tr("ya gai dou"),tr("铁Five出品"));
}

void MainWindow::show_annotate()
{
    QString code = text[current_page]->text();
    fit temp;
    int i;
    while(!x[current_page].empty())
    {
        temp = x[current_page].top();
        x[current_page].pop();
        if(temp.code.length()>0)
            i=code.lastIndexOf(temp.code);
        else
            i=-1;
        qDebug()<<"code="+code;
        qDebug("i=%d",i);
        if(i>=0)
            code.insert(i,temp.annotation);
        else
            code += temp.annotation;
    }
    text[current_page]->setText(code);
}

//打开文件夹
void MainWindow::on_open_dir()
{
    bool f = false;
    QString dirpath = QFileDialog::getExistingDirectory(this,tr("打开文件夹"),QDir::currentPath(),0);
    if(!dirpath.isEmpty())//文件夹路径不为空
    {
        QDir dir(dirpath);
        if(!dir.exists())
            return ;
        for(int i = 0; i < dir_count ;i++)
        {
            if(dirpath == dir_path[i])
            {
                f = true;
                break;
            }
        }
        if(f)
        {
            QMessageBox::warning(this,tr("提示"),tr("已经打开文件夹"));
            return;
        }
        QString dirname = dir.dirName();
        QTreeWidgetItem *project_item=new QTreeWidgetItem();
        project_item->setText(0,dirname);
        project_item->setText(1,dirpath);
        itemtree->addTopLevelItem(project_item);
        all->setHidden(false);
        getAllFiles(dirpath,dirname,project_item);
        dir_path[dir_count++] = dirpath;
    }
}

//显示文件
void MainWindow::getAllFiles(QString _file_path,QString filename,QTreeWidgetItem *project_item)//_file_path:即将遍历的目录or文件的绝对路径
{
    QDir dir(_file_path);//创一个QDir对象
    //dir取所有的文件和文件夹,去掉.文件夹和..文件夹
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    QFileInfoList file_list=dir.entryInfoList();//获取所有的文件和目录，形成一个总列表
    QStringList info_list = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);//获取所有的文件，形成一个文件列表
    //qDebug()<<info_list<<endl;
    if(file_list.size()<1)
    {
        return ;
    }

    if(file_list.size()==1&&info_list.size()==1)
    {
        //_file_path所指的只有一个文件，project_item的名称就是此文件的名称，不在添加新节点
        return ;
    }
    else
    {
        int i=0;
        do
        {
            QFileInfo file_info=file_list.at(i);//获取总列表中的所有元素
            bool _flag=file_info.isDir();//_flag判断该file_info是否是目录
            if(_flag)
            {
                //如果是目录,将本目录添加进project_item节点，并继续遍历
                QTreeWidgetItem *newitem = new QTreeWidgetItem();
                newitem->setText(0,file_info.fileName());
                newitem->setText(1,file_info.filePath());
                project_item->addChild(newitem);
                getAllFiles(file_info.filePath(),file_info.fileName(),newitem);
            }
            else
            {
                QString filter = file_info.suffix();
                //qDebug()<<filter<<endl;
                if(filter!="cpp" && filter!="c" && filter!="h" && filter!="hpp")
                {
                    //qDebug()<<"111"<<endl;
                    i++;
                    continue ;
                }
                QTreeWidgetItem *newitem_1 = new QTreeWidgetItem();
                newitem_1->setText(0,file_info.fileName());
                newitem_1->setText(1,file_info.filePath());
                project_item->addChild(newitem_1);
            }
            i++;
        }while(i<file_list.size());//知道当前目录下的所有文件和文件夹全部遍历，循环结束
    }
}

//节点点击
void MainWindow::openitem()
{
    if(qApp->mouseButtons() == Qt::LeftButton)
    {
        //如果item是一个文件的话，无法打开

        bool f = false;
        QTreeWidgetItem *tempitem = itemtree->currentItem();
        if (tempitem == NULL)
        {
            return ;
        }
        QString item_a_path = tempitem->text(1);
        QFile file(item_a_path);
        QFileInfo *fileinfo = new QFileInfo(item_a_path);
        bool flag = fileinfo->isDir();
        if(flag)
        {
            return ;
        }
        for(int i = 0; i < file_count ;i++)
        {
            if(item_a_path == file_path[i])
            {
                f = true;
                break;
            }
        }
        if(f)
        {
            QMessageBox::warning(this,tr("提示"),tr("已经打开文件"));
            return;
        }
        if(!item_a_path.isEmpty())
        {
            text[file_count] = new QsciScintilla(text_tab);
            QString file_text = QFileInfo(item_a_path).fileName();
            text_tab->addTab(text[file_count],file_text);
            text_tab->setCurrentIndex(file_count);
            text[file_count]->clear();
            if(!file.open(QIODevice::ReadOnly)||!file.isReadable())
            {
                QMessageBox::warning(this,tr("错误"),tr("不能打开文件"));
                return ;
            }

            QTextStream ins(&file);
            QFont font("Consolas",current_size);
            text[file_count]->setFont(font);
            text[file_count]->setText(ins.readAll());
            file_path[file_count] = item_a_path;
            file.close();
            file_count += 1;
            text_init(font);
        }
    }
}

