/**
 * Copyright (c) 2018 Parrot Drones SAS
 * Copyright (c) 2017 Aurelien Barre
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the copyright holders nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mcw_jni.h"
#include <jni.h>
#include <malloc.h>


/* clang-format off */
static const char *mcw_jni_mediaformat_KEY_AAC_PROFILE =
	"aac-profile";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_BIT_RATE =
	"bitrate";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_CHANNEL_COUNT =
	"channel-count";			/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_CHANNEL_MASK =
	"channel-mask";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_COLOR_FORMAT =
	"color-format";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_DURATION =
	"durationUs";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_FLAC_COMPRESSION_LEVEL =
	"flac-compression-level";		/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_FRAME_RATE =
	"frame-rate";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_HEIGHT =
	"height";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_IS_ADTS =
	"is-adts";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_IS_AUTOSELECT =
	"is-autoselect";			/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_IS_DEFAULT =
	"is-default";				/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_IS_FORCED_SUBTITLE =
	"is-forced-subtitle";			/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_I_FRAME_INTERVAL =
	"i-frame-interval";			/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_LANGUAGE =
	"language";				/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_MAX_HEIGHT =
	"max-height";				/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_MAX_INPUT_SIZE =
	"max-input-size";			/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_MAX_WIDTH =
	"max-width";				/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_MIME =
	"mime";					/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_PUSH_BLANK_BUFFERS_ON_STOP =
	"push-blank-buffers-on-shutdown";	/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_REPEAT_PREVIOUS_FRAME_AFTER =
	"repeat-previous-frame-after";		/* API level 19 */
static const char *mcw_jni_mediaformat_KEY_SAMPLE_RATE =
	"sample-rate";				/* API level 16 */
static const char *mcw_jni_mediaformat_KEY_STRIDE =
	"stride";				/* API level 23 */
static const char *mcw_jni_mediaformat_KEY_WIDTH =
	"width";				/* API level 16 */


static struct mcw_jni {
	JavaVM *jvm;
	struct {
		jclass clazz;
		jmethodID nnew;				/* API level 16 */
		jfieldID flags;				/* API level 16 */
		jfieldID offset;			/* API level 16 */
		jfieldID presentationTimeUs;		/* API level 16 */
		jfieldID size;				/* API level 16 */
	} BufferInfo;
	struct {
		jclass clazz;
		jmethodID nnew;				/* API level 16 */
		jmethodID toString;			/* API level 16 */
		jmethodID getInteger;			/* API level 16 */
		jmethodID getLong;			/* API level 16 */
		jmethodID getFloat;			/* API level 16 */
		jmethodID getByteBuffer;		/* API level 16 */
		jmethodID getString;			/* API level 16 */
		jmethodID setInteger;			/* API level 16 */
		jmethodID setLong;			/* API level 16 */
		jmethodID setFloat;			/* API level 16 */
		jmethodID setByteBuffer;		/* API level 16 */
		jmethodID setString;			/* API level 16 */
	} MediaFormat;
	struct {
		jclass clazz;
		jmethodID createByCodecName;		/* API level 16 */
		jmethodID createDecoderByType;		/* API level 16 */
		jmethodID createEncoderByType;		/* API level 16 */
		jmethodID release;			/* API level 16 */
		jmethodID configure;			/* API level 16 */
		jmethodID start;			/* API level 16 */
		jmethodID stop;				/* API level 16 */
		jmethodID flush;			/* API level 16 */
		jmethodID getInputBuffer;		/* API level 21 */
		jmethodID getInputBuffers;		/* API level 16 */
		jmethodID getOutputBuffer;		/* API level 21 */
		jmethodID getOutputBuffers;		/* API level 16 */
		jmethodID dequeueInputBuffer;		/* API level 16 */
		jmethodID queueInputBuffer;		/* API level 16 */
		jmethodID dequeueOutputBuffer;		/* API level 16 */
		jmethodID getOutputFormat;		/* API level 16 */
		jmethodID releaseOutputBuffer;		/* API level 16 */
		jmethodID releaseOutputBufferAtTime;	/* API level 21 */
	} MediaCodec;

	//将MediaExtractor也转成jni
	struct {
        jclass clazz;
        jmethodID nnew;   /* API level ? */
        jmethodID setDataSource;   /* API level ? */
        jmethodID getTrackCount;   /* API level ? */
        jmethodID getTrackFormat;  /* API level ? */
        jmethodID selectTrack;     /* API level ? */
        jmethodID readSampleData;  /* API level ? */
        jmethodID getSampleTime;   /* API level ? */
        jmethodID advance;         /* API level ? */
        jmethodID release;         /* API level ? */
	}MediaExtractor;
} mcw_jni;
/* clang-format on */


static const struct {
	const char *name;
	int offset;
} mcw_jni_class[] = {
	{"android/media/MediaCodec$BufferInfo",
	 offsetof(struct mcw_jni, BufferInfo.clazz)},
	{"android/media/MediaFormat",
	 offsetof(struct mcw_jni, MediaFormat.clazz)},
	{"android/media/MediaCodec",
	 offsetof(struct mcw_jni, MediaCodec.clazz)},
	{"android/media/MediaExtractor",
	offsetof(struct mcw_jni, MediaExtractor.clazz)},
};


enum mcw_jni_member_type {
	MCW_JNI_MEMBER_TYPE_METHOD = 0,
	MCW_JNI_MEMBER_TYPE_STATIC_METHOD,
	MCW_JNI_MEMBER_TYPE_FIELD,
};

