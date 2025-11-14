#!/usr/bin/env python3
import sys
import time
import argparse
import math

import bno08x


def main():
    parser = argparse.ArgumentParser(description="Simple BNO08x SPI test via pybind11")
    parser.add_argument("--cs", type=int, default=-1, help="GPIO for CS (default 17)")
    parser.add_argument("--int", dest="int_pin", type=int, default=27, help="GPIO for INT (default 27)")
    parser.add_argument("--rst", type=int, default=22, help="GPIO for RST (default 22)")
    parser.add_argument("--speed", type=int, default=1_000_000, help="SPI speed in Hz (default 1MHz)")
    parser.add_argument("--dev", type=str, default="/dev/spidev0.0", help="SPI device (default /dev/spidev0.0)")
    parser.add_argument("--gpiochip", type=str, default="gpiochip0", help="GPIO chip (default gpiochip0)")
    parser.add_argument("--interval", type=int, default=50, help="Report interval in ms (default 50)")
    parser.add_argument("--samples", type=int, default=1000, help="Number of samples to print (default 50)")
    args = parser.parse_args()

    imu = bno08x.BNO08x()
    imu.enableDebugging(False)

    print("Opening SPI:", args.dev, "cs=", args.cs, "int=", args.int_pin, "rst=", args.rst, "speed=", args.speed)
    ok = imu.beginSPI(args.int_pin, args.rst, args.cs, args.speed, args.dev, args.gpiochip)
    if not ok:
        print("Failed to initialize BNO08x over SPI. Check wiring and permissions.")
        sys.exit(1)

    # Enable rotation vector
    if not imu.enableRotationVector(args.interval):
        print("Failed to enable rotation vector report")

    time.sleep(1) 

    count = 0
    try:
        if imu.wasReset():
            print("Sensor was reset.")
            if not imu.enableRotationVector(args.interval):
                print("Failed to re-enable rotation vector report after reset.")
                
        while count < args.samples:
            if imu.getSensorEvent():
                # Print Euler angles (roll, pitch, yaw) whenever a new event arrives
                roll = imu.getRoll() * 180.0 / math.pi
                pitch = imu.getPitch() * 180.0 / math.pi
                yaw = imu.getYaw() * 180.0 / math.pi
                print(f"euler: roll={roll:.3f} pitch={pitch:.3f} yaw={yaw:.3f}")
                count += 1
            time.sleep(0.01)
    except KeyboardInterrupt:
        pass

    print("Done.")


if __name__ == "__main__":
    main()
