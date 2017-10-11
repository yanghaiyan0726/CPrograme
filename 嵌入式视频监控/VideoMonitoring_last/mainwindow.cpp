#include"dialog.h"
#include "VideoPlay.h"
#include "videoEncoder.h"
#include"videoproduce.h"
#include "ui_VideoPlay.h"
VideoPlay::VideoPlay(QWidget *parent) :
    QVideoPlay(parent),
    ui(new Ui::VideoPlay)
{   
    VideoPlayInit();
}

VideoPlay::~VideoPlay()
{
    delete ui;
}
QImage  Mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)    //3 channels color image
    {
        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols, cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)     //grayscale image
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
 * @brief VideoPlay::MianWindowinit
 **/
void VideoPlay::VideoPlayInit()
{
    ui->setupUi(this);
    flag=false;
    pthotonum=2;
    this->setGeometry(0,0,600,450);
    ui->VideoBtn->setStyleSheet("border:1px groove white;border-radius:5px;padding:2px 4px;color:rgb(245, 245, 245);");
    ui->OverVideoBtd->setStyleSheet("border:1px groove white;border-radius:5px;padding:2px 4px;color:rgb(245, 245, 245);");
    ui->TakePhotoBtn->setStyleSheet("border:1px groove white;border-radius:5px;padding:2px 4px;color:rgb(245, 245, 245);");
    ui->comboBox->setStyleSheet("background-color: rgb(0, 0, 0);color: rgb(186, 189, 182); border: 1px solid gray; border-radius: 3px;padding: 1px 2px 1px 2px;  min-width: 6em; ");
    ui->comboBox->show();


}
/**
 * @brief VideoPlay::CloseCamarea
 */
void VideoPlay::CloseCamarea()
{
    if (capture.isOpened())
    {
        capture.release();
    }
    VideoPlayInit();
}

/**
 * @brief VideoPlay::nextFrame
 */
void VideoPlay::nextFrame()
{
    capture >> frame;
    if (!frame.empty())
    {
        image = Mat2QImage(frame);
        ui->label->setPixmap(QPixmap::fromImage(image));
    }
}
/**
 * @brief VideoPlay::Savaphoto
 * @param Filename
 * @param photoname
 * Sava Photo
 */
void VideoPlay::Savaphoto(char Filename[],char photoname[] )
{
    cv::Mat tempframe;
    capture.read(tempframe);
    char image_name[40];
    vector<int> quality;
    quality.push_back(CV_IMWRITE_JPEG_QUALITY);
    quality.push_back(90);
    sprintf(image_name,"%s%s%s",Filename,photoname,".bmp");
    cv::imwrite(image_name,tempframe,quality);
}
/**
 * @brief VideoPlay::SetLable
 * @param frame
 * ACTION: Show One Mat
 */
void VideoPlay::SetLable(cv::Mat frame)
{
    image = Mat2QImage(frame);
    ui->label->setPixmap(QPixmap::fromImage(image));
    timer = new QTimer(this);
    timer->setInterval(1000/rate);   //set timer match with FPS
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    timer->start();
}
/**
 *
 * @brief VideoPlay::showPhotoList
 * Show the Photo in the List
 */
void VideoPlay::showPhotoList()
{
    //this->hide();
    ShowUi *showui=new ShowUi;
    showui->ShowPhotoList();
   // this->show();
}
/**
 * @brief VideoPlay::showVideoList
 *Show Video in the List
 */
void VideoPlay::showVideoList()
{
    //this->hide();
    ShowUi *showui=new ShowUi;
    showui->ShowVideoList();
    //this->show();
}
/**
 * @brief VideoPlay::GetNowTime
 * @return
 */
QString VideoPlay::GetNowTime()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString(" hh-mm-ss-zzz"); //设置显示格式
    return str;
}
/**
 * @brief VideoPlay::TakePhotOne
 * Get one ScreenShot
 */
void VideoPlay::TakePhotOne()
{
    char file[]=PhotoFile;
    Savaphoto(file,GetNowTime().toLocal8Bit().data());
}
/**
 * @brief VideoPlay::TakePhotMore
 * @param num
 * ACTION Take Number Photo
 */