static const struct {
	enum mcw_jni_member_type type;
	const char *name;
	const char *signature;
	int class_offset;
	int offset;
	int critical;
} mcw_jni_member[] = {
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "<init>",
	 "()V",
	 offsetof(struct mcw_jni, BufferInfo.clazz),
	 offsetof(struct mcw_jni, BufferInfo.nnew),
	 1},
	{MCW_JNI_MEMBER_TYPE_FIELD,
	 "flags",
	 "I",
	 offsetof(struct mcw_jni, BufferInfo.clazz),
	 offsetof(struct mcw_jni, BufferInfo.flags),
	 1},
	{MCW_JNI_MEMBER_TYPE_FIELD,
	 "offset",
	 "I",
	 offsetof(struct mcw_jni, BufferInfo.clazz),
	 offsetof(struct mcw_jni, BufferInfo.offset),
	 1},
	{MCW_JNI_MEMBER_TYPE_FIELD,
	 "presentationTimeUs",
	 "J",
	 offsetof(struct mcw_jni, BufferInfo.clazz),
	 offsetof(struct mcw_jni, BufferInfo.presentationTimeUs),
	 1},
	{MCW_JNI_MEMBER_TYPE_FIELD,
	 "size",
	 "I",
	 offsetof(struct mcw_jni, BufferInfo.clazz),
	 offsetof(struct mcw_jni, BufferInfo.size),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "<init>",
	 "()V",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.nnew),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "toString",
	 "()Ljava/lang/String;",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.toString),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getInteger",
	 "(Ljava/lang/String;)I",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.getInteger),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getLong",
	 "(Ljava/lang/String;)J",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.getLong),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getFloat",
	 "(Ljava/lang/String;)F",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.getFloat),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getByteBuffer",
	 "(Ljava/lang/String;)Ljava/nio/ByteBuffer;",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.getByteBuffer),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getString",
	 "(Ljava/lang/String;)Ljava/lang/String;",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.getString),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "setInteger",
	 "(Ljava/lang/String;I)V",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.setInteger),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "setLong",
	 "(Ljava/lang/String;J)V",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.setLong),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "setFloat",
	 "(Ljava/lang/String;F)V",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.setFloat),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "setByteBuffer",
	 "(Ljava/lang/String;Ljava/nio/ByteBuffer;)V",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.setByteBuffer),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "setString",
	 "(Ljava/lang/String;Ljava/lang/String;)V",
	 offsetof(struct mcw_jni, MediaFormat.clazz),
	 offsetof(struct mcw_jni, MediaFormat.setString),
	 1},
	{MCW_JNI_MEMBER_TYPE_STATIC_METHOD,
	 "createByCodecName",
	 "(Ljava/lang/String;)Landroid/media/MediaCodec;",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.createByCodecName),
	 1},
	{MCW_JNI_MEMBER_TYPE_STATIC_METHOD,
	 "createDecoderByType",
	 "(Ljava/lang/String;)Landroid/media/MediaCodec;",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.createDecoderByType),
	 1},
	{MCW_JNI_MEMBER_TYPE_STATIC_METHOD,
	 "createEncoderByType",
	 "(Ljava/lang/String;)Landroid/media/MediaCodec;",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.createEncoderByType),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "release",
	 "()V",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.release),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "configure",
	 "(Landroid/media/MediaFormat;Landroid/view/Surface;"
	 "Landroid/media/MediaCrypto;I)V",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.configure),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "start",
	 "()V",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.start),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "stop",
	 "()V",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.stop),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "flush",
	 "()V",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.flush),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getInputBuffer",
	 "(I)Ljava/nio/ByteBuffer;",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.getInputBuffer),
	 0},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getInputBuffers",
	 "()[Ljava/nio/ByteBuffer;",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.getInputBuffers),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getOutputBuffer",
	 "(I)Ljava/nio/ByteBuffer;",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.getOutputBuffer),
	 0},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getOutputBuffers",
	 "()[Ljava/nio/ByteBuffer;",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.getOutputBuffers),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "dequeueInputBuffer",
	 "(J)I",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.dequeueInputBuffer),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "queueInputBuffer",
	 "(IIIJI)V",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.queueInputBuffer),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "dequeueOutputBuffer",
	 "(Landroid/media/MediaCodec$BufferInfo;J)I",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.dequeueOutputBuffer),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "getOutputFormat",
	 "()Landroid/media/MediaFormat;",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.getOutputFormat),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "releaseOutputBuffer",
	 "(IZ)V",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.releaseOutputBuffer),
	 1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
	 "releaseOutputBuffer",
	 "(IJ)V",
	 offsetof(struct mcw_jni, MediaCodec.clazz),
	 offsetof(struct mcw_jni, MediaCodec.releaseOutputBufferAtTime),
	 0},

	 //mediaExtractor method init
    {MCW_JNI_MEMBER_TYPE_METHOD,
            "<init>",
            "()V",
            offsetof(struct mcw_jni, MediaExtractor.clazz),
            offsetof(struct mcw_jni, MediaExtractor.nnew),
            1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
			"setDataSource",
			"(Ljava/lang/String;)V",
			offsetof(struct mcw_jni, MediaExtractor.clazz),
			offsetof(struct mcw_jni, MediaExtractor.setDataSource),
			1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
			"getTrackCount",
			"()I",
			offsetof(struct mcw_jni, MediaExtractor.clazz),
			offsetof(struct mcw_jni, MediaExtractor.getTrackCount),
			1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
			"getTrackFormat",
			"(I)Landroid/media/MediaFormat;",
			offsetof(struct mcw_jni, MediaExtractor.clazz),
			offsetof(struct mcw_jni, MediaExtractor.getTrackFormat),
			1},
    {MCW_JNI_MEMBER_TYPE_METHOD,
                "selectTrack",
                "(I)V",
                offsetof(struct mcw_jni, MediaExtractor.clazz),
                offsetof(struct mcw_jni, MediaExtractor.selectTrack),
                1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
			"readSampleData",
			"(Ljava/nio/ByteBuffer;I)I",
			offsetof(struct mcw_jni, MediaExtractor.clazz),
			offsetof(struct mcw_jni, MediaExtractor.readSampleData),
			1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
			"getSampleTime",
			"()J",
			offsetof(struct mcw_jni, MediaExtractor.clazz),
			offsetof(struct mcw_jni, MediaExtractor.getSampleTime),
			1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
			"advance",
			"()Z",
			offsetof(struct mcw_jni, MediaExtractor.clazz),
			offsetof(struct mcw_jni, MediaExtractor.advance),
			1},
	{MCW_JNI_MEMBER_TYPE_METHOD,
			"release",
			"()V",
			offsetof(struct mcw_jni, MediaExtractor.clazz),
			offsetof(struct mcw_jni, MediaExtractor.release),
			1},
};


int mcw_get_sdk_version(void *jvm)
{
	JNIEnv *env = NULL;
	JavaVM *_jvm = (JavaVM *)jvm;
	bool attached = false;
	jint res, status;
	jclass BuildVersion = NULL;
	jfieldID sdkInt;
	jint sdk_int = -EPROTO;
	
	status = _jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		res = _jvm->AttachCurrentThread(&env, NULL);
		if (res != JNI_OK) {
			LOGE("failed to attach thread to VM (%d)", res);
			return -EPROTO;
		}
		attached = true;
	} else if (status != JNI_OK) {
		LOGE("failed to get JNI environment (%d)", status);
		return -EPROTO;
	}

	BuildVersion = env->FindClass( "android/os/Build$VERSION");
	if (BuildVersion == NULL) {
		LOGE("could not find Build.VERSION class");
		goto exit;
	}
	sdkInt = env->GetStaticFieldID(BuildVersion, "SDK_INT", "I");
	if (sdkInt == NULL) {
		LOGE("could not find SDK_INT field");
		goto exit;
	}

	sdk_int = env->GetStaticIntField(BuildVersion, sdkInt);

exit:
	if (BuildVersion != NULL)
		env->DeleteLocalRef(BuildVersion);

	if (attached) {
		res = _jvm->DetachCurrentThread();
		if (res != JNI_OK)
			LOGE("failed to detach thread from VM (%d)", res);
	}

	return (int)sdk_int;
}


char *mcw_get_codec_name(void *jvm, const char *mime_type, int encoder)
{
	JNIEnv *env = NULL;
	JavaVM *_jvm = (JavaVM*)jvm;
	bool attached = false;
	int i, num_codecs;
	jint res, status, j_num_codecs;
	char *codec_name = NULL;
	jclass MediaCodecList = NULL;
	jclass MediaCodecInfo = NULL;
	jmethodID getCodecCount;
	jmethodID getCodecInfoAt;
	jmethodID isEncoder;
	jmethodID getSupportedTypes;
	jmethodID getName;

	status = _jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		res = _jvm->AttachCurrentThread(&env, NULL);
		if (res != JNI_OK) {
			LOGE("failed to attach thread to VM (%d)", res);
			return NULL;
		}
		attached = true;
	} else if (status != JNI_OK) {
		LOGE("failed to get JNI environment (%d)", status);
		return NULL;
	}

	MediaCodecList = env->FindClass("android/media/MediaCodecList");
	if (MediaCodecList == NULL) {
		LOGE("could not find MediaCodecList class");
		goto exit;
	}
	getCodecCount = env->GetStaticMethodID(
		 MediaCodecList, "getCodecCount", "()I");
	if (getCodecCount == NULL) {
		LOGE("could not find MediaCodecList.getCodecCount method");
		goto exit;
	}
	getCodecInfoAt =
		env->GetStaticMethodID(
					  MediaCodecList,
					  "getCodecInfoAt",
					  "(I)Landroid/media/MediaCodecInfo;");
	if (getCodecInfoAt == NULL) {
		LOGE("could not find MediaCodecList.getCodecInfoAt method");
		goto exit;
	}
	MediaCodecInfo = env->FindClass("android/media/MediaCodecInfo");
	if (MediaCodecInfo == NULL) {
		LOGE("could not find MediaCodecInfo class");
		goto exit;
	}
	isEncoder =
		env->GetMethodID(MediaCodecInfo, "isEncoder", "()Z");
	if (isEncoder == NULL) {
		LOGE("could not find MediaCodecInfo.isEncoder method");
		goto exit;
	}
	getSupportedTypes = env->GetMethodID(
						MediaCodecInfo,
						"getSupportedTypes",
						"()[Ljava/lang/String;");
	if (getSupportedTypes == NULL) {
		LOGE("could not find MediaCodecInfo.getSupportedTypes "
		      "method");
		goto exit;
	}
	getName = env->GetMethodID(MediaCodecInfo, "getName", "()Ljava/lang/String;");
	if (getName == NULL) {
		LOGE("could not find MediaCodecInfo.getName method");
		goto exit;
	}

	j_num_codecs =
		env->CallStaticIntMethod(MediaCodecList, getCodecCount);
	num_codecs = (int)j_num_codecs;

	for (i = 0; i < num_codecs; i++) {
		jobject j_codec_info;
		jboolean j_is_encoder;
		int is_encoder;
		jobject j_types;
		jobjectArray j_types_arr;
		jsize types_count, j;

		j_codec_info = env->CallStaticObjectMethod(MediaCodecList, getCodecInfoAt, (jint)i);
		if (j_codec_info == NULL) {
			LOGE("failed to get codec info at #%d", i);
			continue;
		}

		j_is_encoder =
			env->CallBooleanMethod(j_codec_info, isEncoder);
		is_encoder = (j_is_encoder) ? 1 : 0;

		if (is_encoder != encoder) {
			env->DeleteLocalRef(j_codec_info);
			continue;
		}

		j_types = env->CallObjectMethod(j_codec_info, getSupportedTypes);
		if (j_types == NULL) {
			env->DeleteLocalRef(j_codec_info);
			LOGE("failed to get codec supported types");
			continue;
		}
		j_types_arr = (jobjectArray)j_types;
		types_count = env->GetArrayLength(j_types_arr);

		for (j = 0; j < types_count; j++) {
			jstring j_mime_type;
			const char *c_mime_type;

			j_mime_type = (jstring)(env->GetObjectArrayElement(j_types_arr, j));
			if (j_mime_type == NULL) {
				LOGE("failed to get codec supported types");
				continue;
			}
			c_mime_type =
				env->GetStringUTFChars(j_mime_type, 0);

			if (strcmp(c_mime_type, mime_type) == 0) {
				jstring j_codec_name =
					(jstring)env->CallObjectMethod(j_codec_info, getName);
				if (j_codec_name == NULL) {
					LOGE("failed to get codec name");
				} else {
					const char *c_codec_name =
						env->GetStringUTFChars(j_codec_name, 0);
					codec_name = strdup(c_codec_name);
					env->ReleaseStringUTFChars(
						j_codec_name,
						c_codec_name);
					env->DeleteLocalRef(
							       j_codec_name);
				}
			}

			env->ReleaseStringUTFChars(j_mime_type, c_mime_type);
			env->DeleteLocalRef(j_mime_type);

			if (codec_name != NULL)
				break;
		}

		env->DeleteLocalRef(j_types);
		env->DeleteLocalRef(j_codec_info);

		if (codec_name != NULL)
			break;
	}

