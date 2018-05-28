// this file implements the PID controller used to stabilize
// the rotational rate at a desired value for 30s before
// ramping down. At the end, it outputs a file
// \texttt{rate_data.txt} containing the time, thrust command
// and gyro rate at each time step during the entire program's
// run. The time step is 10ms.

#ifndef HAVE_RC
#define HAVE_RC


#include <stdio.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <time.h>
//#include "dscud.h"
#include <dscud.h>


#include "utils.cpp"

#define GV0 2.4

#ifndef HAVE_DAQ
#define TOL 0.1
#define FILENAME "rate_data.txt"
#define ALLSAVE  0
#endif

#ifdef HAVE_DAQ
#define TOL 0.1
#define FILENAME "tsat_data.txt"
#define ALLSAVE 1
#endif

using namespace std;

// gets an input in GV from the user
double inGyroVolt()
{
    double desiredGV = 0.;

    printf("Enter desired gyro rate (0 to 5V):\n");
    scanf("%lf", &desiredGV);
    if (desiredGV < 0. || desiredGV > 5.)
    {
        printf("Invalid input.");
        desiredGV = -1;
    }
    return desiredGV;
}

// PID controller to reach the desired speed
vector< vector<double> > PID(double desiredGV,
                             int f,
                             struct timeval start_time,
                             DSCSAMPLE *samples, DSCB dscb, DSCADSCAN dscadscan,
                             double Kp, double Ki, double Kd)
{
    vector< vector<double> > data;
    vector<double>           dataLine(10);

    SensorReadings_t sens = readGyro(samples, dscb, dscadscan, start_time);
    double effectiveGV = 0.,
           thrust;
    double error,
           error_sum = 0.,
           error_diff,
           errTemp = 0.;
    vector<int> stab(2);
    int    eps;
    time_t t1,t2;
    bool   stabilized   = 0,
           stabZone     = 0;

    // initialization of the thrust
    eps = ((desiredGV > GV0) ? -1 : 1);
    thrust = eps*7.5;
    commandFan(thrust);
    // monitoring for 10s until we get some stabilization
    monitorData(&data, 30, f, thrust, start_time, samples, dscb, dscadscan);
    effectiveGV = (readGyro(samples, dscb, dscadscan,start_time)).GyroVolts;

    while (!stabilized)
    {
        if (stabZone)
        {
            t2 = time(NULL);
        }
        printf("--------------------------\n");
        printf("Thrust = %lf ; EffectiveGV = %.2f\n", thrust, effectiveGV);
        error = desiredGV - effectiveGV;
        error_sum += error;
        error_diff = error - errTemp;
        errTemp = error;
        printf("Prop = %lf ; Integ = %lf ; Diff = %lf \n", Kp*error, Ki*error_sum, Kd*error_diff);
        thrust = thrust - Kp * error - Ki * error_sum - Kd * error_diff;
        // in case of saturation
        thrust = (eps*thrust>10)?eps*10:thrust;
        printf("Thrust command = %lf\n", thrust);
        commandFan(thrust);
        monitorData(&data, 1, f, thrust, start_time, samples, dscb, dscadscan);
        effectiveGV = (readGyro(samples, dscb, dscadscan,start_time)).GyroVolts;
        // checks if the velocity has stabilized at desiredGV
        if (effectiveGV < desiredGV + TOL && effectiveGV > desiredGV - TOL) 
        {
            if (!stabZone)
            {
                stabZone = 1;
                printf("*******************************\n");
                printf("Entering stabilization phase.\n");
                printf("*******************************\n");
                stab[0] = data.size();
                t1 = time(NULL);
            }
            // maintain speed for 30s
            if (difftime(t2,t1) > 30) 
            {
                printf("Stabilization successfull.\n");
                stab[1] = data.size();
                printf("Starting step = %d ; "
                       "Stopping step = %d\n", stab[0], stab[1]);
                stabilized = 1;
            }
        }
        else
        {
            if (stabZone)
            {
                printf("Stabilization failure. Re-entering control loop.\n");
            }
            stabZone = 0;
            t2 = time(NULL);
        }
    }

    if (ALLSAVE)
    {
        saveData(FILENAME, data, ALLSAVE, stab);
    }

    return data;
}

void rampDown(vector< vector<double> > data,
              int f,
              struct timeval start_time,
              DSCSAMPLE *samples, DSCB dscb, DSCADSCAN dscadscan)
{
    SensorReadings_t sens = readGyro(samples, dscb, dscadscan,start_time);
    double effectiveGV = sens.GyroVolts;
    
    // at t=0, we stop the fan
    printf("Entering ramp down phase.\n");
    commandFan(0.);

    // we monitor the data as long as the satellite is moving
    while (effectiveGV > GV0 + TOL || effectiveGV < GV0 - TOL)
    {
        monitorData(&data, 1., f, 0., start_time, samples, dscb, dscadscan);
        effectiveGV = (readGyro(samples, dscb, dscadscan,start_time)).GyroVolts;
    }

    dscFree();

    vector<int> stab (2);
    stab[0] = 0 ; stab[1] = data.size()-1;

    if (!ALLSAVE)
    {
        saveData(FILENAME, data, ALLSAVE, stab);
    }
}

#ifndef HAVE_DAQ
int main(int argc, char *argv[])
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

    double gV = inGyroVolt();
    if (gV != -1.)
    {
        struct timeval start_time;
        gettimeofday(&start_time, NULL);

        vector< vector<double> > data = PID(gV, 100, start_time, samples, dscb, dscadscan, Kp, Ki, Kd);
        rampDown(data, 100, start_time, samples, dscb, dscadscan);
    }
    return 0;
}
#endif
#endif
