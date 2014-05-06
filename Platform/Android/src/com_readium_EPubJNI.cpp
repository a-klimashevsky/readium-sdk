#import <ePub3/archive.h>
#import <ePub3/container.h>
#include <ePub3/package.h>
#include "com_readium_EPubJNI.h"
#include "general.h"
#include <android/log.h>
#include <boost/exception/info.hpp>

#define APPNAME "READIUM"

/*
 * Class:     com_readium_EPubJNI
 * Method:    openBook
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_readium_EPubJNI_openBook(JNIEnv *env,
		jobject thiz, jstring jPath) {
	char *path = env->GetStringUTFChars(jPath, NULL);

	ePub3::Container *c = new ePub3::Container(path);
	bool isEncrypted = c->IsContainerEncrypted();
	setBooleanProperty(env, thiz, "isEncrypted", isEncrypted);
	if(c->IsContainerEncrypted()){
		const ePub3::EncryptionKeyInfo *info = c->KeyInfo();

		const ePub3::string cipher = info->Cipher();
		setStringProperty(env, thiz, "cipher", cipher.c_str());
	}

	return (jlong)(new ePub3::Container(path));
}

/*
 * Class:     com_readium_EPubJNI
 * Method:    isPathEncrypted
 * Signature: (JLjava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_readium_EPubJNI_isPathEncrypted
  (JNIEnv *env, jobject thiz, jlong jNativePointer, jstring jPath){
	char *path = env->GetStringUTFChars(jPath, NULL);
	ePub3::Container *container = (ePub3::Container *) jNativePointer;
	return (jboolean) container->IsPathEncrypted(path);
}

/*
 * Class:     com_readium_EPubJNI
 * Method:    getKeyIV
 * Signature: (JLjava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_readium_EPubJNI_getKeyIV
  (JNIEnv *env, jobject thiz, jlong jNativePointer, jstring jPath){
	char *path = env->GetStringUTFChars(jPath, NULL);
	ePub3::Container *container = (ePub3::Container *) jNativePointer;
	const ePub3::EncryptionInfo *info = container->EncryptionInfoForPath(path);
	const ePub3::string vector = info->KeyIV();
	jstring jstrResult = env->NewStringUTF(vector.c_str());
	return jstrResult;
}

/*
 * Class:     com_readium_EPubJNI
 * Method:    closeBook
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_EPubJNI_closeBook
(JNIEnv *env, jobject thiz, jlong jNativePointer) {
	delete (ePub3::Container *)jNativePointer;
}

/*
 * Class:     com_readium_EPubJNI
 * Method:    getPackages
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_readium_EPubJNI_getPackages(JNIEnv *env,
		jobject thiz, jlong jNativePointer) {
	ePub3::Container * container = (ePub3::Container *) jNativePointer;
	ePub3::Container::PackageList vec = container->Packages();
	jlong *pointers = new jlong[vec.size()];
	int i = 0;
	for (auto it = vec.begin(); it != vec.end(); it++) {
		pointers[i++] = (jlong)(*it);
	}
	jlongArray result = env->NewLongArray((jsize) vec.size());
	env->SetLongArrayRegion(result, 0, vec.size(), pointers); // copy
	return result;
}

/*
 * Class:     com_readium_EPubJNI
 * Method:    staticInitialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_readium_EPubJNI_staticInitialize
(JNIEnv *, jclass) {
	ePub3::Archive::Initialize();
}

void setBooleanProperty(JNIEnv *env, jobject thiz, char* fieldName,
		bool value) {
	jclass thisClass = env->GetObjectClass(thiz);
	jfieldID fid = env->GetFieldID(thisClass, fieldName, "Z");
	if (NULL == fid)
		return;

	env->SetBooleanField(thiz, fid, (jboolean) value);
}
