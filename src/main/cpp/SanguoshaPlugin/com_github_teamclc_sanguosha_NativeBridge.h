/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_github_teamclc_sanguosha_NativeBridge */

#ifndef _Included_com_github_teamclc_sanguosha_NativeBridge
#define _Included_com_github_teamclc_sanguosha_NativeBridge
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_github_teamclc_sanguosha_NativeBridge
 * Method:    log
 * Signature: (Ljava/lang/String;ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_github_teamclc_sanguosha_NativeBridge_log
  (JNIEnv *, jclass, jstring, jint, jstring);

/*
 * Class:     com_github_teamclc_sanguosha_NativeBridge
 * Method:    sendGroupMessage
 * Signature: (Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_com_github_teamclc_sanguosha_NativeBridge_sendGroupMessage
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     com_github_teamclc_sanguosha_NativeBridge
 * Method:    sendPrivateMesaage
 * Signature: (Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_com_github_teamclc_sanguosha_NativeBridge_sendPrivateMesaage
  (JNIEnv *, jclass, jstring, jlong);

#ifdef __cplusplus
}
#endif
#endif