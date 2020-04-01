#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "ros_rand/custom.h"
#include <iostream>

using namespace std;

class ROS_SUB {

	public:
		ROS_SUB();
        void sub_callback(ros_rand::customConstPtr);
	private:
		ros::NodeHandle _nh;
        ros::Subscriber _topic_sub;
};

ROS_SUB::ROS_SUB() {
	_topic_sub = _nh.subscribe("/rand_sum", 0, &ROS_SUB::sub_callback, this);
}

void ROS_SUB::sub_callback(ros_rand::customConstPtr data){
    ROS_INFO("Sum: %.2f", data->sum);
}

int main( int argc, char** argv ) {
	ros::init(argc, argv, "ros_Subscriber");
	ROS_SUB sub;
	ros::spin();
	return 0;
}