import time

from robot_auto import RobotAuto

LOOP_TIME = 0.5
TURN_TIME = 0.5

robot = RobotAuto("COM6:")
robot.connect()
robot.move()

while True:
    for look in [30, 90, 150, 90]:
        robot.servo(look)
        dist = robot.dist()
        if dist < 30:
            if look < 90:
                robot.right(-150)
                time.sleep(TURN_TIME)
            elif look > 90:
                robot.left(-150)
                time.sleep(TURN_TIME)
            else:
                robot.move(-150)
                time.sleep(TURN_TIME)
                robot.left(-150)
                time.sleep(TURN_TIME)
            robot.move()
        else:
            robot.move()
            time.sleep(LOOP_TIME)
