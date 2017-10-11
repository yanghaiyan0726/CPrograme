#include "listvideo.h"
#include"showvideo.h"
#include "ui_listvideo.h"
ListVideo::ListVideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListVideo)
{
    ui->setupUi(this);
    FilePath=VideoFile;
}

ListVideo::~ListVideo()
{
    delete ui;
}
/**
 * @brief ListVideo::Mat2QImage
 * @param cvImg
 * @return QImage
 */
QImage ListVideo:: Mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols, cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols,cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols,cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_RGB888);
    }
    return qImg;

}
/**
 * @brief ListVideo::closeVideo
 */
void ListVideo::closeVideo()
{

    if(capture.isOpened())
    {
        capture.release();
    }
}
/**
 * @brief ListVideo::GetVideoIcon
 * @param videoPath
 * Get first Frame in the Video
 * And Set the Icon
 */
void ListVideo::GetVideoIcon(QString videoPath)
{

    capture.open(videoPath.toLocal8Bit().data());
    capture >> frame;
    if (!frame.empty())
    {
        image = Mat2QImage(frame);
    }
    closeVideo();
}
/**
 * @brief ListVideo::ShowVideoOne
 * @param VideoPath
 *
 */
void ListVideo::ShowVideoOne(QString VideoPath)
{
    ShowVideo *showvideo=new ShowVideo(this, VideoPath);
    showvideo->show();
    //showvideo->PlayVideo();
    showvideo->exec();
}
/**
 * @brief ListVideo::on_listWidget_itemActivated
 * @param item
 */
void ListVideo::on_listWidget_itemActivated(QListWidgetItem *item)
{
    if(!item)
    {
        return;
    }
    QString VideoPath=FilePath;
    VideoPath.append(item->text());

    ShowVideoOne(VideoPath);
}
/**
 * @brief ListVideo::on_OpenVideoBtd_clicked
 */
void ListVideo::on_OpenVideoBtd_clicked()
{
    ui->listWidget->setObjectName(QString::fromUtf8("ui->listWidget"));
    ui->listWidget->setGeometry(QRect(0, 0, 0, 0));
    QDirIterator m_DirIterator(FilePath,QDir::Files|QDir::NoSymLinks,QDirIterator::Subdirectories);
    ui->listWidget->clear();//保证每次进入ui->listWidget的时候都会是空的
    while (m_DirIterator.hasNext())
    {
        QString tempFile=m_DirIterator.next();
        ui->listWidget->setIconSize(QSize(90,60));        //设置QListWidget中的单元项的图片大小
        ui->listWidget->setResizeMode(QListView::Adjust);
        ui->listWidget->setViewMode(QListView::IconMode);   //设置QListWidget的显示模式
        ui->listWidget->setMovement(QListView::Static);    //设置QListWidget中的单元项不可被拖动
        ui->listWidget->setSpacing(10);                   //设置QListWidget中的单元项的间距
        GetVideoIcon(tempFile);
        QPixmap objPixmap=QPixmap::fromImage(image);                      //生成图像objPixmap                                                          //tempfile当中的/media/sd/PICTURES字符串删去，并返回剩余部分
        tempFile=tempFile.remove(FilePath, Qt::CaseSensitive);
        //生成QListWidgetItem对象
        QListWidgetItem *pItem = new QListWidgetItem(QIcon(objPixmap.scaled(QSize(80,60))),tempFile);
        pItem->setSizeHint(QSize(90,75));            //设置单元项的宽度和高9
        ui->listWidget->addItem(pItem);              //添加QListWidgetItem项
    }
    ui->listWidget->setGeometry(0,0,335,440);
}