exit:
	if (MediaCodecList != NULL)
		env->DeleteLocalRef(MediaCodecList);
	if (MediaCodecInfo != NULL)
		env->DeleteLocalRef(MediaCodecInfo);

	if (attached) {
		res = _jvm->DetachCurrentThread();
		if (res != JNI_OK)
			LOGE("failed to detach thread from VM (%d)", res);
	}

	return codec_name;
}


static inline bool mcw_jni_attach_and_get_env(JNIEnv **env)
{
	bool attached = false;
	JNIEnv *_env = NULL;
	jint status =
		mcw_jni.jvm
			->GetEnv((void **)&_env, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		jint ret =
			mcw_jni.jvm
				->AttachCurrentThread( &_env, NULL);
		if (ret != JNI_OK) {
			LOGE("failed to attach thread to VM (%d)", ret);
			return false;
		}
		attached = true;
	} else if (status != JNI_OK) {
		LOGE("failed to get JNI environment (%d)", status);
		return false;
	}

	if (env)
		*env = _env;
	return attached;
}


static inline void mcw_jni_detach(bool attached)
{
	if (attached) {
		jint ret = mcw_jni.jvm->DetachCurrentThread();
		if (ret != JNI_OK)
			LOGE("failed to detach thread from VM (%d)", ret);
	}
}


static inline jstring mcw_jni_string_new(JNIEnv *env, const char *str)
{
	jstring jstr = env->NewStringUTF(str);
	if (env->ExceptionCheck()) {
		env->ExceptionClear();
		return NULL;
	} else
		return jstr;
}


static void mcw_jni_mediacodec_get_input_buffers(JNIEnv *env,
						 struct mcw_jni_mediacodec *mc)
{
	jobject j_buffers = NULL;

	LOGI("[Buffer] get_input_buffers");
	if (mcw_jni.MediaCodec.getInputBuffers) {
		LOGI("[Buffer] CallObjectMethod");
		j_buffers = env->CallObjectMethod(
			mc->mediacodec,
			mcw_jni.MediaCodec.getInputBuffers);

		if (env->ExceptionCheck()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
			goto end;
		}

		mc->in_buffers = env->NewGlobalRef(j_buffers);
		LOGI("[Buffer] NewGlobalRef ok");
	}else{
        LOGI("[Buffer] no get_input_buffers method");
	}

end:

    //todo why delete ???
	if (j_buffers)
		LOGI("[Buffer] DeleteLocalRef");
		env->DeleteLocalRef(j_buffers);
}


static void mcw_jni_mediacodec_get_output_buffers(JNIEnv *env,
						  struct mcw_jni_mediacodec *mc)
{
	jobject j_buffers = NULL;

	if (mcw_jni.MediaCodec.getOutputBuffers) {
		j_buffers = env->CallObjectMethod(
			mc->mediacodec,
			mcw_jni.MediaCodec.getOutputBuffers);

		if (env->ExceptionCheck()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
			goto end;
		}

		mc->out_buffers = env->NewGlobalRef(j_buffers);
	}

end:
	if (j_buffers)
		env->DeleteLocalRef(j_buffers);
}


static struct mcw_mediaformat *mcw_jni_mediaformat_new(void)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = NULL;
	jobject j_fmt = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	fmt = (struct mcw_jni_mediaformat *)calloc(1, sizeof(*fmt));
	if (fmt == NULL) {
		LOGE("calloc", ENOMEM);
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	if (!env) {
		error = 1;
		goto end;
	}

	j_fmt = env->NewObject(mcw_jni.MediaFormat.clazz, mcw_jni.MediaFormat.nnew);
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_fmt) {
		LOGE("format creation failed");
		error = 1;
		goto end;
	}
	fmt->mediaformat = env->NewGlobalRef(j_fmt);

end:
	if (j_fmt)
		env->DeleteLocalRef(j_fmt);

	mcw_jni_detach(attached);

	if (error) {
		free(fmt);
		return NULL;
	} else
		return (struct mcw_mediaformat *)fmt;
}


static enum mcw_media_status
mcw_jni_mediaformat_delete(struct mcw_mediaformat *format)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	env->DeleteGlobalRef(fmt->mediaformat);

	free(fmt);

	mcw_jni_detach(attached);

	return MCW_MEDIA_STATUS_OK;
}


static const char *mcw_jni_mediaformat_to_string(struct mcw_mediaformat *format)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_str = NULL;
	const char *str = NULL, *ret_str = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);

    j_str = (jstring)env->CallObjectMethod(fmt->mediaformat, mcw_jni.MediaFormat.toString);

	if (!j_str)
		goto end;

	str = env->GetStringUTFChars(j_str, NULL);
	if (!str)
		goto end;

	ret_str = strdup(str);

end:
	if (str)
		env->ReleaseStringUTFChars(j_str, str);

	if (j_str)
		env->DeleteLocalRef(j_str);

	mcw_jni_detach(attached);

	return ret_str;
}


