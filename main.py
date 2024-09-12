import serial
import pyautogui
import time

# Replace 'COM3' with your Arduino port name (e.g., 'COM3' for Windows, '/dev/ttyACM0' for Linux)
arduino_port = 'COM3'
baud_rate = 9600

# Connect to Arduino
ser = serial.Serial(arduino_port, baud_rate)
time.sleep(2)  # Wait for the connection to be established


def process_joystick_data(x, y, sw):
    """Translate joystick data to Google Earth Pro control actions using raw values and print actions."""
    # Dead zone thresholds (adjust based on your joystick's central values)
    DEAD_ZONE_MIN = 600
    DEAD_ZONE_MAX = 600

    # action_taken = "None"  # Variable to keep track of action taken

    # Move Google Earth Pro view with the joystick if outside dead zone
    if x < 500:  # Left movement
        # pyautogui.press('up')
        pyautogui.keyDown('up')
        # pyautogui.keyUp('down')
        # action_taken = "Moving Up"

        # pyautogui.keyDown('left')
        # pyautogui.keyUp('right')
        # action_taken = "Moving Left"
    elif x > 600:  # Right movement
        # pyautogui.press('down')
        pyautogui.keyDown('down')
        # pyautogui.keyUp('up')
        # action_taken = "Moving Down"

        # pyautogui.keyDown('right')
        # pyautogui.keyUp('left')
        # action_taken = "Moving Right"
    # else:
    #     # pyautogui.keyUp('left')
    #     # pyautogui.keyUp('right')
    #     pyautogui.keyUp('up')
    #     pyautogui.keyUp('down')

    if 580 < y < 800:  # Up movement
        # pyautogui.press('left')
        pyautogui.keyDown('left')
        # pyautogui.keyUp('right')
        # action_taken = "Moving Left"

        # pyautogui.keyDown('up')
        # pyautogui.keyUp('down')
        # action_taken = "Moving Up"
    elif 450 > y > 250:  # Down movement
        # pyautogui.press('right')
        pyautogui.keyDown('right')
        # pyautogui.keyUp('left')
        # action_taken = "Moving Right"

        # pyautogui.keyDown('down')
        # pyautogui.keyUp('up')
        # action_taken = "Moving Down"
    # else:
        # pyautogui.keyUp('left')
        # pyautogui.keyUp('right')

    # If the joystick button is pressed, reset the view
    if sw == 0:  # Button press (LOW)
        pyautogui.press('r')  # Example action: 'r' key resets view

    # Print the joystick values and action taken
    # print(f"x: {x}, y: {y}, sw: {sw} => Action: {action_taken}")


try:
    while True:
        # Read data from Arduino
        data = ser.readline().decode('utf-8', errors='ignore').strip()  # Ignore any decode errors

        if data:
            # Ensure the data is in the expected format and handle errors
            try:
                x_data, y_data, sw_data = map(int, data.split(','))
                # Process joystick data and control Google Earth Pro
                process_joystick_data(x_data, y_data, sw_data)
            except ValueError:
                print(f"Received malformed data: {data}")  # Print malformed data for debugging

except KeyboardInterrupt:
    print("Program interrupted by user.")
finally:
    ser.close()  # Close the serial connection when done
