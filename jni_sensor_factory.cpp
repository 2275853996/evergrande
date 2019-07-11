/*
factory test app层调用 jni库
*/

#include "jni.h"
#include <android_runtime/AndroidRuntime.h>
#include <utils/Log.h>
#include <fcntl.h>

#define JNIREG_CLASS "com/xingluo/engineermode/test/SensorTest"  //指定要注册的类
#define PSENSOR_FACTORY_PATH "/dev/psensor"

#define SENSOR_SET_MODE             1
#define SENSOR_CALI_DATA_NOSCRREN   2
#define SENSOR_CALI_DATA_SCREEN     3
#define SENSOR_GET_DATA             4
#define SENSOR_GET_STATUS           5



using namespace android;
int fd;

 JNIEXPORT jint JNICALL native_psensor_open(JNIEnv *env,jobject obj)
 {
     fd = open(PSENSOR_FACTORY_PATH,O_WRONLY);
     if(fd < 0)
     {
         printf("faild to open %s /n",PSENSOR_FACTORY_PATH);
         return -1;
     }
     return 0;
 }

JNIEXPORT jint JNICALL native_psensor_cmd(JNIEnv *env, jobject obj,jint cmd)
 {
    int ret = -1;
    int rBuffer[2];
    switch(cmd)
    {
        case SENSOR_SET_MODE:
            ioctrl(fd,PROXIMITY_IOCTL_SET_MODE); 
            break;
        case SENSOR_CALI_DATA_NOSCRREN:
            ioctrl(fd,PROXIMITY_IOCTL_CALI_DATA_NOSCRREN,rBuffer);
            goto RETURN_PARA;
        case SENSOR_CALI_DATA_SCREEN:
            ioctrl(fd,PROXIMITY_IOCTL_CALI_DATA_SCREEN,rBuffer);
            goto RETURN_PARA;
        case SENSOR_GET_DATA:
            ioctrl(fd,PROXIMITY_IOCTL_GET_PS_DATA,rBuffer);
            goto RETURN_PARA;
        case SENSOR_GET_STATUS:
            ioctrl(fd,PROXIMITY_IOCTL_GET_PS_STATUS,rBuffer);
            goto RETURN_PARA;
        default:            
            printf ("error cmd");
            break;
    }    
    return ret;
RETURN_PARA:
    ret = rbuffer[0]&0xFF00 | rbuffer[1]&0x00FF;
    return ret;    
 }


 static JNINativeMethod gSensorFacotryMethods[] = {
    /* name, signature, funcPtr */
    { "nativePsensorOpen","()I", (jint *)native_psensor_open },
    { "nativePsensorCmd",  "(I)I",(jint *)native_psensor_cmd },
};


static int registerNativeMethods(JNIEnv* env, const char* className,
JNINativeMethod* gMethods, int numMethods) {
    jclass clazz;
    clazz = (*env)->FindClass(env, className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}


/*
* Register native methods for all classes we know about.
*/
 static int registerNatives(JNIEnv* env)
{
	if (!registerNativeMethods(env, JNIREG_CLASS, gSensorFacotryMethods, 
                                 sizeof(gSensorFacotryMethods) / sizeof(gSensorFacotryMethod[0])))
		return JNI_FALSE;

	return JNI_TRUE;
}

/*
* Set some test stuff up.
*
* Returns the JNI version on success, -1 on failure.
*/
 JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK){	
		return -1;
	}
	assert(env != NULL);

	if (!registerNatives(env)) {//注册
		return -1;
	}
	/* success -- return valid version number */
	result = JNI_VERSION_1_4;

	return result;
}
