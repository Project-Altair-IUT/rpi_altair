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


int16_t ch[8];

void rcCallback(const std_msgs::Int16MultiArray& msg)
{
  for(int i=0; i<8; i++)
    ch[i] = msg.data[i];

//   ROS_INFO("%d", ch[0]);
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "prog");


  ros::NodeHandle n2;
  
  ros::Publisher science_cmd = n2.advertise<std_msgs::Int16MultiArray>("science_cmd", 10);

  ros::Rate rate(10);
  
  std_msgs::Int16MultiArray sci_msg;

  ros::Subscriber sub = n2.subscribe("rc_signal", 100, rcCallback);

  
  while(ros::ok()){

    sci_msg.data.clear();

    if(ch[4] >= 1750 and ch[4] <= 2000)
    if(0)
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
    else
    {
      sci_msg.data.push_back(0);
      sci_msg.data.push_back(0);
      sci_msg.data.push_back(0);
      sci_msg.data.push_back(0);
    }
    int8_t xxr = 0;
    int16_t arr[4] = {1,2,3,4};
    sci_msg.data.push_back(0);
    

    science_cmd.publish(sci_msg);

     
    rate.sleep();
    ros::spinOnce();      //Notice this

  }
  
  return 0;
}
