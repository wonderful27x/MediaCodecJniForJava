#include <jni.h>
#include <string>

extern "C"{
#include "libmcw.h"
#include "mcw.h"
#include "mcw_jni.h"
#include "mcw_ndk.h"
#include <mcw.h>
}

// for native window JNI
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "media/NdkMediaExtractor.h"
#include <media/NdkMediaFormat.h>
#include <mcw.h>
#include "DecodeInterface.h"
#include "DataCollector.h"

static JNIEnv *mJEnv = NULL;
static JavaVM *mJavaVM = NULL;

list<string> DataCollector:: markNameList;

DataCollector* dataCollector0 = new DataCollector("codec",false);
DataCollector* dataCollector = new DataCollector("codec",true);

ANativeWindow* window;
CyberDecoderI *cyberDecoder = nullptr;

extern "C" {

struct mcw_mediacodec *codec;
struct mcw_mediaformat *mcw_format;
struct mcw *mcwMedia;

jint JNI_OnLoad(JavaVM *vm, void * /*reserved*/) {
    mJavaVM = vm;

    JNIEnv *env;

    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *vm, void * /*reserved*/) {
    mJavaVM = NULL;
}


JNIEXPORT void JNICALL
Java_com_example_mediacodecjava_MainActivity_init(JNIEnv *env, jobject thiz) {
    // TODO: implement init()
    mcw_new(mJavaVM, MCW_IMPLEMENTATION_JNI, &mcwMedia);
//    LOGE("[CODEC] init mcwMedia = %p", mcwMedia);


//    cyberDecoder = new CyberDecoderJni;
//    cyberDecoder->init(mJavaVM);
}

JNIEXPORT jlong JNICALL
Java_com_example_mediacodecjava_MainActivity_createCodec(JNIEnv *env, jobject thiz, jstring mine) {
    // TODO: implement createCodec()
    const char *type = env->GetStringUTFChars(mine, 0);
    LOGE("[CODEC] createCodec type = %s.", type);
    LOGE("[CODEC] createCodec %p", mcwMedia->mediacodec.create_decoder_by_type);
    codec = mcwMedia->mediacodec.create_decoder_by_type(type);
    LOGI("[CODEC] createCodec Ok");
    env->ReleaseStringUTFChars(mine, type);
    return reinterpret_cast<long>(codec);

//    const char *type = env->GetStringUTFChars(mine, 0);
//    LOGI("[CODEC] createDecoderByType start");
//    cyberDecoder->createDecoderByType(type);
//    LOGI("[CODEC] createDecoderByType end");
//    env->ReleaseStringUTFChars(mine, type);
//    return 0;

}


JNIEXPORT void JNICALL
Java_com_example_mediacodecjava_MainActivity_config(JNIEnv *env, jobject thiz, jobject format,
                                                    jobject surface) {
    // TODO: implement config()
    //window = ANativeWindow_fromSurface(env, surface);

    //jni
    LOGV("@@@ setsurface %p", window);
    mcw_format = mcwMedia->mediaformat.nnew();
    struct mcw_jni_mediaformat *fmt;
    fmt = (struct mcw_jni_mediaformat *)mcw_format;
    fmt->mediaformat = format;
    LOGI("[config] config ...");
    mcwMedia->mediacodec.configure(codec, mcw_format, surface, nullptr, 0);
    LOGI("[config] config Ok");

//    //ndk
//    LOGV("@@@ setsurface %p", window);
//    mcw_format = mcwMedia->mediaformat.nnew();
//    mcwMedia->mediaformat.set_string(mcw_format,"mime", "video/avc");
//    mcwMedia->mediaformat.set_int32(mcw_format,mcwMedia->mediaformat.KEY_WIDTH, 426);
//    mcwMedia->mediaformat.set_int32(mcw_format,mcwMedia->mediaformat.KEY_HEIGHT, 240);
////    mcwMedia->mediaformat.set_buffer(mcw_format,"csd-0", sps, spsSize);
////    mcwMedia->mediaformat.set_buffer(mcw_format,"csd-1", pps, ppsSize);
//    mcwMedia->mediacodec.configure(codec, mcw_format, surface, nullptr, 0);
//    LOGI("[config] config Ok");

    //cyberDecoder->config(cyberDecoder->createFormat(format),surface, nullptr,0);
}

JNIEXPORT void JNICALL
Java_com_example_mediacodecjava_MainActivity_startDecode(JNIEnv *env, jobject thiz) {
    // TODO: implement startDecode()
    mcwMedia->mediacodec.start(codec);

    //cyberDecoder->startCodec();
}

JNIEXPORT jint JNICALL
Java_com_example_mediacodecjava_MainActivity_dequeueInputBuffer(JNIEnv *env, jobject thiz,
                                                                jlong time) {
    // TODO: implement dequeueInputBuffer()
    dataCollector->captureInputBuffTimes();
    return mcwMedia->mediacodec.dequeue_input_buffer(codec, time);

    //return cyberDecoder->dequeueInputBuffer(time);
}

JNIEXPORT void JNICALL
Java_com_example_mediacodecjava_MainActivity_queueInputBuffer(JNIEnv *env, jobject thiz, jint index,
                                                              jint offset, jint size,
                                                              jlong presentation_time_us,
                                                              jint flags) {
    // TODO: implement queueInputBuffer()
    mcw_media_status status = mcwMedia->mediacodec.queue_input_buffer(codec, index, offset, size, presentation_time_us,flags);
    //cyberDecoder->queueInputBuffer(index, offset, size, presentation_time_us,flags);
    if(size >0){
        dataCollector->captureQueueInputBuffNum();
    } else{
        dataCollector->captureQueueInputBuffTimes();
    }
}

JNIEXPORT jint JNICALL
Java_com_example_mediacodecjava_MainActivity_dequeueOutputBuffer(JNIEnv *env, jobject thiz,
                                                                 jobject info, jlong timeout_us) {
    // TODO: implement dequeueOutputBuffer()
    auto *bufferinfo = new mcw_mediacodec_bufferinfo;
    dataCollector->captureDequeueOutputBuffTimes();
    return mcwMedia->mediacodec.dequeue_output_buffer(codec, bufferinfo, timeout_us);
    //return cyberDecoder->dequeueOutputBuffer(cyberDecoder->createBufferInfo(),timeout_us);
}

JNIEXPORT jobject JNICALL
Java_com_example_mediacodecjava_MainActivity_getInputBuffers(JNIEnv *env, jobject thiz) {
    // TODO: implement getInputBuffers()
    struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *) codec;
    LOGI("[Buffer] mc: %p",mc);
    return mc->in_buffers;
    //return static_cast<jobject>(cyberDecoder->getInputBuffers());
}

