#ifndef LISTVIDEO_H
#define LISTVIDEO_H
#include"source.h"
namespace Ui {
class ListVideo;
}

class ListVideo : public QDialog
{
    Q_OBJECT

public:

     ~ListVideo();
    void closeVideo();
    QImage Mat2QImage(cv::Mat cvImg);
    void GetVideoIcon(QString videoPath);
    void ShowVideoOne(QString VideoPath);
    explicit ListVideo(QWidget*parent = 0);

private slots:

    void on_listWidget_itemActivated(QListWidgetItem *item);

    void on_OpenVideoBtd_clicked();

private:
    QImage  image;
    cv::Mat frame;
    QString FilePath;
    Ui::ListVideo *ui;
    cv::VideoCapture capture;
};

#endif // LISTVIDEO_H
