#include <iostream>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <thread>

using namespace std;

HANDLE hpipe;

void executePythonScript() {
    // Specify full path if needed, or check your Python environment setup
    int result = system("python client.py"); // Adjust command based on Python installation
    if (result != 0) {
        cerr << "C++: Error executing Python script.\n";
    }
}

void wp(vector<int>& data) {
    DWORD bwt;

    // Send the size of the array
    int size = data.size();
    if (!WriteFile(hpipe, &size, sizeof(int), &bwt, NULL)) {
        cerr << "Failed to write array size! Error: " << GetLastError() << endl;
        return;
    }
    cout << "Wrote array size (" << size << ") to the pipe." << endl;

    // Send the array data
    DWORD btw = size * sizeof(int);
    if (!WriteFile(hpipe, data.data(), btw, &bwt, NULL)) {
        cerr << "Failed to write array data! Error: " << GetLastError() << endl;
    } else {
        cout << "Wrote " << bwt << " bytes of array data to the pipe." << endl;
    }
    Sleep(500);  // Delay to simulate work
}

int main() {
    // Create a named pipe
    hpipe = CreateNamedPipe(
        "\\\\.\\pipe\\MyPipe",       // Pipe name
        PIPE_ACCESS_OUTBOUND,        // Pipe access mode
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
        1,                            // Max instances of the pipe
        1024,                         // Output buffer size
        1024,                         // Input buffer size
        0,                            // Default timeout
        NULL                          // Default security attributes
    );

    if (hpipe == INVALID_HANDLE_VALUE) {
        cerr << "Failed to create the named pipe!" << endl;
        return 1;
    }
    
    cout << "Named pipe created successfully!" << endl;

    // if (ConnectNamedPipe(hpipe, NULL) == FALSE) {
    //     cerr << "Failed to connect to the pipe!" << endl;
    //     return 1;
    // }

    // cout << "Client connected!" << endl;

    // Launch the Python script in a separate thread
    
    // Handle input for array and sorting options
    char in;
    cout << "Do you want to use pre-filled data or enter numbers on your own? Enter Y for using data and N for entering your numbers: ";
    cin >> in;

    vector<int> v;
    int n;
    if (in == 'Y') {
        system("python numgeneratorscript.py");
        ifstream file("randomscript.txt");
        int num;
        while (file >> num) {
            v.push_back(num);
        }
        file.close();
    } else {
        cout << "Enter the number of elements in the array: ";
        cin >> n;
        int no;
        for (int i = 0; i < n; i++) {
            cout << "Enter the " << i + 1 << " number: ";
            cin >> no;
            v.push_back(no);
        }
    }
    //thread pythonThread(executePythonScript);


    // Write data to the pipe
    wp(v);

    // Wait for the Python thread to complete
    //pythonThread.join();

    // Close the pipe after communication
    CloseHandle(hpipe);
    cout << "Pipe closed." << endl;

    return 0;
}
