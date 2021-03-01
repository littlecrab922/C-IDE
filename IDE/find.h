#ifndef FIND_H
#define FIND_H

#include <QWidget>

namespace Ui {
class Find;
}

class Find : public QWidget
{
    Q_OBJECT

public:
    explicit Find(QWidget *parent = 0);
    ~Find();
signals:
    void find_text(QString);
    void replace_text(QString);
    void replace_all(QString,QString);

private slots:
    void line1_text();
    void line2_text();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Find *ui;
};

#endif // FIND_H
