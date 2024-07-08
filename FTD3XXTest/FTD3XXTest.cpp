#include <FTD3XX.h>
#include <stdio.h>
#include <bitset>
#include <iostream>

FT_HANDLE ftHandle;
FT_STATUS ftStatus;
//read declarations
DWORD bytesRead;
const ULONG size_buffer = 65536;
UCHAR buffer[size_buffer];
UCHAR data_in1[size_buffer];



using namespace std;

int main() {
    // Initialize the device
    ftStatus = FT_Create(0, FT_OPEN_BY_INDEX, &ftHandle);
    if (ftStatus != FT_OK) {
        printf("Failed to create device\n");
        return 1;
    }
    else {

        printf("FT_Create Successful\n");
    }


#pragma region ReadPreparations
    ftStatus = FT_SetStreamPipe(ftHandle, FALSE, FALSE, 0x82, size_buffer);
    if (ftStatus != FT_OK)
    {
        printf("FT_SetStreamPipe (read) has been failed...");
        return 1;
    }



    // Configure the device
    ftStatus = FT_SetPipeTimeout(ftHandle, 0x82, 0); // Set timeout to 5 seconds for IN endpoint
    if (ftStatus != FT_OK) {
        printf("Failed to set pipe (read) timeout\n");
        FT_Close(ftHandle);
        return 1;
    } 
    else {
        printf("FT_SetPipeTimeout (read) Successful\n");

    }
#pragma endregion

#pragma region WritePreparations
    bool bitStreamTracker = false;

    ftStatus = FT_SetStreamPipe(ftHandle, FALSE, FALSE, 0x02, size_buffer);
    if (ftStatus != FT_OK)
    {
        printf("FT_SetStreamPipe (write) has been failed...");
        return 1;
    }



    // Configure the device
    ftStatus = FT_SetPipeTimeout(ftHandle, 0x02, 0); // Set timeout to 5 seconds for IN endpoint
    if (ftStatus != FT_OK) {
        printf("Failed to set pipe (write) timeout\n");
        FT_Close(ftHandle);
        return 1;
    }
    else {
        printf("FT_SetPipeTimeout (write) Successful\n");

    }
#pragma endregion


    while (1) {
#pragma region WriteDataFromUSB

        if (!bitStreamTracker) {
            //get ready the data
            uint32_t data[4] = { 0x12345678, 0x9ABCDEF0, 0x13579BDF, 0x2468ACE0 };//example bitstream
            DWORD bytesToWrite = sizeof(data);
            DWORD bytesWritten = 0;

            //write data
            ftStatus = FT_WritePipe(ftHandle, 0x02, (PUCHAR)data, bytesToWrite, &bytesWritten, NULL);
            if (ftStatus != FT_OK || bytesWritten != bytesToWrite) {
                std::cerr << "Failed to write data to the pipe" << std::endl;
                //FT_Close(ftHandle);
                //continue;
            }
            else
            {
                bitStreamTracker = true;
                cout << "Data transferred successfully! Write size:" << bytesWritten << endl;
            }



        }

#pragma endregion
#pragma region ReadDataFromUSB
        // Read data from the device using IN endpoint (0x82)
        Sleep(1000);
        ftStatus = FT_ReadPipe(ftHandle, 0x82, buffer, size_buffer, &bytesRead, NULL);
        if (ftStatus != FT_OK) {
            printf("Failed to read pipe\n");
            //FT_Close(ftHandle);
            continue;

        }
        printf("Read %d bytes from the device\n", bytesRead);
        /*if (bytesRead > 3) {
            char* dataReaded;
            dataReaded = (char*)malloc(bytesRead);
            memcpy(dataReaded, buffer, bytesRead);

            cout << "bytesRead: " << bytesRead << endl;
            for (int i = 0; i < bytesRead - 3; i++)
            {
                std::cout << "bytesRead[" << i << "]: "
                    << bitset<8>(dataReaded[i + 3]).to_string() << "-" << bitset<8>(dataReaded[i + 2]).to_string() << "-"
                    << bitset<8>(dataReaded[i + 1]).to_string() << "-" << bitset<8>(dataReaded[i]).to_string()
                    << std::endl;
            }
        }*/

        if (bytesRead >7) {
            char* dataReaded;
            dataReaded = (char*)malloc(bytesRead);
            memcpy(dataReaded, buffer, bytesRead);

            cout << "bytesRead: " << bytesRead << endl;
            for (int i = 0; i < bytesRead-7; i++)
            {
                std::cout << "bytesRead[" << i << "]: " 
                    << bitset<8>(dataReaded[i+7]).to_string() << "-" << bitset<8>(dataReaded[i+6]).to_string() << "-" 
                    << bitset<8>(dataReaded[i+5]).to_string() << "-" << bitset<8>(dataReaded[i+4]).to_string() << "-"
                    << bitset<8>(dataReaded[i+3]).to_string() << "-" << bitset<8>(dataReaded[i+2]).to_string() << "-"
                    << bitset<8>(dataReaded[i+1]).to_string() << "-" << bitset<8>(dataReaded[i]).to_string()
                    << std::endl;
            }
        }
#pragma endregion   


    }
        

        // Close the device
        FT_Close(ftHandle);
        return 0;
    }