JNIEXPORT jobject JNICALL
Java_com_example_mediacodecjava_MainActivity_getOutputBuffers(JNIEnv *env, jobject thiz) {
    // TODO: implement getOutputBuffers()
    struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *) codec;
    return mc->out_buffers;

    //return static_cast<jobject>(cyberDecoder->getOutputBuffers());
}

JNIEXPORT jint JNICALL
Java_com_example_mediacodecjava_MainActivity_releaseOutputBuffer(JNIEnv *env, jobject thiz,
                                                                 jint index, jboolean render) {
    // TODO: implement releaseOutputBuffer()
    if(render){
        dataCollector->captureDequeueOutputBuffNum();
    }
    return mcwMedia->mediacodec.release_output_buffer(codec,index,render);
    //return cyberDecoder->releaseOutputBuffer(index,render);
}

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_mediacodecjava_MainActivity_loopBegin(JNIEnv *env, jobject thiz) {
    // TODO: implement loopBegin()
    dataCollector->startTrance();
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_mediacodecjava_MainActivity_loopEnd(JNIEnv *env, jobject thiz) {
    // TODO: implement loopEnd()
    LOGD("[DATA] ......");
    const char* info = dataCollector->takeSample(1000,100);
    if(info != nullptr){
        LOGD("[DataCollector] %s",info);
    }
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_mediacodecjava_MainActivity_dequeueInputBufferOk(JNIEnv *env, jobject thiz) {
    // TODO: implement dequeueInputBufferOk()
    dataCollector->captureInputBuffNum();
}






int64_t systemnanotime() {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000LL + now.tv_nsec;
}

bool runnig = true;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_mediacodecjava_MainActivity_playVideo(JNIEnv *env, jobject thiz, jobject surface,
                                                       jstring path) {
    // TODO: implement playVideo()
    struct mcw *mcwMediaJni = nullptr;
    struct mcw_mediacodec *codecJni = nullptr;
    struct mcw_mediaformat *mcwFormatJni = nullptr;
    struct mcw_mediaExtractor *extractor = nullptr;
    bool EOS = false;
    int64_t timeDif = -1;

    //init
    mcw_new(mJavaVM, MCW_IMPLEMENTATION_JNI, &mcwMediaJni);
    extractor = mcwMediaJni->mediaExtractor.nnew();
    if(extractor == nullptr){
        LOGE("[JNI CODEC] create extractor failed");
    }

    //setDataSource
    const char* dataPath = env->GetStringUTFChars(path,0);
    LOGD("[JNI CODEC] set_data_source...");
    mcwMediaJni->mediaExtractor.set_data_source(extractor,dataPath);
    env->ReleaseStringUTFChars(path,dataPath);

    //init codec from track format
    int trackCount = mcwMediaJni->mediaExtractor.get_track_count(extractor);
    LOGD("[JNI CODEC] trackCount: %d",trackCount);
    for (int i = 0; i < trackCount; ++i) {
        mcwFormatJni = mcwMediaJni->mediaExtractor.get_track_format(extractor,i);
        const char* mime;
        mcwMediaJni->mediaformat.get_string(mcwFormatJni,mcwMediaJni->mediaformat.KEY_MIME,&mime);
        LOGD("[JNI CODEC] mediaFormat: %s",mime);
        if(!strncmp(mime, "video/", 6)){
            mcwMediaJni->mediaExtractor.select_track(extractor,i);
            codecJni = mcwMediaJni->mediacodec.create_decoder_by_type(mime);
            if(codecJni == nullptr) {
                LOGE("[JNI CODEC] create codec failed");
                return;
            }
            break;
        }
    }

    //configure and start the codec
    if(codecJni == nullptr){
        LOGE("[JNI CODEC] codec has not been create");
        return;
    }
    mcwMediaJni->mediacodec.configure(codecJni,mcwFormatJni,surface,nullptr,0);
    mcwMediaJni->mediacodec.start(codecJni);

    while(runnig && !EOS){
        //read the data and queue
        ssize_t index = mcwMediaJni->mediacodec.dequeue_input_buffer(codecJni,10000);
        if(index >= 0) {
            size_t size;
            uint8_t* buff = mcwMediaJni->mediacodec.get_input_buffer(codecJni,index,&size);
            if(buff != nullptr) {
                LOGD("[JNI CODEC] read sample ...");
                size_t sampleSize = mcwMediaJni->mediaExtractor.read_sample_data(extractor,buff,size);
                if(sampleSize > 0){
                    LOGD("[JNI CODEC] read sample ok: %d",sampleSize);
                    long time = mcwMediaJni->mediaExtractor.get_sample_time(extractor);
                    mcwMediaJni->mediacodec.queue_input_buffer(codecJni,index,0,sampleSize,time,0);
                    mcwMediaJni->mediaExtractor.advance(extractor);
                }else{
                    EOS = true;
                    mcwMediaJni->mediacodec.queue_input_buffer(codecJni,index,0,0,0,MCW_BUFFER_FLAG_END_OF_STREAM);
                }
            }else{
                LOGD("[JNI CODEC] get_input_buffer is null !!!");
            }
        }

        //dequeue to get the decoded data and render
        struct mcw_mediacodec_bufferinfo bufferinfo;
        int outIndex = mcwMediaJni->mediacodec.dequeue_output_buffer(codecJni,&bufferinfo,10000);

        if (timeDif == -1){
            timeDif = systemnanotime() / 1000 - bufferinfo.presentation_time_us;
        }

        switch (outIndex){
            case MCW_INFO_OUTPUT_BUFFERS_CHANGED:
                break;
            case MCW_INFO_OUTPUT_FORMAT_CHANGED:
                break;
            case MCW_INFO_TRY_AGAIN_LATER:
                break;
            default:

            LOGD("[JNI CODEC] init time and render");
            int64_t delay = timeDif + bufferinfo.presentation_time_us / 1000 - systemnanotime() / 1000000;
            if (delay >0){
                LOGD("[JNI CODEC] sleep: %d",delay);
                //usleep(delay);
            }
            usleep(50000);
            LOGD("[JNI CODEC] render: %d",outIndex > 0);
            mcwMediaJni->mediacodec.release_output_buffer(codecJni,index,outIndex > 0);

            break;
        }

        if ((bufferinfo.flags & MCW_BUFFER_FLAG_END_OF_STREAM) != 0){
            break;
        }
    }

    //release
    LOGD("[JNI CODEC] release");
    mcwMediaJni->mediacodec.stop(codecJni);
    mcwMediaJni->mediaExtractor.release(extractor);

}