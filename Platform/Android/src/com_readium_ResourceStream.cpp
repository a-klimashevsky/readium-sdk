#import <ePub3/archive.h>
#import <ePub3/container.h>
#include <ePub3/package.h>
#include "com_readium_ResourceStream.h"
#include <android/log.h>

#define APPNAME "READIUM"

/*
 * Class:     com_readium_ResourceStream
 * Method:    readNative
 * Signature: ([BII)I
 */
JNIEXPORT jint JNICALL Java_com_readium_ResourceStream_readNative(JNIEnv *env,
		jobject thiz, jlong jNativePointer, jbyteArray jBuffer, jint jOffset,
		jint jLength) {
	if(jNativePointer == 0) {
		__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "jNativePointer == 0");
		return 0;
	}
	try {
		ePub3::ArchiveReader *reader = (ePub3::ArchiveReader *) jNativePointer;
		uint8_t *buffer = new uint8_t[jLength];
		ssize_t count = reader->read(buffer, jLength);
		env->SetByteArrayRegion(jBuffer, 0, jLength, buffer); // copy
		delete buffer;
		return count;
	} catch(...){

	}
	return 0;
}

/*
 * Class:     com_readium_ResourceStream
 * Method:    bytesLeft
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_readium_ResourceStream_bytesLeft
  (JNIEnv *env, jobject thiz, jlong jNativePointer){
	ePub3::ArchiveReader *reader = (ePub3::ArchiveReader *) jNativePointer;
	return (jint)reader->bytesLeft();
}

/*
 * Class:     com_readium_ResourceStream
 * Method:    freeResourceStream
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_ResourceStream_freeResourceStream
(JNIEnv *env, jobject thiz, jlong jNativePointer) {
    ePub3::ArchiveReader *reader = (ePub3::ArchiveReader *) jNativePointer;
    delete reader;
}

