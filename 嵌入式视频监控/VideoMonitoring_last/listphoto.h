#ifndef LISTPHOTO_H
#define LISTPHOTO_H
#include<QDialog>
#include<QWidget>
#include<QDirIterator>
#include<QListWidgetItem>
namespace Ui {
class ListPhoto;
}

class ListPhoto : public QDialog
{
    Q_OBJECT

public:
    void ShowPhotoDialog(QString PhotoPath);
    explicit ListPhoto(QWidget* parent = 0);
    ~ListPhoto();

private slots:
    void on_OpenListBtd_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);



    //void on_listWidget_activated(const QModelIndex &index);

private:
    Ui::ListPhoto *ui;
    QString FilePath;

};

#endif // LISTPHOTO_H
