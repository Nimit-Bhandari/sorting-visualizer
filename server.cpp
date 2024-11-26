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


void wp( vector<int>& data ) {
    DWORD bwt;

    //Send the size of the array
    int size = data.size();
        if (!WriteFile(hpipe, &size, sizeof(int), &bwt, NULL)) {
            cerr << "Failed to write array size! Error: " << GetLastError() << endl;
            return; 
    }
    cout << "Wrote array size (" <<size<< ") to the pipe." << endl;

    //Send the array data
    DWORD btw = size* sizeof(int);
    if (!WriteFile(hpipe, data.data(), btw, &bwt, NULL)) {
        cerr << "Failed to write array data! Error: " << GetLastError() << endl;
    } else {
        cout << "Wrote " << bwt << " bytes of array data to the pipe." << endl;
    }
    //cout<<"delay";
    Sleep(100);
}




void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);  // Temporary arrays for left and right sub-arrays

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int i = 0; i < n2; i++) R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
        wp(arr);
        // Print the array after placing an element from either L or R
    
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;

        wp(arr);
        
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;

        wp(arr);
        // Print the array after copying an element from R[]
        
    }
}



void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        wp(arr);
        mergeSort(arr, left, mid); // Recursively sort the left half

        wp(arr);
           
        mergeSort(arr, mid + 1, right); // Recursively sort the right half

        // Print the array after sorting the right half
        wp(arr);
       
        merge(arr, left, mid, right); // Merge the sorted halves

        wp(arr);
    }
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
    
    thread pythonThread(executePythonScript);




    if (ConnectNamedPipe(hpipe, NULL) == FALSE) {
        cerr << "Failed to connect to the pipe!" << endl;
        return 1;
    }

    cout << "Client connected!" << endl;

    char in;
    cout << "Do you want to use randomly generated data or enter data on your own? Enter Y for using data and N for entering your numbers: "<<endl;
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
    } else if(in=='N') {
        cout << "Enter the number of elements in the array: ";
        cin >> n;
        int no;
        for (int i = 0; i < n; i++) {
            cout << "Enter the " << i + 1 << " number: ";
            cin >> no;
            v.push_back(no);
        }
    }
    else{cout<<"Invalid Response."<<endl<<"Execute the program again and enter only 'Y' or 'N'"<<endl; return 1;}
    int ch;

// Display sorting options
cout << "\nChoose a sorting algorithm to execute:\n";
cout << "----------------------------------------\n";
cout << "1. Quick Sort\n";
cout << "2. Bubble Sort\n";
cout << "3. Insertion Sort\n";
cout << "4. Selection Sort\n";
cout << "5. Merge Sort\n";
cout << "----------------------------------------\n";
cout << "Enter your choice (1-5): ";
cin >> ch;

cout << "\nProcessing your choice...\n";

switch (ch) {
    case 1:
        cout << "You selected Quick Sort.\n";
        wp(v);
        quickSort(v, 0, n - 1, n); // Indices are passed
        break;

    case 2:
        cout << "You selected Bubble Sort.\n";
        wp(v);
        bubbleSort(v, n);
        break;

    case 3:
        cout << "You selected Insertion Sort.\n";
        wp(v);
        insertionSort(v, n);
        break;

    case 4:
        cout << "You selected Selection Sort.\n";
        wp(v);
        selectionSort(v, n);
        break;

    case 5:
        cout << "You selected Merge Sort.\n";
        wp(v);
        mergeSort(v, 0, n - 1); // here,Indices are passed and not elements.
        break;

    default:
        cerr << "Invalid choice! Please run the program again and choose a valid option (1-5).\n";
        return 1;
}

    //wp(v);

   // bubbleSort(v, n);  // Sort the array and write to the pipe
    //quickSort(arr,0,n-1,n);
    
    
    
    
    
    
    
    
    
    
    // Keep the pipe open for the client to read, do not close prematurely
    //cout << "Data written, pipe open. Waiting for client to finish..." << endl;

    // Close the pipe after communication is finished
    pythonThread.join();
    CloseHandle(hpipe);
    cout<<"Sorting Done.";

    return 0;
}