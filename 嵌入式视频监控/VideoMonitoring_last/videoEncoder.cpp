#include "videoEncoder.h"

/**
 * @brief videoEncoder::videoEncoder
 */
videoEncoder::videoEncoder():isOpenedFlag(false)
{
    pFormatCtx = nullptr;
    pCodecCtx = nullptr;
    pCodec = nullptr;
    picture_buf= nullptr;
    picture = nullptr;
    isSetEncoderFlag = false;
    bpsMode = 1;
    bpsValue = 28;
    frameIndex = 0;
    init();
}
/**
 * @brief videoEncoder::videoEncoder
 * @param filename
 * @param codecID
 * @param frameRate
 * @param imgWidth
 * @param imgHeight
 */
videoEncoder::videoEncoder(std::string filename, int codecID,double frameRate,
                           int imgWidth,int imgHeight)
{

    fourcc = codecID;
    isSetEncoderFlag = true;
    outputFileName = filename;
    fps = frameRate;
    img_width = imgWidth;
    img_height = imgHeight;
    bpsMode = 1;
    bpsValue = 28;
    frameIndex = 0;
    init();
    if(!allocParamMem())
        isOpenedFlag = false;
    else
        isOpenedFlag = true;
}

videoEncoder::~videoEncoder()
{   

}
/**
 * @brief videoEncoder::init
 * 初始化
 */
void videoEncoder::init()
{
    av_register_all();      //注册ffmpeg所有组件
}
/**
 * @brief videoEncoder::isOpened
 * @return
 * 判断视频编码器是否已经打开
 */
bool videoEncoder::isOpened()
{
    return isOpenedFlag;
}

/**
 * @brief videoEncoder::open
 * @param filename
 * @param codecID
 * @param frameRate
 * @param imgWidth
 * @param imgHeight
 * @return
 * 打开写视频器
 */
bool videoEncoder::open(std::string filename, int codecID, double frameRate,
                        int imgWidth,int imgHeight)
{
    this->outputFileName = filename;
    this->fourcc = codecID;
    isSetEncoderFlag = true;
    this->fps = frameRate;
    this->img_width = imgWidth;
    this->img_height = imgHeight;
    if(!allocParamMem())
        isOpenedFlag = false;
    else
        isOpenedFlag = true;
    return isOpenedFlag;
}

/**
 * @brief videoEncoder::setParam
 * @param codecID
 * @param frameRate
 * @param imgWidth
 * @param imgHeight
 * 设置编码器和帧率
 */
void videoEncoder::setParam(int codecID, double frameRate,int imgWidth,int imgHeight)
{
    this->fourcc = codecID;
    this->isSetEncoderFlag = true;
    this->fps = frameRate;
    this->img_width = imgWidth;
    this->img_height = imgHeight;
}

/**
 * @brief videoEncoder::setBitrateControl
 * @param controlMode
 * @param bpsValue
 * 设置编码器的码率控制方式
 */
void videoEncoder::setBitrateControl(std::string controlMode, int bpsValue)
{
    if(controlMode.compare("abr")==0)
        bpsMode = 0;
    else if(controlMode.compare("crf")==0)
        bpsMode = 1;
    else if(controlMode.compare("cqp")==0)
        bpsMode = 2;
    else
        bpsMode = 0;
    this->bpsValue = bpsValue;
}
/**
 * @brief videoEncoder::close
 *
 *关闭写视频器
 */
void videoEncoder::close()
{
    int ret = flushEncoder(pFormatCtx,0);
    if(ret<0)
        return;
    //写文件尾
    av_write_trailer(pFormatCtx);
    //清理
    if (video_st)
    {
        avcodec_close(video_st->codec);
        av_free(picture);
        av_free(picture_buf);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);
    return;
}
/**
 * @brief videoEncoder::write
 * @param frame
 * @return
 * 写视频帧
 */
bool videoEncoder::write(const cv::Mat& frame)
{
    cv::Mat temp;
    cvtColor(frame,temp,COLOR_BGR2YUV_I420);

    picture_buf = temp.data;

    picture->data[0] = picture_buf;         //Y
    picture->data[1] = picture_buf+y_size;  //U
    picture->data[2] = picture_buf+(y_size*5/4);//V

    picture->pts = frameIndex++;

    int got_picture = 0;
    int ret = avcodec_encode_video2(pCodecCtx,&pkt,picture,&got_picture);
    if(ret<0)
    {
        return false;
    }
    if(got_picture==1)
    {
        pkt.stream_index = video_st->index;
        ret = av_interleaved_write_frame(pFormatCtx,&pkt);
        av_free_packet(&pkt);
    }
    return true;
}

/**
 * @brief videoEncoder::allocParamMem
 * @return
 * 分配参数空间
 */
