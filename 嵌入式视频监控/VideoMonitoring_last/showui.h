#ifndef SHOWUI_H
#define SHOWUI_H
#include<dialog.h>
class ShowUi
{
public:
    ShowUi();
    void ShowDialog();
    void ShowPhotoList();
    void ShowVideoList();
    int Getnum();
private:
    int Takephotonum;

};

#endif // SHOWUI_H
