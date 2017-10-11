#ifndef SHOWVIDEO_H
#define SHOWVIDEO_H
#include"source.h"
namespace Ui {
class ShowVideo;
}

class ShowVideo : public QDialog
{
    Q_OBJECT

public:
    void PlayVideo();
    void closeVideo();
    void SetLable(cv::Mat frame);
    QImage Mat2QImage(cv::Mat cvImg);
    explicit ShowVideo(QWidget *parent = 0,QString VideoPath="");
    ~ShowVideo();

private slots:

    void nextFrame();

    void on_ReturnBtd_clicked();

private:
    double  rate;
    cv::Mat frame;
    QImage  image;
    QTimer *timer;
    Ui::ShowVideo *ui;
    QString VideoPath;
    cv::VideoCapture capture;
};

#endif // SHOWVIDEO_H
