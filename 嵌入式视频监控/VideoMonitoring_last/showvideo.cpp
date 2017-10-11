#include "showvideo.h"
#include "ui_showvideo.h"

ShowVideo::ShowVideo(QWidget *parent,QString VideoPath) :
    QDialog(parent),
    ui(new Ui::ShowVideo)
{
    ui->setupUi(this);
    this->VideoPath=VideoPath;
    PlayVideo();
}

ShowVideo::~ShowVideo()
{
    delete ui;
}
/**
 * @brief ShowVideo::Mat2QImage
 * @param cvImg
 * @return
 */
QImage ShowVideo::Mat2QImage(cv::Mat cvImg)
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
 * @brief ShowVideo::nextFrame
 */
void ShowVideo::nextFrame()
{
    capture >> frame;
    if (!frame.empty())
    {
        image = Mat2QImage(frame);
        ui->label->setPixmap(QPixmap::fromImage(image));
        //this->update();
    }

}
/**
 * @brief ShowVideo::closeVideo
 */
void ShowVideo::closeVideo()
{

    if(capture.isOpened())
    {
        capture.release();
    }
}
/**
 * @brief ShowVideo::PlayVideo
 * Action Play the video
 */
void ShowVideo::PlayVideo()
{
    capture.open(VideoPath.toLocal8Bit().data());
    if (capture.isOpened())
    {
        rate= capture.get(CV_CAP_PROP_FPS);
        capture >> frame;
        if (!frame.empty())
        {
            SetLable(frame);
        }
    }
}
void ShowVideo::on_ReturnBtd_clicked()
{
    this->close();
}
/**
 * @brief ShowVideo::SetLable
 * @param frame
 */
void ShowVideo::SetLable(cv::Mat frame)
{
    image = Mat2QImage(frame);
    QImage result = image.scaled(ui->label->width(), ui->label->height(),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
    ui->label->setPixmap(QPixmap::fromImage(result));
    timer = new QTimer(this);
    timer->setInterval(1000/rate);   //set timer match with FPS
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    timer->start();
}
