#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HIstar.h"
#include <mysql.h>
#include <QDebug>
#include<iostream>
#include<qdialog.h>
#include<qmessagebox.h>
#include<qtablewidget.h>
#include <ctime>
#include <string>
using std::string;
#include <iostream>


#define DB_HOST "192.168.0.37"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "starbucks"

class HIstar : public QMainWindow
{
    Q_OBJECT

public:
    HIstar(QWidget *parent = Q_NULLPTR);
    void dbtest();

    QString item;
    int each111;

private:
    Ui::HIstarClass ui;
public slots:
    void search();
    void login();
    void refresh();
    void add_item();
    void order();

    //void remove();
};
