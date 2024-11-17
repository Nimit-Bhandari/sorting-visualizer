import win32file
import win32pipe
import struct
import time

PIPE_NAME = r'\\.\pipe\MyPipe'

# Wait for the pipe to be available
while True:
    try:
        # Open the named pipe
        pipe = win32file.CreateFile(
            PIPE_NAME,
            win32file.GENERIC_READ,
            0,  # No sharing
            None,  # Default security attributes
            win32file.OPEN_EXISTING,
            0,  # Default attributes
            None
        )
        break  # Exit the loop once the pipe is available
    except Exception as e:
        print("Pipe not available yet. Retrying...")
        time.sleep(1)  # Wait before trying again

print("Connected to the pipe.")

# Deduplication logic: track received arrays
received_arrays = set()

# Read data from the pipe
while True:
    try:
        # Read the size of the array (4 bytes for an integer)
        result, size_data = win32file.ReadFile(pipe, 4)
        if not size_data:
            print("No more data, closing connection.")
            break

        # Unpack the size of the array
        array_size = struct.unpack('i', size_data)[0]

        # Read the array data
        result, array_data = win32file.ReadFile(pipe, array_size * 4)
        integers = struct.unpack(f'{array_size}i', array_data)

        # Check if this array has already been received
        if integers not in received_arrays:
            received_arrays.add(integers)
            print(f"Received array: {integers}")
    except Exception as e:
        print(f"Error reading from pipe: {e}")
        break

# Close the pipe after communication is complete
win32file.CloseHandle(pipe)
print("Pipe closed.")
