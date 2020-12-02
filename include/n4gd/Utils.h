//
// Created by kozlov-an on 02.12.2020.
//

#ifndef N4GD_UTILS_H
#define N4GD_UTILS_H

#if _WIN32

#include "synchapi.h"
void Wait(float seconds)
{
    Sleep((int)(seconds * 1000.0f));
}

#else

#include <unistd.h>
	void wait( float seconds ) { usleep( (int) ( seconds * 1000000.0f ) ); }

#endif

#endif //N4GD_UTILS_H
