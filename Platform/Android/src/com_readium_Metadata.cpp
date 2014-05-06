#import <ePub3/archive.h>
#import <ePub3/container.h>
#include <ePub3/package.h>
#import <ePub3/metadata.h>
#include "com_readium_Metadata.h"
#include <android/log.h>
#include "general.h"

#define APPNAME "READIUM"

/*
 * Class:     com_readium_NavigationElement
 * Method:    initNavigationElement
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_Metadata_initMetadata
(JNIEnv *env, jobject thiz, jlong jNativePointer) {
	if(jNativePointer != 0) {

		ePub3::Metadata *element = (ePub3::Metadata *) jNativePointer;

		setStringProperty(env,thiz,"value",element->Value().c_str());
		setStringProperty(env,thiz,"identifier",element->Property().IRIString().c_str());
	}
}

