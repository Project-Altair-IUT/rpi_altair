#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Int8.h"
#include <std_msgs/Int16MultiArray.h>
#include <geometry_msgs/Twist.h>

#define rc_min 1000
#define rc_max 2000

#define linear_min -0.6
#define linear_max 0.6
#define angular_min -5.5
#define angular_max 5.5

int16_t ch[8];

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
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */

  ros::init(argc, argv, "prog");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
  **/

  ros::NodeHandle n;
  
  ros::Publisher cmd_vel = n.advertise<geometry_msgs::Twist>("cmd_vel", 100);

  ros::Rate rate(10);
  
  geometry_msgs::Twist velocity;

  velocity.linear.x = 0.0;
  velocity.linear.y = 0.0;
  velocity.linear.z = 0.0;
  velocity.angular.x = 0.0;
  velocity.angular.y = 0.0;
  velocity.angular.z = 0.0;
  
  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  ros::Subscriber sub = n.subscribe("rc_signal", 100, rcCallback);

  /**
   * ros::spin() will enter a loop, pumping callbacks.  With this version, all
   * callbacks will be called from within this thread (the main one).  ros::spin()
   * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
  **/
  
  while(ros::ok()){

    velocity.linear.x = rangeMap(ch[2], rc_min, rc_max, linear_min, linear_max);
    velocity.angular.z = rangeMap(ch[3], rc_min, rc_max, angular_min, angular_max);

    cmd_vel.publish(velocity);
     
    rate.sleep();
    ros::spinOnce();      //Notice this

  }
  
  return 0;
}
