#ifndef VIDEOCODER_H
#define VIDEOCODER_H

extern "C"
{
#include <math.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
//added for ipl_2_frame
#include <libavutil\frame.h>
#include <basetsd.h>
#include <libswscale\swscale.h>
}
#include "cv.h"
#include "global.h"
#include <QUdpSocket>
#include <QTcpSocket>

class VideoCoder
{
public:
    VideoCoder(int No);
    ~VideoCoder();
    int init_coder(IplImage*);
    int ENCODE(IplImage *);//编码
private:
    int iplimg2frame_A(IplImage*,AVFrame *);//方法一：视频有花屏，不清晰
    int iplimg2frame_B(IplImage*,AVFrame *);//方法二：清晰，但是会造成内存泄露
    int iplimg2frame_C(IplImage*,AVFrame *);//方法三：OK
    int save_to_file();//将AVPacket保存到文件
    void print_all();
    const int CoderNo;
    const AVCodecID codec_id;
    AVCodec *codec;
    AVCodecContext *c;
    unsigned int index;
    int ret,got_output;
    FILE *fw;
    AVFrame *frame;
    AVPacket pkt;
    uint8_t *YuvBuffer;
    char appendix[7];

    QUdpSocket* PktSocket;
    QTcpSocket* PktSocketTcp;
    QHostAddress IP;
    unsigned short PORT;
    int init_socket();
};

#endif
