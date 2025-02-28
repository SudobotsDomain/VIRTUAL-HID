#include <jni.h>
#include <libusb-1.0/libusb.h>

static libusb_device_handle *handle = NULL;

extern "C" JNIEXPORT jint JNICALL
Java_com_example_virtualserial_VirtualSerial_initUSB(JNIEnv *env, jobject thiz) {
    libusb_init(NULL);
    handle = libusb_open_device_with_vid_pid(NULL, YOUR_VID, YOUR_PID);
    if (!handle) {
        return -1; // Error opening device
    }
    return 0; // Success
}
