/* 
**  File:   simtime.h
**  Author: Maurizio Munafo'
**  Description: Definizioni del tipo per il tempo di simulazione
*/


#ifndef SIMTIME_H
#define SIMTIME_H

typedef double            Time;

#ifdef __linux__

#include <float.h>
#define MAX_TIME	DBL_MAX

#else

#define MAX_TIME	1e+308	

#endif 

#endif /* SIMTIME_H */
