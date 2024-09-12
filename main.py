import serial
import pyautogui
import time

# Replace 'COM3' with your Arduino port name (e.g., 'COM3' for Windows, '/dev/ttyACM0' for Linux)
arduino_port = 'COM3'
baud_rate = 9600

# Connect to Arduino
ser = serial.Serial(arduino_port, baud_rate)
time.sleep(2)  # Wait for the connection to be established


def map_value(value, in_min, in_max, out_min, out_max):
    """Map a value from one range to another."""
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min


def process_joystick_data(x, y, sw):
    """Translate joystick data to Google Earth Pro control actions."""
    # Mapping joystick values to mouse or keyboard actions
    x_mapped = map_value(x, 0, 1023, -10, 10)  # Adjust range for sensitivity
    y_mapped = map_value(y, 0, 1023, -10, 10)

    # Move Google Earth Pro view with the joystick
    if x_mapped > 2:
        pyautogui.keyDown('right')  # Move right
    elif x_mapped < -2:
        pyautogui.keyDown('left')  # Move left
    else:
        pyautogui.keyUp('right')
        pyautogui.keyUp('left')

    if y_mapped > 2:
        pyautogui.keyDown('up')  # Move up
    elif y_mapped < -2:
        pyautogui.keyDown('down')  # Move down
    else:
        pyautogui.keyUp('up')
        pyautogui.keyUp('down')

    # If the joystick button is pressed, reset the view
    if sw == 0:  # Button press (LOW)
        pyautogui.press('r')  # Example action: 'r' key resets view

try:
    while True:
        # Read data from Arduino
        data = ser.readline().decode('utf-8').strip()
        if data:
            # Split data from Arduino (x_data, y_data, sw_data)
            x_data, y_data, sw_data = map(int, data.split(','))

            # Process joystick data and control Google Earth Pro
            process_joystick_data(x_data, y_data, sw_data)

except KeyboardInterrupt:
    print("Program interrupted by user.")
finally:
    ser.close()  # Close the serial connection when done
