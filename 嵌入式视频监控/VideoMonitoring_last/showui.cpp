#include "showui.h"
#include"listphoto.h"
#include"listvideo.h"
ShowUi::ShowUi()
{
}
/**
 * @brief ShowUi::ShowDialog
 */
void ShowUi::ShowDialog()
{
    Dialog* dialog=new Dialog();
    dialog->show();
    dialog->exec();
    Takephotonum=dialog->GetPhotoNum();
}
/**
 * @brief ShowUi::Getnum
 * @return
 */
int ShowUi::Getnum()
{
    return Takephotonum;
}
/**
 * @brief ShowUi::ShowPhotoList
 */
void ShowUi::ShowPhotoList()
{
    ListPhoto*listPhoto=new ListPhoto();
    listPhoto->show();
    listPhoto->exec();
}
/**
 * @brief ShowUi::ShowVideoList
 */
void ShowUi::ShowVideoList()
{
    ListVideo* listvideo=new ListVideo();
    listvideo->show();
    listvideo->exec();
}