static bool mcw_jni_mediaformat_get_int32(struct mcw_mediaformat *format,
					  const char *name,
					  int32_t *out)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jint ret = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return false;
	}
	if (!out) {
		LOGE("invalid output parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	ret = env->CallIntMethod(fmt->mediaformat, mcw_jni.MediaFormat.getInteger, j_name);

end:
	if (j_name)
		env->DeleteLocalRef(j_name);

	if (!error)
		*out = (int32_t)ret;

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static bool mcw_jni_mediaformat_get_int64(struct mcw_mediaformat *format,
					  const char *name,
					  int64_t *out)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jlong ret = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return false;
	}
	if (!out) {
		LOGE("invalid output parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	ret = env->CallLongMethod(fmt->mediaformat, mcw_jni.MediaFormat.getLong, j_name);

end:
	if (j_name)
		env->DeleteLocalRef(j_name);

	if (!error)
		*out = (int64_t)ret;

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static bool mcw_jni_mediaformat_get_float(struct mcw_mediaformat *format,
					  const char *name,
					  float *out)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jfloat ret = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return false;
	}
	if (!out) {
		LOGE("invalid output parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	ret = env->CallFloatMethod(fmt->mediaformat, mcw_jni.MediaFormat.getFloat, j_name);

end:
	if (j_name)
		env->DeleteLocalRef(j_name);

	if (!error)
		*out = (float)ret;

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static bool mcw_jni_mediaformat_get_buffer(struct mcw_mediaformat *format,
					   const char *name,
					   void **data,
					   size_t *size)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jobject j_buf = NULL;
	void *ptr = NULL;
	size_t sz = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return false;
	}
	if (!data) {
		LOGE("invalid output data parameter");
		return false;
	}
	if (!size) {
		LOGE("invalid output size parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_buf = env->CallObjectMethod(
					 fmt->mediaformat,
					 mcw_jni.MediaFormat.getByteBuffer,
					 j_name);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_buf) {
		error = 1;
		goto end;
	}

	ptr = env->GetDirectBufferAddress(j_buf);
	sz = env->GetDirectBufferCapacity(j_buf);

end:
	if (j_name)
		env->DeleteLocalRef(j_name);
	if (j_buf)
		env->DeleteLocalRef(j_buf);

	if (!error) {
		*data = ptr;
		*size = sz;
	}

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static bool mcw_jni_mediaformat_get_string(struct mcw_mediaformat *format,
					   const char *name,
					   const char **out)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jstring j_str = 0;
	const char *str = NULL, *ret_str = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return false;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return false;
	}
	if (!out) {
		LOGE("invalid output parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_str = (jstring)env->CallObjectMethod(fmt->mediaformat, mcw_jni.MediaFormat.getString, j_name);
	if (!j_str) {
		error = 1;
		goto end;
	}

	str = env->GetStringUTFChars(j_str, NULL);
	if (!str) {
		error = 1;
		goto end;
	}

	ret_str = strdup(str);

end:
	if (str)
		env->ReleaseStringUTFChars(j_str, str);
	if (j_str)
		env->DeleteLocalRef(j_str);
	if (j_name)
		env->DeleteLocalRef(j_name);

	if (!error)
		*out = ret_str;
	else
		free((char *)ret_str);

	mcw_jni_detach(attached);

	return (error) ? false : true;
}


static void mcw_jni_mediaformat_set_int32(struct mcw_mediaformat *format,
					  const char *name,
					  int32_t value)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		goto end;
	}

	env->CallVoidMethod(
			       fmt->mediaformat,
			       mcw_jni.MediaFormat.setInteger,
			       j_name,
			       (jint)value);

end:
	if (j_name)
		env->DeleteLocalRef(j_name);

	mcw_jni_detach(attached);
}


static void mcw_jni_mediaformat_set_int64(struct mcw_mediaformat *format,
					  const char *name,
					  int64_t value)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		goto end;
	}

	env->CallVoidMethod(
			       fmt->mediaformat,
			       mcw_jni.MediaFormat.setLong,
			       j_name,
			       (jlong)value);

end:
	if (j_name)
		env->DeleteLocalRef(j_name);

	mcw_jni_detach(attached);
}


static void mcw_jni_mediaformat_set_float(struct mcw_mediaformat *format,
					  const char *name,
					  float value)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		goto end;
	}

	env->CallVoidMethod(
			       fmt->mediaformat,
			       mcw_jni.MediaFormat.setInteger,
			       j_name,
			       (jfloat)value);

end:
	if (j_name)
		env->DeleteLocalRef(j_name);

	mcw_jni_detach(attached);
}


static void mcw_jni_mediaformat_set_string(struct mcw_mediaformat *format,
					   const char *name,
					   const char *value)
{
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jstring j_val = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return;
	}
	if (!value) {
		LOGE("invalid value parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		goto end;
	}

	j_val = mcw_jni_string_new(env,value);
	if (!j_val) {
		LOGE("string creation failed");
		goto end;
	}

	env->CallVoidMethod(
			       fmt->mediaformat,
			       mcw_jni.MediaFormat.setString,
			       j_name,
			       j_val);

end:
	if (j_val)
		env->DeleteLocalRef(j_val);
	if (j_name)
		env->DeleteLocalRef(j_name);

	mcw_jni_detach(attached);
}


static void mcw_jni_mediaformat_set_buffer(struct mcw_mediaformat *format,
					   const char *name,
					   void *data,
					   size_t size)
{
	int error = 0;
	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
	jstring j_name = NULL;
	jobject j_buf = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!fmt) {
		LOGE("invalid format parameter");
		return;
	}
	if (!name) {
		LOGE("invalid name parameter");
		return;
	}
	if (!data) {
		LOGE("invalid data parameter");
		return;
	}
	if (size == 0) {
		LOGE("invalid size parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_buf = env->NewDirectByteBuffer(data, size);
	if (!j_buf) {
		LOGE("buffer creation failed");
		error = 1;
		goto end;
	}

	env->CallVoidMethod(
			       fmt->mediaformat,
			       mcw_jni.MediaFormat.setByteBuffer,
			       j_name,
			       j_buf);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}

end:
	if (j_name)
		env->DeleteLocalRef(j_name);
	if (j_buf)
		env->DeleteLocalRef(j_buf);

	mcw_jni_detach(attached);
}


static struct mcw_mediacodec *
mcw_jni_mediacodec_create_codec_by_name(const char *name)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = NULL;
	jobject j_mc = NULL;
	jobject j_buf_info = NULL;
	jstring j_name = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	mc = (struct mcw_jni_mediacodec *)calloc(1, sizeof(*mc));
	if (mc == NULL) {
		LOGE("calloc", ENOMEM);
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	if (!env) {
		error = 1;
		goto end;
	}

	j_name = mcw_jni_string_new(env,name);
	if (!j_name) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_mc = env->CallStaticObjectMethod(
		mcw_jni.MediaCodec.clazz,
		mcw_jni.MediaCodec.createByCodecName,
		j_name);
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_mc) {
		LOGE("codec creation failed (name: %s)", name);
		error = 1;
		goto end;
	}
	mc->mediacodec = env->NewGlobalRef(j_mc);

	j_buf_info = env->NewObject(mcw_jni.BufferInfo.clazz, mcw_jni.BufferInfo.nnew);
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_buf_info) {
		LOGE("buffer info object creation failed");
		error = 1;
		goto end;
	}
	mc->bufferinfo = env->NewGlobalRef(j_buf_info);

end:
	if (j_buf_info)
		env->DeleteLocalRef(j_buf_info);
	if (j_name)
		env->DeleteLocalRef(j_name);

	mcw_jni_detach(attached);

	if (error) {
		free(mc);
		return NULL;
	} else
		return (struct mcw_mediacodec *)mc;
}


static struct mcw_mediacodec *
mcw_jni_mediacodec_create_decoder_by_type(const char *mime_type)
{
	LOGE("[CODEC] mcw_jni_mediacodec_create_decoder_by_type...");
	int error = 0;
	struct mcw_jni_mediacodec *mc = NULL;
	jobject j_mc = NULL;
	jobject j_buf_info = NULL;
	jstring j_mime_type = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	LOGE("[CODEC] calloc...");
	mc = (struct mcw_jni_mediacodec *)calloc(1, sizeof(*mc));
	if (mc == NULL) {
		LOGE("calloc", ENOMEM);
		return NULL;
	}

	LOGE("[CODEC] attached...");
	attached = mcw_jni_attach_and_get_env(&env);
	if (!env) {
		error = 1;
		goto end;
	}

	LOGE("[CODEC] string_new...");
	j_mime_type = mcw_jni_string_new(env,mime_type);
	if (!j_mime_type) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	LOGE("[CODEC] createDecoderByType...");
	j_mc = env->CallStaticObjectMethod(
		mcw_jni.MediaCodec.clazz,
		mcw_jni.MediaCodec.createDecoderByType,
		j_mime_type);
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_mc) {
		LOGE("codec creation failed (mime type: %s)", mime_type);
		error = 1;
		goto end;
	}
	mc->mediacodec = env->NewGlobalRef(j_mc);

	j_buf_info = env->NewObject(mcw_jni.BufferInfo.clazz, mcw_jni.BufferInfo.nnew);
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_buf_info) {
		LOGE("buffer info object creation failed");
		error = 1;
		goto end;
	}
	mc->bufferinfo = env->NewGlobalRef(j_buf_info);

