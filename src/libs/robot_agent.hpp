#ifndef __ROBOT_AGENT
#define __ROBOT_AGENT

class robotAgent {
public:
	robotAgent();
	robotAgent(simulationGroup game);
	~robotAgent();
	
	set_handlers();
	set_goal();
	get_positions();
	set_velocities();
};


#endif