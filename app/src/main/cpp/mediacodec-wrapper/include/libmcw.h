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

#ifndef _LIBMCW_H_
#define _LIBMCW_H_

#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <jni.h>
#include <android/native_window.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* To be used for all public API */
#ifdef MCW_API_EXPORTS
#	ifdef _WIN32
#		define MCW_API __declspec(dllexport)
#	else /* !_WIN32 */
#		define MCW_API __attribute__((visibility("default")))
#	endif /* !_WIN32 */
#else /* !MCW_API_EXPORTS */
#	define MCW_API
#endif /* !MCW_API_EXPORTS */


/* MediaCodec implentation to use */
enum mcw_implementation {
	/* Automatically select implementation (NDK will be used
	 * if available, JNI otherwise) */
	MCW_IMPLEMENTATION_AUTO = 0,

	/* Use Android's NDK C AMediaCodec API */
	MCW_IMPLEMENTATION_NDK,

	/* Use Android's SDK Java MediaCodec API through JNI */
	MCW_IMPLEMENTATION_JNI,
};


/**
 * See https://developer.android.com/reference/android/media/MediaCodec and
 * https://developer.android.com/ndk/reference/group/media for Android's
 * complete and official NDK and SDK MediaCodec API documentation
 */

enum mcw_media_status {
	MCW_MEDIA_STATUS_OK = 0,

	MCW_MEDIA_STATUS_ERROR_BASE = -10000,
	MCW_MEDIA_STATUS_ERROR_UNKNOWN = MCW_MEDIA_STATUS_ERROR_BASE,
	MCW_MEDIA_STATUS_ERROR_MALFORMED = MCW_MEDIA_STATUS_ERROR_BASE - 1,
	MCW_MEDIA_STATUS_ERROR_UNSUPPORTED = MCW_MEDIA_STATUS_ERROR_BASE - 2,
	MCW_MEDIA_STATUS_ERROR_INVALID_OBJECT = MCW_MEDIA_STATUS_ERROR_BASE - 3,
	MCW_MEDIA_STATUS_ERROR_INVALID_PARAMETER =
		MCW_MEDIA_STATUS_ERROR_BASE - 4,
};


#define MCW_BUFFER_FLAG_KEY_FRAME 1
#define MCW_BUFFER_FLAG_CODEC_CONFIG 2
#define MCW_BUFFER_FLAG_END_OF_STREAM 4

#define MCW_CONFIGURE_FLAG_ENCODE 1

#define MCW_INFO_OUTPUT_BUFFERS_CHANGED -3
#define MCW_INFO_OUTPUT_FORMAT_CHANGED -2
#define MCW_INFO_TRY_AGAIN_LATER -1

#define MCW_COLOR_FORMAT_YUV420_PLANAR 0x00000013
#define MCW_COLOR_FORMAT_YUV420_PACKED_PLANAR 0x00000014
#define MCW_COLOR_FORMAT_YUV420_SEMIPLANAR 0x00000015
#define MCW_COLOR_FORMAT_YUV420_PACKED_SEMIPLANAR 0x00000027
#define MCW_COLOR_FORMAT_TI_YUV420PackedSemiPlanar 0x7F000100
#define MCW_COLOR_FORMAT_QCOM_YUV420SemiPlanar 0x7FA30C00
#define MCW_COLOR_FORMAT_QCOM_YUV420PackedSemiPlanar64x32Tile2m8ka 0x7FA30C03
#define MCW_COLOR_FORMAT_QCOM_YUV420SemiPlanar32m 0x7FA30C04

#define VIDEO_SCALING_MODE_SCALE_TO_FIT 0x00000001
#define VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING 0x00000002

/* Forward declarations */
struct mcw_mediaformat;
struct mcw_mediacodec;
struct mcw_mediacrypto;
struct mcw_mediaExtractor;


struct mcw_mediacodec_bufferinfo {
	int32_t offset;
	int32_t size;
	int64_t presentation_time_us;
	uint32_t flags;
};


/* mediacodec-wrapper instance context, containing the function pointers
 * to call the MediaCodec API functions */
struct mcw {
	/* MediaCodec implentation used */
	enum mcw_implementation implem;

	/* MediaFormat API */
	struct {
		//函数指针，指向一个用于创建mcw_mediaformat的函数
		struct mcw_mediaformat *(*nnew)(void);

