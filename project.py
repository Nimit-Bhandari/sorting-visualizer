import win32file
import win32pipe
import struct
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import threading

PIPE_NAME = r'\\.\pipe\MyPipe'

# Initialize the figure and axis
fig, ax = plt.subplots()
bars = ax.bar([], [])  # Initialize empty bar chart

# Global variable to hold the latest data
latest_data = []

def initialize_plot():
    """
    Initialize the plot with default settings.
    """
    ax.set_ylim(0, 10)  # Set default y-axis range
    ax.set_title("Real-Time Bar Chart")
    ax.set_xlabel("Index")
    ax.set_ylabel("Value")
    return bars

def update_plot(frame):
    """
    Update the plot for each frame in the animation.
    """
    global latest_data
    if latest_data:
        # Clear the axis and redraw bars
        ax.clear()
        ax.bar(range(len(latest_data)), latest_data, align='center')
        ax.set_ylim(0, max(latest_data) + 10)  # Adjust y-axis range
        ax.set_title(f"Current Array: {latest_data}")

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
    except Exception:
        print("Pipe not available yet. Retrying...")
        time.sleep(1)  # Wait before trying again

print("Connected to the pipe.")

# Function to read pipe data
def read_pipe_data():
    global latest_data
    try:
        while True:
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

            # Update the latest data
            latest_data = integers
    except Exception as e:
        print(f"Error reading from pipe: {e}")
    finally:
        # Ensure the pipe is closed
        win32file.CloseHandle(pipe)
        print("Pipe closed.")

# Start a thread for reading data from the pipe
thread = threading.Thread(target=read_pipe_data, daemon=True)
thread.start()

# Create animation with caching disabled
ani = FuncAnimation(fig, update_plot, init_func=initialize_plot, interval=100, cache_frame_data=False)

plt.show()
