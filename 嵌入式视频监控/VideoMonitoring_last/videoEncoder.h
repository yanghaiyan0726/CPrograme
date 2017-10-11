#ifndef VIDEOENCODER_H
#define VIDEOENCODER_H

#include "ffCodecs.hpp"
#include "source.h"
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
}

using namespace std;
using namespace cv;

typedef unsigned long long uInt64;


class videoEncoder
{
public:
    videoEncoder();
    videoEncoder(std::string filename, int codecID, double frameRate, int imgWidth, int imgHeight);   //编码器和输出文件名
    ~videoEncoder();
                                                   //初始化
    bool isOpened();                                                //编码器是否已经打开
    bool open(std::string filename, int codecID, double frameRate, int imgWidth, int imgHeight); //打开编码器
    void setParam(int codecID, double frameRate, int imgWidth, int imgHeight);                    //设置参数
    void setBitrateControl(std::string controlMode, int bpsValue);         //码流控制方式及大小
    void close();                                                   //关闭编码器
    bool write(const Mat &frame);                                      //写视频

protected:
    void init();
    bool allocParamMem();               //为编码器参数分配空间
    std::string int2String(int interger);
    int flushEncoder(AVFormatContext *fmt_ctx,unsigned int stream_index);  //清空编码器的缓冲区

    //设置编码器
    int setCodec(char codec[4]);

private:
    AVFormatContext* pFormatCtx;        //描述了一个媒体文件或媒体流的构成和基本信息
    AVOutputFormat* fmt;				//输出文件格式
    AVStream* video_st;					//描述一个媒体流
    AVCodecContext* pCodecCtx;			//描述编解码器上下文的数据结构
    AVCodec* pCodec;                    //编码器格式,可自主指定，默认由封装格式后缀名决定
    AVPacket pkt;                       //视频帧packet

    //uint8_t* picture_buf;               //图像buffer
    unsigned char* picture_buf;         //图像buffer
    AVFrame* picture;
    int size;
    int y_size;                         //帧大小

    int bpsValue;                       //码流控制方式的对应值
    int bpsMode;                        //码流控制方式，0表示平均码率(abr)，1表示固定码率(crf)，2表示固定质量（cqp）

    cv::Mat image;                      //opencv中传来待编码的帧
    int img_width;                      //帧宽
    int img_height;                     //帧高
    std::string outputFileName;         //输出文件名，由后缀决定封装格式

    int fourcc;                         //编码器的四字节码
    double fps;                         //帧率
    uInt64 frameIndex;                   //当前帧序号

    bool isSetEncoderFlag;               //是否自设定编码器
    bool isOpenedFlag;                  //编码器是否已经打开

};

#endif // VIDEOENCODER_H