end:
	LOGE("[CODEC] end...");
	if (j_buf_info)
		env->DeleteLocalRef(j_buf_info);
	if (j_mime_type)
		env->DeleteLocalRef(j_mime_type);

	LOGE("mcw_jni_detach...");
	mcw_jni_detach(attached);

	if (error) {
		LOGE("[CODEC] end...");
		free(mc);
		return NULL;
	} else
		return (struct mcw_mediacodec *)mc;
}


static struct mcw_mediacodec *
mcw_jni_mediacodec_create_encoder_by_type(const char *mime_type)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = NULL;
	jobject j_mc = NULL;
	jobject j_buf_info = NULL;
	jstring j_mime_type = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	mc = (struct mcw_jni_mediacodec *)calloc(1, sizeof(*mc));
	if (mc == NULL) {
		LOGE("calloc", ENOMEM);
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);
	if (!env) {
		error = 1;
		goto end;
	}

	j_mime_type = mcw_jni_string_new(env,mime_type);
	if (!j_mime_type) {
		LOGE("string creation failed");
		error = 1;
		goto end;
	}

	j_mc = env->CallStaticObjectMethod(
		mcw_jni.MediaCodec.clazz,
		mcw_jni.MediaCodec.createEncoderByType,
		j_mime_type);
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_mc) {
		LOGE("codec creation failed (mime type: %s)", mime_type);
		error = 1;
		goto end;
	}
	mc->mediacodec = env->NewGlobalRef(j_mc);

	j_buf_info = env->NewObject(mcw_jni.BufferInfo.clazz, mcw_jni.BufferInfo.nnew);
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_buf_info) {
		LOGE("buffer info object creation failed");
		error = 1;
		goto end;
	}
	mc->bufferinfo = env->NewGlobalRef(j_buf_info);

end:
	if (j_buf_info)
		env->DeleteLocalRef(j_buf_info);
	if (j_mime_type)
		env->DeleteLocalRef(j_mime_type);

	mcw_jni_detach(attached);

	if (error) {
		free(mc);
		return NULL;
	} else
		return (struct mcw_mediacodec *)mc;
}


static enum mcw_media_status
mcw_jni_mediacodec_delete(struct mcw_mediacodec *codec)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	env->CallVoidMethod(mc->mediacodec, mcw_jni.MediaCodec.release);

	env->DeleteGlobalRef(mc->mediacodec);

	env->DeleteGlobalRef(mc->bufferinfo);

	if (mc->in_buffers)
		env->DeleteGlobalRef(mc->in_buffers);

	if (mc->out_buffers)
		env->DeleteGlobalRef(mc->out_buffers);

	free(mc);

	mcw_jni_detach(attached);

	return MCW_MEDIA_STATUS_OK;
}

// todo why change to ANativeWindow ???
//static enum mcw_media_status
//mcw_jni_mediacodec_configure(struct mcw_mediacodec *codec,
//			     const struct mcw_mediaformat *format,
//			     ANativeWindow *surface,
//			     struct mcw_mediacrypto *crypto,
//			     uint32_t flags)
//{
//	LOGI("[config] config...");
//	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
//	struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
//	jobject j_surface = NULL;
//	enum mcw_media_status ret = MCW_MEDIA_STATUS_OK;
//	bool attached = false;
//	JNIEnv *env = NULL;
//
//	if (!mc) {
//		LOGE("[config] invalid codec parameter");
//		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
//	}
//
//	LOGI("[config] attached...");
//	attached = mcw_jni_attach_and_get_env(&env);
//
//#if 0
//	/* TODO: ANativeWindow_toSurface is introduced in API 26 */
//	if (surface)
//		j_surface = ANativeWindow_toSurface(surface);
//#endif
//
//	LOGI("[config] CallVoidMethod...");
//	env->CallVoidMethod(
//			       mc->mediacodec,
//			       mcw_jni.MediaCodec.configure,
//			       (fmt) ? fmt->mediaformat : NULL,
//			       j_surface,
//			       NULL, /* crypto is not supported */
//			       (jint)flags);
//
//	if (env->ExceptionCheck()) {
//		env->ExceptionDescribe();
//		env->ExceptionClear();
//		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
//	}
//
//	if (j_surface)
//		env->DeleteLocalRef(j_surface);
//
//	LOGI("[config] mcw_jni_detach...");
//	mcw_jni_detach(attached);
//
//	return ret;
//}


static enum mcw_media_status
mcw_jni_mediacodec_configure(struct mcw_mediacodec *codec,
                             const struct mcw_mediaformat *format,
                             jobject surface,
                             struct mcw_mediacrypto *crypto,
                             uint32_t flags)
{
    LOGI("[config] config...");
    struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
    struct mcw_jni_mediaformat *fmt = (struct mcw_jni_mediaformat *)format;
    jobject j_surface = NULL;
    enum mcw_media_status ret = MCW_MEDIA_STATUS_OK;
    bool attached = false;
    JNIEnv *env = NULL;

    if (!mc) {
        LOGE("[config] invalid codec parameter");
        return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
    }

    LOGI("[config] attached...");
    attached = mcw_jni_attach_and_get_env(&env);

#if 0
    /* TODO: ANativeWindow_toSurface is introduced in API 26 */
	if (surface)
		j_surface = ANativeWindow_toSurface(surface);
#endif

    LOGI("[config] CallVoidMethod...");
    env->CallVoidMethod(
                           mc->mediacodec,
                           mcw_jni.MediaCodec.configure,
                           (fmt) ? fmt->mediaformat : NULL,
                           surface,
                           NULL, /* crypto is not supported */
                           (jint)flags);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
    }

    if (j_surface)
        env->DeleteLocalRef(j_surface);

    LOGI("[config] mcw_jni_detach...");
    mcw_jni_detach(attached);

    return ret;
}


static enum mcw_media_status
mcw_jni_mediacodec_start(struct mcw_mediacodec *codec)
{
    LOGE("[Buffer] mcw_jni_mediacodec_start.");
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret = MCW_MEDIA_STATUS_OK;
	bool attached = false;
	JNIEnv *env = NULL;
    LOGE("[Buffer] mcw_jni_mediacodec_start..");
	if (!mc) {
		LOGE("[Buffer] invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

    LOGI("[Buffer] attached");
	attached = mcw_jni_attach_and_get_env(&env);

    LOGI("[Buffer] CallVoidMethod mc: %p",mc);
    LOGI("[Buffer] CallVoidMethod mediacodec: %p",mc->mediacodec);
	env->CallVoidMethod(mc->mediacodec, mcw_jni.MediaCodec.start);
    LOGI("[Buffer] CallVoidMethod end ");

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		goto end;
	}

	LOGI("[Buffer] get_input/output_buffers");
	mcw_jni_mediacodec_get_input_buffers(env,mc);
	mcw_jni_mediacodec_get_output_buffers(env,mc);

end:
	mcw_jni_detach(attached);

	return ret;
}


static enum mcw_media_status
mcw_jni_mediacodec_stop(struct mcw_mediacodec *codec)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	env->CallVoidMethod(mc->mediacodec, mcw_jni.MediaCodec.stop);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


static enum mcw_media_status
mcw_jni_mediacodec_flush(struct mcw_mediacodec *codec)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	env->CallVoidMethod(mc->mediacodec, mcw_jni.MediaCodec.flush);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


static uint8_t *
mcw_jni_mediacodec_get_input_buffer(struct mcw_mediacodec *codec,
				    size_t idx,
				    size_t *out_size)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	jobject j_buf = NULL;
	uint8_t *ptr = NULL;
	size_t sz = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return NULL;
	}
	if (!out_size) {
		LOGE("invalid output size parameter");
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	if (mc->in_buffers) {
		j_buf = env->GetObjectArrayElement((jobjectArray)mc->in_buffers, (jint)idx);
	} else {
		j_buf = env->CallObjectMethod(
			mc->mediacodec,
			mcw_jni.MediaCodec.getInputBuffer,
			(jint)idx);

		if (env->ExceptionCheck()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
			error = 1;
			goto end;
		}
	}

	if (!j_buf) {
		error = 1;
		goto end;
	}

	ptr = (uint8_t *)env->GetDirectBufferAddress(j_buf);
	sz = env->GetDirectBufferCapacity(j_buf);

end:
	if (j_buf)
		env->DeleteLocalRef(j_buf);

	mcw_jni_detach(attached);

	if (error)
		return NULL;
	else {
		if (out_size)
			*out_size = sz;
		return ptr;
	}
}


