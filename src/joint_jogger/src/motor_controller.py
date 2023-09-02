#!/usr/bin/env python

import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Int16

class MotorController:
    def __init__(self):
        rospy.init_node('motor_controller')
        self.pub_wheel_l = rospy.Publisher('wheel_L', Int16, queue_size=10)
        self.pub_wheel_r = rospy.Publisher('wheel_R', Int16, queue_size=10)
        self.speed_range_min = 1000
        self.speed_range_max = 2000

    def cmd_vel_callback(self, cmd_vel):
        x = int(cmd_vel.linear.x * (self.speed_range_max - self.speed_range_min) + self.speed_range_min)
        z = int(cmd_vel.angular.z * (self.speed_range_max - self.speed_range_min) + self.speed_range_min)

        lv = max(min(x + z // 2, self.speed_range_max), self.speed_range_min)
        rv = max(min(x - z // 2, self.speed_range_max), self.speed_range_min)

        wheel_l_msg = Int16()
        wheel_l_msg.data = lv
        self.pub_wheel_l.publish(wheel_l_msg)

        wheel_r_msg = Int16()
        wheel_r_msg.data = rv
        self.pub_wheel_r.publish(wheel_r_msg)

if __name__ == '__main__':
    try:
        controller = MotorController()
        rospy.Subscriber('cmd_vel', Twist, controller.cmd_vel_callback)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
