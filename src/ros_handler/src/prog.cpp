/* Bismillah
  code written by 
  A. K. M. Rakinuzzaman
  for Team Avijatrik
*/

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Int8.h"
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/Int8MultiArray.h>
#include <geometry_msgs/Twist.h>

#define rc_min 1000
#define rc_max 2000

#define linear_min -0.6
#define linear_max 0.6
#define angular_min -5.5
#define angular_max 5.5

int16_t ch[8];
int8_t arm_arr[6];



float rangeMap(int16_t x, int16_t in_min, int16_t in_max, float out_min, float out_max)
{
  if(x <= 2000 and x >= 1000 )
  return (1.0*x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  else return 0.0;
}


void rcCallback(const std_msgs::Int16MultiArray& msg)
{
  for(int i=0; i<8; i++)
    ch[i] = msg.data[i];

//   ROS_INFO("%d", ch[0]);
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "prog");


  ros::NodeHandle n;
  
  ros::Publisher cmd_vel = n.advertise<geometry_msgs::Twist>("cmd_vel", 100);

  ros::Rate rate(10);
  
  geometry_msgs::Twist velocity;  
  
  ros::Subscriber sub = n.subscribe("rc_signal", 100, rcCallback);

  velocity.linear.y = 0.0;
  velocity.linear.z = 0.0;
  velocity.angular.x = 0.0;
  velocity.angular.y = 0.0;
  
  while(ros::ok()){

    if(ch[4]>=1000 and ch[4] <= 1200)
    {
      //rover drive
      velocity.linear.x = rangeMap(ch[2], rc_min, rc_max, linear_min, linear_max);
      velocity.angular.z = -rangeMap(ch[3], rc_min, rc_max, angular_min, angular_max);
    }

    else
    {
      velocity.linear.x = 0.0;
      velocity.angular.z = 0.0;
    }
    
    cmd_vel.publish(velocity);

    rate.sleep();
    ros::spinOnce();      //Notice this

  }
  
  return 0;
}
