/*
* CoolQ Demo for VC++ 
* Api Version 9
* Written by Coxxs & Thanks for the help of orzFly
*/

#include "stdafx.h"
#include "string"
#include "cqp.h"
#include "appmain.h" //Ӧ��AppID����Ϣ������ȷ��д�������Q�����޷�����
#include <jni.h>
#include "jnihelper.h"
#include "com_github_teamclc_sanguosha_NativeBridge.h"

using namespace std;

int ac = -1; //AuthCode ���ÿ�Q�ķ���ʱ��Ҫ�õ�
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
* Type=1003 Ӧ���ѱ�����
* ��Ӧ�ñ����ú󣬽��յ����¼���
* �����Q����ʱӦ���ѱ����ã�����_eventStartup(Type=1001,��Q����)�����ú󣬱�����Ҳ��������һ�Ρ�
* ��Ǳ�Ҫ����������������ش��ڡ���������Ӳ˵������û��ֶ��򿪴��ڣ�
*/
CQEVENT(int32_t, __eventEnable, 0)() {
	enabled = true;

	JNIEnv *env = nullptr;
	if (!isJVMStarted()) {
		auto ret = startJVM(CQ_getAppDirectory(ac));
		if (ret != S_OK) {
			// �Ա�
			LPSTR msg = new char[512];
			sprintf(msg, "����ɱ���������Java�����ʧ�ܣ���ȷ��ϵͳ����Java Runtime Enviroment 8�����ϰ汾���������%#010x", (int)ret);
			CQ_setFatal(ac, msg);
		}
		env = allocJNIEnv();
		if (env == nullptr) {
			CQ_setFatal(ac, "����ɱ������޷���ȡJNIEnv����������Java��װ�Ƿ�����Java�Ƿ��𻵣�");
		}
		initalizeJNI(env);
	}
	else env = allocJNIEnv();
		
	env->CallStaticVoidMethod(classBridge, methodInit);
	return 0;
}


/*
* Type=1004 Ӧ�ý���ͣ��
* ��Ӧ�ñ�ͣ��ǰ�����յ����¼���
* �����Q����ʱӦ���ѱ�ͣ�ã��򱾺���*����*�����á�
* ���۱�Ӧ���Ƿ����ã���Q�ر�ǰ��������*����*�����á�
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	enabled = false;

	WITH_JNIENV_BEGIN
		env->CallStaticVoidMethod(classBridge, methodDestroy);
	WITH_JNIENV_END
	return 0;
}


/*
* Type=2 Ⱥ��Ϣ
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
* Type=301 ����-�������
* msg ����
* responseFlag ������ʶ(����������)
*/
CQEVENT(int32_t, __eventRequest_AddFriend, 24)(int32_t subType, int32_t sendTime, int64_t fromQQ, const char *msg, const char *responseFlag) {

	CQ_setFriendAddRequest(ac, responseFlag, REQUEST_ALLOW, "");
	return EVENT_BLOCK;
}

/*
* �˵������� .json �ļ������ò˵���Ŀ��������
* �����ʹ�ò˵������� .json ���˴�ɾ�����ò˵�
*/
CQEVENT(int32_t, __menuSettings, 0)() {
	WITH_JNIENV_BEGIN
		env->CallStaticVoidMethod(classBridge, methodOpenSetting);
	WITH_JNIENV_END
	return 0;
}

CQEVENT(int32_t, __menuSetJavaPath, 0)() {
	MessageBox(nullptr, L"���ڿ�Q��װĿ¼��app\\com.github.teamclc.sanguosha�ļ������½��ļ�javahome.txt����������Ϊһ��32λJava��jvm.dllλ�ã�����C:\\Program Files (x86)\\Java\\jre1.8.0_161\\bin\\client����server��\\jvm.dll������Ҫ�Զ�������ɾ�����ļ����ɡ�ע�����ڲ���Ѿ��������������Ҫ����Java��װ��������Q��", L"ָ������", MB_ICONINFORMATION);
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

