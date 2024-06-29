import serial
import time

# Replace with your port name (e.g., 'COM3' on Windows or '/dev/ttyACM0' on Linux)
arduino_port = 'COM3'  # Update this as per your system
baud_rate = 9600

# Get input from the user
temperature = float(input("Enter temperature (°C): "))
heart_rate = int(input("Enter heart rate (BPM): "))
skin_conductance = float(input("Enter skin conductance (µS): "))

def determine_emotion(temperature, heart_rate, skin_conductance):
    # Define the ranges for each emotion at different levels
    emotions = {
        'Sadness': {
            'Normal': {'temp_range': (27, 32), 'heart_rate_range': (60, 80), 'skin_cond_range': (10, 20)},
            'High': {'temp_range': (25, 30), 'heart_rate_range': (80, 100), 'skin_cond_range': (15, 25)},
            'Extreme': {'temp_range': (20, 25), 'heart_rate_range': (100, 140), 'skin_cond_range': (20, 30)}
        },
        'Anger': {
            'Normal': {'temp_range': (35, 37), 'heart_rate_range': (80, 100), 'skin_cond_range': (20, 35)},
            'High': {'temp_range': (36, 38), 'heart_rate_range': (100, 140), 'skin_cond_range': (25, 40)},
            'Extreme': {'temp_range': (37, 39), 'heart_rate_range': (120, 160), 'skin_cond_range': (30, 40)}
        },
        'Fear': {
            'Normal': {'temp_range': (33, 35), 'heart_rate_range': (60, 80), 'skin_cond_range': (15, 20)},
            'High': {'temp_range': (32, 34), 'heart_rate_range': (100, 140), 'skin_cond_range': (25, 35)},
            'Extreme': {'temp_range': (31, 32), 'heart_rate_range': (120, 160), 'skin_cond_range': (30, 40)}
        }
    }

    # Check which emotion's criteria match the input values
    for emotion, levels in emotions.items():
        for level, criteria in levels.items():
            temp_min, temp_max = criteria['temp_range']
            hr_min, hr_max = criteria['heart_rate_range']
            skin_min, skin_max = criteria['skin_cond_range']
            if temp_min <= temperature <= temp_max and hr_min <= heart_rate <= hr_max and skin_min <= skin_conductance <= skin_max:
                return f"{emotion},{level}"

    return "No matching emotion found."

result = determine_emotion(temperature, heart_rate, skin_conductance)
print(result)

# Send result to Arduino
if result != "No matching emotion found.":
    with serial.Serial(arduino_port, baud_rate, timeout=1) as ser:
        time.sleep(2)  # Wait for the serial connection to initialize
        ser.write(result.encode())
