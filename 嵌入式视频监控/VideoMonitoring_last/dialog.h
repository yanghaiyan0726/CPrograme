#ifndef DIALOG_H
#define DIALOG_H
#include <QDialog>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    int GetPhotoNum();
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_ExitBtn_clicked();

    void on_SaveButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
