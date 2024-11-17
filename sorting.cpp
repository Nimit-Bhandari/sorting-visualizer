#include <iostream>
#include <windows.h>

using namespace std;

HANDLE hpipe;
void wp(int *ar, int si) {
    DWORD bwt;

    // Step 1: Send the size of the array
    int array_size = si;
    if (!WriteFile(hpipe, &array_size, sizeof(int), &bwt, NULL)) {
        cerr << "Failed to write array size! Error: " << GetLastError() << endl;
        return;
    }
    cout << "Wrote array size (" << array_size << ") to the pipe." << endl;

    // Step 2: Send the array data
    DWORD btw = si * sizeof(int);
    if (!WriteFile(hpipe, ar, btw, &bwt, NULL)) {
        cerr << "Failed to write array data! Error: " << GetLastError() << endl;
    } else {
        cout << "Wrote " << bwt << " bytes of array data to the pipe." << endl;
    }
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        bool swapped = false;
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                swapped = true;
                wp(arr, n);  // Write to pipe after swap
            }
        }
        if (!swapped) {
            break;
        }
    }
}

int main() {
    hpipe = CreateNamedPipe(
        "\\\\.\\pipe\\MyPipe",       
        PIPE_ACCESS_OUTBOUND,        
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
        1,                           
        1024,                        
        1024,                        
        0,                           
        NULL                         
    );

    if (hpipe == INVALID_HANDLE_VALUE) {
        cerr << "Failed to create the named pipe!" << endl;
        return 1;
    }

    cout << "Named pipe created successfully!" << endl;

    if (ConnectNamedPipe(hpipe, NULL) == FALSE) {
        cerr << "Failed to connect to the pipe!" << endl;
        return 1;
    }

    cout << "Client connected!" << endl;

    int arr[] = {5, 3, 8, 1, 2};
    int n = sizeof(arr) / sizeof(arr[0]);

    bubbleSort(arr, n);  // Sort the array and write to the pipe

    // Keep the pipe open for the client to read, do not close prematurely
    cout << "Data written, pipe open. Waiting for client to finish..." << endl;

    // Close the pipe after communication is finished
    CloseHandle(hpipe);
    cout<<"closd";

    return 0;
}
