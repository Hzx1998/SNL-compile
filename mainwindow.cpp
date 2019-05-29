#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"compile.h"
#include<iostream>
#include<string>
#include<vector>
#include<iterator>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
int flag=0;
vector<TokenNode> tokenList;
void MainWindow::on_pushButton_clicked()
{
    tokenList.clear();
    QString temp;
    ui->textBrowser->setTextColor(QColor(0,0,0));
    temp=ui->textEdit->toPlainText();
    string s=temp.toStdString();
    temp=QString::fromStdString(s);

    if(temp.length()!=0){
    setTokenList(s,tokenList);
    }
    int tl = tokenList.size();
    QString tokenlist1;
    QString tokenlist2;
    QString tokenlist3;
    for(int i=0;i<tl;++i){
        tokenlist1+=QString::fromStdString(tokenList[i].word)+"\n";
        tokenlist2+=QString::fromStdString(tokenword[tokenList[i].Lex])+"\n";
        tokenlist3+="line:"+QString::number(tokenList[i].linenum,10)+"\n";
    }
    ui->textBrowser_A->setText(tokenlist1);
    ui->textBrowser_B->setText(tokenlist2);
    ui->textBrowser_C->setText(tokenlist3);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser->setTextColor(QColor(255,0,0));
    if(tokenList.size()==0){
            ui->textBrowser->setText("token序列为空");
    }
    else{
        vector<TokenNode>::iterator iter=tokenList.begin();
        node *t=parse(iter);
        QString Err;
        Err=QString::fromStdString(err);
        if(err==""){
          ui->textBrowser->setText("no error");
        }
        ui->textBrowser->setText(Err);
        err="";
    }

}


