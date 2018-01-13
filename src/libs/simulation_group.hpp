#ifndef __SIMULATION_GROUP
#define __SIMULATION_GROUP

#define ABSOLUTE_POSITION -1


#include <vector>
#include <string>

extern "C" {
#include "extApi.h"
	/*  #include "extApiCustom.h" if you wanna use custom remote API functions! */
}

#define _GOAL_A_BEGIN_X -0.73750
#define _GOAL_A_END_X   -0.86250
#define _GOAL_A_BEGIN_Y  0.21250
#define _GOAL_A_END_Y   -0.21250
#define _GOAL_B_BEGIN_X  0.73750
#define _GOAL_B_END_X    0.86250
#define _GOAL_B_BEGIN_Y  0.21250
#define _GOAL_B_END_Y   -0.21250

using namespace std;

typedef struct Position {
	simxFloat x; // x position
	simxFloat y; // y position
	simxFloat z; // z position
	simxFloat t; // theta angle
} Position;

class simulationGroup {
public:
	// Variables about the game.
	simxInt ball_handler;			// The ball handler.
	vector<simxInt> teamA_handler;	// All the team A robots handlers.
	vector<simxInt> teamB_handler;	// All the team B robots handlers.
	vector<string> teamA_names;		// All the team A robot names.
	vector<string> teamB_names;		// All the team B robot names.
	string ball_name;				// The ball name.
	int clientID;					// The client connection ID.

	/**
	 * @brief      Generate a new simulation instance without starting anything in the simulation.
	 */
	simulationGroup();
	
	/**
	 * @brief      Generate a new simulation instance and connects to VREP to prepare the field.
	 *
	 * @param[in]  teamA_names  The team a names.
	 * @param[in]  teamB_names  The team b names.
	 * @param[in]  ball_name    The ball name.
	 */
	simulationGroup(vector<string> teamA_names, vector<string> teamB_names, string ball_name);
	
	/**
	 * @brief      Gets the ball position.
	 *
	 * @return     The ball position.
	 */
	Position get_ballPos();
	
	/**
	 * @brief      Determines if goal.
	 *
	 * @return     1 if goal for team A, -1 if goal for team B, 0 otherwise.
	 */
	int is_goal();
	
	/**
	 * @brief      Connects to the VREP simulator.
	 *
	 * @param[in]  ip_addr  The ip address
	 * @param[in]  port     The port
	 * @param[in]  timeout  The timeout
	 *
	 * @return     The connection status, 0 if okay, -1 otherwise.
	 */
	int connect(string ip_addr, int port, int timeout = 2000);

	/**
	 * @brief      Defines the team A members in the actual simulation.
	 *
	 * @param[in]  teamA_names  The team a names
	 *
	 * @return     The setup status, 0 if okay, -1 otherwise.
	 */
	int set_teamA(vector<string> teamA_names);

	/**
	 * @brief      Defines the team B members in the actual simulation.
	 *
	 * @param[in]  teamB_names  The team b names
	 *
	 * @return     The setup status, 0 if okay, -1 otherwise.
	 */
	int set_teamB(vector<string> teamB_names);

	/**
	 * @brief      Defines the ball in the simulation
	 *
	 * @param[in]  ball_name  The ball name
	 *
	 * @return     The setup status, 0 if okay, -1 otherwise
	 */
	int set_ball(string ball_name);

	/**
	 * @brief      Gets the team b position.
	 *
	 * @return     The team b position.
	 */
	vector<Position> get_teamBPos();

	/**
	 * @brief      Gets the team a position.
	 *
	 * @return     The team a position.
	 */
	vector<Position> get_teamAPos();

	/**
	 * @brief      Start the simulation.
	 */
	void start();

	/**
	 * @brief      Stops the simulation.
	 */
	void stop();

	/**
	 * @brief      Pauses the simulation.
	 */
	void pause();

	/**
	 * @brief      Closes a com.
	 */
	void endCom();

	/**
	 * @brief      Setup the startup positions in the simulation
	 *
	 * @param      ballPos  The ball position
	 * @param[in]  teamA    The team a
	 * @param[in]  teamB    The team b
	 */
	void setup_startPos(vector<Position> teamA, vector<Position> teamB, Position ballPos);

};

#endif