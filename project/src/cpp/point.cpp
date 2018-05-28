// this file relies on \texttt{calibration.cpp} and implements a bang-bang
// controller allowing the user to make the satellite point to a desired
// heading.

#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <vector>
#include <string>

#include <dscud.h>

#include "utils.cpp"
#include "calibration.cpp"

#define TOL_D 5
#define GV0 2.37
#define TOL 0.1

using namespace std;

// starts an impulse in the direction eps for a duration of timer in ms
void impulse(int eps, int timer)
{
    commandFan(eps*10.);
    dscSleep(timer);
}

// outputs a filtered sensor reading
vector<double> filter(int n, vector<double> sens, SensorReadings_t sensor, DSCSAMPLE *samples, DSCB dscb, DSCADSCAN dscadscan)
{
    double mag1Temp = 0.,
           mag2Temp = 0.,
           mag3Temp = 0.; 
    struct timeval t ;
    for (int i=0 ; i<n ; ++i)
    {
        sensor = readGyro(samples, dscb, dscadscan, t);
        dscSleep(10);
        mag1Temp += sensor.MagXVolts;
        mag2Temp += sensor.MagYVolts;
        mag3Temp += sensor.MagZVolts;
    }
    sens[0] = mag1Temp/n ; sens[1] = mag2Temp/n ; sens[2] = mag3Temp/n;
    return sens;
}

// stops tablesat and waits until the sped is 0
void stop(DSCSAMPLE *samples, DSCB dscb, DSCADSCAN dscadscan)
{
    commandFan(0);
    struct timeval t ;
    double currentGV = (readGyro(samples, dscb, dscadscan, t)).GyroVolts;
    while (currentGV < GV0-TOL || currentGV > GV0 +TOL)
    {
        dscSleep(1000);
        currentGV = (readGyro(samples, dscb, dscadscan, t)).GyroVolts;
    }
    dscSleep(4000);
}

// implements a bang bang controller to get to the desired heading
void control(double expectedDeg, int t,
             DSCSAMPLE *samples, DSCB dscb, DSCADSCAN dscadscan)
{
    double currentDeg,
           currentGV,
           error,
           errorTemp=0.,
           abs1,abs2, abs3;
    int eps=1,
        timer1 = t,
        timer2 = t/2,
        timer=t,
        n = 100;
    vector<double> sens(6);
    sens = degToSens(0);
    vector< vector<double> > samp = sampleDeg();
    SensorReadings_t sensor;

    
    //impulse(1, 10000);
    stop(samples, dscb, dscadscan);
    sens = filter(n, sens, sensor, samples, dscb, dscadscan);

    currentDeg = sensToDeg(sens, samp);
    error = expectedDeg - currentDeg;
    printf("Current Heading = %lf\n", currentDeg);

    while (error < -TOL_D || error > TOL_D)
    {

        // reduces the impulse if we approach the desired value
        timer = (abs(error)<30)?timer2:timer1;

        impulse(eps, timer);
        stop(samples, dscb, dscadscan);
        sens = filter(n, sens, sensor, samples, dscb, dscadscan);

        printf("-----------------------------------\n");

        currentDeg = sensToDeg(sens,samp);
        printf("Mag1 = %lf ; Mag2 = %lf ; Mag3 = %lf\n", sens[0], sens[1], sens[2]);
        printf("Current Heading = %lf\n", currentDeg);
        if (currentDeg != -11.)
        {
            errorTemp = error;
            abs1 = abs(expectedDeg - currentDeg);
            abs2 = abs(expectedDeg - currentDeg-360);
            abs3 = abs(expectedDeg - currentDeg+360);
            if (abs1 < abs2 && abs1 < abs3)
            {
                error = expectedDeg - currentDeg;
            }
            else if (abs2 < abs1 && abs2 < abs3)
            {
                error = expectedDeg - currentDeg-360;
            }
            else
            {
                error = expectedDeg - currentDeg+360;
            }
            //error = expectedDeg-currentDeg;

            // if the error grows we change the orientation of the impulse
            eps = ((abs(errorTemp) > abs(error)) &&
                   (error*errorTemp > 0))?eps:-eps;
            //eps = ((abs(error) > abs(errorTemp)) &&
                   //(error*errorTemp > 0))?-eps:eps;
            printf("Error = %lf\n", error);
        }
    }

    sens = filter(n, sens, sensor, samples, dscb, dscadscan);
    currentDeg = sensToDeg(sens,samp);

    printf("Final Heading = %lf\n", currentDeg);
}


int main(int argc, char* argv[])
{

    int t = 1300;
    if (argc ==2)
    {
        t = atoi(argv[1]);
    }
    DSCSAMPLE *samples;
    DSCB dscb;
    DSCADSCAN dscadscan;
    AthenaInit(&samples, &dscb, &dscadscan);

    double desiredDeg = 0.;

    printf("Enter desired degree (0 to 359.999):\n");
    scanf("%lf", &desiredDeg);
    if (desiredDeg < 0 || desiredDeg > 359.999)
    {
        printf("Invalid input.");
    }
    else
    {
        control(desiredDeg,t, samples, dscb, dscadscan);
    }
    return 0;
}
