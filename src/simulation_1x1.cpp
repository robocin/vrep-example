// Copyright 2006-2015 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// -------------------------------------------------------------------
// THIS FILE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// You are free to use/modify/distribute this file for whatever purpose!
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.2.1 on May 3rd 2015

// Make sure to have the server side running in V-REP!
// Start the server from a child script with following command:
// simExtRemoteApiStart(portNumber) -- starts a remote API server service on the specified port

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define ini_x 1.7
#define ini_y 1.3
#define ABSOLUTE_POSITION -1

#define _GOAL_A_BEGIN_X -0.73750
#define _GOAL_A_END_X   -0.86250
#define _GOAL_A_BEGIN_Y  0.21250
#define _GOAL_A_END_Y   -0.21250
#define _GOAL_B_BEGIN_X  0.73750
#define _GOAL_B_END_X    0.86250
#define _GOAL_B_BEGIN_Y  0.21250
#define _GOAL_B_END_Y   -0.21250


extern "C" {
#include "extApi.h"
	/*  #include "extApiCustom.h" if you wanna use custom remote API functions! */
}



simxInt BolaHandle;

simxInt robot1_Handle;
simxInt leftMotor1_Handle;
simxInt rightMotor1_Handle;

simxInt robot2_Handle;
simxInt leftMotor2_Handle;
simxInt rightMotor2_Handle;

simxInt sensorHandle;
simxInt graphOdometryHandle;
simxInt sensorLHandle, sensorFHandle, sensorRHandle, uiHandle;

simxFloat pos_robot1[3];
simxFloat pos_robot2[3];
simxFloat orientation_robot1[3];
simxFloat orientation_robot2[3];
simxFloat objetivo[3];
simxFloat lwprev;
simxFloat rwprev;

simxInt leftMotorHandle_1;
simxInt rightMotorHandle_1;

int clientID;

simxFloat fieldBall[3];
simxFloat obstaculo[3];
simxInt obstaculo_handle;
simxInt obstaculo_handle_2;
simxInt obstaculo_handle_3;


using namespace std;

inline double to_deg(double radians) {
	return radians * (180.0 / M_PI);
}

double to180range(double angle) {
	angle = fmod(angle, 2 * M_PI);
	if (angle < -M_PI)
	{
		angle = angle + 2 * M_PI;
	}
	else if (angle > M_PI)
	{
		angle = angle - 2 * M_PI;
	}
	return angle;
}

double to_positive_angle(double angle) {
	angle = fmod(angle, 2 * M_PI);
	while (angle < 0)
	{
		angle = angle + 2 * M_PI;
	}
	return angle;
}

double smallestAngleDiff(double target, double source) {
	double a;
	a = to_positive_angle(target) - to_positive_angle(source);

	if (a > M_PI)
	{
		a = a - 2 * M_PI;
	}
	else if (a < -M_PI)
	{
		a = a + 2 * M_PI;
	}
	return a;
}

void conexao() {

	printf("Conexao efetuada\n");
	simxGetObjectHandle(clientID, "RobotFrame", &robot1_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "LeftMotor_1", &leftMotor1_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "RightMotor_1", &rightMotor1_Handle, simx_opmode_oneshot_wait);

	simxGetObjectHandle(clientID, "RobotFrame0", &robot2_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "LeftMotor_2", &leftMotor2_Handle, simx_opmode_oneshot_wait);
	simxGetObjectHandle(clientID, "RightMotor_2", &rightMotor2_Handle, simx_opmode_oneshot_wait);
	
	simxGetObjectHandle(clientID, "Bola", &BolaHandle, simx_opmode_oneshot_wait);
	// simxGetObjectHandle(clientID, "Obstaculo", &obstaculo_handle, simx_opmode_oneshot_wait);


	// printf("RobotFrame: %d\n", ddRobotHandle);
	// printf("LeftMotor: %d\n", leftMotorHandle);
	// printf("RightMotor: %d\n", rightMotorHandle);
	// printf("GraphOdometry: %d\n", graphOdometryHandle);
}

void pega_pos() {

	simxInt ret = simxGetObjectPosition(clientID, robot1_Handle, -1, pos_robot1, simx_opmode_oneshot_wait);
	if (ret > 0) {
		printf("Error reading robot_1 position\n");
		return;
	}

	ret = simxGetObjectPosition(clientID, robot2_Handle, -1, pos_robot2, simx_opmode_oneshot_wait);
	if (ret > 0) {
		printf("Error reading robot_2 position\n");
		return;
	}

	ret = simxGetObjectOrientation(clientID, robot1_Handle, -1, orientation_robot1, simx_opmode_oneshot_wait);
	if (ret > 0) {
		printf("Error reading robot_1 orientation\n");
		return;
	}
	pos_robot1[2] = orientation_robot1[2];

	ret = simxGetObjectOrientation(clientID, robot2_Handle, -1, orientation_robot2, simx_opmode_oneshot_wait);
	if (ret > 0) {
		printf("Error reading robot_2 orientation\n");
		return;
	}
	pos_robot2[2] = orientation_robot2[2];

	simxInt ret_eh = simxGetObjectPosition(clientID, BolaHandle, -1, fieldBall, simx_opmode_oneshot_wait);
	if (ret_eh>0) {
		printf("Error reading ball position\n");
	}

	// ret_eh = simxGetObjectPosition(clientID, obstaculo_handle, -1, obstaculo, simx_opmode_oneshot_wait);
	// if (ret_eh>0) {
	//     printf("Error reading obstaculo position\n");
	// }
}


