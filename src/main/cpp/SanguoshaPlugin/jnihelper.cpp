#include "stdafx.h"
#include "jnihelper.h"

HMODULE jmodule = nullptr;
JavaVM *vm = nullptr;

#define MAX_PATH_SIZE 512
HRESULT tryLoadJava(std::wstring path, std::atring jarPath)
{
	JavaVMInitArgs startupArgs;
	JavaVMOption options[3];
	options[0].optionString = "-Djava.compiler=NONE";
	options[1].optionString = new char[MAX_PATH_SIZE];
	strcpy(options[1].optionString, jarPath.c_str());
	options[2].optionString = "-verbose:NONE";

	startupArgs.version = JNI_VERSION_1_8;
	startupArgs.nOptions = 3;
	startupArgs.options = options;
	startupArgs.ignoreUnrecognized = JNI_TRUE;

	jmodule = LoadLibrary(path.c_str());
	if (jmodule == nullptr) return HRESULT_FROM_WIN32(GetLastError());
	
	auto JNICreateJavaVM = reinterpret_cast<JNICREATEPROC>(GetProcAddress(jmodule, "JNI_CreateJavaVM"));
	if (JNICreateJavaVM == nullptr) {
		FreeLibrary(jmodule);
		return HRESULT_FROM_WIN32(GetLastError());
	}

	JNIEnv *env;
	jint retv = JNICreateJavaVM(&vm, reinterpret_cast<void**>(&env), &startupArgs);
	if (retv != JNI_OK) {
		FreeLibrary(jmodule);
		return retv;
	}
	return S_OK;
}

std::wstring getJVMModulePath1(LPWSTR javaHome) {
	std::wstring path = javaHome;
	wchar_t lastChar = *(path.end() - 1);
	if (lastChar != '\\' && lastChar != '/')
		path += '\\';
	path += L"bin\\server\\jvm.dll";
	return path;
}

std::wstring getJVMModulePath2(LPWSTR javaHome) {
	std::wstring path = javaHome;
	wchar_t lastChar = *(path.end() - 1);
	if (lastChar != '\\' && lastChar != '/')
		path += '\\';
	path += L"jre\\bin\\server\\jvm.dll";
	return path;
}

HRESULT tryLoadJavaFromJavaHome(std::string jarPath) {
	LPWSTR javaHome = new wchar_t[MAX_PATH_SIZE];
	DWORD envCode = GetEnvironmentVariable(L"JAVA_HOME", javaHome, MAX_PATH_SIZE);
	if (envCode <= 0) return E_FAIL;
	auto path = getJVMModulePath1(javaHome);
	
	auto res = tryLoadJava(path, jarPath);
	if (res == S_OK) return res;

	path = getJVMModulePath2(javaHome);
	return tryLoadJava(path, jarPath);
}

HRESLT tryLoadJavaFromRegistry(std::string jarPath) {
	HKEY rootKey;
	LSTATUS rest;
	rest = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\JavaSoft\\Java Runtime Environment", 0, KEY_READ, &rootKey);
	if (rest != ERROR_SUCCESS) {
		return HRESULT_FROM_WIN32(rest);
	}

	LONG cbLength = 0;
	rest = RegQueryValue(rootKey, L"CurrentVersion", nullptr, &cbLength);
	if (rest != ERROR_SUCCESS) {
		RegCloseKey(rootKey);
		return HRESULT_FROM_WIN32(rest);
	}
	LPWSTR version = new wchar_t[cbLength];
	rest = RegQueryValue(rootKey, L"CurrentVersion", version, &cbLength);
	if (rest != ERROR_SUCCESS) {
		RegCloseKey(rootKey);
		return HRESULT_FROM_WIN32(rest);
	}

	HKEY versionKey;
	rest = RegOpenKeyEx(rootKey, version, 0, KEY_READ, &versionKey);
	if (rest != ERROR_SUCCESS) {
		RegCloseKey(rootKey);
		return HRESULT_FROM_WIN32(rest);
	}
	rest = RegQueryValue(versionKey, L"JavaHome", nullptr, &cbLength);
	LPWSTR javaHome = new wchar_t[cbLength];
	if (rest != ERROR_SUCCESS) {
		RegCloseKey(versionKey);
		RegCloseKey(rootKey);
		return HRESULT_FROM_WIN32(rest);
	}
	rest = RegQueryValue(versionKey, L"JavaHome", javaHome, &cbLength);
	if (rest != ERROR_SUCCESS) {
		RegCloseKey(versionKey);
		RegCloseKey(rootKey);
		return HRESULT_FROM_WIN32(rest);
	}

	return tryLoadJava(getJVMModulePath1(javaHome), jarPath);
}

HRESULT startJVM(std::string pluginPath)
{
	if (vm != nullptr)
		return S_OK; // OK to call for many times!

	auto jarPath = "-Djava.class.path=.;" + pluginPath + "sgsplugin.jar";
	if (tryLoadJavaFromJavaHome(jarPath) == S_OK)
		return S_OK;
	return tryLoadJavaFromRegistry(jarPath);
}

HRESULT destroyJVM()
{
	if (vm == nullptr)
		return S_OK; // OK to call for many times!

	vm->DestroyJavaVM();
	vm = nullptr;

	if (jmodule != nullptr)
		FreeModule(jmodule);
	return S_OK;
}

JNIEnv * allocJNIEnv()
{
	if (vm == nullptr)
		return nullptr;
	JNIEnv* env = nullptr;
	vm->AttachCurrentThread((void**)(&env), nullptr);
	return env;
}

BOOL isJVMStarted()
{
	return vm != nullptr;
}

void freeJNIEnvInThisThread()
{
	if (vm == nullptr)
		return;
	vm->DetachCurrentThread();
}

LPSTR jstringToLPSTR(JNIEnv *env, jstring str) {
	int length = (env)->GetStringLength(str);
	const jchar* jcstr = (env)->GetStringChars(str, 0);
	LPSTR rtn = new char[length * 2 + 1];
	int size = 0;
	size = WideCharToMultiByte(CP_GB2312, 0, (LPCWSTR)jcstr, length, rtn,
		(length * 2 + 1), NULL, NULL);
	if (size <= 0)
		return NULL;
	(env)->ReleaseStringChars(str, jcstr);
	rtn[size] = 0;
	return rtn;
}

jstring LPCSTRTojstring(JNIEnv * env, LPCSTR str)
{
	jstring rtn = 0;
	int slen = strlen(str);
	jchar * buffer = 0;
	if (slen == 0)
		rtn = env->NewStringUTF(str);
	else {
		int length = MultiByteToWideChar(CP_GB2312, 0, str, slen, NULL, 0);
		buffer = new jchar[length * 2 + 1];
		if (MultiByteToWideChar(CP_GB2312, 0, str, slen, (LPWSTR)buffer, length) > 0)
			rtn = env->NewString(buffer, length);
		delete[] buffer;
	}
	return rtn;
}