bool videoEncoder::allocParamMem()
{
    avformat_alloc_output_context2(&pFormatCtx,NULL,NULL,outputFileName.c_str());   //分配format空间
    AVCodecID codec_id = AV_CODEC_ID_NONE;
    if(isSetEncoderFlag)
    {
        /* Lookup codec_id for given fourcc */
#if LIBAVCODEC_VERSION_INT<((51<<16)+(49<<8)+0)
        if( (codec_id = codec_get_bmp_id( fourcc )) == AV_CODEC_ID_NONE )
            return false;
#else
        const struct AVCodecTag * tags[] = { codec_bmp_tags, NULL};
        if( (codec_id = av_codec_get_id(tags, fourcc)) == AV_CODEC_ID_NONE)
            return false;
#endif

        pFormatCtx->oformat->video_codec = codec_id;
    }
    fmt = pFormatCtx->oformat;
    if(avio_open(&pFormatCtx->pb,outputFileName.c_str(),AVIO_FLAG_WRITE)<0)
    {
        return false;
    }
    video_st = avformat_new_stream(pFormatCtx,0);
    if(video_st==NULL)
        return false;
    video_st->codec->codec_tag = 0;
    if(pFormatCtx->oformat->flags&AVFMT_GLOBALHEADER)
        video_st->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    video_st->time_base.den = fps;
    video_st->time_base.num = 1;

    pCodecCtx = video_st->codec;
    pCodecCtx->codec_id = fmt->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = img_width;
    pCodecCtx->height = img_height;
    pCodecCtx->gop_size=250;
    pCodecCtx->max_b_frames = 4;
    pCodecCtx->time_base.den=25;
    pCodecCtx->time_base.num=1;
    pCodecCtx->qmin = 10;
    pCodecCtx->qmax = 51;
    pCodecCtx->qcompress = 0.6;

    //码率控制方式
    string modeValue = int2String(bpsValue);
    switch (bpsMode) {
    case 0:
        pCodecCtx->bit_rate = bpsValue*1000;
        break;
    case 1:
        av_opt_set(pCodecCtx->priv_data,"crf",modeValue.c_str(),AV_OPT_SEARCH_CHILDREN);
        break;
    case 2:
        av_opt_set(pCodecCtx->priv_data,"qp",modeValue.c_str(),AV_OPT_SEARCH_CHILDREN);
        break;
    default:
        pCodecCtx->bit_rate = bpsValue;
        break;
    }


    //编码器预设
    AVDictionary *param = 0;
    if(pCodecCtx->codec_id == AV_CODEC_ID_H264)
    {
        av_dict_set(&param,"preset","medium",0);
        av_dict_set(&param,"tune","zerolatency",0);
        av_dict_set(&param,"profile","main",0);
    }

    if(pCodecCtx->codec_id == AV_CODEC_ID_H265)
    {
        av_dict_set(&param,"preset","ultrafast",0);
        av_dict_set(&param,"tune","zerolatency",0);
        av_dict_set(&param,"profile","main",0);
    }

    //输出格式信息
    av_dump_format(pFormatCtx, 0, outputFileName.c_str(), 1);

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec)
    {
        return false;
    }
    if (avcodec_open2(pCodecCtx, pCodec,&param) < 0)
    {
        return false;
    }

    picture = av_frame_alloc();
    picture->width = pCodecCtx->width;
    picture->height = pCodecCtx->height;
    picture->format = pCodecCtx->pix_fmt;
    size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    picture_buf = (uint8_t *)av_malloc(size);
    avpicture_fill((AVPicture *)picture, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    //写文件头
    avformat_write_header(pFormatCtx,NULL);
    y_size = pCodecCtx->width * pCodecCtx->height;
    av_new_packet(&pkt,y_size*3);
    return true;
}

//整数转string
std::string videoEncoder::int2String(int interger)
{
    strstream sstream;
    string str;
    sstream<<interger;
    sstream>>str;
    return str;
}

/**
 * @brief videoEncoder::flushEncoder
 * @param fmt_ctx
 * @param stream_index
 * @return
 * 清空编码器缓冲区
 */
int videoEncoder::flushEncoder(AVFormatContext *fmt_ctx, unsigned int stream_index)
{
    int ret;
    int got_frame;
    AVPacket enc_pkt;
    if (!(fmt_ctx->streams[stream_index]->codec->codec->capabilities &
          CODEC_CAP_DELAY))
        return 0;
    while (1)
    {
        //printf("Flushing stream #%u encoder\n", stream_index);
        enc_pkt.data = NULL;
        enc_pkt.size = 0;
        av_init_packet(&enc_pkt);
        ret = avcodec_encode_video2 (fmt_ctx->streams[stream_index]->codec, &enc_pkt,
                                     NULL, &got_frame);
        av_frame_free(NULL);
        if (ret < 0)
            break;
        if (!got_frame)
        {
            ret=0;
            break;
        }
        /* mux encoded frame */
        ret = av_write_frame(fmt_ctx, &enc_pkt);
        if (ret < 0)
            break;
    }
    return ret;
}
/**
 * @brief videoEncoder::setCodec
 * @param codec
 * @return
 */
int videoEncoder::setCodec(char codec[4])
{
    union MyUnion
    {
        int value;
        char code[4];
    }codeValue;
    codeValue.code[0] = codec[0];
    codeValue.code[1] = codec[1];
    codeValue.code[2] = codec[2];
    codeValue.code[3] = codec[3];

    return codeValue.value;
}
