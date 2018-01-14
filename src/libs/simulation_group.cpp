#include "simulation_group.hpp"
#include <vector>
#include <string>
#include <iostream>

// comment this if you want not the connection to be reestabilished once it have been lost
// #define __RETRY_CONNECTION

extern "C" {
#include "extApi.h"
	/*  #include "extApiCustom.h" if you wanna use custom remote API functions! */
}

using namespace std;


simulationGroup::simulationGroup() {}

simulationGroup::simulationGroup(vector<string> teamA_names, vector<string> teamB_names, string ball_name) {
	this->connect("127.0.0.1", 19997, 2000);
	this->set_teamA(teamA_names);
	this->set_teamB(teamB_names);
	this->set_ball(ball_name);
}

int simulationGroup::connect(string ip_addr, int port, int timeout) {
	// blocks untill connected and do not retry the connection once it is lost
	#ifndef __RETRY_CONNECTION
	this->clientID = simxStart((simxChar*) (simxChar*) ip_addr.c_str(), port, true, true, timeout, 5);
	#else
	// blocks untill connected but retry the connection once it is lost
	this->clientID = simxStart((simxChar*) (simxChar*) ip_addr.c_str(), port, true, 0, timeout, 5);
	#endif
	return this->clientID;
}

int simulationGroup::set_teamA(vector<string> teamA_names) {
	simxInt status;
	simxInt buff;

	for(int i = 0; i < teamA_names.size(); i++) {
		this->teamA_names.push_back(teamA_names[i]);
		
		status = simxGetObjectHandle(this->clientID, (simxChar*) (simxChar*) teamA_names[i].c_str(),
									 &buff, simx_opmode_oneshot_wait);

		this->teamA_handler.push_back(buff);
		
		if (status == -1) {
			cout << "Error Loading TEAM A Robots!" << endl;
			return -1;
		}
	}

	return 0;
}

int simulationGroup::set_teamB(vector<string> teamB_names) {
	simxInt status;
	simxInt buff;

	for(int i = 0; i < teamA_names.size(); i++) {
		this->teamB_names.push_back(teamB_names[i]);
		
		status = simxGetObjectHandle(this->clientID, (simxChar*) (simxChar*) teamB_names[i].c_str(),
									 &buff, simx_opmode_oneshot_wait);

		this->teamB_handler.push_back(buff);
		
		if (status == -1) {
			cout << "Error Loading TEAM B Robots!" << endl;
			return -1;
		}
	}

	return 0;
}

int simulationGroup::set_ball(string ball_name) {
	simxInt status;
	this->ball_name = ball_name;
	status = simxGetObjectHandle(this->clientID, (simxChar*) (simxChar*) ball_name.c_str(),
								 &(this->ball_handler), simx_opmode_oneshot_wait);
	if (status == -1) {
		cout << "Error Loading Ball!" << endl;
		return -1;
	}
}

int simulationGroup::is_goal() {
	Position pos = this->get_ballPos();
	int result = 0;

	if (pos.x <= _GOAL_A_BEGIN_X && pos.x >= _GOAL_A_END_X && pos.y <= _GOAL_A_BEGIN_Y && pos.y >= _GOAL_A_END_Y) {
		result = 1;
	}

	if (pos.x >= _GOAL_B_BEGIN_X && pos.x <= _GOAL_B_END_X && pos.y <= _GOAL_B_BEGIN_Y && pos.y >= _GOAL_B_END_Y) {
		result = -1;
	}

	return result;
}

Position simulationGroup::get_ballPos() {
	Position pos;
	simxFloat buff[3];
	simxInt status;

	status = simxGetObjectPosition(clientID, this->ball_handler, -1, buff, simx_opmode_oneshot_wait);
	pos.x = buff[0];
	pos.y = buff[1];
	pos.z = buff[2];

	return pos;
}


vector<Position> simulationGroup::get_teamAPos() {
	vector<Position> team_pos;
	Position pos;
	simxFloat buff[3];
	simxInt status;

	for(int i = 0; i < this->teamA_handler.size(); i++) {
		simxGetObjectPosition(this->clientID, this->teamA_handler[i], -1, buff, simx_opmode_oneshot_wait);
		pos.x = buff[0];
		pos.y = buff[1];
		pos.z = buff[2];
		simxGetObjectOrientation(this->clientID, this->teamA_handler[i], -1, buff, simx_opmode_oneshot_wait);
		// buff[0] -> zy axys angle
		// buff[1] -> zx axys angle
		// buff[2] -> xy axys angle
		pos.t = buff[2];

		team_pos.push_back(pos);
	}

	return team_pos;
}


vector<Position> simulationGroup::get_teamBPos() {
	vector<Position> team_pos;
	Position pos;
	simxFloat buff[3];
	simxInt status;

	for(int i = 0; i < this->teamB_handler.size(); i++) {
		simxGetObjectPosition(this->clientID, this->teamB_handler[i], -1, buff, simx_opmode_oneshot_wait);
		pos.x = buff[0];
		pos.y = buff[1];
		pos.z = buff[2];
		simxGetObjectOrientation(this->clientID, this->teamB_handler[i], -1, buff, simx_opmode_oneshot_wait);
		// buff[0] -> zy axys angle
		// buff[1] -> zx axys angle
		// buff[2] -> xy axys angle
		pos.t = buff[2];

		team_pos.push_back(pos);
	}

	return team_pos;
}


void simulationGroup::setup_startPos(vector<Position> teamA, vector<Position> teamB, Position ballPos) {
	simxFloat ball_pos[] = {ballPos.x,ballPos.y, ballPos.y};
	simxSetObjectPosition(this->clientID, this->ball_handler, ABSOLUTE_POSITION, ball_pos, simx_opmode_oneshot);
	
	for (int i = 0; i < teamA.size(); i++) {
		simxFloat pos[] = {teamA[i].x,teamA[i].y, teamA[i].y};
		simxSetObjectPosition(this->clientID, this->teamA_handler[i], ABSOLUTE_POSITION, pos, simx_opmode_oneshot);
	}

	for (int i = 0; i < teamB.size(); i++) {
		simxFloat pos[] = {teamB[i].x,teamB[i].y, teamB[i].y};
		simxSetObjectPosition(this->clientID, this->teamA_handler[i], ABSOLUTE_POSITION, pos, simx_opmode_oneshot);
	}
}


void simulationGroup::start() {
	simxInt status;
	
	do {
		status = simxStartSimulation(this->clientID, simx_opmode_oneshot_wait);
		extApi_sleepMs(2);
	} while(status != simx_return_ok);
}

void simulationGroup::stop() {
	simxInt status;
	
	do {
		status = simxStopSimulation(this->clientID, simx_opmode_oneshot);
		extApi_sleepMs(2);
	} while(status != simx_return_ok);
}	

void simulationGroup::pause() {
	simxInt status;
	
	do {
		status = simxPauseSimulation(this->clientID, simx_opmode_oneshot);
		extApi_sleepMs(2);
	} while(status != simx_return_ok);
}

void simulationGroup::endCom() {
	simxFinish(this->clientID);
	extApi_sleepMs(2);
}