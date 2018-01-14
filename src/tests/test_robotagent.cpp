/**
 * Problem: The routines of starting the match and so on are problematic because they need a time 
 * to stop the simulation an just them being able to restart, because of that, make the system recover
 * the handlers everytime it's going to restart the simulation because it's enought time to the system
 * restar the simulation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "../libs/simulation_group.hpp"
#include "../libs/robot_agent.hpp"

extern "C" {
#include "extApi.h"
	/*  #include "extApiCustom.h" if you wanna use custom remote API functions! */
}

using namespace std;

void print_positions(simulationGroup sim) {
	vector<Position> teamA = sim.get_teamAPos();
	vector<Position> teamB = sim.get_teamBPos();
	Position ball = sim.get_ballPos();

	cout << "Ball" << "\t";
	cout << "ball " << " at -> " << "x: " << ball.x << " y: " << 
				ball.y << " z: " << ball.z << " t: " << ball.t << endl;

	cout << "Team A" << "\t";
	for (int i = 0; i < teamA.size(); ++i) {
		cout << "robot " << i << " at -> " << "x: " << teamA[i].x << " y: " << 
				teamA[i].y << " z: " << teamA[i].z << " t: " << teamA[i].t << endl;
	}

	cout << "Team B " << "\t";
	for (int i = 0; i < teamB.size(); ++i) {
		cout << "robot " << i << " at -> " << "x: " << teamB[i].x << " y: " << 
				teamB[i].y << " z: " << teamB[i].z << " t: " << teamB[i].t << endl;
	}
	cout << endl << endl;
}

void setup_pos(simulationGroup sim) {
	Position start_ball = {0, 0, 0, 0};
	Position start_robot1 = {0.1, 0, 0, 0};
	Position start_robot2 = {-0.1, 0, 0, 0};
	vector<Position> teamAPos;
	vector<Position> teamBPos;

	teamAPos.push_back(start_robot1);
	teamBPos.push_back(start_robot2);
	
	sim.setup_startPos(teamAPos, teamBPos, start_ball);
}

int main() {
	string robots[] = {"RobotFrame", "RobotFrame0"};
	string motors[]= {"LeftMotor_1", "RightMotor_1", "LeftMotor_2", "RightMotor_2"};
	vector<string> teamA(robots, robots + 1);
	vector<string> teamB(robots + 1, robots + 2);
	string ball = "Bola";
	simulationGroup sim(teamA, teamB, ball);

	srand(time(NULL));

	// Motor handlers -------------------------------------------//
	RobotAgent robot1(robots[0], motors[0], motors[1], sim);
	RobotAgent robot2(robots[1], motors[2], motors[3], sim);

	robot1.set_team(1);
	robot2.set_team(2);
	//-----------------------------------------------------------//
	
	cout << "Start of the system simulation!" << endl;
	for(int i = 0; i < 2 && sim.clientID != -1; i++) {
		cout << "Startin the simulation for the " << i << "nd time!" << endl;

		// Motor handlers -------------------------------------------//
		robot1.set_motors(motors[0], motors[1]);
		robot2.set_motors(motors[2], motors[3]);
		//-----------------------------------------------------------//
		
		// Start position -------------------------------------------//
		setup_pos(sim);
		sim.start();
		//-----------------------------------------------------------//

		// std::this_thread::sleep_for(std::chrono::milliseconds(100));
		
		for(int i = 0; i < 30 && simxGetConnectionId(sim.clientID) != -1; i++) {
			// setup velocities
			robot1.set_power(rand()%20, rand()%20);
			robot2.set_power(rand()%20, rand()%20);

			print_positions(sim);
			extApi_sleepMs(2);
		}

		// setup velocities -----------------------------------------//
		robot1.set_power(0,0);
		robot2.set_power(0,0);
		//-----------------------------------------------------------//
		

		cout << "Stoping the simmulation" << endl;
		setup_pos(sim);
		sim.stop();
	}

	cout << "Finishing communication" << endl;
	setup_pos(sim);
	sim.endCom();

	return EXIT_SUCCESS;
}