static uint8_t *
mcw_jni_mediacodec_get_output_buffer(struct mcw_mediacodec *codec,
				     size_t idx,
				     size_t *out_size)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	jobject j_buf = NULL;
	uint8_t *ptr = NULL;
	size_t sz = 0;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return NULL;
	}
	if (!out_size) {
		LOGE("invalid output size parameter");
		return NULL;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	if (mc->out_buffers) {
		j_buf = env->GetObjectArrayElement((jobjectArray)mc->out_buffers, (jint)idx);
	} else {
		j_buf = env->CallObjectMethod(
			mc->mediacodec,
			mcw_jni.MediaCodec.getOutputBuffer,
			(jint)idx);

		if (env->ExceptionCheck()) {
			env->ExceptionDescribe();
			env->ExceptionClear();
			error = 1;
			goto end;
		}
	}

	if (!j_buf) {
		error = 1;
		goto end;
	}

	ptr = (uint8_t*)env->GetDirectBufferAddress(j_buf);
	sz = env->GetDirectBufferCapacity(j_buf);

end:
	if (j_buf)
		env->DeleteLocalRef(j_buf);

	mcw_jni_detach(attached);

	if (error)
		return NULL;
	else {
		if (out_size)
			*out_size = sz;
		return ptr;
	}
}


static ssize_t
mcw_jni_mediacodec_dequeue_input_buffer(struct mcw_mediacodec *codec,
					int64_t timeout_us)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	jint ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return -1;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	ret = env->CallIntMethod(
				    mc->mediacodec,
				    mcw_jni.MediaCodec.dequeueInputBuffer,
				    (jlong)timeout_us);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = -1;
	}

	mcw_jni_detach(attached);

	return (ssize_t)ret;
}


static enum mcw_media_status
mcw_jni_mediacodec_queue_input_buffer(struct mcw_mediacodec *codec,
				      size_t idx,
				      off_t offset,
				      size_t size,
				      uint64_t time,
				      uint32_t flags)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	env->CallVoidMethod(
			       mc->mediacodec,
			       mcw_jni.MediaCodec.queueInputBuffer,
			       (jint)idx,
			       (jint)offset,
			       (jint)size,
			       (jlong)time,
			       (jint)flags);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


static ssize_t
mcw_jni_mediacodec_dequeue_output_buffer(struct mcw_mediacodec *codec,
					 struct mcw_mediacodec_bufferinfo *info,
					 int64_t timeout_us)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	jint ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return -1;
	}
	if (!info) {
		LOGE("invalid info parameter");
		return -1;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	ret = env->CallIntMethod(
				    mc->mediacodec,
				    mcw_jni.MediaCodec.dequeueOutputBuffer,
				    mc->bufferinfo,
				    (jlong)timeout_us);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = -1;
		goto end;
	}

	if (ret == MCW_INFO_OUTPUT_BUFFERS_CHANGED)
		mcw_jni_mediacodec_get_output_buffers(env,mc);

	info->offset = env->GetIntField(mc->bufferinfo, mcw_jni.BufferInfo.offset);
	info->size = env->GetIntField(mc->bufferinfo, mcw_jni.BufferInfo.size);
	info->presentation_time_us = env->GetLongField(mc->bufferinfo, mcw_jni.BufferInfo.presentationTimeUs);
	info->flags = env->GetIntField(mc->bufferinfo, mcw_jni.BufferInfo.flags);

end:
	mcw_jni_detach(attached);

	return (ssize_t)ret;
}


static struct mcw_mediaformat *
mcw_jni_mediacodec_get_output_format(struct mcw_mediacodec *codec, size_t idx)
{
	int error = 0;
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	struct mcw_jni_mediaformat *fmt = NULL;
	jobject j_fmt = NULL;
	bool attached = false;
	JNIEnv *env = NULL;

	attached = mcw_jni_attach_and_get_env(&env);

	fmt = (struct mcw_jni_mediaformat *)calloc(1, sizeof(*fmt));
	if (fmt == NULL) {
		LOGE("calloc", ENOMEM);
		return NULL;
	}

	j_fmt = env->CallObjectMethod(
					 mc->mediacodec,
					 mcw_jni.MediaCodec.getOutputFormat,
					 (jint)idx);
	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		error = 1;
		goto end;
	}
	if (!j_fmt) {
		error = 1;
		goto end;
	}
	fmt->mediaformat = env->NewGlobalRef(j_fmt);

end:
	if (j_fmt)
		env->DeleteLocalRef(j_fmt);

	mcw_jni_detach(attached);

	if (error) {
		free(fmt);
		return NULL;
	} else
		return (struct mcw_mediaformat *)fmt;
}


static enum mcw_media_status
mcw_jni_mediacodec_release_output_buffer(struct mcw_mediacodec *codec,
					 size_t idx,
					 bool render)
{
	LOGI("[render] release_output_buffer...");
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("[render] invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	LOGI("[render] attached...");
	attached = mcw_jni_attach_and_get_env(&env);

	env->CallVoidMethod(
			       mc->mediacodec,
			       mcw_jni.MediaCodec.releaseOutputBuffer,
			       (jint)idx,
			       (jboolean)render);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else{
		ret = MCW_MEDIA_STATUS_OK;
		LOGI("[render] MCW_MEDIA_STATUS_OK...");
	}

	mcw_jni_detach(attached);

	return ret;
}


static enum mcw_media_status
mcw_jni_mediacodec_release_output_buffer_at_time(struct mcw_mediacodec *codec,
						 size_t idx,
						 int64_t timestamp_ns)
{
	struct mcw_jni_mediacodec *mc = (struct mcw_jni_mediacodec *)codec;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mc) {
		LOGE("invalid codec parameter");
		return MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	env->CallVoidMethod(
			       mc->mediacodec,
			       mcw_jni.MediaCodec.releaseOutputBuffer,
			       (jint)idx,
			       (jlong)timestamp_ns);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return ret;
}


//mediaExtractor
struct mcw_mediaExtractor*  mcw_jni_mediaExtractor_nnew(){
    int error = 0;
    struct mcw_jni_mediaExtractor *ext = NULL;
    jobject j_ext = NULL;
    bool attached = false;
    JNIEnv *env = NULL;

	ext = (struct mcw_jni_mediaExtractor *)calloc(1, sizeof(*ext));
    if (ext == NULL) {
        LOGE("calloc", ENOMEM);
        return NULL;
    }

    attached = mcw_jni_attach_and_get_env(&env);
    if (!env) {
        error = 1;
        goto end;
    }

	j_ext = env->NewObject(mcw_jni.MediaExtractor.clazz, mcw_jni.MediaExtractor.nnew);
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        error = 1;
        goto end;
    }
    if (!j_ext) {
        LOGE("MediaExtractor creation failed");
        error = 1;
        goto end;
    }
	ext->mediaExtractor = env->NewGlobalRef(j_ext);

    end:
    if (j_ext)
        env->DeleteLocalRef(j_ext);

    mcw_jni_detach(attached);

    if (error) {
        free(ext);
        return NULL;
    } else
        return (struct mcw_mediaExtractor *)ext;
}


//将const char*类型转换成jstring类型
jstring CStr2Jstring(JNIEnv* env, const char* pat)
{
	//定义java String类 strClass
	jclass strClass = (env)->FindClass("java/lang/String");
	//获取java String类方法String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
	jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	//建立byte数组
	jbyteArray bytes = (env)->NewByteArray((jsize)strlen(pat));
	//将char* 转换为byte数组
	(env)->SetByteArrayRegion(bytes, 0, (jsize)strlen(pat), (jbyte*)pat);
	//设置String, 保存语言类型,用于byte数组转换至String时的参数
	jstring encoding = (env)->NewStringUTF("GB2312");
	//将byte数组转换为java String,并输出
	return (jstring)(env)->NewObject(strClass, ctorID, bytes, encoding);
}

