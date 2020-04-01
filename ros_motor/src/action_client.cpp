#include "ros/ros.h"
#include <iostream>
#include <stdlib.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include "ros_motor/motorAction.h"

int main (int argc, char **argv) {

	ros::init(argc, argv, "motor_action_client");
  if(argc != 5){
		ROS_INFO("%d",argc);
		ROS_WARN("Usage: motor_action_client <pos_iniziale> <pos_finale> <v_max> <time_to_preempt_in_sec>");
		return 1;
	}

  ros_motor::motorGoal goal;
  goal.xiniz = atof(argv[1]);
  goal.xfin = atof(argv[2]);
  goal.vel = atof(argv[3]);

  //Controllo sulla correttezza dei valori
  if ((goal.xiniz<0) || (goal.xfin<0) || (goal.vel<0) || (atoi(argv[4])<0)) {
    ROS_WARN("All values must be positive");
    return 1;
  }

  if (goal.xiniz > goal.xfin) {
    ROS_WARN("Initial position must be greater than final position");
    return 1;
  }



  // create the action client
  actionlib::SimpleActionClient<ros_motor::motorAction> action_client ("motor_action", true);

  ROS_INFO("Waiting for action server to start.");

  // wait for the action server to start
  action_client.waitForServer();

  ROS_INFO("Action server started, sending goals.");

  // send a goal to the action
  ROS_INFO("Sending Goal [xiniz=%.2f] - [xfin=%.2f] - [vel=%.2f] and Preempt time of [%d]",goal.xiniz, goal.xfin, goal.vel, atoi(argv[4]));
  action_client.sendGoal(goal);

  //wait for the action to return
  bool finished_before_timeout = action_client.waitForResult(ros::Duration(atoi(argv[4])));

  if (finished_before_timeout) {
    actionlib::SimpleClientGoalState state = action_client.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
    //Preempting the process
		action_client.cancelGoal();
  }
  else{
    ROS_INFO("Action did not finish before the time out.");
  }
  //exit
  return 0;
}
