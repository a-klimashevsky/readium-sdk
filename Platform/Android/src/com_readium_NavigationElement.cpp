#import <ePub3/archive.h>
#import <ePub3/container.h>
#include <ePub3/package.h>
#import <ePub3/nav_element.h>
#import <ePub3/nav_point.h>
#include "com_readium_NavigationElement.h"
#include <android/log.h>
#include "general.h"

#define APPNAME "READIUM"

/*
 * Class:     com_readium_NavigationElement
 * Method:    initNavigationElement
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_readium_NavigationElement_initNavigationElement
(JNIEnv *env, jobject thiz, jlong jNativePointer) {
	if(jNativePointer != 0) {

		ePub3::NavigationElement *element = (ePub3::NavigationElement *) jNativePointer;

		ePub3::NavigationPoint *point = dynamic_cast<ePub3::NavigationPoint *>(element);
		if(point != NULL){
			const ePub3::string s = point->SourceHref();
			setStringProperty(env,thiz,"content",s.c_str());
		}
		setStringProperty(env,thiz,"title",element->Title().c_str());

	}
}

/*
 * Class:     com_readium_NavigationElement
 * Method:    getChildren
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_readium_NavigationElement_getChildren(
		JNIEnv *env, jobject thiz, jlong jNativePointer) {
	ePub3::NavigationElement *element =
			(ePub3::NavigationElement *) jNativePointer;

	const ePub3::NavigationList list = element->Children();

	jlong *pointers = new jlong[list.size()];
	int i = 0;

	for (auto it = list.begin(); it != list.end(); it++) {
		pointers[i++] = (jlong)(*it);
	}

	jlongArray result = env->NewLongArray((jsize) list.size());
	env->SetLongArrayRegion(result, 0, list.size(), pointers); // copy
	return result;
}

