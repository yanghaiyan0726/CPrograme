#include "dialog.h"
#include "ui_dialog.h"
#include "database.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}
/**
 * @brief Dialog::on_ExitBtn_clicked
 */
void Dialog::on_ExitBtn_clicked()
{
    this->close();

}
/**
 * @brief Dialog::on_SaveButton_clicked
 */
void Dialog::on_SaveButton_clicked()
{
    int PhotoNum=ui->TakePhotolineEdit->text().toInt();
    int VideoNum=ui->VideolineEdit->text().toInt();
    DataBase d;
    d.createConnection();
    //d.createTable();
    d.insert(PhotoNum,VideoNum);
    d.queryAll();
}
/**
 * @brief Dialog::GetPhotoNum
 * @return
 */
int Dialog::GetPhotoNum()
{
    int  PhotoNum=ui->TakePhotolineEdit->text().toInt();
    return PhotoNum;

}
