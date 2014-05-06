#import <ePub3/archive.h>
#import <ePub3/container.h>
#include <ePub3/package.h>
#include "com_readium_EPubSpinelItem.h"
#include <android/log.h>

#define APPNAME "READIUM"

/*
 * Class:     com_readium_EPubSpinelItem
 * Method:    freeSpinelItem
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_EPubSpinelItem_initSpinelItem
(JNIEnv *env, jobject thiz, jlong jNativePointer) {

	ePub3::SpineItem *spineItem = (ePub3::SpineItem *) (jNativePointer);

	setStringProperty(env,thiz,"idref",spineItem->Idref().c_str());

	ePub3::ManifestItem *manifestItem = spineItem->ManifestItem();

	if (manifestItem != NULL) {
		const ePub3::string s = manifestItem->BaseHref();
		setStringProperty(env,thiz,"baseHref",s.c_str());
	}
}

/*
 * Class:     com_readium_EPubSpinelItem
 * Method:    freeSpinelItem
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_EPubSpinelItem_freeSpinelItem
  (JNIEnv *env, jobject thiz, jlong jNativePointer){
	delete (ePub3::SpineItem *) jNativePointer;
}

