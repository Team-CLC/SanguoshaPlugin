#pragma once

#include <jni.h>
#include <jni_md.h>
#include <windows.h>
#include <string>
#include <cstring>
#include <cstdlib>

typedef jint(JNICALL *JNICREATEPROC)(JavaVM **, void **, void *);
HRESULT startJVM(std::string pluginPath);
HRESULT destroyJVM();

JNIEnv* allocJNIEnv();
void freeJNIEnvInThisThread();
BOOL isJVMStarted();

#define CP_GB2312 936
LPSTR jstringToLPSTR(JNIEnv *env, jstring str);
jstring LPCSTRTojstring(JNIEnv* env, LPCSTR str);
