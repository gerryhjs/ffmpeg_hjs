#include "VideoEncoding.h"
#include "VideoDecoding.h"

int main(int argv,char* args[])
{
    VideoEncoding videoEncoding;
    VideoDecoding videoDecoding;

    bool isH265=false;
    const char *inFile = args[1];//"../assets/Sample_1280x534_yuv420p.yuv";
    const char *outFile = "../assets/Sample1.h264";
    const char *outFile2 = "../assets/Sample1.h265";

    if(strcmp(args[2],"1")==0)
        isH265= true;

    if (isH265)
    {
        fprintf(stderr,"\n");
        fprintf(stderr,"[H265:Encoding...]\n");
        fprintf(stderr,"\n");

    }
    else
    {
        fprintf(stderr,"\n");
        fprintf(stderr,"[H264:Encoding...]\n");
        fprintf(stderr,"\n");

    }

    //encode

//    char* timestr=args[2];
//    int times=atoi(timestr);

//    for (int i=0;i<times;i++) {
    videoEncoding.init();
    videoEncoding.initCodecContext(isH265);
    if (isH265) {
        videoEncoding.readFrameProc(inFile, outFile2);
    } else {
        videoEncoding.readFrameProc(inFile, outFile);


    }

    if (isH265)
    {
        fprintf(stderr,"\n");
        fprintf(stderr,"[H265:Decoding...]\n");
        fprintf(stderr,"\n");
    }
    else
    {
        fprintf(stderr,"\n");
        fprintf(stderr,"[H264:Decoding...]\n");
        fprintf(stderr,"\n");
    }


    if (isH265) {
        videoDecoding.init(outFile2);
    } else {
        videoDecoding.init(outFile);

    }

    //decode
    videoDecoding.findStreamIndex();
    videoDecoding.initCodecContext();
    videoDecoding.readFrameProc();
//    }


    if (isH265)
    {
        fprintf(stderr,"\n");
        fprintf(stderr,"[H265:Finished.]\n");
        fprintf(stderr,"\n");
    }
    else
    {
        fprintf(stderr,"\n");
        fprintf(stderr,"[H264:Finished.]\n");
        fprintf(stderr,"\n");
    }


}