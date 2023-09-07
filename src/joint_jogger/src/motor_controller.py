#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Int16

def constrain(val, min_val, max_val):
    return min(max(val, min_val), max_val)

class MotorController:
    def __init__(self):
        rospy.init_node('motor_controller')
        self.pub_wheel_l = rospy.Publisher('wheel_L', Int16, queue_size=10)
        self.pub_wheel_r = rospy.Publisher('wheel_R', Int16, queue_size=10)

    def cmd_vel_callback(self, cmd_vel):
        x = cmd_vel.linear.x
        z = cmd_vel.angular.z

        # Map linear and angular velocities to motor speeds
        lv = constrain(int(x * 500 + z * 500 / 2)+1500, 1000, 2000)
        rv = constrain(int(x * 500 - z * 500 / 2)+1500, 1000, 2000)

        wheel_l_msg = Int16()
        wheel_l_msg.data = lv
        self.pub_wheel_l.publish(wheel_l_msg)

        wheel_r_msg = Int16()
        wheel_r_msg.data = rv
        self.pub_wheel_r.publish(wheel_r_msg)

        print(lv)
        print(rv)

if __name__ == '__main__':
    try:
        controller = MotorController()
        rospy.Subscriber('cmd_vel', Twist, controller.cmd_vel_callback)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
