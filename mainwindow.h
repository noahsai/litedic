#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QClipboard>
#include<QFocusEvent>
#include<QTreeWidget>
#include<QTreeWidgetItem>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QTextStream *dic;
    void readdic();
    void  seach(QString &k,QStringList& l);
    void searchelse(QString &k);
    void searchelse2(QString &k);
    QStringList *abc[26];
    QStringList *tmp;
    QStringList *tmp2;
    QString old;
    bool en2cn;
    void maketree();


protected:

};

#endif // MAINWINDOW_H