        struct mcw_mediaformat *(*create_video_format)(const char* mime,int width,int height);

		enum mcw_media_status (*ddelete)(
			struct mcw_mediaformat *format);

		const char *(*to_string)(struct mcw_mediaformat *format);

		bool (*get_int32)(struct mcw_mediaformat *format,
				  const char *name,
				  int32_t *out);

		bool (*get_int64)(struct mcw_mediaformat *format,
				  const char *name,
				  int64_t *out);

		bool (*get_float)(struct mcw_mediaformat *format,
				  const char *name,
				  float *out);

		bool (*get_buffer)(struct mcw_mediaformat *format,
				   const char *name,
				   void **data,
				   size_t *size);

		bool (*get_string)(struct mcw_mediaformat *format,
				   const char *name,
				   const char **out);

		void (*set_int32)(struct mcw_mediaformat *format,
				  const char *name,
				  int32_t value);

		void (*set_int64)(struct mcw_mediaformat *format,
				  const char *name,
				  int64_t value);

		void (*set_float)(struct mcw_mediaformat *format,
				  const char *name,
				  float value);

		void (*set_string)(struct mcw_mediaformat *format,
				   const char *name,
				   const char *value);

		void (*set_buffer)(struct mcw_mediaformat *format,
				   const char *name,
				   void *data,
				   size_t size);

		const char *KEY_AAC_PROFILE;

		const char *KEY_BIT_RATE;

		const char *KEY_CHANNEL_COUNT;

		const char *KEY_CHANNEL_MASK;

		const char *KEY_COLOR_FORMAT;

		const char *KEY_DURATION;

		const char *KEY_FLAC_COMPRESSION_LEVEL;

		const char *KEY_FRAME_RATE;

		const char *KEY_HEIGHT;

		const char *KEY_IS_ADTS;

		const char *KEY_IS_AUTOSELECT;

		const char *KEY_IS_DEFAULT;

		const char *KEY_IS_FORCED_SUBTITLE;

		const char *KEY_I_FRAME_INTERVAL;

		const char *KEY_LANGUAGE;

		const char *KEY_MAX_HEIGHT;

		const char *KEY_MAX_INPUT_SIZE;

		const char *KEY_MAX_WIDTH;

		const char *KEY_MIME;

		const char *KEY_PUSH_BLANK_BUFFERS_ON_STOP;

		const char *KEY_REPEAT_PREVIOUS_FRAME_AFTER;

		const char *KEY_SAMPLE_RATE;

		const char *KEY_WIDTH;

		const char *KEY_STRIDE;
	} mediaformat;

	/* MediaCodec API */
	struct {
		struct mcw_mediacodec *(*create_codec_by_name)(
			const char *name);

		struct mcw_mediacodec *(*create_decoder_by_type)(
			const char *mime_type);

		struct mcw_mediacodec *(*create_encoder_by_type)(
			const char *mime_type);

		enum mcw_media_status (*ddelete)(struct mcw_mediacodec *codec);

		//todo why change to ANativeWindow???
//		enum mcw_media_status (*configure)(
//			struct mcw_mediacodec *codec,
//			const struct mcw_mediaformat *format,
//			ANativeWindow *surface,
//			struct mcw_mediacrypto *crypto,
//			uint32_t flags);

		enum mcw_media_status (*configure)(
				struct mcw_mediacodec *codec,
				const struct mcw_mediaformat *format,
				jobject surface,
				struct mcw_mediacrypto *crypto,
				uint32_t flags);

		enum mcw_media_status (*start)(struct mcw_mediacodec *codec);

		enum mcw_media_status (*stop)(struct mcw_mediacodec *codec);

		enum mcw_media_status (*flush)(struct mcw_mediacodec *codec);

		uint8_t *(*get_input_buffer)(struct mcw_mediacodec *codec,
					     size_t idx,
					     size_t *out_size);

		uint8_t *(*get_output_buffer)(struct mcw_mediacodec *codec,
					      size_t idx,
					      size_t *out_size);

		ssize_t (*dequeue_input_buffer)(struct mcw_mediacodec *codec,
						int64_t timeout_us);

