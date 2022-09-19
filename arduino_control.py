# Driver code for the arduino

import time
import serial


class Arduino():
    def __init__(self, port):
        self.ser = serial.Serial(port, 115200, timeout=1)  # open serial port

    def serial_connect(self):
        while True:
            for line in self.ser.readlines():
                # Give time for the arduino to reboot and wait until ready
                if str(line).find('<Arduino is ready>'):
                    print('Arduino is serially connected')
                    return True
                else:
                    print('Waiting for Arduino')

    def go_to(self, pan, tilt):
        self.ser.write(bytes(f'<{str(pan)}, {str(tilt)}>\n', encoding="utf-8"))
        # Arduino is sending messages when ready
        print(self.ser.readlines())
        #print('moving to Pan: {}, Tilt: {}'.format(pan, tilt))
        time.sleep(.01)

    def close_serial(self):
        self.ser.close()

    def serial_read(self):
        return self.ser.readlines()

if __name__ == "__main__":
    a = Arduino('/dev/ttyACM0')
    a.serial_connect()
    a.go_to(45,45)
    a.close_serial()
