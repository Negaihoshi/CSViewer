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

        QStringList fileLine = in.readAll().split("\n");
        QStringList fileParts = fileLine[0].split(",");
        int fileLineNum = fileLine.length();

        int list = fileParts.length();
        //ui->textEdit->setText(in.readLine());

        QStandardItemModel *model = new QStandardItemModel(fileLineNum-1,list,this); //2 Rows and 3 Columns

        for(int count = 0; count < list;count++){
            model->setHorizontalHeaderItem(count, new QStandardItem(QString(fileParts[count])));
        }
        ui->tableView->setModel(model);

        for(int count2 = 1 ; count2 < fileLineNum ; count2++){
            QStringList fileParts2 = fileLine[count2].split(",");
            for(int count = 0; count < list;count++){
                QStandardItem *firstRow = new QStandardItem(QString(fileParts2[count]));
                model->setItem(count2-1,count,firstRow);
            }
        }
        ui->tableView->resizeColumnsToContents();
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

