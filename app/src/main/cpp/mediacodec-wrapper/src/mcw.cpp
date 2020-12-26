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

#include <malloc.h>
#include "mcw.h"
#include "mcw_jni.h"
#include "mcw_ndk.h"

int mcw_new(void *jvm, enum mcw_implementation implem, struct mcw **ret_obj)
{
	int ret = 0;

	LOGE("mcw_new");
	struct mcw *self = (struct mcw*)calloc(1, sizeof(*self));
	if (self == NULL) {
		ret = -ENOMEM;
		LOGE("calloc", -ret);
		return ret;
	}

	switch (implem) {
	default:
	case MCW_IMPLEMENTATION_AUTO:
		ret = mcw_ndk_init(self,jvm);
		if (ret < 0) {
			LOGI("mcw_ndk_init() failed: %d(%s)",
			      ret,
			      strerror(-ret));
			ret = mcw_jni_init(self, jvm);
			if (ret < 0) {
				LOGE("mcw_jni_init() failed: %d(%s)",
				      ret,
				      strerror(-ret));
				goto error;
			} else
				LOGI("using JNI implementation");
		} else
			LOGI("using NDK implementation");
		break;
	case MCW_IMPLEMENTATION_NDK:
		LOGE("MCW_IMPLEMENTATION_NDK");
		ret = mcw_ndk_init(self,jvm);
		if (ret < 0) {
			LOGE("mcw_ndk_init() failed: %d(%s)",
			      ret,
			      strerror(-ret));
			goto error;
		} else
			LOGI("using NDK implementation");
		break;
	case MCW_IMPLEMENTATION_JNI:
        LOGI("[mcw_new] mcw_jni_init");
		ret = mcw_jni_init(self, jvm);
		if (ret < 0) {
            LOGE("[mcw_new] mcw_jni_init() failed: %d(%s)",
			      ret,
			      strerror(-ret));
			goto error;
		} else
			LOGI("[mcw_new] using JNI implementation");
		break;
	}

	*ret_obj = self;
	return 0;

error:
    LOGI("[mcw_new] error");
	mcw_destroy(self);
	*ret_obj = NULL;
	return ret;
}


int mcw_destroy(struct mcw *self)
{
	if (self == NULL)
		return 0;

	free(self);

	return 0;
}


const char *mcw_implementation_str(enum mcw_implementation val)
{
	switch (val) {
	case MCW_IMPLEMENTATION_NDK:
		return "NDK";
	case MCW_IMPLEMENTATION_JNI:
		return "JNI";
	default:
		return "UNKNOWN";
	}
}


const char *mcw_color_format_str(int32_t format)
{
	switch (format) {
	case MCW_COLOR_FORMAT_YUV420_PLANAR:
		return "FormatYUV420Planar";
	case MCW_COLOR_FORMAT_YUV420_PACKED_PLANAR:
		return "FormatYUV420PackedPlanar";
	case MCW_COLOR_FORMAT_YUV420_SEMIPLANAR:
		return "FormatYUV420SemiPlanar";
	case MCW_COLOR_FORMAT_YUV420_PACKED_SEMIPLANAR:
		return "FormatYUV420PackedSemiPlanar";
	case MCW_COLOR_FORMAT_TI_YUV420PackedSemiPlanar:
		return "TI_YUV420PackedSemiPlanar";
	case MCW_COLOR_FORMAT_QCOM_YUV420SemiPlanar:
		return "QCOM_YUV420SemiPlanar";
	case MCW_COLOR_FORMAT_QCOM_YUV420PackedSemiPlanar64x32Tile2m8ka:
		return "QCOM_YUV420PackedSemiPlanar64x32Tile2m8ka";
	case MCW_COLOR_FORMAT_QCOM_YUV420SemiPlanar32m:
		return "QCOM_YUV420SemiPlanar32m";
	default:
		return "UNKNOWN";
	}
}
