// this file relies on the PID controller. It also stabilizes at a desired
// speed for 30s before ramping down. It outputs a file \texttt{tsat\_data.txt}
// which gathers all of the sensor data along with the corresponding thrust
// command at each time step during the stabilization phase. The time step is
// 5ms.

#ifndef HAVE_DAQ
#define HAVE_DAQ

#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
#include "dscud.h"
//#include <dscud.h>


#include "utils.cpp"
#include "rate_control.cpp"


#define FILENAME "tsat_data.txt"
#define ALLSAVE 1


int main(int argc, char* argv [])
{
    double Kp, Ki, Kd;
    if (argc == 1)
    {
        Kp = 0.013;
        Ki = 0.00002;
        Kd = 0.22;
    }
    else if (argc == 4)
    {
        Kp = atof(argv[1]);
        Ki = atof(argv[2]);
        Kd = atof(argv[3]);
    }
    else
    {
        printf("Invalid number of argument.");
        return 0;
    }

    DSCSAMPLE *samples;
    DSCB dscb;
    DSCADSCAN dscadscan;
    AthenaInit(&samples, &dscb, &dscadscan);

    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    vector< vector<double> > data = PID(3.5, 200, start_time, samples, dscb, dscadscan, Kp, Ki, Kd);
    rampDown(data, 200, start_time, samples, dscb, dscadscan);

    //vector< vector<double> > data = PID(GV, 200, start_time, samples, dscb, dscadscan, Kp, Ki, Kd);
    //rampDown(data, 200, start_time, samples, dscb, dscadscan);
    
    return 0;
}

#endif
