#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}

class VideoEncoding
{
public:
    VideoEncoding();
    ~VideoEncoding();
    static bool init();
    bool initCodecContext(bool isH265);
    bool readFrameProc(const char *input, const char *output);

private:
    AVCodecContext  *mCodecCtx;
};
