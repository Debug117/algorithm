#include "VideoCoder.h"
#include "DES.h"
#include "global.h"
#include <QDebug>
#include <QDateTime>
#include <QReadWriteLock>
#include <iostream>
#include <string.h>
#include <queue>
#define FRAMES_PERSECOND 25
using namespace std;

queue <AVPacket> PktQue;
QReadWriteLock PktQueLoker;

extern bool RecordOrNot;
extern bool UseQueueOrNot;

VideoCoder::VideoCoder(int No):
    CoderNo(No),
    codec_id(AV_CODEC_ID_H264)
{
    //AV_CODEC_ID_H264;AV_CODEC_ID_MPEG1VIDEO
    index=0;
    codec = NULL;
    c = NULL;
    frame =NULL;
    YuvBuffer=NULL;
    fw = NULL;
    PktSocket=NULL;

    if(UseQueueOrNot==false)
    {
        int result=init_socket();
        qDebug()<<"init_socket result:"<<result;
    }
}

int VideoCoder::init_socket()
{
    QString ip;
    global::instance()->getVideoServerIP(ip);
    IP.setAddress(ip);
    PORT= global::instance()->getVideoPort(CoderNo);
    qDebug()<<"Coder "<<CoderNo<<"  IP:"<<ip<<"  Port:"<<PORT;
    PktSocket = new QUdpSocket();
    return 0;
}

int VideoCoder::init_coder(IplImage* iplimg)
{
    avcodec_register_all();

    codec = avcodec_find_encoder(codec_id);
    if (!codec) {
        cout<<"Codec not found "<<endl;
        return -1;
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        return -2;
    }
    else
    {
        c->bit_rate = 800000;       /* put sample parameters */
        c->width = iplimg->width;   /* resolution must be a multiple of two */
        c->height = iplimg->height;
        cout<<"width:"<<iplimg->width<<"  heigth:"<<iplimg->height<<endl;
        c->time_base.num=1;         /* frames per second */
        c->time_base.den=FRAMES_PERSECOND;//= (AVRational){1,FRAMES_PERSECOND};
        c->gop_size = 10;           /* emit one intra frame every ten frames */
        c->max_b_frames = 1;
        c->pix_fmt = AV_PIX_FMT_YUV420P;
    }

    if (codec_id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        return -3;
    }

    //AVFrame *frame;
    frame = av_frame_alloc();
    if (!frame) {
        cout<<"Could not allocate video frame"<<endl;
        return -4;
    }
    else
    {
        frame->format = c->pix_fmt;
        frame->width  = c->width;
        frame->height = c->height;
    }

    /* the image can be allocated by any means and av_image_alloc() is
         * just the most convenient way if av_malloc() is to be used */
    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
                         c->pix_fmt, 32);
    if (ret < 0) {
        cout<<"Could not allocate raw picture buffer"<<endl;
        return -5;
    }
    qDebug()<<"[ init_encode  ]   SUCCESS ";
    return 0;
}

int VideoCoder::iplimg2frame_A(IplImage* IImg, AVFrame *YuvFrame)
{
    if(IImg==NULL||YuvFrame==NULL)
        return -1;

    /* Y */
    for (int y = 0; y < c->height; y++) {
        uchar *ptr = (uchar *)(IImg->imageData+y*IImg->widthStep);
        for (int x = 0; x < c->width; x++) {
            YuvFrame->data[0][y * YuvFrame->linesize[0] + x] = ptr[x*3+1];
        }
    }
    /* Cb and Cr */
    for (int y = 0; y < c->height/2; y++) {
        uchar *ptr = (uchar *)(IImg->imageData+y*IImg->widthStep);
        for (int x = 0; x < c->width/2; x++) {
            YuvFrame->data[1][y * YuvFrame->linesize[1] + x]=ptr[x];
            YuvFrame->data[2][y * YuvFrame->linesize[2] + x]=ptr[x+1];
        }
    }
    return 0;
}

int VideoCoder::iplimg2frame_B(IplImage *IImg, AVFrame *YuvFrame)
{
    if(IImg==NULL||YuvFrame==NULL)
        return -1;

    int Width=IImg->width;
    int Height=IImg->height;
    char *Ipldata=IImg->imageData;
    if(!YuvBuffer)
        YuvBuffer=(uint8_t *) malloc(((Width * Height)* 3)/2);
    AVFrame *pRgbFrame = new AVFrame;

    SwsContext * scxt = sws_getContext(Width,Height,AV_PIX_FMT_BGR24,Width,Height,AV_PIX_FMT_YUV420P,SWS_POINT,NULL,NULL,NULL);
    avpicture_fill((AVPicture*)pRgbFrame, (uint8_t*)Ipldata, AV_PIX_FMT_RGB24, Width, Height);
    //将YUV buffer 填充YUV Frame
    avpicture_fill((AVPicture*)YuvFrame, (uint8_t*)YuvBuffer, AV_PIX_FMT_YUV420P, Width, Height);
    //将RGB转化为YUV
    if(sws_scale(scxt,pRgbFrame->data,pRgbFrame->linesize,0,Height,YuvFrame->data,YuvFrame->linesize) < 0)
        printf("sws_scale Error\n");
    if(pRgbFrame)
        delete pRgbFrame;
    return 0;
}

