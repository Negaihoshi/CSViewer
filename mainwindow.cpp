#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "QFile"
#include "QFileDialog"
#include "QMessageBox"
#include "QTextStream"
#include "QStandardItemModel"
#include "QTableWidget"
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

void MainWindow::on_pushButton_clicked()
{
    qApp->quit();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("All Files (*);;Text Files (*.txt);;C++ Files (*.cpp *.h);;CSV Files (*.csv)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        QTextStream in(&file);
        //in.setCodec("UTF-8");
        QString line = in.readLine();
        int lineCount = 0;
        while (!line.isNull()) {
            lineCount++;
        }

        QStringList fileParts = in.readLine().split(",");
        int list = fileParts.length();
        //ui->textEdit->setText(in.readLine());

        QStandardItemModel *model = new QStandardItemModel(2,list,this); //2 Rows and 3 Columns
        for(int count2=0;count2<lineCount;count2++){

            for(int count = 0; count < list;count++){
                model->setHorizontalHeaderItem(count, new QStandardItem(QString(fileParts[count])));
            }
        }
        //model->setHorizontalHeaderItem(0, new QStandardItem(QString(fileParts[0])));
        //model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
        //model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));
        ui->tableView->setModel(model);
        QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));
        model->setItem(0,0,firstRow);
        file.close();
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
            QTextStream stream(&file);
            file.close();
        }
    }
}

