// this file uses the MATLAB \texttt{magCalibration.txt} output file to get a
// numerical inverse of the curve fitting the magnetometer data.

#ifndef HAVE_CALIB
#define HAVE_CALIB

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <sys/time.h>
#include <cmath>


#define TOL_P 0.2
#define SAMPLING_STEP 0.1

using namespace std;

// returns [a,b] such that Y = aX + b
vector<double> linreg(vector<double> X, vector<double> Y)
{
    vector<double> coefs(2);

    int i,
        n = X.size();
    double xBar=0., x2Bar = 0.,
           yBar=0., y2Bar = 0.,
           xyBar=0.,
           sx=0.,
           sy=0.,
           rxy,a,b;

    for (i=0 ; i<n ; ++i)
    {
        xBar += X[i]; x2Bar += X[i]*X[i] ;
        yBar += Y[i]; y2Bar += Y[i]*Y[i] ;
        xyBar += X[i]*Y[i];
    }
    xBar /= n ; x2Bar /= n ; yBar/= n ; y2Bar/=n ; xyBar /=n ;

    for (i=0 ; i<n ; ++i)
    {
        sx += (X[i]-xBar)*(X[i]-xBar);
        sy += (Y[i]-yBar)*(Y[i]-yBar);
    }
    sx /= n ; sx = sqrt(sx) ;
    sy /= n ; sy = sqrt(sy) ;

    rxy = (xyBar-xBar*yBar) / (sqrt((x2Bar-xBar*xBar)*(y2Bar - yBar*yBar)));

    a = rxy * sy/sx;
    b = yBar - a*xBar;

    coefs[0] = a ; coefs[1] = b;

    return coefs;
}

double getMax(vector< vector<double> > data, int col)
{
    double max = data[0][col];

    for (int i=1 ; i<data.size() ; ++i)
    {
        if (data[i][col] > max)
        {
            max = data[i][col];
        }
    }

    return max;
}

// indices of the n last periodes for the column col of the data
vector<int> findPeriod(vector< vector<double> > data, int col, int nPeriod)
{
    vector<int> indices(2*(nPeriod + 1));

    double max = getMax(data, col);
    int dataSize = data.size(),
        j,k,
        temp    = 0,
        sizeMax = 0;

    for (j=dataSize-1 ; j>=0 ; --j)
    {
        if (data[j][col] < max + TOL_P*max && data[j][col] > max - TOL_P*max)
        {
            while ((data[j-sizeMax][col] < data[j][col] + TOL_P*max &&
                   data[j-sizeMax][col] > data[j][col] - TOL_P*max) ||
                   (data[j-sizeMax][col] < max + TOL_P*max &&
                   data[j-sizeMax][col] > max - TOL_P*max))
            {
                ++ sizeMax;
            }
            indices[temp] = j-sizeMax/2;
            ++ temp;
            j -= sizeMax;
            sizeMax = 0;
        }
        if (temp > 2*(nPeriod+1))
        {
            break;
        }
    }

    return indices;
}


// gets the calibration coefficients
vector< vector<double> > readFile(string file)
{
    vector< vector<double> > data;

    ifstream infile(file.c_str());
    string line;

    int temp;
    while(getline(infile, line))
    {
        vector<double> row(3);
        istringstream iss(line);

        for (int i=0 ; i<3 ; ++i)
        {
            if (!(iss >> row[i]))
            {
            }
        }
        if (iss >> temp)
        {
        }
        data.push_back(row);
    }

    return data;
}

// converts sensor information in a heading in degrees
// If it fails to find the corresponding heading, outputs -11
double sensToDeg(vector<double> sens, vector< vector<double> > samples)
{
    double deg = -11.;

    vector<double> degTemp;
    double expectedMag1 = sens[0],
           expectedMag2 = sens[1],
           expectedMag3 = sens[2],
           mag1Temp,
           mag2Temp,
           mag3Temp,
           tol1 = sens[3],
           tol2 = sens[4],
           tol3 = sens[5];
    bool   mag1InRange,
           mag2InRange,
           mag3InRange;

    for (int i=0 ; i<samples.size() ; ++i)
    {
        mag1Temp = samples[i][0];
        mag2Temp = samples[i][1];
        mag3Temp = samples[i][2];
        mag1InRange = (expectedMag1 < mag1Temp + tol1 &&
                       expectedMag1 > mag1Temp - tol1);
        mag2InRange = (expectedMag2 < mag2Temp + tol2 &&
                       expectedMag2 > mag2Temp - tol2);
        mag3InRange = (expectedMag3 < mag3Temp + tol3 &&
                       expectedMag3 > mag3Temp - tol3);
        if (int(mag1InRange) + int(mag2InRange) + int(mag3InRange) >= 2)
        {
            degTemp.push_back(i*SAMPLING_STEP);
        }
    }

    if (degTemp.size()>1)
    {
        deg = 0.;
        for (int i=0 ; i<degTemp.size() ; ++i)
        {
            deg += degTemp[i];
        }
        deg /= degTemp.size();
    }

    return deg;
}


