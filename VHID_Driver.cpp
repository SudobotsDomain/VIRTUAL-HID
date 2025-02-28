#include <ntddk.h>
#include <wdf.h>

#define NT_DEVICE_NAME L"\\Device\\VirtualHID"
#define DOS_DEVICE_NAME L"\\DosDevices\\VirtualHID"

void DriverUnload(_In_ PDRIVER_OBJECT DriverObject) {
    UNICODE_STRING dosDeviceName;
    RtlInitUnicodeString(&dosDeviceName, DOS_DEVICE_NAME);
    IoDeleteSymbolicLink(&dosDeviceName);
    IoDeleteDevice(DriverObject->DeviceObject);
}

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
    UNICODE_STRING ntDeviceName;
    UNICODE_STRING dosDeviceName;
    PDEVICE_OBJECT deviceObject = nullptr;
    NTSTATUS status;

    RtlInitUnicodeString(&ntDeviceName, NT_DEVICE_NAME);
    status = IoCreateDevice(DriverObject, 0, &ntDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &deviceObject);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    RtlInitUnicodeString(&dosDeviceName, DOS_DEVICE_NAME);
    status = IoCreateSymbolicLink(&dosDeviceName, &ntDeviceName);

    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(deviceObject);
        return status;
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE] = [] (PDEVICE_OBJECT DeviceObject, PIRP Irp) -> NTSTATUS {
        Irp->IoStatus.Status = STATUS_SUCCESS;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return STATUS_SUCCESS;
    };

    DriverObject->MajorFunction[IRP_MJ_CLOSE] = [] (PDEVICE_OBJECT DeviceObject, PIRP Irp) -> NTSTATUS {
        Irp->IoStatus.Status = STATUS_SUCCESS;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return STATUS_SUCCESS;
    };

    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = [] (PDEVICE_OBJECT DeviceObject, PIRP Irp) -> NTSTATUS {
        PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
        Irp->IoStatus.Status = STATUS_SUCCESS;
        Irp->IoStatus.Information = 0;

        switch (stack->Parameters.DeviceIoControl.IoControlCode) {
            case IOCTL_CUSTOM_COMMAND:
                // Handle custom commands here
                break;
            default:
                Irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
                break;
        }

        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return Irp->IoStatus.Status;
    };

    DriverObject->DriverUnload = DriverUnload;
    return STATUS_SUCCESS;
}
