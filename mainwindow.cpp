#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setVisible(false);
    ui->lineEdit->setToolTip("输入要查询的单词");
    dic=new QTextStream;
    tmp=new QStringList;
    for(int i=0;i<26;i++)
    {
        abc[i]=new QStringList;

    }
    old="";
    readdic();
    maketree();
    ui->centralWidget->setLayout(ui->verticalLayout);
    en2cn=true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readdic()
{
    QFile file("txtdic");
    if(!file.exists()) file.setFileName(":/txtdic");

    if(file.open(QIODevice::ReadOnly))
    {
        dic->setDevice(&file);
    }
    int count=0;
    while(!dic->atEnd())
    {
        QString t=dic->readLine();
        count++;
        for(int i=0;i<26;i++)
        {
            //qDebug()<<QChar('a'+i);
            if(t.at(0)==QChar('a'+i)||t.at(0)==QChar('A'+i))
            {
                (*abc[i]).append(t);
            }
        }
    }
    qDebug()<<"There are "<<count<<"words.";//<<*abc[0];
    dic->flush();
    file.close();
    ui->listWidget->setToolTip("txt词库共有"+QString().setNum(count)+"个单词");

}

void  MainWindow::seach(QString &k,QStringList& l)
{
    old=k;
    ui->listWidget->clear();
    QStringList* tmp2=new QStringList;
    foreach (QString i, l) {
        int j=0;
        for(j=0;j<k.length();j++)
        {
            if(j>=i.length()) break;
            if(k.at(j)!=i.at(j))
            {
                break;
            }
        }
        if(j==k.length()){
            (*tmp2).append(i);
        }
    }
    QListWidgetItem *it;
    foreach (QString i, *tmp2) {
        it=new QListWidgetItem(i);
        ui->listWidget->addItem(it);
    }
    (*tmp)=(*tmp2);
}

void  MainWindow::searchelse(QString &k)
{
    old=k;
    ui->listWidget->clear();
    QStringList* tmp2=new QStringList;
    for(int i=0;i<26;i++)
    {
        foreach (QString i,*abc[i]) {
            if(i.indexOf(k)!=-1)
                (*tmp2).append(i);
        }

    }
    QListWidgetItem *it;
    foreach (QString i, *tmp2) {
        it=new QListWidgetItem(i);
        ui->listWidget->addItem(it);
    }
    (*tmp)=(*tmp2);
}

void  MainWindow::searchelse2(QString &k)
{
    old=k;
    ui->listWidget->clear();
    QStringList* tmp2=new QStringList;

    foreach (QString i,*tmp) {
        if(i.indexOf(k)!=-1)
            (*tmp2).append(i);
    }

    QListWidgetItem *it;
    foreach (QString i, *tmp2) {
        it=new QListWidgetItem(i);
        ui->listWidget->addItem(it);
    }
    (*tmp)=(*tmp2);
}



void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug()<<arg1;
    QString k=arg1.toLower();
    if(arg1!="")
    {
        if(old==""||arg1.indexOf(old)==-1)
        {
            int i;
            for(i=0;i<26;i++)
            {
                qDebug()<<"out";
                if(arg1.at(0)==QChar('a'+i)||arg1.at(0)==QChar('A'+i))
                {
                    en2cn=true;
                    seach(k,(*abc[i]));
                    break;
                }
            }
            if(i>=26)
            {
                en2cn=false;
                searchelse(k);
            }
        }
        else {
            qDebug()<<"in";
            if(en2cn)seach(k,*tmp);
            else searchelse2(k);
        }
    }
    else
    {
        ui->listWidget->clear();
    }

}


void MainWindow::maketree()
{
    qDebug()<<"start making";
    QFile file("txteg");
    if(!file.exists()) file.setFileName(":/txteg");

    QString s;

    QList<QTreeWidgetItem *> items;
    if(file.open(QIODevice::ReadOnly))
    {
        dic->setDevice(&file);
    }
    s=dic->readLine();
    while(!dic->atEnd()&&s.isEmpty()) s=dic->readLine();
//    qDebug()<<"start making2";
    while(!dic->atEnd())
    {
        if(s.at(0).isDigit()&&s.indexOf(QRegularExpression("\\d+\\."))!=-1)
        {
//            qDebug()<<"istop:"<<s;
            QTreeWidgetItem *top=NULL;
            QTreeWidgetItem *mid=NULL;
            QTreeWidgetItem *eg=NULL;
            top=new QTreeWidgetItem(QStringList(s));
            s=dic->readLine();
            while(!dic->atEnd()&&s.isEmpty()) s=dic->readLine();
            while(!dic->atEnd()&&!(s.at(0).isDigit()&&s.indexOf(QRegularExpression("\\d+\\."))!=-1))
            {
                if(s.indexOf("●")!=-1)
                {
                   // qDebug()<<"ismid:"<<s;
                    mid=new QTreeWidgetItem(top,QStringList(s.remove("●")));
                    top->addChild(mid);
                }
                else if(mid!=NULL)
                {
                  //  qDebug()<<"notmid:"<<s;
                    eg=new QTreeWidgetItem(mid,QStringList(s));
                    eg->setToolTip(0,s);
                    mid->addChild(eg);
                }
                else
                {
                   // qDebug()<<"notmid2:"<<s;
                    eg=new QTreeWidgetItem(top,QStringList(s));
                    eg->setToolTip(0,s);
                    top->addChild(eg);
                }
                s=dic->readLine();
                while(!dic->atEnd()&&s.isEmpty()) s=dic->readLine();
            }
            items.append(top);
        }
    }
//    qDebug()<<"start making3";
    ui->treeWidget->insertTopLevelItems(0,items);
    dic->flush();
    file.close();
}


void MainWindow::on_pushButton_clicked()
{
    ui->lineEdit->setVisible(true);
    ui->listWidget->setVisible(true);
    ui->treeWidget->setVisible(false);
}

void MainWindow::on_pushButton_2_clicked()
{

    ui->lineEdit->setVisible(false);
    ui->listWidget->setVisible(false);
    ui->treeWidget->setVisible(true);

}