void mcw_jni_mediaExtractor_set_data_source(mcw_mediaExtractor* extractor,const char* path){
	struct mcw_jni_mediaExtractor *mt = (struct mcw_jni_mediaExtractor *)extractor;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mt) {
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("invalid Extractor parameter");
		return ;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	jstring sourcePath = CStr2Jstring(env,path);

	env->CallVoidMethod(
			mt->mediaExtractor,
			mcw_jni.MediaExtractor.setDataSource,
			sourcePath);

	//todo no sure if right,how to release the jstring sourcePath ???
	//env->ReleaseStringUTFChars(sourcePath,path);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);
}

size_t mcw_jni_mediaExtractor_get_track_count(mcw_mediaExtractor* extractor){
	struct mcw_jni_mediaExtractor *mt = (struct mcw_jni_mediaExtractor *)extractor;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mt) {
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("invalid Extractor parameter");
		return -1;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	size_t trackCount = env->CallIntMethod(
			mt->mediaExtractor,
			mcw_jni.MediaExtractor.getTrackCount);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return trackCount;
}

struct mcw_mediaformat* mcw_jni_mediaExtractor_get_track_format(mcw_mediaExtractor* extractor,size_t index){
	struct mcw_jni_mediaExtractor *mt = (struct mcw_jni_mediaExtractor *)extractor;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mt) {
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("invalid Extractor parameter");
		return nullptr;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	jobject formatObject = env->CallObjectMethod(
			mt->mediaExtractor,
			mcw_jni.MediaExtractor.getTrackFormat,
			index);

	struct mcw_jni_mediaformat *fmt = nullptr;
	fmt = (struct mcw_jni_mediaformat *)calloc(1, sizeof(*fmt));
	if (!fmt){
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("malloc mediaformat failed");
		return nullptr;
	}
	fmt->mediaformat = formatObject;

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return (mcw_mediaformat*)fmt;
}
void mcw_jni_mediaExtractor_select_track(mcw_mediaExtractor* extractor,size_t index){
	struct mcw_jni_mediaExtractor *mt = (struct mcw_jni_mediaExtractor *)extractor;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mt) {
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("invalid Extractor parameter");
		return ;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	env->CallVoidMethod(
			mt->mediaExtractor,
			mcw_jni.MediaExtractor.selectTrack,
			index);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

}

size_t mcw_jni_mediaExtractor_read_sample_data(mcw_mediaExtractor* extractor,uint8_t* buff,size_t buff_size){
	struct mcw_jni_mediaExtractor *mt = (struct mcw_jni_mediaExtractor *)extractor;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mt) {
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("invalid Extractor parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	jobject inputBuffer = env->NewDirectByteBuffer(buff, buff_size);

	if(inputBuffer == nullptr){
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("create inputBuffer object failed");
		return false;
	}

	size_t size = env->CallIntMethod(
			mt->mediaExtractor,
			mcw_jni.MediaExtractor.readSampleData,
			inputBuffer,
			0);

	//todo if should release inputBuffer ???
	if (inputBuffer){
		env->DeleteLocalRef(inputBuffer);
	}

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return size;
}

long mcw_jni_mediaExtractor_get_sample_time(mcw_mediaExtractor* extractor){
	struct mcw_jni_mediaExtractor *mt = (struct mcw_jni_mediaExtractor *)extractor;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mt) {
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("invalid Extractor parameter");
		return -1;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	long sampleTime = env->CallLongMethod(
			mt->mediaExtractor,
			mcw_jni.MediaExtractor.getSampleTime);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return sampleTime;
}
bool mcw_jni_mediaExtractor_advance(mcw_mediaExtractor* extractor){
	struct mcw_jni_mediaExtractor *mt = (struct mcw_jni_mediaExtractor *)extractor;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mt) {
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("invalid Extractor parameter");
		return false;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	bool advanceOk = env->CallBooleanMethod(
			mt->mediaExtractor,
			mcw_jni.MediaExtractor.advance);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);

	return advanceOk;
}
void mcw_jni_mediaExtractor_release(mcw_mediaExtractor* extractor){
	struct mcw_jni_mediaExtractor *mt = (struct mcw_jni_mediaExtractor *)extractor;
	enum mcw_media_status ret;
	bool attached = false;
	JNIEnv *env = NULL;

	if (!mt) {
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
		LOGE("invalid Extractor parameter");
		return;
	}

	attached = mcw_jni_attach_and_get_env(&env);

	env->CallVoidMethod(
			mt->mediaExtractor,
			mcw_jni.MediaExtractor.release);

	if (env->ExceptionCheck()) {
		env->ExceptionDescribe();
		env->ExceptionClear();
		ret = MCW_MEDIA_STATUS_ERROR_UNKNOWN;
	} else
		ret = MCW_MEDIA_STATUS_OK;

	mcw_jni_detach(attached);
}


