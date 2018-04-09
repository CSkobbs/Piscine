//#include "dynamique.h"
#include <iostream>

#ifndef PAS
#define PAS 0.01
#endif

#ifndef REPRO
#define REPRO 10
#endif

#ifndef PREDATION
#define PREDATION 0.5
#endif

#ifndef MORTALITE

#define MORTALITE 2.5
#endif

#ifndef EFFICATITE
#define EFFICATITE 0.7
#endif

#ifndef N_PROIE
#define N_PROIE 20
#endif

#ifndef P_PREDA
#define P_PREDA 5
#endif

int main(int argc, char const *argv[])
{
	int pop_proie = N_PROIE;
	int pop_preda = P_PREDA;

	for (int i = 0; i < 20; ++i)
	{
		pop_proie = pop_proie + i*PAS*(REPRO*pop_proie - EFFICATITE*pop_proie*pop_preda);
		pop_preda = pop_preda + i*PAS*(-MORTALITE*pop_preda + EFFICATITE*PREDATION*pop_preda*pop_proie);
		std::cout << "pop_proie : "<< pop_proie << std::endl; 
		std::cout << "pop_preda : "<< pop_preda << std::endl;
		std::cout << "----------------"<< std::endl;
	}

	return 0;

}