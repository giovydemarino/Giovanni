#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "ros_rand/custom.h"
#include <iostream>

using namespace std;

class ROS_SUBPUB {

	public:
		ROS_SUBPUB();
        void sub_callback(ros_rand::customPtr);
	private:
		ros::NodeHandle _nh;
		ros::Publisher _topic_pub;
        ros::Subscriber _topic_sub;
};


ROS_SUBPUB::ROS_SUBPUB(){
	_topic_sub = _nh.subscribe("/rand", 0, &ROS_SUBPUB::sub_callback, this);
}

void ROS_SUBPUB::sub_callback(ros_rand::customPtr data){
    ROS_INFO("Listener: %.2f --- %2.f", data->num1, data->num2);
    
    float num1 = data->num1;
    float num2 = data->num2;
    data->sum =  num1 + num2;
    ROS_INFO("Sum: %.2f", data->sum);

    _topic_pub = _nh.advertise<ros_rand::custom>("/rand_sum",10);
    _topic_pub.publish(data);
}

int main( int argc, char** argv ) {
	ros::init(argc, argv, "ros_SubscriberPublisher");
	ROS_SUBPUB subpub;
	ros::spin();
	return 0;
}