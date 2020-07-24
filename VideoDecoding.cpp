#include "VideoDecoding.h"

VideoDecoding::VideoDecoding() :
    mFormatCtx(nullptr), mCodecCtx(nullptr), mVideoStreamIndex(-1)
{

}

VideoDecoding::~VideoDecoding()
{
    avcodec_free_context(&mCodecCtx);
    avformat_close_input(&mFormatCtx);
}

bool VideoDecoding::init(const char * file)
{
//    av_register_all();

    if ((avformat_open_input(&mFormatCtx, file, 0, 0)) < 0) {
        printf("Failed to open input file\n");
    }

    if ((avformat_find_stream_info(mFormatCtx, 0)) < 0) {
        printf("Failed to retrieve input stream information\n");
    }

    av_dump_format(mFormatCtx, 0, file, 0);

    return false;
}

bool VideoDecoding::findStreamIndex()
{
    // Find video stream in the file
    mVideoStreamIndex = av_find_best_stream(mFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (mVideoStreamIndex < 0) {
        printf("Could not find stream in input file\n");
        return true;
    }

    return false;
}

// Initialize the AVCodecContext to use the given AVCodec.
bool VideoDecoding::initCodecContext()
{
    // Find a decoder with a matching codec ID
    AVCodec *codec = avcodec_find_decoder(mFormatCtx->streams[mVideoStreamIndex]->codecpar->codec_id);
    if (!codec) {
        printf("Failed to find h264_videotoolbox!\n");
        codec = avcodec_find_decoder(AV_CODEC_ID_H264);
        if (!codec)
        {
            printf("Failed to find codec!\n");
            return true;
        }
    }

    // Allocate a codec context for the decoder
    if (!(mCodecCtx = avcodec_alloc_context3(codec))) {
        printf("Failed to allocate the codec context\n");
        return true;
    }

    // Fill the codec contex based on the supplied codec parameters.
    if (avcodec_parameters_to_context(mCodecCtx, mFormatCtx->streams[mVideoStreamIndex]->codecpar) < 0) {
        printf("Failed to copy codec parameters to decoder context!\n");
        return true;
    }

    // Initialize the AVCodecContext to use the given Codec
    if (avcodec_open2(mCodecCtx, codec, nullptr) < 0) {
        printf("Failed to open codec\n");
        return true;
    }

    return false;
}

bool VideoDecoding::readFrameProc()
{
    AVPacket packet;
    AVFrame *frame = av_frame_alloc();
    int tmpW = mFormatCtx->streams[mVideoStreamIndex]->codecpar->width;
    int tmpH = mFormatCtx->streams[mVideoStreamIndex]->codecpar->height;
    char outFile[40] = { 0 };
    sprintf(outFile, "../assets/Sample_%dx%d_yuv420p_out.yuv", tmpW, tmpH);

    FILE *fd = fopen(outFile, "wb");

    while (int num = av_read_frame(mFormatCtx, &packet) >= 0) {
        // find a video stream
        if (packet.stream_index == mVideoStreamIndex) {
            decodeVideoFrame(&packet, frame, fd);
        }

        av_packet_unref(&packet);
    }

    fclose(fd);

//    printf("Generate video files successfully!\nUse ffplay to play the yuv420p raw video.\n");
//    printf("ffplay -f rawvideo -pixel_format yuv420p -video_size %dx%d %s.\n", tmpW, tmpH, outFile);

    return false;
}

bool VideoDecoding::decodeVideoFrame(AVPacket *pkt, AVFrame *frame, FILE *fd)
{
    avcodec_send_packet(mCodecCtx, pkt);
    int ret = avcodec_receive_frame(mCodecCtx, frame);
    if (!ret) {

        // save YUV420p uncompressed video
        saveYUV(frame, fd);

        printf("."); // program running state
        return false;
    }

    return true;
}

bool VideoDecoding::saveYUV(AVFrame *frame, FILE *fd)
{
    fwrite(frame->data[0], 1, mCodecCtx->width *mCodecCtx->height, fd);
    fwrite(frame->data[1], 1, mCodecCtx->width*mCodecCtx->height / 4, fd);
    fwrite(frame->data[2], 1, mCodecCtx->width*mCodecCtx->height / 4, fd);
    return false;
}