int mcw_jni_init(struct mcw *mcw, void *jvm)
{
    LOGD("[jni_init] mcw_jni_init...");
	static int needs_init = 1;
	static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	int ret = 0;
	unsigned int i;
	bool attached = false;
	JavaVM *_jvm = (JavaVM *)jvm;
	JNIEnv *env = NULL;
	jclass clazz;
	jint res, status;

	pthread_mutex_lock(&mutex);

	if (!needs_init)
		goto end;

    LOGD("[jni_init] memset...");
	memset(&mcw_jni, 0, sizeof(mcw_jni));

	status = _jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	if (status == JNI_EDETACHED) {
		res = _jvm->AttachCurrentThread(&env, NULL);
		if (res != JNI_OK) {
			LOGE("[jni_init] failed to attach thread to VM (%d)", res);
			ret = -EPROTO;
			goto end;
		}
		attached = true;
	} else if (status != JNI_OK) {
		LOGE("[jni_init] failed to get JNI environment (%d)", status);
		ret = -EPROTO;
		goto end;
	}
	mcw_jni.jvm = _jvm;
    LOGD("[jni_init] jvm init ok...");

    LOGD("[jni_init] FindClass...");
	for (i = 0; i < MCW_SIZEOF_ARRAY(mcw_jni_class); i++) {
		clazz = env->FindClass(mcw_jni_class[i].name);
		if (env->ExceptionCheck()) {
			env->ExceptionClear();
			ret = -ENOSYS;
			goto end;
		}
		*(jclass *)((uint8_t *)&mcw_jni + mcw_jni_class[i].offset) =
			(jclass)env->NewGlobalRef(clazz);
	}

    LOGD("[jni_init] getMethodIds...");
	for (i = 0; i < MCW_SIZEOF_ARRAY(mcw_jni_member); i++) {
		clazz = *(jclass *)((uint8_t *)&mcw_jni +
				    mcw_jni_member[i].class_offset);
		switch (mcw_jni_member[i].type) {
		case MCW_JNI_MEMBER_TYPE_METHOD:
			*(jmethodID *)((uint8_t *)&mcw_jni +
				       mcw_jni_member[i].offset) =
				env->GetMethodID(
					clazz,
					mcw_jni_member[i].name,
					mcw_jni_member[i].signature);
			break;
		case MCW_JNI_MEMBER_TYPE_STATIC_METHOD:
			*(jmethodID *)((uint8_t *)&mcw_jni +
				       mcw_jni_member[i].offset) =
				env->GetStaticMethodID(
					clazz,
					mcw_jni_member[i].name,
					mcw_jni_member[i].signature);
			break;
		case MCW_JNI_MEMBER_TYPE_FIELD:
			*(jfieldID *)((uint8_t *)&mcw_jni +
				      mcw_jni_member[i].offset) =
				env->GetFieldID(
						   clazz,
						   mcw_jni_member[i].name,
						   mcw_jni_member[i].signature);
			break;
		default:
			break;
		}
		if (env->ExceptionCheck()) {
			env->ExceptionClear();
			if (mcw_jni_member[i].critical) {
				ret = -ENOSYS;
				LOGD("[jni_init] env... %d",i);
				goto end;
			}
		}
	}

    LOGD("[jni_init] getMethodFinish");
	if ((mcw_jni.MediaCodec.getInputBuffer) &&
	    (mcw_jni.MediaCodec.getOutputBuffer)) {
        //todo why set null ???
//		mcw_jni.MediaCodec.getInputBuffers = NULL;
//		mcw_jni.MediaCodec.getOutputBuffers = NULL;
	} else if ((!mcw_jni.MediaCodec.getInputBuffers) &&
		   (!mcw_jni.MediaCodec.getOutputBuffers)) {
		LOGE("[jni_init] Could not find methods "
		      "getInputBuffer(s)/getOutputBuffer(s)");
		ret = -ENOSYS;
		goto end;
	}

	needs_init = 0;

end:
    LOGD("[jni_init] end...");
	mcw->implem = MCW_IMPLEMENTATION_JNI;
	mcw->mediaformat.nnew = mcw_jni_mediaformat_new;
	mcw->mediaformat.ddelete = mcw_jni_mediaformat_delete;
	mcw->mediaformat.to_string = mcw_jni_mediaformat_to_string;
	mcw->mediaformat.get_int32 = mcw_jni_mediaformat_get_int32;
	mcw->mediaformat.get_int64 = mcw_jni_mediaformat_get_int64;
	mcw->mediaformat.get_float = mcw_jni_mediaformat_get_float;
	mcw->mediaformat.get_buffer = mcw_jni_mediaformat_get_buffer;
	mcw->mediaformat.get_string = mcw_jni_mediaformat_get_string;
	mcw->mediaformat.set_int32 = mcw_jni_mediaformat_set_int32;
	mcw->mediaformat.set_int64 = mcw_jni_mediaformat_set_int64;
	mcw->mediaformat.set_float = mcw_jni_mediaformat_set_float;
	mcw->mediaformat.set_string = mcw_jni_mediaformat_set_string;
	mcw->mediaformat.set_buffer = mcw_jni_mediaformat_set_buffer;
	mcw->mediaformat.KEY_AAC_PROFILE = mcw_jni_mediaformat_KEY_AAC_PROFILE;
	mcw->mediaformat.KEY_BIT_RATE = mcw_jni_mediaformat_KEY_BIT_RATE;
	mcw->mediaformat.KEY_CHANNEL_COUNT =
		mcw_jni_mediaformat_KEY_CHANNEL_COUNT;
	mcw->mediaformat.KEY_CHANNEL_MASK =
		mcw_jni_mediaformat_KEY_CHANNEL_MASK;
	mcw->mediaformat.KEY_COLOR_FORMAT =
		mcw_jni_mediaformat_KEY_COLOR_FORMAT;
	mcw->mediaformat.KEY_DURATION = mcw_jni_mediaformat_KEY_DURATION;
	mcw->mediaformat.KEY_FLAC_COMPRESSION_LEVEL =
		mcw_jni_mediaformat_KEY_FLAC_COMPRESSION_LEVEL;
	mcw->mediaformat.KEY_FRAME_RATE = mcw_jni_mediaformat_KEY_FRAME_RATE;
	mcw->mediaformat.KEY_HEIGHT = mcw_jni_mediaformat_KEY_HEIGHT;
	mcw->mediaformat.KEY_IS_ADTS = mcw_jni_mediaformat_KEY_IS_ADTS;
	mcw->mediaformat.KEY_IS_AUTOSELECT =
		mcw_jni_mediaformat_KEY_IS_AUTOSELECT;
	mcw->mediaformat.KEY_IS_DEFAULT = mcw_jni_mediaformat_KEY_IS_DEFAULT;
	mcw->mediaformat.KEY_IS_FORCED_SUBTITLE =
		mcw_jni_mediaformat_KEY_IS_FORCED_SUBTITLE;
	mcw->mediaformat.KEY_I_FRAME_INTERVAL =
		mcw_jni_mediaformat_KEY_I_FRAME_INTERVAL;
	mcw->mediaformat.KEY_LANGUAGE = mcw_jni_mediaformat_KEY_LANGUAGE;
	mcw->mediaformat.KEY_MAX_HEIGHT = mcw_jni_mediaformat_KEY_MAX_HEIGHT;
	mcw->mediaformat.KEY_MAX_INPUT_SIZE =
		mcw_jni_mediaformat_KEY_MAX_INPUT_SIZE;
	mcw->mediaformat.KEY_MAX_WIDTH = mcw_jni_mediaformat_KEY_MAX_WIDTH;
	mcw->mediaformat.KEY_MIME = mcw_jni_mediaformat_KEY_MIME;
	mcw->mediaformat.KEY_PUSH_BLANK_BUFFERS_ON_STOP =
		mcw_jni_mediaformat_KEY_PUSH_BLANK_BUFFERS_ON_STOP;
	mcw->mediaformat.KEY_REPEAT_PREVIOUS_FRAME_AFTER =
		mcw_jni_mediaformat_KEY_REPEAT_PREVIOUS_FRAME_AFTER;
	mcw->mediaformat.KEY_SAMPLE_RATE = mcw_jni_mediaformat_KEY_SAMPLE_RATE;
	mcw->mediaformat.KEY_WIDTH = mcw_jni_mediaformat_KEY_WIDTH;
	mcw->mediaformat.KEY_STRIDE = mcw_jni_mediaformat_KEY_STRIDE;
	mcw->mediacodec.create_codec_by_name =
		mcw_jni_mediacodec_create_codec_by_name;
    LOGD("[jni_init] pointer -> create_decoder_by_type...");
	mcw->mediacodec.create_decoder_by_type =
		mcw_jni_mediacodec_create_decoder_by_type;
	mcw->mediacodec.create_encoder_by_type =
		mcw_jni_mediacodec_create_encoder_by_type;
	mcw->mediacodec.ddelete = mcw_jni_mediacodec_delete;
	mcw->mediacodec.configure = mcw_jni_mediacodec_configure;
	mcw->mediacodec.start = mcw_jni_mediacodec_start;
	mcw->mediacodec.stop = mcw_jni_mediacodec_stop;
	mcw->mediacodec.flush = mcw_jni_mediacodec_flush;
	mcw->mediacodec.get_input_buffer = mcw_jni_mediacodec_get_input_buffer;
	mcw->mediacodec.get_output_buffer =
		mcw_jni_mediacodec_get_output_buffer;
	mcw->mediacodec.dequeue_input_buffer =
		mcw_jni_mediacodec_dequeue_input_buffer;
	mcw->mediacodec.queue_input_buffer =
		mcw_jni_mediacodec_queue_input_buffer;
	mcw->mediacodec.dequeue_output_buffer =
		mcw_jni_mediacodec_dequeue_output_buffer;
	mcw->mediacodec.get_output_format =
		mcw_jni_mediacodec_get_output_format;
	mcw->mediacodec.release_output_buffer =
		mcw_jni_mediacodec_release_output_buffer;
	mcw->mediacodec.release_output_buffer_at_time =
		mcw_jni_mediacodec_release_output_buffer_at_time;

	//mediaExtractor
    mcw->mediaExtractor.nnew = mcw_jni_mediaExtractor_nnew;
    mcw->mediaExtractor.set_data_source = mcw_jni_mediaExtractor_set_data_source;
    mcw->mediaExtractor.get_track_count = mcw_jni_mediaExtractor_get_track_count;
    mcw->mediaExtractor.get_track_format = mcw_jni_mediaExtractor_get_track_format;
    mcw->mediaExtractor.select_track = mcw_jni_mediaExtractor_select_track;
    mcw->mediaExtractor.read_sample_data = mcw_jni_mediaExtractor_read_sample_data;
    mcw->mediaExtractor.get_sample_time = mcw_jni_mediaExtractor_get_sample_time;
    mcw->mediaExtractor.advance = mcw_jni_mediaExtractor_advance;
    mcw->mediaExtractor.release = mcw_jni_mediaExtractor_release;

	if (ret != 0) {
		for (i = 0; i < MCW_SIZEOF_ARRAY(mcw_jni_class); i++) {
			clazz = *(jclass *)((uint8_t *)&mcw_jni +
					    mcw_jni_class[i].offset);
			if (clazz)
				env->DeleteGlobalRef(clazz);
		}
	}

	if (attached) {
		res = _jvm->DetachCurrentThread();
		if (res != JNI_OK)
			LOGE("[jni_init] failed to detach thread from VM (%d)", res);
	}

	pthread_mutex_unlock(&mutex);
    LOGD("[jni_init] pthread_mutex_unlock,return");

	return ret;
}
