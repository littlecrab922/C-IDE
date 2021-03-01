#include "find.h"
#include "ui_find.h"

Find::Find(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Find)
{
    ui->setupUi(this);
    setWindowTitle("查找替换");
    setFixedSize(this->width(),this->height());
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(line1_text()));
    connect(ui->lineEdit_2,SIGNAL(textChanged(QString)),this,SLOT(line2_text()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(line2_text()));

}

Find::~Find()
{
    delete ui;
}

void Find::line1_text()
{
    if(ui->lineEdit->text().isEmpty())
    {
        ui->pushButton->setEnabled(false);
    }
    else
    {
        ui->pushButton->setEnabled(true);
    }
}

void Find::line2_text()
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty())
    {
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
    }
    else
    {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    }
}


void Find::on_pushButton_clicked()
{
    emit find_text(ui->lineEdit->text());
}

void Find::on_pushButton_2_clicked()
{
    emit replace_text(ui->lineEdit_2->text());
}

void Find::on_pushButton_3_clicked()
{
    emit replace_all(ui->lineEdit->text(),ui->lineEdit_2->text());
}
