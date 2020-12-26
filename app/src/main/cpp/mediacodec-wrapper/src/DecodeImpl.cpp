//
// Created by 123 on 2020/12/2.
//
#include "DecodeInterface.h"

CyberFormatJni::CyberFormatJni() {}

CyberFormatJni::~CyberFormatJni() {
    if (mcwMedia) {
        CyberFormatJni::deleteFormat();
        mcwMedia = nullptr;
    }
}

void CyberFormatJni::setBuffer(const char *name, void *data, size_t size) {
    mcwMedia->mediaformat.set_buffer(mcw_format, name, data, size);
}

void CyberFormatJni::setString(const char *name, const char *value) {
    mcwMedia->mediaformat.set_string(mcw_format, name, value);
}

void CyberFormatJni::setInt32(const char *name, int32_t value) {
    mcwMedia->mediaformat.set_int32(mcw_format, name, value);
}

const char *CyberFormatJni::toString() {
    return mcwMedia->mediaformat.to_string(mcw_format);
}

bool CyberFormatJni::getString(const char *name, const char **out) {
    return mcwMedia->mediaformat.get_string(mcw_format, name, out);
}

enum mcw_media_status CyberFormatJni::deleteFormat() {
    return mcwMedia->mediaformat.ddelete(mcw_format);
}



CyberDecoderJni::CyberDecoderJni() {}

CyberDecoderJni::~CyberDecoderJni() {
    if (mcwMedia) {
        CyberDecoderJni::deleteCodec();
        delete mcwMedia;
    }
}

void CyberDecoderJni::init(JavaVM *javaVm) {
    mcw_new(javaVm, MCW_IMPLEMENTATION_JNI, &mcwMedia);
}

/**
 * 创建format，CyberFormatI的创建应该由CyberDecoderJni创建而不应自己nwe，因为这样容易遗漏一些mcw的环境
 * @param object 传递的java层format的object
 * @return
 */
CyberFormatI *CyberDecoderJni::createFormat(void *object) {
    struct mcw_mediaformat *mediaformat = mcwMedia->mediaformat.nnew();
    CyberFormatJni *cyberFormat = new CyberFormatJni();
    cyberFormat->mcw_format = mediaformat;
    cyberFormat->mcwMedia = mcwMedia;
    if (object){
        struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)mediaformat;
        fmt->mediaformat = static_cast<jobject>(object);
    }
    return cyberFormat;
}

struct mcw_mediacodec_bufferinfo *CyberDecoderJni::createBufferInfo() {
    return new mcw_mediacodec_bufferinfo;
}

void CyberDecoderJni::createDecoderByType(const char *mimeType) {
    LOGI("[CODEC] createDecoderByType");
    LOGI("[CODEC] mcwMedia: %p",mcwMedia);
    LOGI("[CODEC] createFunc: %p",mcwMedia->mediacodec.create_decoder_by_type);
    mcwCodec = mcwMedia->mediacodec.create_decoder_by_type(mimeType);
}

enum mcw_media_status CyberDecoderJni::config(const CyberFormatI *format, void *surface, struct mcw_mediacrypto *crypto,uint32_t flags) {
    return mcwMedia->mediacodec.configure(mcwCodec, ((CyberFormatJni *) format)->mcw_format, (jobject)surface, crypto,flags);
}

enum mcw_media_status CyberDecoderJni::startCodec() {
    LOGI("[CODEC] startCodec: %p",mcwCodec);
    return mcwMedia->mediacodec.start(mcwCodec);
}

void* CyberDecoderJni::getInputBuffers() {
    struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *) mcwCodec;
    return mc->in_buffers;
}

void* CyberDecoderJni::getOutputBuffers() {
    struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *) mcwCodec;
    return mc->out_buffers;
}

int CyberDecoderJni::dequeueInputBuffer(long timeoutUs) {
    return mcwMedia->mediacodec.dequeue_input_buffer(mcwCodec, timeoutUs);
}

enum mcw_media_status CyberDecoderJni::queueInputBuffer(size_t index, off_t offset, size_t size,
                                    uint64_t presentation_time_us, uint32_t flags) {
    return mcwMedia->mediacodec.queue_input_buffer(mcwCodec, index, offset, size, presentation_time_us,
                                            flags);
}

int CyberDecoderJni::dequeueOutputBuffer(struct mcw_mediacodec_bufferinfo *info, int64_t timeout_us) {
    return mcwMedia->mediacodec.dequeue_output_buffer(mcwCodec, info, timeout_us);
}

enum mcw_media_status CyberDecoderJni::releaseOutputBuffer(int index, bool render) {
    return mcwMedia->mediacodec.release_output_buffer(mcwCodec, index, render);
}

enum mcw_media_status CyberDecoderJni::flushCodec() {
    return mcwMedia->mediacodec.flush(mcwCodec);
}

enum mcw_media_status CyberDecoderJni::stopCodec() {
    return mcwMedia->mediacodec.stop(mcwCodec);
}

