# Sorting Algorithm Visualizer

This project provides an interactive tool to visualize sorting algorithms like **Bubble Sort**, **Quick Sort**, **Merge Sort**, and **Insertion Sort**. Built using **C++** and **Python**, it allows users to understand the step-by-step process of sorting algorithms with real-time animations. The system uses **named pipes** for communication between the C++ backend and Python frontend, ensuring efficient data transfer between the modules.

## Features
- **Interactive Visualization**: Real-time visual updates of sorting processes.
- **Multiple Algorithm Support**: Visualizes **Bubble Sort**, **Quick Sort**, **Merge Sort**, **Insertion Sort**, and others.
- **Named Pipes**: Utilizes named pipes for inter-process communication between C++ and Python.
- **Graphical Interface**: Built using Python for user interaction and visualization.
- **Step-by-Step Execution**: Users can see the sorting process step by step.

## Prerequisites

Ensure you have the following installed:

### C++ Dependencies:
- **C++ Compiler** (e.g., GCC, Clang)
- **Windows API** for **named pipe** implementation (use Windows for compatibility with `windows.h` and related libraries).

### Python Dependencies:
- **Python 3.x**
- **Python Libraries**:
  - `pywin32` for using `win32file` and `win32pipe` to interact with named pipes.
  - `matplotlib` for visualization.
  - `threading` and `time` for handling multi-threaded execution.

To install the necessary Python libraries, run:

```bash
pip install pywin32 matplotlib
```
## How It Works

- **C++ Code**: The C++ code implements the sorting algorithms (e.g., Bubble Sort, Quick Sort) and generates data about each sorting step.
- **Named Pipes**: Named pipes are used to send the sorting data from the C++ program to the Python script.
- **Python Script**: The Python script receives the data and visualizes it using libraries like Tkinter and Matplotlib. The real-time data is displayed on the GUI to show each step of the sorting process.

## Example Algorithms Implemented

- **Bubble Sort**: Compares adjacent elements and swaps them if they are in the wrong order. Repeats this process until the list is sorted.
- **Quick Sort**: Divides the array into smaller parts using a pivot element, recursively sorts the sub-arrays.
- **Merge Sort**: Divides the array into halves, sorts each half recursively, and then merges the sorted halves together.
- **Insertion Sort**: Builds the final sorted array one element at a time by repeatedly picking the next element and inserting it into its correct position.

## Technologies Used

- **C++**: Sorting algorithms implementation, data processing, Windows API (for named pipes).
- **Python**: Real-time visualization, inter-process communication, user interface.
- **Named Pipes**: Communication between C++ and Python processes.

## Contribution

Feel free to fork this repository, improve the code, and submit pull requests. Contributions are always welcome!
