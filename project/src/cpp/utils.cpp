// this file contains all of the functions that are used
// by every other files, such as the \texttt{commandFan}
// or \texttt{readGyro} functions.

#ifndef HAVE_UTILS
#define HAVE_UTILS

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <sys/time.h>
//#include <string>
//#include "dscud.h"
#include "da.h"
#include <dscud.h>

#include "read_gyro.h"

using namespace std;

// makes the fan running at a desired thrust
int commandFan(double desiredThrust)
{
    unsigned short output_code;
    //double xblahblah;

    // Initialize Prometheus hardware
    //if (da_init()) return(-1);


    // Set thrust levels
    if (desiredThrust >= 0.0) {
      da_write(1, 0);  // Zero negative motor
      output_code = (unsigned short) (4095*(desiredThrust/10.0));
      da_write(3, output_code);
    } else { 
      da_write(3, 0);  // Zero positive motor
      output_code = (unsigned short) (4095*(-desiredThrust/10.0));
      da_write(1, output_code);
    }

    return(0);
}

// returns the sensors info 
SensorReadings_t readGyro(DSCSAMPLE *samples,
                          DSCB dscb,
                          DSCADSCAN dscadscan,
                          struct timeval start_time)
{
    struct timeval cur_time;
    float nowtime;
    BYTE result;
    SensorReadings_t SensorReadings;

    /* Start initial A/D Scan */
	if( (result = dscADScan( dscb, &dscadscan, samples) )!= DE_NONE)
	{
		fprintf(stderr, "%s%s\n", ERROR_PREFIX, dscGetErrorString(result) );
		free(samples);
	}
    
    gettimeofday(&cur_time, NULL);
    nowtime = ((float) (cur_time.tv_sec - start_time.tv_sec))
             +  (((float) (cur_time.tv_usec - start_time.tv_usec))/1.0E6);

    SensorReadings.TimeStamp = nowtime;
    SensorReadings.GyroVolts =  
      (double) (((short) dscadscan.sample_values[0] + 32768)/65536.0 * 5.0);
    SensorReadings.MagXVolts =  
      (double) dscadscan.sample_values[1];
    SensorReadings.MagYVolts =  
      (double) dscadscan.sample_values[2];
    SensorReadings.MagZVolts =  
      (double) dscadscan.sample_values[3];
    SensorReadings.CSS1Volts =  
      (double) dscadscan.sample_values[4];
    SensorReadings.CSS2Volts =  
      (double) dscadscan.sample_values[5];
    SensorReadings.CSS3Volts =  
      (double) dscadscan.sample_values[6];
    SensorReadings.CSS4Volts =  
      (double) dscadscan.sample_values[7];
    
    return SensorReadings;
}

// saves a vector of data into a txt file
void saveData(string filename,
              vector< vector<double> > data,
              bool saveAll,
              vector<int> indices)
{
    ofstream f;
    f.open(filename.c_str());
    
    int JMAX;

    JMAX = saveAll?10:3;
    for (int i=indices[0] ; i<indices[1] ; ++i)
    {
        for (int j = 0 ; j<JMAX ; ++j)
        {
            f << data[i][j] << " ";
        }
        f << "\n";
    }

    f.close();
}

void monitorData(vector< vector<double> > *data,
                 double timer,
                 int f,                      // frequence of data collection
                 double thrust,
                 struct timeval start_time,
                 DSCSAMPLE *samples,
                 DSCB dscb,
                 DSCADSCAN dscadscan)
{
    vector<double> dataLine(10);

    SensorReadings_t sens;
    int tt = 1000/f-2,
        t = 0;


    while (t < timer*1000)
    {
        sens = readGyro(samples, dscb, dscadscan, start_time);
        dataLine[0] = sens.TimeStamp ;
        dataLine[1] = thrust;
        dataLine[2] = sens.GyroVolts ;
        dataLine[3] = sens.CSS1Volts ;
        dataLine[4] = sens.CSS2Volts;
        dataLine[5] = sens.CSS3Volts ;
        dataLine[6] = sens.CSS4Volts ;
        dataLine[7] = sens.MagXVolts ;
        dataLine[8] = sens.MagYVolts ;
        dataLine[9] = sens.MagZVolts ;
        data->push_back(dataLine);

        t += tt;
        dscSleep(tt);
    }
}

#endif