vector<double> degToSensFour(double deg)
{
    vector<double> sens(6);

    vector< vector<double> > coefs = readFile("magCalibration.txt");
    double p1 = coefs[0][0],
           p2 = coefs[0][1],
           X = coefs[1][0],
           T = coefs[1][1],
           tol1 = coefs[2][0],
           tol2 = coefs[2][1],
           tol3 = coefs[2][2],
           x = X+T*deg/360. + T;
    double mag1 = coefs[2][0],
           mag2 = coefs[2][1];
    double a1,b1, a2,b2;

    for (int i=0 ; i<8 ; ++i)
    {
        a1 = coefs[2*i+3][0] ; b1 = coefs[2*i+4][0] ;
        a2 = coefs[2*i+3][1] ; b2 = coefs[2*i+4][1] ;
        mag1 += a1*cos(p1*x*(i+1)) + b1*sin(p1*x*(i+1));
        mag2 += a2*cos(p2*x*(i+1)) + b2*sin(p2*x*(i+1));
    }

    sens[0] = mag1 ; sens[1] = mag2 ;

    return sens;
}

vector<double> degToSensSin(double deg)
{
    vector<double> sens(6);

    vector< vector<double> > coefs = readFile("magCalibration.txt");
    double X = coefs[1][0],
           T = coefs[1][1], 
           tol1 = coefs[2][0],
           tol2 = coefs[2][1],
           tol3 = coefs[2][2],
           x = X+T*deg/360.+T;
    double mag1 = 0.,
           mag2 = 0.,
           mag3 = 0.;
    double a1,b1,c1, a2,b2,c2, a3,b3,c3;

    for (int i=0 ; i<8 ; ++i)
    {
        a1 = coefs[3*i+3][0] ; b1 = coefs[3*i+4][0] ; c1 = coefs[3*i+5][0] ;
        a2 = coefs[3*i+3][1] ; b2 = coefs[3*i+4][1] ; c2 = coefs[3*i+5][1] ;
        a3 = coefs[3*i+3][2] ; b3 = coefs[3*i+4][2] ; c3 = coefs[3*i+5][2] ;
        mag1 += a1*sin(b1*x + c1);
        mag2 += a2*sin(b2*x + c2);
        mag3 += a3*sin(b3*x + c3);
    }

    sens[0] = mag1 ; sens[1] = mag2 ; sens[2] = mag3 ;
    sens[3] = 600 ; sens[4] = 600 ; sens[5] = 0 ;

    return sens;
}

vector<double> degToSens(double deg)
{
    vector< vector<double> > coefs = readFile("magCalibration.txt");
    double method = coefs[0][0];
    if (method == 2.)
    {
        return degToSensSin(deg);
    }
    else
    {
    
        return degToSensFour(deg);
    }
}


vector< vector<double> > sampleDeg()
{
    vector< vector<double> > samples (3600, vector<double> (3));

    double deg;
    vector<double> coefs(3);
    int i;

    for (i=0 ; i<3600 ; ++i)
    {
        deg = i*SAMPLING_STEP;
        coefs = degToSens(deg);
        samples[i][0] = coefs[0] ;
        samples[i][1] = coefs[1] ;
        samples[i][2] = coefs[2] ;

    }

    return  samples;
}

// calirates the gyroVolts on nPeriods using a linear function
vector<double> calibrateGyro(vector< vector<double> > data, int col, int nPeriod)
{
    vector<double> coefs;

    vector<int> indices = findPeriod(data, col, nPeriod);

    vector<double> gv,
                   ds;
    double tempGV,
           tempDS,
           sGV;
    int i,j;

    for (i=0 ; i<nPeriod+1 ; ++i)
    {
        tempDS = data[indices[2*i]][0] - data[indices[2*i+1]][0];
        tempGV = 0. ; sGV = 0. ;
        for (j=indices[2*i+1] ; j<indices[2*i] ; ++j)
        {
            tempGV += data[j][2];
        }

        tempGV /= (indices[2*i] - indices[2*i+1]);
        tempDS = 360. / tempDS;
        for (j=indices[2*i+1] ; j<indices[2*i] ; ++j)
        {
            sGV += (data[j][2]-tempGV)*(data[j][2]-tempGV);
        }
        sGV /= (indices[2*i] - indices[2*i+1]); sGV = sqrt(sGV) ;
        //cout << tempGV << " " << sGV <<endl; 

        gv.push_back(tempGV);
        ds.push_back(tempDS);
    }

    coefs = linreg(gv,ds);

    return coefs;
}

double test(double expectedDeg, double currentDeg)
{
    double abs1, abs2, abs3, error;
    int eps = 1;

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

    error = -160;
    double errorTemp = 170;

    eps = ((abs(errorTemp) > abs(error)) &&
           (error*errorTemp > 0))?eps:-eps;
    return eps;
}

//int main(int argc, char* argv[])
//{
    
    ////cout << test(250,0) << endl;
    //cout << argc << endl;
    //vector<double> sens = degToSens(90);
    //printf("Mag1 = %lf ; Mag2 = %lf\n", sens[0],sens[1]);
    ////sens[0] =-5989.200000 ; sens[1] = -2349.500000 ; sens[2] = -6217.400000;
    ////sens[0] += 50 ; sens[1] -= 70;
    //printf("Mag1 = %lf ; Mag2 = %lf\n", sens[0],sens[1]);
    //vector< vector<double> > samples = sampleDeg();
    //double deg = sensToDeg(sens, samples);
    //printf("Deg = %lf\n", deg);
    //return 0;
//}

#endif

