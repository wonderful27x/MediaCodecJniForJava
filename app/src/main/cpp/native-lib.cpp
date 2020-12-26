
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
#include "DataCollector.h"


static JNIEnv *mJEnv = NULL;
static JavaVM *mJavaVM = NULL;

list<string> DataCollector:: markNameList;
DataCollector* dataCollector = new DataCollector("codec",true);
DataCollector* dataCollector2 = new DataCollector("codec",true);

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

        struct mcw *mcwMediaJni = nullptr;
        struct mcw_mediacodec *codecJni = nullptr;
        struct mcw_mediaformat *mcwFormatJni = nullptr;
        struct mcw_mediaExtractor *extractor = nullptr;
        bool EOS = false;
        int64_t timeDif = -1;

        //init
        mcw_new(mJavaVM, MCW_IMPLEMENTATION_JNI, &mcwMediaJni);
        extractor = mcwMediaJni->mediaExtractor.nnew();

        //setDataSource
        const char *dataPath = env->GetStringUTFChars(path, 0);
        mcwMediaJni->mediaExtractor.set_data_source(extractor, dataPath);
        env->ReleaseStringUTFChars(path, dataPath);

        //init codec from track format
        int trackCount = mcwMediaJni->mediaExtractor.get_track_count(extractor);
        for (int i = 0; i < trackCount; ++i) {
            mcwFormatJni = mcwMediaJni->mediaExtractor.get_track_format(extractor, i);
            const char *mime;
            mcwMediaJni->mediaformat.get_string(mcwFormatJni, mcwMediaJni->mediaformat.KEY_MIME, &mime);
            if (!strncmp(mime, "video/", 6)) {
                mcwMediaJni->mediaExtractor.select_track(extractor, i);
                codecJni = mcwMediaJni->mediacodec.create_decoder_by_type(mime);
                if (codecJni == nullptr) {
                    return;
                }
                break;
            }
        }

        //configure and start the codec
        if (codecJni == nullptr) {
            return;
        }
        mcwMediaJni->mediacodec.configure(codecJni, mcwFormatJni, surface, nullptr, 0);
        mcwMediaJni->mediacodec.start(codecJni);

        while (running && !EOS) {

            dataCollector->startTrance();
            const char* info = dataCollector->takeSample(1000,100);
            if(info != nullptr){
                LOGD("DECODE INFO: %s",info);
            }

            //read the data and queue
            ssize_t index = mcwMediaJni->mediacodec.dequeue_input_buffer(codecJni, 10000);
            if (index >= 0) {
                size_t size;
                uint8_t *buff = mcwMediaJni->mediacodec.get_input_buffer(codecJni, index, &size);
                dataCollector->captureInputBuffTimes();
                if (buff != nullptr) {
                    dataCollector->captureInputBuffNum();
                    size_t sampleSize = mcwMediaJni->mediaExtractor.read_sample_data(extractor, buff,size);
                    if (sampleSize > 0) {
                        long time = mcwMediaJni->mediaExtractor.get_sample_time(extractor);
                        mcwMediaJni->mediacodec.queue_input_buffer(codecJni, index, 0, sampleSize, time,0);
                        mcwMediaJni->mediaExtractor.advance(extractor);
                        dataCollector->captureQueueInputBuffTimes();
                        dataCollector->captureQueueInputBuffNum();
                    } else {
                        EOS = true;
                        mcwMediaJni->mediacodec.queue_input_buffer(codecJni, index, 0, 0, 0,
                                                                   MCW_BUFFER_FLAG_END_OF_STREAM);
                    }
                }
            }

            //dequeue to get the decoded data and render
            struct mcw_mediacodec_bufferinfo bufferinfo;
            int outIndex = mcwMediaJni->mediacodec.dequeue_output_buffer(codecJni, &bufferinfo, 10000);
            dataCollector->captureDequeueOutputBuffTimes();
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
                    mcwMediaJni->mediacodec.release_output_buffer(codecJni, outIndex, outIndex > 0);
                    if(outIndex > 0){
                        dataCollector->captureDequeueOutputBuffNum();
                    }
                    break;
            }

            if ((bufferinfo.flags & MCW_BUFFER_FLAG_END_OF_STREAM) != 0) {
                break;
            }
        }

        //release
        mcwMediaJni->mediacodec.stop(codecJni);
        mcwMediaJni->mediaExtractor.release(extractor);

    }


    struct mcw *mcwMediaJni = nullptr;
    struct mcw_mediacodec *codecJni = nullptr;
    struct mcw_mediaformat *mcwFormatJni = nullptr;
    struct mcw_mediaExtractor *extractor = nullptr;
    bool EOS = false;
    int64_t timeDif = -1;

    pthread_t decodeThread;

    void *decodeTask(void *) {
        while (running && !EOS) {

            dataCollector2->startTrance();
            const char* info = dataCollector2->takeSample(1000,100);
            if(info != nullptr){
                LOGD("DECODE INFO: %s",info);
            }

            //read the data and queue
            ssize_t index = mcwMediaJni->mediacodec.dequeue_input_buffer(codecJni, 10000);
            if (index >= 0) {
                size_t size;
                uint8_t *buff = mcwMediaJni->mediacodec.get_input_buffer(codecJni, index, &size);
                dataCollector2->captureInputBuffTimes();
                if (buff != nullptr) {
                    dataCollector2->captureInputBuffNum();
                    LOGD("[JNI CODEC] read sample ...");
                    size_t sampleSize = mcwMediaJni->mediaExtractor.read_sample_data(extractor, buff,
                                                                                     size);
                    if (sampleSize > 0) {
                        LOGD("[JNI CODEC] read sample ok: %d", sampleSize);
                        long time = mcwMediaJni->mediaExtractor.get_sample_time(extractor);
                        mcwMediaJni->mediacodec.queue_input_buffer(codecJni, index, 0, sampleSize, time,0);
                        mcwMediaJni->mediaExtractor.advance(extractor);
                        dataCollector2->captureQueueInputBuffTimes();
                        dataCollector2->captureQueueInputBuffNum();
                    } else {
                        EOS = true;
                        mcwMediaJni->mediacodec.queue_input_buffer(codecJni, index, 0, 0, 0,
                                                                   MCW_BUFFER_FLAG_END_OF_STREAM);
                    }
                } else {
                    LOGD("[JNI CODEC] get_input_buffer is null !!!");
                }
            }

            //dequeue to get the decoded data and render
            struct mcw_mediacodec_bufferinfo bufferinfo;
            int outIndex = mcwMediaJni->mediacodec.dequeue_output_buffer(codecJni, &bufferinfo, 10000);
            dataCollector2->captureDequeueOutputBuffTimes();

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
                    mcwMediaJni->mediacodec.release_output_buffer(codecJni, outIndex, outIndex > 0);
                    if(outIndex > 0){
                        dataCollector2->captureDequeueOutputBuffNum();
                    }

                    break;
            }

            if ((bufferinfo.flags & MCW_BUFFER_FLAG_END_OF_STREAM) != 0) {
                break;
            }
        }

        //release
        LOGD("[JNI CODEC] release");
        mcwMediaJni->mediacodec.stop(codecJni);
        mcwMediaJni->mediaExtractor.release(extractor);

        return nullptr;
    }

    JNIEXPORT void JNICALL
    Java_com_example_mediacodecjava_MainActivity_playVideoThread(JNIEnv *env, jobject thiz,
                                                                 jobject surface, jstring path) {
        // TODO: implement playVideoThread()

        //init
        mcw_new(mJavaVM, MCW_IMPLEMENTATION_JNI, &mcwMediaJni);
        extractor = mcwMediaJni->mediaExtractor.nnew();
        if (extractor == nullptr) {
            LOGE("[JNI CODEC] create extractor failed");
        }

        //setDataSource
        const char *dataPath = env->GetStringUTFChars(path, 0);
        LOGD("[JNI CODEC] set_data_source...");
        mcwMediaJni->mediaExtractor.set_data_source(extractor, dataPath);
        env->ReleaseStringUTFChars(path, dataPath);

        //init codec from track format
        int trackCount = mcwMediaJni->mediaExtractor.get_track_count(extractor);
        LOGD("[JNI CODEC] trackCount: %d", trackCount);
        for (int i = 0; i < trackCount; ++i) {
            mcwFormatJni = mcwMediaJni->mediaExtractor.get_track_format(extractor, i);
            const char *mime;
            mcwMediaJni->mediaformat.get_string(mcwFormatJni, mcwMediaJni->mediaformat.KEY_MIME, &mime);
            LOGD("[JNI CODEC] mediaFormat: %s", mime);
            if (!strncmp(mime, "video/", 6)) {
                mcwMediaJni->mediaExtractor.select_track(extractor, i);
                codecJni = mcwMediaJni->mediacodec.create_decoder_by_type(mime);
                if (codecJni == nullptr) {
                    LOGE("[JNI CODEC] create codec failed");
                    return;
                }
                break;
            }
        }

        //configure and start the codec
        if (codecJni == nullptr) {
            LOGE("[JNI CODEC] codec has not been create");
            return;
        }
        mcwMediaJni->mediacodec.configure(codecJni, mcwFormatJni, surface, nullptr, 0);
        mcwMediaJni->mediacodec.start(codecJni);

        //create a thread to decode
        pthread_create(&decodeThread, nullptr, decodeTask, nullptr);
    }

    JNIEXPORT void JNICALL
    Java_com_example_mediacodecjava_MainActivity_stop(JNIEnv *env, jobject thiz) {
        // TODO: implement stop()
        running = false;
    }

}