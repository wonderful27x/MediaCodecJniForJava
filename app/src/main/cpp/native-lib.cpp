
#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <media/NdkMediaFormat.h>
#include <pthread.h>


#include "libmcw.h"
#include "mcw.h"
#include "mcw_jni.h"
#include "mcw_ndk.h"
#include "media/NdkMediaExtractor.h"
#include "DecodeInterface.h"
#include "MediaDataCollector.h"
#include "DataCollector.h"


static JNIEnv *mJEnv = NULL;
static JavaVM *mJavaVM = NULL;

DataCollector* dataCollector = new DataCollector("codec");
MediaDataCollector* mediaDataCollector = new MediaDataCollector("codec");


extern "C" {

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


    int64_t systemnanotime() {
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return now.tv_sec * 1000000000LL + now.tv_nsec;
    }
    bool running = true;

    JNIEXPORT void JNICALL
    Java_com_example_mediacodecjava_MainActivity_playVideo(JNIEnv *env, jobject thiz, jobject surface,
                                                           jstring path) {
        // TODO: implement playVideo()

        struct mcw *mcwMedia = nullptr;
        struct mcw_mediacodec *codec = nullptr;
        struct mcw_mediaformat *mcwFormat = nullptr;
        struct mcw_mediaExtractor *extractor = nullptr;
        bool EOS = false;
        int64_t timeDif = -1;

        //init
        mcw_new(mJavaVM, MCW_IMPLEMENTATION_AUTO, &mcwMedia);
        extractor = mcwMedia->mediaExtractor.nnew();

        //setDataSource
        const char *dataPath = env->GetStringUTFChars(path, 0);
        mcwMedia->mediaExtractor.set_data_source(extractor, dataPath);
        env->ReleaseStringUTFChars(path, dataPath);

        //init codec from track format
        int trackCount = mcwMedia->mediaExtractor.get_track_count(extractor);
        for (int i = 0; i < trackCount; ++i) {
            mcwFormat = mcwMedia->mediaExtractor.get_track_format(extractor, i);
            const char *mime;
            mcwMedia->mediaformat.get_string(mcwFormat, mcwMedia->mediaformat.KEY_MIME, &mime);
            if (!strncmp(mime, "video/", 6)) {
                mcwMedia->mediaExtractor.select_track(extractor, i);
                codec = mcwMedia->mediacodec.create_decoder_by_type(mime);
                if (codec == nullptr) {
                    return;
                }
                break;
            }
        }

        LOGD("config");
        //configure and start the codec
        if (codec == nullptr) {
            return;
        }
        mcwMedia->mediacodec.configure(codec, mcwFormat, surface, nullptr, 0);
        mcwMedia->mediacodec.start(codec);

        while (running && !EOS) {

            dataCollector->startTrance();
            const char* info = dataCollector->takeSample();
            if(info != nullptr){
                LOGD("DECODE INFO: %s",info);
            }

            //read the data and queue
            ssize_t index = mcwMedia->mediacodec.dequeue_input_buffer(codec, 10000);
            if (index >= 0) {
                size_t size;
                uint8_t *buff = mcwMedia->mediacodec.get_input_buffer(codec, index, &size);
                dataCollector->capture("get_input_buffer_times");
                if (buff != nullptr) {
                    dataCollector->capture("get_input_buffer_num");
                    size_t sampleSize = mcwMedia->mediaExtractor.read_sample_data(extractor, buff,size);
                    if (sampleSize >= 0) {
                        long time = mcwMedia->mediaExtractor.get_sample_time(extractor);
                        mcwMedia->mediacodec.queue_input_buffer(codec, index, 0, sampleSize, time,0);
                        mcwMedia->mediaExtractor.advance(extractor);
                        dataCollector->capture("queue_input_buffer_times");
                        dataCollector->capture("queue_input_buffer_num");
                    } else {
                        EOS = true;
                        mcwMedia->mediacodec.queue_input_buffer(codec, index, 0, 0, 0,MCW_BUFFER_FLAG_END_OF_STREAM);
                    }
                }
            }

            //dequeue to get the decoded data and render
            struct mcw_mediacodec_bufferinfo bufferinfo;
            int outIndex = mcwMedia->mediacodec.dequeue_output_buffer(codec, &bufferinfo, 10000);
            dataCollector->capture("dequeue_output_buffer_times");
            if (timeDif == -1) {
                timeDif = systemnanotime() / 1000 - bufferinfo.presentation_time_us;
            }

            switch (outIndex) {
                case MCW_INFO_OUTPUT_BUFFERS_CHANGED:
                    break;
                case MCW_INFO_OUTPUT_FORMAT_CHANGED:
                    break;
                case MCW_INFO_TRY_AGAIN_LATER:
                    break;
                default:

                    int64_t delay = timeDif + bufferinfo.presentation_time_us - systemnanotime() / 1000;
                    if (delay > 0) {
                        //usleep(delay);
                    }
                    mcwMedia->mediacodec.release_output_buffer(codec, outIndex, outIndex > 0);
                    if(outIndex > 0){
                        dataCollector->capture("dequeue_output_buffer_num");
                    }
                    break;
            }

            if ((bufferinfo.flags & MCW_BUFFER_FLAG_END_OF_STREAM) != 0) {
                //break;
            }
        }

        //release
        mcwMedia->mediacodec.stop(codec);
        mcwMedia->mediaExtractor.release(extractor);

    }


    struct mcw *mcwMedia = nullptr;
    struct mcw_mediacodec *codec = nullptr;
    struct mcw_mediaformat *mcwFormat = nullptr;
    struct mcw_mediaExtractor *extractor = nullptr;
    bool EOS = false;
    int64_t timeDif = -1;

    pthread_t decodeThread;

    void *decodeTask(void *) {
        while (running && !EOS) {

            mediaDataCollector->startTrance();
            const char* info = mediaDataCollector->takeSample();
            if(info != nullptr){
                LOGD("DECODE INFO: %s",info);
            }

            //read the data and queue
            ssize_t index = mcwMedia->mediacodec.dequeue_input_buffer(codec, 10000);
            if (index >= 0) {
                size_t size;
                uint8_t *buff = mcwMedia->mediacodec.get_input_buffer(codec, index, &size);
                mediaDataCollector->capture("captureInputBuffTimes");
                if (buff != nullptr) {
                    mediaDataCollector->capture("captureInputBuffNum");
                    LOGD("[JNI CODEC] read sample ...");
                    size_t sampleSize = mcwMedia->mediaExtractor.read_sample_data(extractor, buff,
                                                                                     size);
                    if (sampleSize >= 0) {
                        LOGD("[JNI CODEC] read sample ok: %d", sampleSize);
                        long time = mcwMedia->mediaExtractor.get_sample_time(extractor);
                        mcwMedia->mediacodec.queue_input_buffer(codec, index, 0, sampleSize, time,0);
                        mcwMedia->mediaExtractor.advance(extractor);
                        mediaDataCollector->capture("captureQueueInputBuffTimes");
                        mediaDataCollector->capture("captureQueueInputBuffNum");
                    } else {
                        EOS = true;
                        mcwMedia->mediacodec.queue_input_buffer(codec, index, 0, 0, 0,MCW_BUFFER_FLAG_END_OF_STREAM);
                        LOGD("[JNI CODEC] end of stream !");
                    }
                } else {
                    LOGD("[JNI CODEC] get_input_buffer is null !!!");
                }
            }

            //dequeue to get the decoded data and render
            struct mcw_mediacodec_bufferinfo bufferinfo;
            int outIndex = mcwMedia->mediacodec.dequeue_output_buffer(codec, &bufferinfo, 10000);
            mediaDataCollector->capture("captureDequeueOutputBuffTimes");

            if (timeDif == -1) {
                timeDif = systemnanotime() / 1000 - bufferinfo.presentation_time_us;
            }

            switch (outIndex) {
                case MCW_INFO_OUTPUT_BUFFERS_CHANGED:
                    break;
                case MCW_INFO_OUTPUT_FORMAT_CHANGED:
                    break;
                case MCW_INFO_TRY_AGAIN_LATER:
                    break;
                default:

                    LOGD("[JNI CODEC] init time and render");
                    int64_t currentTimeUs = systemnanotime() / 1000;
                    int64_t delay = timeDif + bufferinfo.presentation_time_us - currentTimeUs;
                    LOGD("[JNI CODEC] timeDif: %lld us", timeDif);
                    LOGD("[JNI CODEC] pts: %lld us", bufferinfo.presentation_time_us);
                    LOGD("[JNI CODEC] currentTimeUs: %lld us", currentTimeUs);
                    LOGD("[JNI CODEC] sleep: %lld us", delay);
                    if (delay > 0) {
                        usleep(delay);
                    }
                    LOGD("[JNI CODEC] render: %d", outIndex > 0);
                    mcwMedia->mediacodec.release_output_buffer(codec, outIndex, outIndex > 0);
                    if(outIndex > 0){
                        mediaDataCollector->capture("captureDequeueOutputBuffNum");
                    }
                    break;
            }

            LOGD("[JNI CODEC] info flag : %d",bufferinfo.flags);
            if ((bufferinfo.flags & MCW_BUFFER_FLAG_END_OF_STREAM) != 0) {
                LOGD("[JNI CODEC] EOS = true");
                //break;
            }
        }

        //release
        LOGD("[JNI CODEC] release");
        mcwMedia->mediacodec.stop(codec);
        mcwMedia->mediaExtractor.release(extractor);

        return nullptr;
    }

    string getFormatParameters(const char* name){
        uint8_t data[512];
        size_t data_size;
        mcwMedia->mediaformat.get_buffer(mcwFormat, name, (void **)(&data), &data_size);
        string data_content = name;
        data_content += " size: " + to_string(data_size) + "\n";
        data_content += "content:\n";
        if(data == nullptr){
            return data_content;
        }
        int count = 0;
        for (int i = 0; i < data_size; ++i) {
            char dataX[16];
            sprintf(dataX,"%x",data[i]);
            string dataStringX = dataX;
            data_content += dataStringX + " ";
            count++;
            if(count == 20){
                data_content += "\n";
                count = 0;
            }
        }
        return data_content;
    }

    JNIEXPORT void JNICALL
    Java_com_example_mediacodecjava_MainActivity_playVideoThread(JNIEnv *env, jobject thiz,
                                                                 jobject surface, jstring path) {
        // TODO: implement playVideoThread()

        //init
        mcw_new(mJavaVM, MCW_IMPLEMENTATION_JNI, &mcwMedia);
        extractor = mcwMedia->mediaExtractor.nnew();
        if (extractor == nullptr) {
            LOGE("[JNI CODEC] create extractor failed");
        }

        //setDataSource
        const char *dataPath = env->GetStringUTFChars(path, 0);
        LOGD("[JNI CODEC] set_data_source...");
        mcwMedia->mediaExtractor.set_data_source(extractor, dataPath);
        env->ReleaseStringUTFChars(path, dataPath);

        //init codec from track format
        int trackCount = mcwMedia->mediaExtractor.get_track_count(extractor);
        LOGD("[JNI CODEC] trackCount: %d", trackCount);
        for (int i = 0; i < trackCount; ++i) {
            mcwFormat = mcwMedia->mediaExtractor.get_track_format(extractor, i);
            const char *mime;
            mcwMedia->mediaformat.get_string(mcwFormat, mcwMedia->mediaformat.KEY_MIME, &mime);
            LOGD("[JNI CODEC] mediaFormat-mime: %s", mime);
            LOGD("[JNI CODEC] mediaFormat: %s", mcwMedia->mediaformat.to_string(mcwFormat));

            string parameter = getFormatParameters("csd-0");
            LOGD("[JNI CODEC] csd0: %s",parameter.c_str());
            //parameter = getFormatParameters("csd-1");
            //LOGD("[JNI CODEC] csd1: %s",parameter.c_str());

            //TEST
            mcwMedia->mediaformat.create_video_format(mime,100,200);

            if (!strncmp(mime, "video/", 6)) {
                mcwMedia->mediaExtractor.select_track(extractor, i);
                codec = mcwMedia->mediacodec.create_decoder_by_type(mime);
                if (codec == nullptr) {
                    LOGE("[JNI CODEC] create codec failed");
                    return;
                }
                break;
            }
        }

        //configure and start the codec
        if (codec == nullptr) {
            LOGE("[JNI CODEC] codec has not been create");
            return;
        }
        mcwMedia->mediacodec.configure(codec, mcwFormat, surface, nullptr, 0);
        mcwMedia->mediacodec.start(codec);

        //create a thread to decode
        pthread_create(&decodeThread, nullptr, decodeTask, nullptr);
    }

    JNIEXPORT void JNICALL
    Java_com_example_mediacodecjava_MainActivity_stop(JNIEnv *env, jobject thiz) {
        // TODO: implement stop()
        running = false;
    }

}