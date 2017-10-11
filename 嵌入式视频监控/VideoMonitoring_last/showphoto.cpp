#include "showphoto.h"
#include "ui_showphoto.h"

ShowPhoto::ShowPhoto(QWidget *parent,QString PhotoPath) :
    QDialog(parent),
    ui(new Ui::ShowPhoto)
{
    ui->setupUi(this);
    this->PhotoPath=PhotoPath;
    SetLable();
}
/**
 * @brief ShowPhoto::SetLable
 */
void ShowPhoto::SetLable()
{
    QImage image,result;
    image.load(PhotoPath);//temppath为图片的路径
    result = image.scaled(ui->label->width(), ui->label->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->label->setPixmap(QPixmap::fromImage(result));//在Label控件上显示图片
}
/**
 * @brief ShowPhoto::~ShowPhoto
 */
ShowPhoto::~ShowPhoto()
{
    delete ui;
}
/**
 * @brief ShowPhoto::on_CloseBtd_clicked
 */
void ShowPhoto::on_CloseBtd_clicked()
{
    this->close();
}
