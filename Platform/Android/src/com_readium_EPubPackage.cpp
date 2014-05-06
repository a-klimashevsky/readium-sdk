#import <ePub3/archive.h>
#import <ePub3/container.h>
#include <ePub3/package.h>
#include "com_readium_EPubPackage.h"
#include "general.h"
#include <android/log.h>

#define APPNAME "READIUM"

/*
 * Class:     com_readium_EPubPackage
 * Method:    initWithPackage
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_EPubPackage_initWithPackage
(JNIEnv *env, jobject thiz, jlong jNativePointer) {
	ePub3::Package *package = (ePub3::Package *) jNativePointer;

	setStringProperty(env,thiz,"authors",package->Authors().c_str());

	setStringProperty(env,thiz,"basePath",package->BasePath().c_str());

	setStringProperty(env,thiz,"fullTitle",package->FullTitle().c_str());

	setStringProperty(env,thiz,"isbn",package->ISBN().c_str());

	setStringProperty(env,thiz,"packageID",package->PackageID().c_str());

	setStringProperty(env,thiz,"title",package->Title().c_str());

	setStringProperty(env,thiz,"subtitle",package->Subtitle().c_str());

}

/*
 * Class:     com_readium_EPubPackage
 * Method:    getSpinItems
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_readium_EPubPackage_getSpinItems(
		JNIEnv *env, jobject thiz, jlong jNativePointer) {
	ePub3::Package *package = (ePub3::Package *) jNativePointer;

	const ePub3::SpineItem *firstSpineItem = package->FirstSpineItem();

	int count = (firstSpineItem == NULL) ? 0 : firstSpineItem->Count();
	jlong *pointers = new jlong[count];

	for (int i = 0; i < count; i++) {
		const ePub3::SpineItem *spineItem = package->SpineItemAt(i);
		pointers[i] = (jlong) spineItem;
	}
	jlongArray result = env->NewLongArray(count);
	env->SetLongArrayRegion(result, 0, count, pointers); // copy
	return result;
}

/*
 * Class:     com_readium_EPubPackage
 * Method:    getManifestItems
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_readium_EPubPackage_getManifestItems(
		JNIEnv *env, jobject thiz, jlong jNativePointer) {
	ePub3::Package *package = (ePub3::Package *) jNativePointer;
	ePub3::ManifestTable manifest = package->Manifest();
	int count = manifest.size();
	jlong *pointers = new jlong[count];
	int index = 0;
	for (auto i = manifest.begin(); i != manifest.end(); i++) {
		ePub3::ManifestItem *item = i->second;
		pointers[index++] = (jlong) item;
	}
	jlongArray result = env->NewLongArray(count);
	env->SetLongArrayRegion(result, 0, count, pointers); // copy
	return result;
}

/*
 * Class:     com_readium_EPubPackage
 * Method:    freePackage
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_EPubPackage_freePackage
(JNIEnv *env, jobject thiz, jlong jNativePointer) {
	delete (ePub3::Package *) jNativePointer;
}

/*
 * Class:     com_readium_EPubPackage
 * Method:    getArchiveReader
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_readium_EPubPackage_getArchiveReader(
		JNIEnv *env, jobject thiz, jlong jNativePointer, jstring jBaseHref) {

	char *baseHref = env->GetStringUTFChars(jBaseHref, NULL);
	ePub3::Package *package = (ePub3::Package *) jNativePointer;
	return (jlong) package->ReaderForRelativePath(baseHref);
}

/*
 * Class:     com_readium_EPubPackage
 * Method:    getTableOfContents
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_readium_EPubPackage_getTableOfContents(
		JNIEnv *env, jobject thiz, jlong jNativePointer) {
	ePub3::Package *package = (ePub3::Package *) jNativePointer;
	return (jlong) package->TableOfContents();
}

/*
 * Class:     com_readium_EPubPackage
 * Method:    getMetadata
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_readium_EPubPackage_getMetadata
  (JNIEnv *env, jobject thiz, jlong jNativePointer){
	ePub3::Package *package = (ePub3::Package *) jNativePointer;
	int count = package->Metadata().size();
	jlong *pointers = new jlong[count];
	int index = 0;
	for (auto i = package->Metadata().begin(); i != package->Metadata().end(); i++) {
		ePub3::Metadata *item = (ePub3::Metadata *)(*i);
		pointers[index++] = (jlong) item;
	}
	jlongArray result = env->NewLongArray(count);
	env->SetLongArrayRegion(result, 0, count, pointers); // copy
	return result;
}

void setStringProperty(JNIEnv *env, jobject thiz, char* fieldName,
		char* value) {
	jclass thisClass = env->GetObjectClass(thiz);
	jfieldID fid = env->GetFieldID(thisClass, fieldName, "Ljava/lang/String;");
	if (NULL == fid)
		return;
	jstring jvalue = env->NewStringUTF(value);
	if (NULL == jvalue)
		return;
	env->SetObjectField(thiz, fid, jvalue);
}

