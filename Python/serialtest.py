import serial

# COMポートを開く
print("Open Port")
ser = serial.Serial("COM3", 9600)

try:
    while True:
        try:
            command = input("type 0 or 1: ")
            if command == "0" or command == "1":
                ser.write(command.encode())
            else:
                print("Invalid command. Please enter 0 or 1.")
        except KeyboardInterrupt:
            break
finally:
    print("Close Port")
    ser.close()