int main(int argc, char* argv[]) {
	//Objetivo inicializado
	objetivo[0] = 0;
	objetivo[1] = 0;
	objetivo[2] = 0;

	srand(time(NULL));

	std::string ipAddr = "127.0.0.1";//10.0.2.2";
	int portNb = 19997;//1999;

	if (argc>1) {
		ipAddr = argv[1];      
	}

	printf("Iniciando conexao com: %s...\n", ipAddr.c_str());

	clientID = simxStart((simxChar*) (simxChar*) ipAddr.c_str(), portNb, true, true, 2000, 5);
	int count = 0, goals = 0;

	while (clientID != -1) {
		printf("\n\nconnecting for the %dnd time...\n\n", count++);
		// Setup the handlers
		conexao();
		
		simxFloat start_ball[] = {0, 0, 0};
		simxFloat start_robot1[] = {1.7, 0, 0};
		simxFloat start_robot2[] = {-1.7, 0, 0};

		// simxSetObjectPosition(clientID, BolaHandle, ABSOLUTE_POSITION, start_ball, simx_opmode_oneshot);
		simxSetObjectPosition(clientID, robot1_Handle, ABSOLUTE_POSITION, start_robot1, simx_opmode_oneshot);
		simxSetObjectPosition(clientID, robot2_Handle, ABSOLUTE_POSITION, start_robot2, simx_opmode_oneshot);
		

		//start simulation
		int ret = simxStartSimulation(clientID,simx_opmode_oneshot_wait);

		int reset_ct = 0;
		//Loop da simulacao
		while (simxGetConnectionId(clientID) != -1) {
			//Posicoes referentes ao robo, bola e obstaculo serao armazenadas nos respectivos vetores (float)
			// Robo: 		(eixo x) position[0], (eixo y) position[1] e (angulo) position[2]
			// Bola: 		(eixo x) fieldBall[0], (eixo y) fieldBall[1]
			// Obstaculo: 	(eixo x) obstaculo[0], (eixo y) obstaculo[1]
			pega_pos();
			
			printf("Robô 1: x = %lf; y = %lf; angle = %f\n", pos_robot1[0], pos_robot1[1], pos_robot1[2]);
			printf("Robô 2: x = %lf; y = %lf; angle = %f\n", pos_robot2[0], pos_robot2[1], pos_robot2[2]);
			printf("Bola: x = %lf; y = %lf;\n", fieldBall[0], fieldBall[1]);
			printf("Obstaculo: x = %lf; y = %lf;\n", obstaculo[0], obstaculo[1]);
			
			// Goal situations
			if((fieldBall[0] <= -0.73750 || fieldBall[0] >= 0.73750) && 
				fieldBall[1] <= 0.21250 && fieldBall[1] >= -0.21250) {
				printf("\n\n\nGOL!!!!\n\n\n");
				simxStopSimulation(clientID, simx_opmode_oneshot);
				break;
			}
 
			//Implemente ou chame sua funcao aqui ***************************inicio

			//******************************************************************fim
			
			
			//Envia velocidade para os motores do robo
			// simxSetJointTargetVelocity(clientID, leftMotorHandle, 20, simx_opmode_oneshot); // Send speed to left motor
			// simxSetJointTargetVelocity(clientID, rightMotorHandle, 0, simx_opmode_oneshot); // Send speed to right motor
			
			// Robot 1
			simxSetJointTargetVelocity(clientID, leftMotor1_Handle, rand()%20, simx_opmode_oneshot); // Send speed to left motor
			simxSetJointTargetVelocity(clientID, rightMotor1_Handle, rand()%20, simx_opmode_oneshot); // Send speed to right motor

			// Robot 2
			simxSetJointTargetVelocity(clientID, leftMotor2_Handle, rand()%20, simx_opmode_oneshot); // Send speed to left motor
			simxSetJointTargetVelocity(clientID, rightMotor2_Handle, rand()%20, simx_opmode_oneshot); // Send speed to right motor

			extApi_sleepMs(2);
			if(reset_ct++ == 100) {
				simxStopSimulation(clientID, simx_opmode_oneshot);
				break;
			}
		}
		if(simxGetConnectionId(clientID) == -1) {
			simxPauseSimulation(clientID, simx_opmode_oneshot_wait);
			simxFinish(clientID);
			printf("Error on restart!");
			break;
		}
	} 
	// else printf("Nao foi possivel conectar.\n");
	return (0);
}
