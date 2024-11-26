#include <iostream>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <thread>

using namespace std;

HANDLE hpipe;
void wp( vector<int>& data ) {
    DWORD bwt;

    // Step 1: Send the size of the array
    int size = data.size();
        if (!WriteFile(hpipe, &size, sizeof(int), &bwt, NULL)) {
            cerr << "Failed to write array size! Error: " << GetLastError() << endl;
            return; 
    }
    cout << "Wrote array size (" <<size<< ") to the pipe." << endl;

    // Step 2: Send the array data
    DWORD btw = size* sizeof(int);
    if (!WriteFile(hpipe, data.data(), btw, &bwt, NULL)) {
        cerr << "Failed to write array data! Error: " << GetLastError() << endl;
    } else {
        cout << "Wrote " << bwt << " bytes of array data to the pipe." << endl;
    }
    //cout<<"delay";
    Sleep(100);
}

void bubbleSort(vector<int>& arr, int n) {
    for (int i = 0; i < n-1; i++) {
        bool swapped = false;
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                swapped = true;
                wp(arr);  // Write to pipe after swap
            }
        }
        if (!swapped) {
            break;
        }
    }
}

void insertionSort(vector<int>& arr, int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        // Move elements of arr[0..i-1] that are greater than key
        // to one position ahead of their current position
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
            
        }
        arr[j + 1] = key;
        wp(arr);
    }
}

void selectionSort(vector<int>& arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        
        // Find the minimum element in the unsorted part of the array
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        
        // Swap the found minimum element with the first element
        swap(arr[minIndex], arr[i]);
        wp(arr);

    }
}


int partition(vector<int>& arr, int low, int high,int n) {
    int pivot = arr[high];  // Pivot element
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // If the current element is smaller than or equal to the pivot
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
            wp(arr);
        }
    }
    swap(arr[i + 1], arr[high]);
    wp(arr);
    return (i + 1); 
}
void quickSort(vector<int>& arr, int low, int high,int n) {
    if (low < high) {
        int pi = partition(arr, low, high,n);

        // Recursively sort the elements before and after partition
        quickSort(arr, low, pi - 1,n);
        quickSort(arr, pi + 1, high,n);
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

    char in;
    cout << "Do you want to use pre-filled data or enter numbers on your own? Enter Y for using data and N for entering your numbers: ";
    cin >> in;

    vector<int> v;
    int n=0;
    if (in == 'Y') {
        system("python numgeneratorscript.py");
        ifstream file("randomscript.txt");
        int num;
        while (file >> num) {
            v.push_back(num);
            n++;
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
    int ch;
    cout<<"enter 1 for qs,2 for bs,3 for is,4 for ss"<<endl;
    cin>>ch;
    wp(v);
    switch (ch)
    {
    case 1:
        quickSort(v,0,n-1,n);//here indices will be passed not the element
        break;
    case 2:
        bubbleSort(v,n);
        break;
    case 3:
        insertionSort(v,n);
        break;
    case 4:
        selectionSort(v,n);
        break;
    default:
        cout<<"Enter a valid choice";
        break;
    }

    //wp(v);

   // bubbleSort(v, n);  // Sort the array and write to the pipe
    //quickSort(arr,0,n-1,n);
    
    
    
    
    
    
    
    
    
    
    // Keep the pipe open for the client to read, do not close prematurely
    cout << "Data written, pipe open. Waiting for client to finish..." << endl;

    // Close the pipe after communication is finished
    CloseHandle(hpipe);
    cout<<"Closed";

    return 0;
}