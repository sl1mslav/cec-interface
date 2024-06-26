/*
 * CECInterface.cpp
 *
 *  Created on: Mar 26, 2017
 *      Author: Matt
 */

#include <jni.h>
#include "cec_LibCEC.h"
#include "handle.h"

extern "C" JNIEXPORT void JNICALL Java_com_example_cec_1interface_LibCEC_init
  (JNIEnv * env, jobject obj, jboolean loggingOn)
{
    CECInterface* interface = new CECInterface();
    interface->setup_adapter(loggingOn);
    setHandle(env, obj, interface);
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_open
  (JNIEnv * env, jobject obj, jstring strPort)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    const char *nativeString = env->GetStringUTFChars(strPort, 0);

    bool result = adapter->Open(nativeString);

    env->ReleaseStringUTFChars(strPort, nativeString);

    return result ? JNI_TRUE : JNI_FALSE;

}

extern "C" JNIEXPORT jobjectArray JNICALL Java_com_example_cec_1interface_LibCEC_getAdapters
  (JNIEnv * env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);

    CEC::cec_adapter_descriptor devices[10];
    int8_t iDevicesFound = adapter->DetectAdapters(devices, 10, NULL, true);

    jstring str = NULL;
    jclass strCls = env->FindClass("java/lang/String");
    jobjectArray strarray = env->NewObjectArray(iDevicesFound,strCls,NULL);
    for(int i=0; i<iDevicesFound; i++)
    {

        str = env->NewStringUTF(devices[i].strComName);
        env->SetObjectArrayElement(strarray,i,str);
        env->DeleteLocalRef(str);

    }

     return strarray;

}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_powerOnDevice
  (JNIEnv * env, jobject obj, jint address)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    bool result = adapter->PowerOnDevices((CEC::cec_logical_address)address);

    return result ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_powerOnTV
  (JNIEnv * env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    /* Calls function with default device address which is TV's address */
    bool result = adapter->PowerOnDevices();

    return result ? JNI_TRUE : JNI_FALSE;

}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_powerOffDevice
  (JNIEnv * env, jobject obj, jint address)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    bool result = adapter->StandbyDevices((CEC::cec_logical_address)address);

    return result ? JNI_TRUE : JNI_FALSE;
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_powerOffTV
  (JNIEnv * env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    bool result = adapter->StandbyDevices(CEC::CECDEVICE_TV);

    return result ? JNI_TRUE : JNI_FALSE;

}

extern "C" JNIEXPORT jintArray JNICALL Java_com_example_cec_1interface_LibCEC_getActiveDevices
  (JNIEnv * env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    CEC::cec_logical_addresses addresses = adapter->GetActiveDevices();

    jintArray jAddresses = env->NewIntArray(sizeof(addresses.addresses));
    env->SetIntArrayRegion(jAddresses, 0, sizeof(addresses.addresses), addresses.addresses);

    return jAddresses;

}

extern "C" JNIEXPORT jlong JNICALL Java_com_example_cec_1interface_LibCEC_getDeviceVendorId
  (JNIEnv * env, jobject obj, jint address)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    return adapter->GetDeviceVendorId((CEC::cec_logical_address)address);

}

extern "C" JNIEXPORT jint JNICALL Java_com_example_cec_1interface_LibCEC_getDevicePowerStatus
  (JNIEnv * env, jobject obj, jint address)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    return adapter->GetDevicePowerStatus((CEC::cec_logical_address)address);

}

extern "C" JNIEXPORT void JNICALL Java_com_example_cec_1interface_LibCEC_close
  (JNIEnv *env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    adapter->Close();
}

extern "C" JNIEXPORT void JNICALL Java_com_example_cec_1interface_LibCEC_dispose
  (JNIEnv *env, jobject obj)
{
    CECInterface *interface = getInterfaceHandle(env, obj);
    delete interface;
    setHandle(env, obj, (CECInterface*)NULL);    
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_sendKeyPress
  (JNIEnv *env, jobject obj, jint address, jint controlCode)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    return adapter->SendKeypress((CEC::cec_logical_address)address,(CEC::cec_user_control_code)controlCode,true);
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_sendKeyRelease
  (JNIEnv *env, jobject obj, jint address)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    return adapter->SendKeyRelease((CEC::cec_logical_address)address,true);
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_isActiveDevice
  (JNIEnv *env, jobject obj, jint address)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    return adapter->IsActiveDevice((CEC::cec_logical_address)address);
}

extern "C" JNIEXPORT jboolean JNICALL Java_com_example_cec_1interface_LibCEC_isActiveDeviceType
  (JNIEnv *env, jobject obj, jint type)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    return adapter->IsActiveDeviceType((CEC::cec_device_type)type);
}

extern "C" JNIEXPORT jint JNICALL Java_com_example_cec_1interface_LibCEC_sendVolumeUp
  (JNIEnv *env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    return adapter->VolumeUp(true);
}

extern "C" JNIEXPORT jint JNICALL Java_com_example_cec_1interface_LibCEC_sendVolumeDown
  (JNIEnv *env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    return adapter->VolumeDown(true);
}

//TODO move to interface class
extern "C" JNIEXPORT void JNICALL Java_com_example_cec_1interface_LibCEC_enableLogging
  (JNIEnv *env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    CEC::libcec_configuration config;
    if (adapter->GetCurrentConfiguration(&config))
    {
        if(!config.callbacks || !config.callbacks->logMessage)
        {
            //this will have to be updated in future if other callbacks are
            //used but for now logging is only callback I'm using
            CEC::ICECCallbacks callbacks;
            callbacks.Clear();
    	    callbacks.logMessage = &LogMessage;
            config.callbacks = &callbacks;
            adapter->SetConfiguration(&config);
        }
    }
}

//TODO move to interface class
extern "C" JNIEXPORT void JNICALL Java_com_example_cec_1interface_LibCEC_disableLogging
  (JNIEnv *env, jobject obj)
{
    CEC::ICECAdapter *adapter = getAdapterHandle(env, obj);
    CEC::libcec_configuration config;
    if (adapter->GetCurrentConfiguration(&config))
    {
        if(config.callbacks && config.callbacks->logMessage)
        {
            CEC::ICECCallbacks callbacks;
            callbacks.Clear();
            config.callbacks = &callbacks;
            adapter->SetConfiguration(&config);
        }
    }
}