void VideoPlay::TakePhotMore(int num)
{
    for(int i=0;i<num;i++)
    {
        Phototimer[i] = new QTimer(this);
        connect(Phototimer[i], SIGNAL(timeout()), this, SLOT(TakePhotOne()));
        Phototimer[i]->start(1000*(i+1));
        Phototimer[i]->setSingleShot(true);
    }
}

/**
 * @brief VideoPlay::on_TakePhotoBtn_clicked
 */
void VideoPlay::on_TakePhotoBtn_clicked()
{
    TakePhotMore(pthotonum);
    showPhotoList();  
}

/**
 * @brief VideoPlay::OpenFileCapture
 * Open Video File
 */
void VideoPlay::OpenFileCapture()
{
    //decide if capture is already opened; if so,close it
    if (capture.isOpened())
        capture.release();
    filename =QFileDialog::getOpenFileName(this,tr("Open Video File"),".",tr("Video Files(*.avi *.mp4 *.flv *.mkv *.h264 )"));
    capture.open(filename.toLocal8Bit().data());
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

/**
 * @brief VideoPlay::OpenCameraCapture
 * Open the Camera
 */
void VideoPlay::OpenCameraCapture()
{
    filename="";
    if (capture.isOpened())
    {
        capture.release();
    }
    //decide if capture is already opened; if so,close it
    capture.open(0);
    //open the default camera
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

/**
 * @brief VideoPlay::on_VideoBtn_clicked
 */
void VideoPlay::on_VideoBtn_clicked()
{
    SavaVideo();
}

/**
 * @brief VideoPlay::SavaVideoFrame
 * @param filename
 * Action Sava one Frame of the Video
 */
void VideoPlay::SavaVideoFrame(char filename[])
{
    flag=false;
    ///decide if capture is already opened; if so,close it
    if (capture.isOpened())
    {
        capture.release();
    }
    capture.open(0); //open the default camera
    if (capture.isOpened())
    {
        capture >> frame;
        videoEncoder* myEncoder = new videoEncoder();
        if (!frame.empty())
        {
            myEncoder->setBitrateControl("crf",32);
            myEncoder->open(filename,CV_FOURCC('h','2','6','4'),25,VideoWidth,VideoHight);
            while(!flag)
            {
                capture>>frame;
                if(frame.empty())
                    break;
                cv::resize(frame,frame,Size(VideoWidth,VideoHight));
                myEncoder->write(frame);
                image = Mat2QImage(frame);
                ui->label->setPixmap(QPixmap::fromImage(image));
                cv::waitKey(27);
            }
            capture.release();
            myEncoder->close();
        }
    }
    delete []filename;
}
/**
 * @brief VideoPlay::SavaVideo
 * Action Sava one Video
 */
void VideoPlay::SavaVideo()
{
    QString TempVideoFile=VideoFile;
    TempVideoFile.append(GetNowTime());
    TempVideoFile.append(".h264");
    SavaVideoFrame(TempVideoFile.toLocal8Bit().data());
}

/**
 * @brief VideoPlay::Videoprocess
 */
void VideoPlay::Videoprocess()
{
    Videoproduce vproducer;
    vproducer.start();
}

/**
 * @brief VideoPlay::on_OverVideoBtd_clicked
 */
void VideoPlay::on_OverVideoBtd_clicked()
{
    flag=true;
    QMessageBox::information(NULL, "information", "Sava Successful",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    CloseCamarea();
    showVideoList();
}

/**
 * @brief VideoPlay::on_Setaction_triggered
 */
void VideoPlay::on_Setaction_triggered()
{
    ShowUi *showui=new ShowUi;
    showui->ShowDialog();
    pthotonum=showui->Getnum();
    qDebug()<<"SET:"<<pthotonum;
    this->show();
}
/**
 * @brief VideoPlay::on_actionShowLIst_triggered
 */
void VideoPlay::on_actionShowLIst_triggered()
{
    showPhotoList();
}
/**
 * @brief VideoPlay::on_actionVideoList_triggered
 */
void VideoPlay::on_actionVideoList_triggered()
{
    showVideoList();
}
/**
 * @brief VideoPlay::on_comboBox_activated
 * @param arg1
 */
void VideoPlay::on_comboBox_activated(const QString &arg1)
{
    if(!QString::compare(arg1,"打开文件"))
    {
        ui->comboBox->hide();
        OpenFileCapture();
    }
    else
    {
        ui->comboBox->hide();
        OpenCameraCapture();
    }
}
