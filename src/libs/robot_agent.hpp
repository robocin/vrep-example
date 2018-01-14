#ifndef __ROBOT_AGENT
#define __ROBOT_AGENT

#include <string>
#include <vector>
#include "simulation_group.hpp"

extern "C" {
#include "extApi.h"
	/*  #include "extApiCustom.h" if you wanna use custom remote API functions! */
}

class RobotAgent {
public:
	int team;
	string robot_name;
	string leftMotor_name;
	string rightMotor_name;
	simxInt robot_handler;
	simxInt leftMotor_handler;
	simxInt rightMotor_handler;
	simulationGroup game;

	/**
	 * @brief      Setup a robot agent without any instaciation.
	 */
	RobotAgent();
	
	/**
	 * @brief      Setup the robot agent with everything necessary.
	 *
	 * @param[in]  robot_name  The robot name.
	 * @param[in]  leftMotor   The left motor.
	 * @param[in]  rightMotor  The right motor.
	 * @param[in]  game        The game.
	 */
	RobotAgent(string robot_name, string leftMotor, string rightMotor, simulationGroup game);

	/**
	 * @brief      Sets the motors.
	 *
	 * @param[in]  leftMotor   The left motor.
	 * @param[in]  rightMotor  The right motor.
	 */
	void set_motors(string leftMotor, string rightMotor);

	/**
	 * @brief      Sets the robot.
	 *
	 * @param[in]  robot_name  The robot name.
	 */
	void set_robot(string robot_name);

	/**
	 * @brief      Sets the team.
	 *
	 * @param[in]  team_side  The team side
	 */
	void set_team(int team_side);

	/**
	 * @brief      Sets the game simulation.
	 *
	 * @param[in]  game  The game simulation.
	 */
	void set_game(simulationGroup game);

	/**
	 * @brief      Gets the position.
	 *             
	 * @return     The position.
	 */
	Position get_position();

	/**
	 * @brief      Sets the power for the motors of the robot.
	 *
	 * @param[in]  left_power   The left power.
	 * @param[in]  right_power  The right power.
	 */
	void set_power(int left_power, int right_power);
};


#endif