int VideoCoder::iplimg2frame_C(IplImage *IImg, AVFrame *YuvFrame)
{
    if(IImg==NULL||YuvFrame==NULL)
        return -1;

    int Width   =IImg->width;
    int Height  =IImg->height;
    int channels    = IImg->nChannels;
    int line = IImg->widthStep;
    char *ImgData=IImg->imageData;

    unsigned char* yptr = YuvFrame->data[0];
    unsigned char* uptr = YuvFrame->data[1];
    unsigned char* vptr = YuvFrame->data[2];

    for(int y=0; y<Height; y++)
    {
        for(int x=0; x<Width; x++)
        {
            unsigned char B = (unsigned char)(ImgData[y*line+x*channels]);
            unsigned char G = (unsigned char)(ImgData[y*line+x*channels+1]);
            unsigned char R = (unsigned char)(ImgData[y*line+x*channels+2]);

            unsigned char Y = (unsigned char)( ( 66 * R + 129 * G +  25 * B + 128) >> 8) + 16  ;
            unsigned char U = (unsigned char)( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128 ;
            unsigned char V = (unsigned char)( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128 ;

            *yptr++ = Y;
            if(x%2==0 && y%2==0)
            {
                *uptr++ = U;
                *vptr++ = V;
            }
        }
    }
    return 0;
}

int VideoCoder::ENCODE(IplImage *iplImg)
{
    av_init_packet(&pkt);
    pkt.data = NULL;// packet data will be allocated by the encoder
    pkt.size = 0;
    fflush(stdout);

    if(frame==NULL)
    {
        cout<<"AVFrame* frame init fail"<<endl;
        return -1;
    }
    else
    {
        if(iplimg2frame_C(iplImg,frame)!=0)
            return -2;
    }
    frame->pts = index++; /* Presentation timestamp in time_base units */

    /* encode the image */
    ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
    if (ret < 0) {
        cout<<"Error encoding frame"<<endl;
        return -2;
    }

    if (got_output) //如果got_output=0，表示pkt为空、编码可能失败了，否则不为空
    {
        this->save_to_file();   //保存到文件
        if(UseQueueOrNot==false)//不使用队列，直接发送
        {
            if(PktSocket!=NULL)
            {
#if DES
                static char Encrypted[1024*50]={0};
                memset( Encrypted, 0, sizeof(Encrypted) );
                if( DecryptData((char*)pkt.data ,pkt.size , (char*)Encrypted ,ENCRYPT) ==0)
                {
                    //qDebug()<<"des ......  "<<pkt.size;
                     if((ret =PktSocket->writeDatagram(Encrypted, pkt.size, IP, PORT))<=0)
                         qDebug()<<"[WARN]  PktSocket writeDatagram  ERROR";
                }
                else
                    qDebug()<<"[WARN]  Encrypt ERROR";
#else
                if((ret =PktSocket->writeDatagram((char*)pkt.data,pkt.size, IP, PORT))<=0)
                {
                    //printf("AVPacket :%d__",ret);
                    //printf("[WARN] AVPacket %d Send Error:%d  \n",index,ret);//注意：pkt.pts为64为整数，冒然改变它可能造成data无法传输
                }
#endif
                PktSocket->flush();
            }
        }
        else //使用队列,发送到packet队列，等待被发送
        {
            PktQueLoker.lockForWrite();
            PktQue.push(pkt);
            PktQueLoker.unlock();
        }
        av_free_packet(&pkt);
    }
    return 0;
}

int VideoCoder::save_to_file()
{
    if(RecordOrNot==true)
    {
        if(fw==NULL)
        {
            QString OutputFile=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss").append(".h264");
            fw = fopen(OutputFile.toStdString().c_str(),"wb");
            if (!fw)
                return -1;
            else
            {
                int n=fwrite(pkt.data, 1, pkt.size, fw);//写入第一个packet
                cout<<"writed bytes:"<<n<<endl;
            }
        }
        else
        {
            int n=fwrite(pkt.data, 1, pkt.size, fw);
            cout<<"writed bytes:"<<n<<endl;
        }
    }
    else
    {
        if(fw!=NULL)
        {
            /* get the delayed frames */
            /*
            for (got_output = 1; got_output; index++)
            {
                fflush(stdout);

                ret = avcodec_encode_video2(c, &pkt, NULL, &got_output);
                if (ret < 0) {
                    //fprintf(stderr, "Error encoding frame\n");
                    return -2;
                }

                if (got_output) {
                    printf("Write frame %3d (size=%5d)\n", index, pkt.size);
                    fwrite(pkt.data, 1, pkt.size, fw);
                    av_free_packet(&pkt);
                }
            }
            */

            /* add sequence end code to have a real mpeg file */
            uint8_t endcode[] = { 0, 0, 1, 0xb7 };
            fwrite(endcode,1,sizeof(endcode),fw);
            fclose(fw);
            fw=NULL;
            cout<<"------------------ close file"<<endl;
        }
    }
    return 0;
}


void VideoCoder::print_all()
{
    if(c!=NULL)
    {
        cout<<"width "<<c->width<<endl;
        cout<<"height "<<c->height<<endl;
        cout<<"sample_rate "<<c->sample_rate<<endl;
        cout<<"bit_rate "<<c->bit_rate<<endl;
        cout<<"channels "<<c->channels<<endl;
        cout<<"time_base.den "<<c->time_base.den<<endl;//= (AVRational){1,FRAMES_PERSECOND};
        cout<<"gop_size "<<c->gop_size<<endl;           /* emit one intra frame every ten frames */
        cout<<"max_b_frames "<<c->max_b_frames<<endl;
        cout<<"pix_fmt "<<c->pix_fmt<<endl;
    }
}

VideoCoder::~VideoCoder()
{
    if(YuvBuffer!=NULL)
        free(YuvBuffer);
    if(fw!=NULL)
    {   /* add sequence end code to have a real mpeg file */
        uint8_t endcode[] = { 0, 0, 1, 0xb7 };
        fwrite(endcode,1,sizeof(endcode),fw);
        fclose(fw);
        fw=NULL;
    }
    avcodec_close(c);
    av_free(c);
    av_freep(&frame->data[0]);
    av_frame_free(&frame);
}

