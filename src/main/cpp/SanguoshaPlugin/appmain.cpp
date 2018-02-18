/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //应用AppID等信息，请正确填写，否则酷Q可能无法加载
#include <jni.h>
#include "jnihelper.h"
#include "com_github_teamclc_sanguosha_NativeBridge.h"

using namespace std;

int ac = -1; //AuthCode 调用酷Q的方法时需要用到
bool enabled = false;

jclass classBridge;
jmethodID methodInit;
jmethodID methodDestroy;
jmethodID methodGroupMessageReceived;
jmethodID methodOpenSetting;

void initalizeJNI(JNIEnv * env);

#define WITH_JNIENV_BEGIN if (isJVMStarted()) { \
JNIEnv *env = allocJNIEnv();
#define WITH_JNIENV_END freeJNIEnvInThisThread(); \
}

CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}

CQEVENT(int32_t, __eventExit, 0)() {
	destroyJVM();
	return 0;
}

/*
* Type=1003 应用已被启用
* 当应用被启用后，将收到此事件。
* 如果酷Q载入时应用已被启用，则在_eventStartup(Type=1001,酷Q启动)被调用后，本函数也将被调用一次。
* 如非必要，不建议在这里加载窗口。（可以添加菜单，让用户手动打开窗口）
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;

	JNIEnv *env = nullptr;
	if (!isJVMStarted()) {
		auto ret = startJVM(CQ_getAppDirectory(ac));
		if (ret != S_OK) {
			// 自爆
			LPSTR msg = new char[512];
			sprintf(msg, "三国杀插件：加载Java虚拟机失败！请确保系统中有Java Runtime Enviroment 8或以上版本！错误代码%#010x", (int)ret);
			CQ_setFatal(ac, msg);
		}
		env = allocJNIEnv();
		if (env == nullptr) {
			CQ_setFatal(ac, "三国杀插件：无法获取JNIEnv！请检查您的Java安装是否有误！Java是否损坏！");
		}
		initalizeJNI(env);
	}
	else env = allocJNIEnv();
		
	env->CallStaticVoidMethod(classBridge, methodInit);
	return 0;
}


/*
* Type=1004 应用将被停用
* 当应用被停用前，将收到此事件。
* 如果酷Q载入时应用已被停用，则本函数*不会*被调用。
* 无论本应用是否被启用，酷Q关闭前本函数都*不会*被调用。
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;

	WITH_JNIENV_BEGIN
		env->CallStaticVoidMethod(classBridge, methodDestroy);
	WITH_JNIENV_END
	return 0;
}


/*
* Type=2 群消息
*/
CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
	jboolean b = JNI_FALSE;
	WITH_JNIENV_BEGIN
		b = env->CallStaticBooleanMethod(classBridge, methodGroupMessageReceived, LPCSTRTojstring(env, msg), subType, msgId, fromQQ, fromGroup);
	WITH_JNIENV_END
	if (b == JNI_TRUE)
		return EVENT_BLOCK;
	return EVENT_IGNORE;
}


/*
* Type=301 请求-好友添加
* msg 附言
* responseFlag 反馈标识(处理请求用)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");
	return EVENT_BLOCK;
}

/*
* 菜单，可在 .json 文件中设置菜单数目、函数名
* 如果不使用菜单，请在 .json 及此处删除无用菜单
*/
CQEVENT(int32_t, __menuSettings, 0)() {
	WITH_JNIENV_BEGIN
		env->CallStaticVoidMethod(classBridge, methodOpenSetting);
	WITH_JNIENV_END
	return 0;
}

CQEVENT(int32_t, __menuSetJavaPath, 0)() {
	MessageBox(nullptr, L"请在酷Q安装目录的app\\com.github.teamclc.sanguosha文件夹下新建文件javahome.txt，其中内容为一个32位Java的jvm.dll位置，例如C:\\Program Files (x86)\\Java\\jre1.8.0_161\\bin\\client（或server）\\jvm.dll，若需要自动查找则删除该文件即可。注意若在插件已经启动的情况下需要更换Java安装须重启酷Q。", L"指定方法", MB_ICONINFORMATION);
	return 0;
}

void initalizeJNI(JNIEnv * env) {
	classBridge = (jclass) (env->NewGlobalRef(env->FindClass("com/github/teamclc/sanguosha/NativeBridge")));
	methodInit = env->GetStaticMethodID(classBridge, "init", "()V");
	methodDestroy = env->GetStaticMethodID(classBridge, "destroy", "()V");
	methodGroupMessageReceived = env->GetStaticMethodID(classBridge, "groupMessageReceived", "(Ljava/lang/String;IIJJ)Z");
	methodOpenSetting = env->GetStaticMethodID(classBridge, "openSetting", "()V");

	const JNINativeMethod methods[] = { 
		{ "log", "(Ljava/lang/String;ILjava/lang/String;)V", (void*)&Java_com_github_teamclc_sanguosha_NativeBridge_log },
		{ "sendGroupMessage", "(Ljava/lang/String;J)I", (void*)&Java_com_github_teamclc_sanguosha_NativeBridge_sendGroupMessage },
		{ "sendPrivateMessage", "(Ljava/lang/String;J)I", (void*)&Java_com_github_teamclc_sanguosha_NativeBridge_sendPrivateMessage }
	};
	env->RegisterNatives(classBridge, methods, 3);
}

extern "C" JNIEXPORT void JNICALL Java_com_github_teamclc_sanguosha_NativeBridge_log
(JNIEnv * env, jclass klass, jstring message, jint priority, jstring category) {
	LPCSTR msg = jstringToLPSTR(env, message);
	LPCSTR cat = jstringToLPSTR(env, category);
	CQ_addLog(ac, priority, cat, msg);
	delete[] msg, cat;
}

extern "C" JNIEXPORT jint JNICALL Java_com_github_teamclc_sanguosha_NativeBridge_sendGroupMessage
(JNIEnv * env, jclass klass, jstring message, jlong toGroup) {
	LPCSTR msg = jstringToLPSTR(env, message);
	int ret = CQ_sendGroupMsg(ac, toGroup, msg);
	delete[] msg;
	return ret;
}

extern "C" JNIEXPORT jint JNICALL Java_com_github_teamclc_sanguosha_NativeBridge_sendPrivateMessage
(JNIEnv * env, jclass klass, jstring message, jlong toQQ) {
	LPCSTR msg = jstringToLPSTR(env, message);
	int ret = CQ_sendPrivateMsg(ac, toQQ, msg);
	delete[] msg;
	return ret;
}

