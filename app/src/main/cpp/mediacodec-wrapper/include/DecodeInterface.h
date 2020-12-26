//
// Created by 123 on 2020/12/2.
//
#ifndef MEDIACODECJAVA_DECODEINTERFACE_H
#define MEDIACODECJAVA_DECODEINTERFACE_H

#include <stdint.h>
#include <jni.h>

#include "androidlog.h"
#include "libmcw.h"
#include "mcw_jni.h"

/**
 * @author wonderful
 * @date 2020-12-3
 * @version 1.0
 * @descreption mediaCodec-format抽象接口
 */
class CyberFormatI{
public:

    CyberFormatI(){}

    ~CyberFormatI(){}

    virtual void setBuffer(const char *name,void *data,size_t size) = 0;
    virtual void setString(const char *name,const char *value) = 0;
    virtual void setInt32(const char *name,int32_t value) = 0;
    virtual const char * toString() = 0;
    virtual bool getString(const char *name,const char **out) = 0;
    virtual enum mcw_media_status deleteFormat() = 0;

};

/**
 * @author wonderful
 * @date 2020-12-3
 * @version 1.0
 * @descreption mediaCodec抽象接口，主要为扩展而设计，允许他又不同的实现
 */
class CyberDecoderI{
public:

    CyberDecoderI(){};
    ~CyberDecoderI(){};

    virtual void init(JavaVM *javaVm) = 0;
    virtual CyberFormatI* createFormat(void *format = nullptr) = 0;
    virtual struct mcw_mediacodec_bufferinfo* createBufferInfo() = 0;
    virtual void createDecoderByType(const char* mimeType) = 0;
    virtual enum mcw_media_status config(const CyberFormatI* format,void* surface,struct mcw_mediacrypto *crypto,uint32_t flags) = 0;
    virtual enum mcw_media_status startCodec() = 0;
    virtual void* getInputBuffers() = 0;
    virtual void* getOutputBuffers() = 0;
    virtual uint8_t * getInputBuffer(size_t idx,size_t *out_size) = 0;
    virtual uint8_t* getOutputBuffer(size_t idx,size_t *out_size) = 0;
    virtual int dequeueInputBuffer(long timeoutUs) = 0;
    virtual enum mcw_media_status queueInputBuffer(size_t index,off_t offset,size_t size,uint64_t presentation_time_us,uint32_t flags) = 0;
    virtual int dequeueOutputBuffer(struct mcw_mediacodec_bufferinfo *info,int64_t timeout_us) = 0;
    virtual enum mcw_media_status releaseOutputBuffer(int index, bool render) = 0;
    virtual enum mcw_media_status flushCodec() = 0;
    virtual enum mcw_media_status stopCodec() = 0;
    virtual enum mcw_media_status deleteCodec() = 0;
};


/**
 * @author wonderful
 * @date 2020-12-3
 * @version 1.0
 * @descreption mediaCodec-format抽象接口JNI实现
 */
class CyberFormatJni : public CyberFormatI{
public:

    struct mcw *mcwMedia;
    struct mcw_mediaformat *mcw_format;

    CyberFormatJni();

    ~CyberFormatJni();

    void setBuffer(const char *name,void *data,size_t size) override ;
    void setString(const char *name,const char *value) override ;
    void setInt32(const char *name,int32_t value) override ;
    const char *  toString() override ;
    bool getString(const char *name,const char **out) override ;
    enum mcw_media_status deleteFormat() override ;
};



/**
 * @author wonderful
 * @date 2020-12-3
 * @version 1.0
 * @descreption mediaCodec抽象接口JNI实现，主要兼容api 19以下，他是对MCW第三方库的封装，、
 * https://github.com/Parrot-Developers/libmediacodec-wrapper
 * 这个库实现了从jni到java层的mediaCodec调用，从而实现api19的兼容，同时还提供了NDK和JNI的切换，
 * 此类仅仅实现对jni的调用封装，如果我们不想使用第三方库，我们仍然可以通过继承CyberDecoderI接口自己实现
 */
class CyberDecoderJni : public CyberDecoderI{
public:

    struct mcw *mcwMedia;
    struct mcw_mediacodec *mcwCodec;

    CyberDecoderJni();

    ~CyberDecoderJni();

    void init(JavaVM *javaVm) override ;

    CyberFormatI* createFormat(void *format = nullptr) override ;

    struct mcw_mediacodec_bufferinfo* createBufferInfo() override ;

    void createDecoderByType(const char* mimeType) override  ;

    enum mcw_media_status config(const CyberFormatI* format,void* surface,struct mcw_mediacrypto *crypto,uint32_t flags) override ;

    enum mcw_media_status startCodec() override ;

    void* getInputBuffers() override ;

    void* getOutputBuffers() override ;