enum mcw_media_status CyberDecoderJni::deleteCodec() {
    return mcwMedia->mediacodec.ddelete(mcwCodec);
}

uint8_t *CyberDecoderJni::getInputBuffer(size_t idx, size_t *out_size) {
    return mcwMedia->mediacodec.get_input_buffer(mcwCodec,idx,out_size);
}

uint8_t *CyberDecoderJni::getOutputBuffer(size_t idx, size_t *out_size) {
    return mcwMedia->mediacodec.get_output_buffer(mcwCodec,idx,out_size);
}





//#include "DecodeInterface.h"
//
//CyberFormat::CyberFormat() {}
//
//CyberFormat::~CyberFormat() {
//    if (mcwMedia) {
//        mcwMedia->mediaformat.ddelete(mcw_format);
//    }
//}
//
//void CyberFormat::setBuffer(const char *name, void *data, size_t size) {
//    mcwMedia->mediaformat.set_buffer(mcw_format, name, data, size);
//}
//
//void CyberFormat::setString(const char *name, const char *value) {
//    mcwMedia->mediaformat.set_string(mcw_format, name, value);
//}
//
//void CyberFormat::setInt32(const char *name, int32_t value) {
//    mcwMedia->mediaformat.set_int32(mcw_format, name, value);
//}
//
//const char *CyberFormat::toString() {
//    return mcwMedia->mediaformat.to_string(mcw_format);
//}
//
//bool CyberFormat::getString(const char *name, const char **out) {
//    return mcwMedia->mediaformat.get_string(mcw_format, name, out);
//}
//
//CyberDecoder::CyberDecoder() {}
//
//CyberDecoder::~CyberDecoder() {
//    if (mcwMedia) {
//        mcwMedia->mediacodec.ddelete(mcwCodec);
//        delete mcwMedia;
//    }
//}
//
//void CyberDecoder::init(JavaVM *javaVm, enum mcw_implementation type) {
//    mcw_new(javaVm, type, &mcwMedia);
//}
//
//CyberFormatI *CyberDecoder::createFormat(void *object) {
//    struct mcw_mediaformat *mediaformat = mcwMedia->mediaformat.nnew();
//    CyberFormat cyberFormat = CyberFormat();
//    cyberFormat.mcw_format = mediaformat;
//    cyberFormat.mcwMedia = mcwMedia;
//    if (object){
//        struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)mediaformat;
//        fmt->mediaformat = static_cast<jobject>(object);
//    }
//    return &cyberFormat;
//}
//
//struct mcw_mediacodec_bufferinfo *CyberDecoder::createBufferInfo() {
//    return new mcw_mediacodec_bufferinfo;
//}
//
//void CyberDecoder::createDecoderByType(const char *mimeType) {
//    mcwCodec = mcwMedia->mediacodec.create_decoder_by_type(mimeType);
//}
//
//void CyberDecoder::config(const CyberFormatI *format, void *surface, struct mcw_mediacrypto *crypto,
//                          uint32_t flags) {
//    mcwMedia->mediacodec.configure(mcwCodec, ((CyberFormat *) format)->mcw_format, surface, crypto,
//                                   flags);
//}
//
//void CyberDecoder::startCodec() {
//    mcwMedia->mediacodec.start(mcwCodec);
//}
//
//void *CyberDecoder::getInputBuffers() {
//    struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *) mcwCodec;
//    return mc->in_buffers;
//}
//
//void *CyberDecoder::getOutputBuffers() {
//    struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *) mcwCodec;
//    return mc->out_buffers;
//}
//
//int CyberDecoder::dequeueInputBuffer(long timeoutUs) {
//    return mcwMedia->mediacodec.dequeue_input_buffer(mcwCodec, timeoutUs);
//}
//
//void CyberDecoder::queueInputBuffer(size_t index, off_t offset, size_t size,
//                                    uint64_t presentation_time_us, uint32_t flags) {
//    mcwMedia->mediacodec.queue_input_buffer(mcwCodec, index, offset, size, presentation_time_us,
//                                            flags);
//}
//
//int CyberDecoder::dequeueOutputBuffer(struct mcw_mediacodec_bufferinfo *info, int64_t timeout_us) {
//    return mcwMedia->mediacodec.dequeue_output_buffer(mcwCodec, info, timeout_us);
//}
//
//int CyberDecoder::releaseOutputBuffer(int index, bool render) {
//    return mcwMedia->mediacodec.release_output_buffer(mcwCodec, index, render);
//}
//
//void CyberDecoder::flushCodec() {
//    mcwMedia->mediacodec.flush(mcwCodec);
//}
//
//void CyberDecoder::stopCodec() {
//    mcwMedia->mediacodec.stop(mcwCodec);
//}
//
//void CyberDecoder::deleteCodec() {
//    mcwMedia->mediacodec.ddelete(mcwCodec);
//}
