import sys
import serial as s


conn = s.Serial("/dev/tty.usbmodemfd131")

while True:
    sys.stdout.write(conn.read())
