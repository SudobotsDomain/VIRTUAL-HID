package com.example.virtualserial;

public class VirtualSerial {
    static {
        System.loadLibrary("virtualserial");
    }

    public native int initUSB();
    public native int readData(byte[] buffer);
    public native int writeData(byte[] data);

    // Example method to retrieve diagnostic data
    public String getDiagnosticData() {
        byte[] buffer = new byte[1024];
        int bytesRead = readData(buffer);
        if (bytesRead > 0) {
            return new String(buffer, 0, bytesRead);
        }
        return null;
    }
  }
