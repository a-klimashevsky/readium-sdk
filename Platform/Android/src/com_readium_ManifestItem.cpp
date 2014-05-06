#import <ePub3/archive.h>
#import <ePub3/container.h>
#include <ePub3/package.h>
#include "com_readium_ManifestItem.h"
#include "general.h"
#include <android/log.h>

#define APPNAME "READIUM"

/*
 * Class:     com_readium_ManifestItem
 * Method:    init
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_ManifestItem_init
  (JNIEnv *env, jobject thiz, jlong jNativePointer){
	ePub3::ManifestItem *item = (ePub3::ManifestItem *)jNativePointer;
	setStringProperty(env,thiz,"href",item->Href().c_str());
	setStringProperty(env,thiz,"mediaType",item->MediaType().c_str());
}

