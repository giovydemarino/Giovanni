#include "ros/ros.h"
#include "fibonacci/service.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "fib_client");

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<fibonacci::service>("service");

while (ros::ok()){
  fibonacci::service srv;
  cout<<"Insert index: ";
  cin>>srv.request.index;
  cout<<"Insert length: ";
  cin>>srv.request.len;

  if (client.call(srv))
  {
    ROS_INFO("Request index=%d \t length=%d", srv.request.index, srv.request.len);
  }
  else
  {
    ROS_ERROR("Failed to call the service");
    return 1;
  }
  ros::spinOnce();
}
  return 0;
}