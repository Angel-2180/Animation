// AnimationProgramming.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Engine.h"
#include "MySimulation.hpp"

int main()
{
    Simulation simulation;
    Run(&simulation, 1400, 800);

    return 0;
}