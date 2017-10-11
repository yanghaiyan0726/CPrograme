#ifndef SHOWPHOTO_H
#define SHOWPHOTO_H
#include <QDialog>
#include<QImage>
namespace Ui {
class ShowPhoto;
}

class ShowPhoto : public QDialog
{
    Q_OBJECT

public:
    explicit ShowPhoto(QWidget *parent = 0,QString PhotoPath="");
    ~ShowPhoto();
    void SetLable();

private slots:
    void on_CloseBtd_clicked();

private:
    Ui::ShowPhoto *ui;
    QString PhotoPath;
};

#endif // SHOWPHOTO_H
