import serial


class RobotAuto:
    def __init__(self, port):
        self._port = port
        self._serial_port = None

    def __term__(self):
        self._serial_port.close()

    def connect(self, port=None):
        if port:
            self._port = port
        self._serial_port = serial.Serial(self._port, 9600, timeout=5)

    def move(self, speed=150):
        command = ("MOVE:%d\n"%speed).encode('utf-8')
        self._serial_port.write(command)
        self._serial_port.readline()
        print("move:%d"%speed)

    def brake(self):
        command = "BRAKE\n".encode('utf-8')
        self._serial_port.write(command)
        self._serial_port.readline()
        print("brake")

    def left(self, speed=150):
        command = ("LEFT:%d\n"%speed).encode('utf-8')
        self._serial_port.write(command)
        self._serial_port.readline()
        print("left:%d"%speed)

    def right(self, speed=150):
        command = ("RIGHT:%d\n"%speed).encode('utf-8')
        self._serial_port.write(command)
        self._serial_port.readline()
        print("right:%d"%speed)

    def servo(self, angle):
        command = ("SERVO:%d\n"%angle).encode('utf-8')
        self._serial_port.write(command)
        self._serial_port.readline()
        print("servo:%d"%angle)

    def dist(self):
        command = ("DIST\n").encode('utf-8')
        self._serial_port.write(command)
        result = self._serial_port.readline().decode('utf-8').strip()
        while result == "":
            result = self._serial_port.readline().decode('utf-8').strip()
        try:
            int_result = int(result[5:])
        except ValueError:
            int_result = 0
        print("dist:%d"%int_result)
        return int_result

if __name__ == '__main__':
    m = RobotAuto("COM6:")
    m.connect()
    lresult = m.dist()
    print(lresult)
