import sys
import serial as s
import glob

conn = s.Serial(glob.glob("/dev/tty.usbmodem*")[0])

while True:
    sys.stdout.write(conn.read())
