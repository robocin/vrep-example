# RoboCIMulator
This repository contains a simulator of the game of soccer Very Small Size in a simulation platform called VREP. To be able to use this software you must have installed the VREP Simulator software and also the CMake for build the application. This software contains a set of tests for testing and verify the functionality of the libs built to create the simulations. The libs are built to help you to build your own simulation are plenty documented in the .hpp files. If you want to copy paste an example and use it to build your own applications, copy the file in src/tests/test_robotagent.cpp, it will allow you to easy modify the software to insert your own agent functions.
The used classes are found in src/libs and the tests in src/tests.

## Running the tests:
1. Open VREP software
2. Open the file 1x1.ttt
3. Go to the folder build/
4. Execute the command "cmake .."
5. Execute the command make
6. Execute ./test_agent or ./test_sim
7. Feel free to build your own software

In this moment, for easy development use the src/tests/test_robotagent.cpp as a reference and insert your own agent software for the game.

Reference:
> # V-REP Example  
Example of communication with VREP using C++ and a [IEEE Very Small Size Soccer](http://> www.cbrobotica.org/?page_id=81&lang=pt) scenario.  
>  
> ## Usage  
> 1. Run [V-REP](http://www.coppeliarobotics.com/).  
> 2. Open `Cenario.ttt` on V-REP.  
> 3. Go to `src` folder and run `make`.  