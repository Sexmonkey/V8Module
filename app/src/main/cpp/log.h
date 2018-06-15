//
// Created by JerryZhu on 2018/5/18.
//
#include <android/log.h>

#ifndef V8PP_TEST
#define V8PP_TEST

#define  LOG_TAG    "V8PP_TEST"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


#endif //V8PP_TEST

