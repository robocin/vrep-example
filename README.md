# RoboCIMulator
Esse repositório é um simulador de jogo de robôs construído sobre um software denominado VREP, para poder utilizar o software você necessita instalar o VREP Simulator em sua máquina e o CMake. Esse software contém um conjunto de testes para verificar a funcionalidade e a documentação das bibliotecas se encontra em src/libs/.

## Rodando os testes:
1. Abrir o VREP
2. Abrir o 1x1.ttt
3. Ir para build/
4. Executar o comando "cmake .."
5. Executar o comando make
6. Executar ./test_agent ou ./test_sim

No atual momento a biblioteca tem como exemplo principal o src/tests/test_robotagent.cpp
Para um fácil desenvolvimento copie esse arquivo e modifique ele adicionando seus próprios agentes no jogo

Referência:
> # V-REP Example  
Example of communication with VREP using C++ and a [IEEE Very Small Size Soccer](http://> www.cbrobotica.org/?page_id=81&lang=pt) scenario.  
>  
> ## Usage  
> 1. Run [V-REP](http://www.coppeliarobotics.com/).  
> 2. Open `Cenario.ttt` on V-REP.  
> 3. Go to `src` folder and run `make`.  