    uint8_t * getInputBuffer(size_t idx,size_t *out_size) override ;

    uint8_t* getOutputBuffer(size_t idx,size_t *out_size) override ;

    int dequeueInputBuffer(long timeoutUs) override ;

    enum mcw_media_status queueInputBuffer(size_t index,off_t offset,size_t size,uint64_t presentation_time_us,uint32_t flags) override ;

    int dequeueOutputBuffer(struct mcw_mediacodec_bufferinfo *info,int64_t timeout_us) override ;

    enum mcw_media_status releaseOutputBuffer(int index, bool render) override ;

    enum mcw_media_status flushCodec() override ;

    enum mcw_media_status stopCodec() override ;

    enum mcw_media_status deleteCodec() override ;
};

#endif //MEDIACODECJAVA_DECODEINTERFACE_H




//#ifndef MEDIACODECJAVA_DECODEINTERFACE_H
//#define MEDIACODECJAVA_DECODEINTERFACE_H
//
//#include <stdint.h>
//#include <jni.h>
//#include "androidlog.h"
//
//extern "C"{
//#include "libmcw.h"
//#include "mcw_jni.h"
//}
//
//class CyberFormatI{
//public:
//
//    CyberFormatI(){}
//
//    ~CyberFormatI(){}
//
//    virtual void setBuffer(const char *name,void *data,size_t size) = 0;
//    virtual void setString(const char *name,const char *value) = 0;
//    virtual void setInt32(const char *name,int32_t value) = 0;
//    virtual const char * toString() = 0;
//    virtual bool getString(const char *name,const char **out) = 0;
//
//};
//
//class CyberDecoderI{
//public:
//
//    CyberDecoderI(){};
//    ~CyberDecoderI(){};
//
//    virtual void init(JavaVM *javaVm,enum mcw_implementation type) = 0;
//    virtual CyberFormatI* createFormat(void *) = 0;
//    virtual struct mcw_mediacodec_bufferinfo* createBufferInfo() = 0;
//    virtual void createDecoderByType(const char* mimeType) = 0;
//    virtual void config(const CyberFormatI* format,void* surface,struct mcw_mediacrypto *crypto,uint32_t flags) = 0;
//    virtual void startCodec() = 0;
//    virtual void* getInputBuffers() = 0;
//    virtual void* getOutputBuffers() = 0;
//    virtual int dequeueInputBuffer(long timeoutUs) = 0;
//    virtual void queueInputBuffer(size_t index,off_t offset,size_t size,uint64_t presentation_time_us,uint32_t flags) = 0;
//    virtual int dequeueOutputBuffer(struct mcw_mediacodec_bufferinfo *info,int64_t timeout_us) = 0;
//    virtual int releaseOutputBuffer(int index, bool render) = 0;
//    virtual void flushCodec() = 0;
//    virtual void stopCodec() = 0;
//    virtual void deleteCodec() = 0;
//};
//
//
//class CyberFormat : public CyberFormatI{
//public:
//
//    struct mcw *mcwMedia;
//    struct mcw_mediaformat *mcw_format;
//
//    CyberFormat();
//
//    ~CyberFormat();
//
//    void setBuffer(const char *name,void *data,size_t size) override ;
//    void setString(const char *name,const char *value) override ;
//    void setInt32(const char *name,int32_t value) override ;
//    const char *  toString() override ;
//    bool getString(const char *name,const char **out) override ;
//};
//
//
//class CyberDecoder : public CyberDecoderI{
//public:
//
//    struct mcw *mcwMedia;
//    struct mcw_mediacodec *mcwCodec;
//
//    CyberDecoder();
//
//    ~CyberDecoder();
//
//    void init(JavaVM *javaVm,enum mcw_implementation type) override ;
//
//    CyberFormatI* createFormat(void *) override ;
//
//    struct mcw_mediacodec_bufferinfo* createBufferInfo() override ;
//
//    void createDecoderByType(const char* mimeType) override  ;
//
//    void config(const CyberFormatI* format,void* surface,struct mcw_mediacrypto *crypto,uint32_t flags) override ;
//
//    void startCodec() override ;
//
//    void* getInputBuffers() override ;
//
//    void* getOutputBuffers() override ;
//
//    int dequeueInputBuffer(long timeoutUs) override ;
//
//    void queueInputBuffer(size_t index,off_t offset,size_t size,uint64_t presentation_time_us,uint32_t flags) override ;
//
//    int dequeueOutputBuffer(struct mcw_mediacodec_bufferinfo *info,int64_t timeout_us) override ;
//
//    int releaseOutputBuffer(int index, bool render) override ;
//
//    void flushCodec() override ;
//
//    void stopCodec() override ;
//
//    void deleteCodec() override ;
//};
//
//#endif //MEDIACODECJAVA_DECODEINTERFACE_H
