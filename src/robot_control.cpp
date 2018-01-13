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

extern "C" {
#include "extApi.h"
    /*  #include "extApiCustom.h" if you wanna use custom remote API functions! */
}

simxInt BolaHandle; 
simxInt ddRobotHandle;
simxInt leftMotorHandle;
simxInt rightMotorHandle;
simxInt sensorHandle;
simxInt graphOdometryHandle;
simxInt sensorLHandle, sensorFHandle, sensorRHandle, uiHandle;
simxFloat position[3];
simxFloat orientation[3];
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

#define ini_x 1.7
#define ini_y 1.3

using namespace std;

inline double to_deg(double radians)
{
    return radians * (180.0 / M_PI);
}

double to180range(double angle)
{
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

double to_positive_angle(double angle)
{

    angle = fmod(angle, 2 * M_PI);
    while (angle < 0)
    {
        angle = angle + 2 * M_PI;
    }
    return angle;
}

double smallestAngleDiff(double target, double source)
{
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

void conexao(){
    printf("Conexao efetuada\n");
    simxGetObjectHandle(clientID, "RobotFrame", &ddRobotHandle, simx_opmode_oneshot_wait);
    simxGetObjectHandle(clientID, "LeftMotor", &leftMotorHandle, simx_opmode_oneshot_wait);
    simxGetObjectHandle(clientID, "RightMotor", &rightMotorHandle, simx_opmode_oneshot_wait);
    simxGetObjectHandle(clientID, "Bola", &BolaHandle, simx_opmode_oneshot_wait);
    simxGetObjectHandle(clientID, "Obstaculo", &obstaculo_handle, simx_opmode_oneshot_wait);

    simxGetObjectHandle(clientID, "LeftMotor0", &leftMotorHandle_1, simx_opmode_oneshot_wait);
    simxGetObjectHandle(clientID, "RightMotor0", &rightMotorHandle_1, simx_opmode_oneshot_wait);

    // printf("RobotFrame: %d\n", ddRobotHandle);
    // printf("LeftMotor: %d\n", leftMotorHandle);
    // printf("RightMotor: %d\n", rightMotorHandle);
    // printf("GraphOdometry: %d\n", graphOdometryHandle);

    //start simulation
    int ret = simxStartSimulation(clientID,simx_opmode_oneshot_wait);
}

void pega_pos(){

    simxInt ret = simxGetObjectPosition(clientID, ddRobotHandle, -1, position, simx_opmode_oneshot_wait);
    if (ret>0) {
        printf("Error reading robot position\n");
        return;
    }
    
    ret = simxGetObjectOrientation(clientID, ddRobotHandle, -1, orientation, simx_opmode_oneshot_wait);
    if (ret>0) {
        printf("Error reading robot orientation\n");
        return;
    }
    position[2] = orientation[2];

    simxInt ret_eh = simxGetObjectPosition(clientID, BolaHandle, -1, fieldBall, simx_opmode_oneshot_wait);
    if (ret_eh>0) {
        printf("Error reading ball position\n");
    }

    ret_eh = simxGetObjectPosition(clientID, obstaculo_handle, -1, obstaculo, simx_opmode_oneshot_wait);
    if (ret_eh>0) {
        printf("Error reading obstaculo position\n");
    }
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
    if (clientID != -1) {
        conexao();
        int count = 0;
        //Loop da simulacao
        while (simxGetConnectionId(clientID) != -1) {
            //Posicoes referentes ao robo, bola e obstaculo serao armazenadas nos respectivos vetores (float)
            // Robo: 		(eixo x) position[0], (eixo y) position[1] e (angulo) position[2]
            // Bola: 		(eixo x) fieldBall[0], (eixo y) fieldBall[1]
            // Obstaculo: 	(eixo x) obstaculo[0], (eixo y) obstaculo[1]
            pega_pos();
            
            printf("Robô: x = %lf; y = %lf; angle = %f\n", position[0], position[1], position[2]);
            printf("Bola: x = %lf; y = %lf;\n", fieldBall[0], fieldBall[1]);
            printf("Obstaculo: x = %lf; y = %lf;\n", obstaculo[0], obstaculo[1]);
 
            //Implemente ou chame sua funcao aqui ***************************inicio

            //******************************************************************fim
            
            
            //Envia velocidade para os motores do robo
            // simxSetJointTargetVelocity(clientID, leftMotorHandle, 20, simx_opmode_oneshot); // Send speed to left motor
            // simxSetJointTargetVelocity(clientID, rightMotorHandle, 0, simx_opmode_oneshot); // Send speed to right motor
            
            // Robot 1
            simxSetJointTargetVelocity(clientID, leftMotorHandle, rand()%20, simx_opmode_oneshot); // Send speed to left motor
            simxSetJointTargetVelocity(clientID, rightMotorHandle, rand()%20, simx_opmode_oneshot); // Send speed to right motor

            // Robot 2
            simxSetJointTargetVelocity(clientID, leftMotorHandle_1, rand()%20, simx_opmode_oneshot); // Send speed to left motor
            simxSetJointTargetVelocity(clientID, rightMotorHandle_1, rand()%20, simx_opmode_oneshot); // Send speed to right motor

            extApi_sleepMs(2);
        }
        simxPauseSimulation(clientID, simx_opmode_oneshot_wait);
        simxFinish(clientID);
    } else
        printf("Nao foi possivel conectar.\n");
    return (0);
}
