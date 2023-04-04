#include <jni.h>
#include "hash-library/headers/sha256.h"
#include "hash-library/headers/md5.h"
#include <string>
#include "iostream"
#include "android/log.h"

#define LOG_TAG "EASY"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,     LOG_TAG, __VA_ARGS__)

class Student {
private:
    char *name;

public:
    char *sayHi() {
        char result[1024];
        memset(result, 0, sizeof(result));
        strcpy(result, "Hello, ");
        strcat(result, name);
        return result;
    }

    Student(char *name) {
        LOGI("调用构造函数");
        this->name = name;
    };

    ~Student() {
        LOGI("调用析构函数");
    };
};


extern "C" JNIEXPORT jstring JNICALL
get_name_from_jni(
        JNIEnv *env,
        jobject /* this */) {
    MD5 md5;
    std::string hello = "Hello from C++";
    jstring result = env->NewStringUTF(md5(hello).c_str());
    return result;
}
extern "C"
JNIEXPORT void JNICALL
update_name(JNIEnv *env, jobject thiz) {
    jclass mainCls = env->GetObjectClass(thiz);
    // jfieldID GetFieldID(jclass clazz, const char* name, const char* sig)
    jfieldID nameField = env->GetFieldID(mainCls, "name", "Ljava/lang/String;");
    Student *student = new Student("Dougie");
    // void SetObjectField(jobject obj, jfieldID fieldID, jobject value)
    env->SetObjectField(thiz, nameField, env->NewStringUTF(student->sayHi()));
    delete student;
}

static JNINativeMethod methods[] = {
        {"getStringFromJNI", "()Ljava/lang/String;", (void *) get_name_from_jni},
        {"updateName",    "()V",                  (void *) update_name}
};

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    const char *className = "com/easy/jni_sample/MainActivity";
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        return -1;
    }
    if (env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        return -1;
    }
    return JNI_VERSION_1_6;
};
