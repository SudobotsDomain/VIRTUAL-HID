public class VirtualHIDJNI {
    static {
        System.loadLibrary("virtualhid");
    }

    public native void initializeHID();
    public native void sendHIDReport(byte[] report);
}
