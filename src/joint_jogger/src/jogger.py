#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16
import sys
import select
import tty
import termios

class Jogger:
    def __init__(self):
        rospy.init_node('jogger', anonymous=True)
        self.pub = rospy.Publisher('/jointX_val', Int16, queue_size=10)
        self.value = 1500

    def publish_value(self):
        self.pub.publish(self.value)
        rospy.loginfo("Published: {}".format(self.value))

    def reset_value(self):
        self.value = 1500
        self.publish_value()

    def run(self):
        old_settings = termios.tcgetattr(sys.stdin)
        tty.setcbreak(sys.stdin.fileno())

        rospy.loginfo("Press 'w' to increment and 's' to decrement. Press any other key to reset. Press 'q' to exit.")

        while not rospy.is_shutdown():
            if select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], []):
                key = sys.stdin.read(1)
                if key == 'q':
                    break
                elif key == 'w':
                    self.value = min(self.value + 50, 2000)
                    self.publish_value()
                elif key == 's':
                    self.value = max(self.value - 50, 1000)
                    self.publish_value()
                else:
                    self.reset_value()

        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)

if __name__ == '__main__':
    try:
        node = Jogger()
        node.run()
    except rospy.ROSInterruptException:
        pass
