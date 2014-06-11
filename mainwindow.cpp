#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "QFile"
#include "QFileDialog"
#include "QMessageBox"
#include "QTextStream"
#include "QStandardItemModel"
#include "QTableWidget"
#include <QtXml>

using namespace std;
QStandardItemModel *model = new QStandardItemModel();
QStringList Header;


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


void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("CSV Files (*.csv);;JSON Files (*.json)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->tableView->model());
            //model->data(model->index(0,0));
            qint32 RowMax = model->rowCount();
            qint32 ColumnMax = model->columnCount();
            stream << Header.join(",");
            stream << "\r\n";
            QStringList csvData;
            for(int count  = 0; count < RowMax;count++){
                for(int count2  = 0; count2 < ColumnMax;count2++){
                    csvData << model->data(model->index(count,count2)).toString();
                    if(count2==ColumnMax-1){
                        stream << csvData.join(",") << "\n";
                        csvData.clear();
                    }
                }
            }
            file.close();
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("CSV Files (*.csv)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);

        in.setCodec("UTF-8");

        QStringList fileLine = in.readAll().split("\n");
        QStringList fileParts = fileLine[0].split(",");
        int fileLineNum = fileLine.length();

        int list = fileParts.length();
        //ui->textEdit->setText(in.readLine());

        QStandardItemModel *model = new QStandardItemModel(fileLineNum-1,list,this); //2 Rows and 3 Columns

        for(int count = 0; count < list;count++){
            model->setHorizontalHeaderItem(count, new QStandardItem(QString(fileParts[count])));
            Header<<fileParts[count];
        }
        ui->tableView->setModel(model);

        for(int count2 = 1 ; count2 < fileLineNum ; count2++){
            QStringList fileParts2 = fileLine[count2].split(",");
            for(int count = 0; count < list;count++){
                QStandardItem *firstRow = new QStandardItem(QString(fileParts2[count]));
                model->setItem(count2-1,count,firstRow);
            }
        }
        //Resizes all columns based on the size hints of the delegate used to render each item in the columns.
        ui->tableView->resizeColumnsToContents();
        //Set Visible
        ui->tableView->verticalHeader()->setVisible(false);

        QStringList fileNameList = fileName.split("/");
        int fileNameListNum = fileNameList.length();
        QString newName;
        newName = "CSViewer | " + fileNameList[fileNameListNum-1];
        this->setWindowTitle(newName);
        file.close();
    }
}

void MainWindow::on_pushButton_4_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("JSON Files (*.json)"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
        } else {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->tableView->model());
            //model->data(model->index(0,0));
            qint32 RowMax = model->rowCount();
            qint32 ColumnMax = model->columnCount();
            stream << "[";
            QStringList jsonData;
            QStringList jsonData2;
            for(int count  = 0; count < RowMax;count++){
                stream << "{";
                for(int count2  = 0; count2 < ColumnMax;count2++){
                    if(isDigitStr(model->data(model->index(count,count2)).toString())==0){
                        jsonData << "\"" << Header[count2] << "\":" << model->data(model->index(count,count2)).toString();
                        if(count == RowMax-1) jsonData << "\" \"";
                    }else{
                        jsonData << "\"" << Header[count2] << "\":\"" << model->data(model->index(count,count2)).toString() << "\"";
                    }
                    jsonData2 << jsonData.join("");
                    jsonData.clear();
                    if(count2==ColumnMax-1){
                        stream << jsonData2.join(",");

                        jsonData2.clear();
                    }
                }
                if(count == RowMax-1) stream << "}";
                else stream << "}," << "\r\n";
            }
            stream << "]";
            file.close();
        }
    }
}
int MainWindow::isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();
     const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s)
    { //not Integer
        return -1;
    }
    else
    { //is Integer
        return 0;
    }
}
