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

int8_t rangeMap(bool f, int16_t x, int16_t in_min, int16_t in_max, int8_t out_min, int8_t out_max)
{
  if(x <= 2000 and x >= 1000 )
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  else return 0;
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
  ros::Publisher arm_cmd = n.advertise<std_msgs::Int8MultiArray>("arm_cmd", 10);
  ros::Publisher science_cmd = n.advertise<std_msgs::Int8MultiArray>("science_cmd", 10);


  ros::Rate rate(10);
  
  geometry_msgs::Twist velocity;  
  
  std_msgs::Int8MultiArray arm_msg;

  std_msgs::Int8MultiArray sci_msg;

  
  ros::Subscriber sub = n.subscribe("rc_signal", 100, rcCallback);

  
  while(ros::ok()){

    velocity.linear.x = 0.0;
    velocity.linear.y = 0.0;
    velocity.linear.z = 0.0;
    velocity.angular.x = 0.0;
    velocity.angular.y = 0.0;
    velocity.angular.z = 0.0;

    for(int i=0; i<6; i++)
    {
      arm_msg.data[i] = 0;
    }

    for(int i=0; i<4; i++)
    {
      sci_msg.data[i] = 0;
    }

    if(ch[4]>=1000 and ch[4] <= 1200)
    {
      //rover drive
      velocity.linear.x = rangeMap(ch[2], rc_min, rc_max, linear_min, linear_max);
      velocity.angular.z = rangeMap(ch[3], rc_min, rc_max, angular_min, angular_max);
    }
    else if(ch[4]>=1300 and ch[4] <= 1700)
    {
      arm_msg.data[0] = rangeMap(1, ch[0], rc_min, rc_max, 0, 255); //base
      arm_msg.data[1] = rangeMap(1, ch[2], rc_min, rc_max, 0, 255); //primary
      arm_msg.data[2] = rangeMap(1, ch[1], rc_min, rc_max, 0, 255); //secndary
      arm_msg.data[3] = rangeMap(1, ch[5], rc_min, rc_max, 0, 255); //wrist pitch

      arm_msg.data[4] = rangeMap(1, ch[3], rc_min, rc_max, 0, 180); //wrist roll servo
      arm_msg.data[5] = rangeMap(1, ch[6], rc_min, rc_max, 0, 180); //gripper sesrvo

      // arm_msg.data_length = 6;
    }
    else if(ch[4] >= 1750 and ch[4] <= 2000)
    {
      //actuator
        if(ch[6] >= 1000 and ch[6] <= 1200)
          sci_msg.data[0] = 2;
        else if(ch[6] >= 1300 and ch[6] <= 1700)
          sci_msg.data[0] = 0;
        else if(ch[6] >= 1750 and ch[6] <= 2000)
          sci_msg.data[0] = 1;


      //carousel servo
        if(ch[7] >= 1000 and ch[6] <= 1200)
          sci_msg.data[1] = 0;
        else if(ch[7] >= 1300 and ch[6] <= 1700)
          sci_msg.data[1] = 1;
        else if(ch[7] >= 1750 and ch[6] <= 2000)
          sci_msg.data[1] = 2;

      //auger
        if(ch[2] >= 1000 and ch[6] <= 1200)
          sci_msg.data[2] = 2;
        else if(ch[2] >= 1300 and ch[6] <= 1700)
          sci_msg.data[2] = 0;
        else if(ch[2] >= 1750 and ch[6] <= 2000)
          sci_msg.data[2] = 1;

      //semi-auto
        if(ch[5] >= 1000 and ch[6] <= 1200)
          sci_msg.data[0] = 0;
        else if(ch[5] >= 1300 and ch[6] <= 1700)
          sci_msg.data[0] = 1;
        else if(ch[5] >= 1750 and ch[6] <= 2000)
          sci_msg.data[0] = 2;
    }
    
    cmd_vel.publish(velocity);

    arm_cmd.publish(arm_msg);

    science_cmd.publish(arm_msg);

     
    rate.sleep();
    ros::spinOnce();      //Notice this

  }
  
  return 0;
}
