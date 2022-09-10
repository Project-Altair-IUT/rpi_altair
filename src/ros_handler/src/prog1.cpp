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

#define rc_min 1000
#define rc_max 2000

#define linear_min -0.6
#define linear_max 0.6
#define angular_min -5.5
#define angular_max 5.5

int16_t ch[8]={0,0,0,0,0,0,0,0};
int8_t arm_arr[6];


int8_t rangeMap(int16_t x, int16_t in_min, int16_t in_max, int8_t out_min, int8_t out_max)
{
  if(x <= 2000 and x >= 1000 )
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  else return 0;
}

void rcCallback(const std_msgs::Int16MultiArray& msg)
{
  for(int i=0; i<8; i++)
    ch[i] = msg.data[i];

  // ROS_INFO("%d", ch[0]);
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "prog");


  ros::NodeHandle n1;
  
  ros::Publisher arm_cmd = n1.advertise<std_msgs::Int8MultiArray>("arm_cmd", 10);

  ros::Rate rate(10);
  

  std_msgs::Int8MultiArray arm_msg;
  
  ros::Subscriber sub = n1.subscribe("rc_signal", 100, rcCallback);

  while(ros::ok()){
    
    arm_msg.data.clear();
    
    if(ch[4]>=1300 and ch[4] <= 1700)
    {
      int8_t vals[6];

      vals[0] = rangeMap(ch[0], rc_min, rc_max, 0, 255); //base
      vals[1] = rangeMap(ch[2], rc_min, rc_max, 0, 255); //primary
      vals[2] = rangeMap(ch[1], rc_min, rc_max, 0, 255); //secndary
      vals[3] = rangeMap(ch[5], rc_min, rc_max, 0, 255); //wrist pitch

      vals[4] = rangeMap(ch[3], rc_min, rc_max, 0, 180); //wrist roll servo
      vals[5] = rangeMap(ch[6], rc_min, rc_max, 0, 180); //gripper servo

      for(int i=0; i<6; i++)
        {
          arm_msg.data.push_back(vals[i]);
        }

    }
    else 
    {
        for(int i=0; i<6; i++)
        {
          arm_msg.data.push_back(0);
        }
    }
    
    arm_cmd.publish(arm_msg);

    rate.sleep();
    ros::spinOnce();      //Notice this

  }
  
  return 0;
}