		enum mcw_media_status (*queue_input_buffer)(
			struct mcw_mediacodec *codec,
			size_t idx,
			off_t offset,
			size_t size,
			uint64_t time,
			uint32_t flags);

		struct mcw_mediaformat *(*get_output_format)(
			struct mcw_mediacodec *codec,
			size_t idx);

        ssize_t (*dequeue_output_buffer)(
                struct mcw_mediacodec *codec,
                struct mcw_mediacodec_bufferinfo *info,
                int64_t timeout_us);

		enum mcw_media_status (*release_output_buffer)(
			struct mcw_mediacodec *codec,
			size_t idx,
			bool render);

		enum mcw_media_status (*release_output_buffer_at_time)(
			struct mcw_mediacodec *codec,
			size_t idx,
			int64_t timestamp_ns);

		void (*set_video_scaling_mode)(struct mcw_mediacodec *codec,int mode);
	} mediacodec;

	/*MediaExtractor API*/
	struct{
		struct mcw_mediaExtractor* (*nnew)();
		void (*set_data_source)(mcw_mediaExtractor* extractor,const char* path);
		void (*set_data_source_fd)(mcw_mediaExtractor* extractor,int descriptor, int64_t offset, int64_t length);
		size_t (*get_track_count)(mcw_mediaExtractor* extractor);
        struct mcw_mediaformat* (*get_track_format)(mcw_mediaExtractor* extractor,size_t index);
        void (*select_track)(mcw_mediaExtractor* extractor,size_t index);
		size_t (*read_sample_data)(mcw_mediaExtractor* extractor,uint8_t* buff,size_t buff_size);
        long (*get_sample_time)(mcw_mediaExtractor* extractor);
        bool (*advance)(mcw_mediaExtractor* extractor);
        void (*release)(mcw_mediaExtractor* extractor);
	}mediaExtractor;
};


/**
 * Create a mediacodec-wrapper instance.
 * The implementation to use can be forced using the implem parameter;
 * to let the library decide, use MCW_IMPLEMENTATION_AUTO.
 * To use the JNI implementation, the jvm pointer must be provided.
 * The instance handle is returned through the ret_obj parameter.
 * When no longer needed, the instance must be freed using the
 * mcw_destroy() function.
 * @param jvm: JVM pointer (JavaVM *)
 * @param implem: implementation to use or MCW_IMPLEMENTATION_AUTO
 * @param ret_obj: instance handle (output)
 * @return 0 on success, negative errno value in case of error
 */
MCW_API int
mcw_new(void *jvm, enum mcw_implementation implem, struct mcw **ret_obj);


/**
 * Free a mediacodec-wrapper instance.
 * This function frees all resources associated with an instance.
 * @param self: instance handle
 * @return 0 on success, negative errno value in case of error
 */
MCW_API int mcw_destroy(struct mcw *self);


/**
 * ToString function for enum mcw_implementation.
 * @param val: implementation value to convert
 * @return a string description of the implementation
 */
MCW_API const char *mcw_implementation_str(enum mcw_implementation val);


/**
 * ToString function for color formats.
 * @param format: format value to convert
 * @return a string description of the format
 */
MCW_API const char *mcw_color_format_str(int32_t format);


/**
 * Get the Android SDK version on the current platform.
 * This function calls Java SDK functions through JNI, therefore the jvm
 * pointer must be provided.
 * @param jvm: JVM pointer (JavaVM *)
 * @return the Android SDK version on success, negative errno value
 *         in case of error
 */
MCW_API int mcw_get_sdk_version(void *jvm);


/**
 * Get the codec name for the provided mime type.
 * The codec for a given mime type can be either an encoder or a decoder; use
 * the encoder parameter to choose: if the parameter is not null the encoder
 * name is returned, otherwise the decoder name is returned.
 * This function calls Java SDK functions through JNI, therefore the jvm
 * pointer must be provided.
 * The returned string is allocated by the function and must be freed by the
 * application once no longer needed.
 * @param jvm: JVM pointer (JavaVM *)
 * @param mime_type: mime type
 * @param encoder: if not null return the codec name for an encoder,
 *                 otherwise for a decoder
 * @return a string pointer on success, NULL in case of error
 */
MCW_API char *mcw_get_codec_name(void *jvm, const char *mime_type, int encoder);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !_LIBMCW_H_ */
