#include"ziyuan.h"
#include"showphoto.h"
#include "listphoto.h"
#include "ui_listphoto.h"
ListPhoto::ListPhoto(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListPhoto)
{
        ui->setupUi(this);
        FilePath=PhotoFile;
}

ListPhoto::~ListPhoto()
{
       delete ui;
}
/**
 * @brief ListPhoto::ShowPhotoDialog
 * @param PhotoPath
 * Show One Photo in the Dialog
 */
void ListPhoto::ShowPhotoDialog(QString PhotoPath)
{
   this->hide();
   ShowPhoto *showphoto=new ShowPhoto(this,PhotoPath);
   showphoto->show();
   showphoto->exec();
}
/**
 * @brief ListPhoto::on_OpenListBtd_clicked
 */
void ListPhoto::on_OpenListBtd_clicked()
{
       ui->listWidget->setObjectName(QString::fromUtf8("ui->listWidget"));
       ui->listWidget->setGeometry(QRect(0, 0, 0, 0));
       QDirIterator m_DirIterator(FilePath,QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
       //保证每次进入ui->listWidget的时候都会是空的
       ui->listWidget->clear();
       while (m_DirIterator.hasNext())
       {
             QString tempFile=m_DirIterator.next();
             ui->listWidget->setIconSize(QSize(90,60));        //设置QListWidget中的单元项的图片大小
             ui->listWidget->setResizeMode(QListView::Adjust);
             ui->listWidget->setViewMode(QListView::IconMode);   //设置QListWidget的显示模式
             ui->listWidget->setMovement(QListView::Static);    //设置QListWidget中的单元项不可被拖动
             ui->listWidget->setSpacing(10);                   //设置QListWidget中的单元项的间距
             QPixmap objPixmap(tempFile);                      //生成图像objPixmap                                                          //tempfile当中的/media/sd/PICTURES字符串删去，并返回剩余部分
             tempFile=tempFile.remove(FilePath, Qt::CaseSensitive);            //生成QListWidgetItem对象
             QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(90,60))),tempFile);
             pItem->setSizeHint(QSize(90,75));            //设置单元项的宽度和高度
             ui->listWidget->addItem(pItem);              //添加QListWidgetItem项
        }
        ui->listWidget->setGeometry(0,0,330,440);
}
/**
 * @brief ListPhoto::on_listWidget_itemClicked
 * @param item
 */
void ListPhoto::on_listWidget_itemClicked(QListWidgetItem *item)
{
        if(!item)
        {
           return;
        }
        QString PhotoPath=FilePath;
        PhotoPath.append(item->text());
        ShowPhotoDialog(PhotoPath);
        this->move((QApplication::desktop()->width() - this->width())/2,
                   (QApplication::desktop()->height() - this->height())/2);
        this->show();
        update();
}




