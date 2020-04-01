#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <actionlib/server/simple_action_server.h>
#include "ros_motor/motorAction.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>

class action_class {

	private:
  	ros::NodeHandle nh_;
  	actionlib::SimpleActionServer<ros_motor::motorAction> action_server; 
	ros_motor::motorFeedback feedback;
	ros_motor::motorResult result;
  	std::string action_name;
  	ros_motor::motorGoal goal;
  	float xc;

	public:
		action_class(std::string name) :
  	  action_server(nh_, name, boost::bind(&action_class::executeCB, this, _1), false),
  	  action_name(name) {
				action_server.registerPreemptCallback(boost::bind(&action_class::preemptCB, this));
				action_server.start();
  		}
	
		void preemptCB(){
			ROS_WARN("%s got preempted!", action_name.c_str());
			result.pos_finale = xc;
			action_server.setPreempted(result,"I got Preempted");
  	}
  
		void executeCB(const ros_motor::motorGoalConstPtr &goal) {

			if(!action_server.isActive() || action_server.isPreemptRequested()) return;

			ros::Rate rate(10);
			ROS_INFO("%s is processing the goal %d", action_name.c_str(), goal->xfin);
			float max_step = (goal->vel)*0.1;
			float step = max_step;
			
			for(xc = goal->xiniz; xc <= goal->xfin; xc= xc + step){
				//Check for ros
				if (!ros::ok()) {
					result.pos_finale = xc;
					action_server.setAborted(result,"I failed !");
					ROS_INFO("%s Shutting down",action_name.c_str());
					break;
				}
		
				if(!action_server.isActive() || action_server.isPreemptRequested()){
					return;
				}	

				if(goal->xfin <= xc) {
					ROS_INFO("%s Succeeded at getting to goal %.2f", action_name.c_str(), goal->xf);
					result.pos_finale = xc;
					action_server.setSucceeded(result);
				}
				else {
					feedback.xc = xc;
					ROS_INFO("Current position: %.2f",feedback.xc);
					action_server.publishFeedback(feedback);
			}
		rate.sleep();
		}	
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "motor_action");
  ROS_INFO("Starting Motor Action Server");
  action_class motor_action_obj(ros::this_node::getName());
  ros::spin();
  return 0;
}
