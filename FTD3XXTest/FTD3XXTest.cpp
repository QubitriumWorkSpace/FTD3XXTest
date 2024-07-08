#include <FTD3XX.h>
#include <stdio.h>

FT_HANDLE ftHandle;
FT_STATUS ftStatus;
DWORD bytesRead;
UCHAR buffer[1024];

int main() {
    // Initialize the device
    ftStatus = FT_Create(0, FT_OPEN_BY_INDEX, &ftHandle);
    if (ftStatus != FT_OK) {
        printf("Failed to create device\n");
        return 1;
    }

    // Configure the device
    ftStatus = FT_SetPipeTimeout(ftHandle, 0x82, 5000); // Set timeout to 5 seconds for IN endpoint
    if (ftStatus != FT_OK) {
        printf("Failed to set pipe timeout\n");
        FT_Close(ftHandle);
        return 1;
    }

    // Read data from the device using IN endpoint (0x82)
    ftStatus = FT_ReadPipe(ftHandle, 0x82, buffer, sizeof(buffer), &bytesRead, NULL);
    if (ftStatus != FT_OK) {
        printf("Failed to read pipe\n");
        FT_Close(ftHandle);
        return 1;
    }

    printf("Read %d bytes from the device\n", bytesRead);

    // Close the device
    FT_Close(ftHandle);
    return 0;
}