#include "robot_agent.hpp"
#include <string>
#include <vector>
#include "simulation_group.hpp"

extern "C" {
#include "extApi.h"
	/*  #include "extApiCustom.h" if you wanna use custom remote API functions! */
}

using namespace std;


RobotAgent::RobotAgent() {}

RobotAgent::RobotAgent(string robot_name, string leftMotor, string rightMotor, simulationGroup game) {
	this->set_motors(leftMotor, rightMotor);
	this->set_robot(robot_name);
	this->game = game;
}

void RobotAgent::set_game(simulationGroup game) {
	this->game = game;
}

void RobotAgent::set_motors(string leftMotor, string rightMotor) {
	this->leftMotor_name = leftMotor;
	this->rightMotor_name = rightMotor;

	simxGetObjectHandle(this->game.clientID, (simxChar*) (simxChar*) rightMotor.c_str(), 
						&(this->rightMotor_handler), simx_opmode_oneshot_wait);

	simxGetObjectHandle(this->game.clientID, (simxChar*) (simxChar*) leftMotor.c_str(), 
						&(this->leftMotor_handler), simx_opmode_oneshot_wait);
}

void RobotAgent::set_robot(string robot_name) {
	this->robot_name = robot_name;
	simxGetObjectHandle(game.clientID, (simxChar*) (simxChar*) robot_name.c_str(), 
						&(this->robot_handler), simx_opmode_oneshot_wait);
}

void RobotAgent::set_team(int team_side) {
	this->team = team_side;
}

void RobotAgent::set_power(int left_power, int right_power) {
	simxSetJointTargetVelocity(this->game.clientID, this->rightMotor_handler, right_power, simx_opmode_oneshot);
	simxSetJointTargetVelocity(this->game.clientID, this->leftMotor_handler, left_power, simx_opmode_oneshot);
}

Position RobotAgent::get_position() {
	Position pos;
	simxFloat buff[3];

	simxGetObjectPosition(this->game.clientID, this->robot_handler, -1, buff, simx_opmode_oneshot_wait);
	pos.x = buff[0]; // x position
	pos.y = buff[1]; // y position
	pos.z = buff[2]; // z position

	simxGetObjectOrientation(this->game.clientID, this->robot_handler, -1, buff, simx_opmode_oneshot_wait);
	// buff[0] -> yz axys angle
	// buff[1] -> xz axys angle
	// buff[2] -> xy axys angle
	pos.t = buff[2];

	return pos;
}
