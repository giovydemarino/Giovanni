#include "ros/ros.h"
#include "fibonacci/service.h"
#include <iostream>
#include <sstream>

using namespace std;

int fib(int n) 
{ 
    if (n <= 1) 
        return n; 
    return fib(n-1) + fib(n-2); 
}

string print_array (vector<int32_t> v){
    stringstream ss;
    ss<<"[ ";
    for (int i=0; i<v.size(); i++){
        ss<<v[i]<<" ";
    }
    ss<<"]";
    return ss.str();
}

bool service_callback (fibonacci::service::Request  &req,
         fibonacci::service::Response &res)
{
  ROS_INFO("request: index=%ld, length=%ld", (long int)req.index, (long int)req.len);

  int index=req.index;
  int len=req.len;
  vector<int32_t> seq (req.len);
  
  for(int i=0; i<=len; i++){
      seq[i]=fib(index);
      index++;
  }

  string ss = print_array(seq);
  res.seq = seq;
  
  ROS_INFO("sending back Fibonacci sequences\n %s", ss.c_str());
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "fib_server");
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("service", service_callback);
  ROS_INFO("Ready!");
  ros::spin();
  
  return 0;
}