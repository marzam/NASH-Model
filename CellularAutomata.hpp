#ifndef _CELLULARAUTOMATA_H_
#define _CELLULARAUTOMATA_H_
#include "App.hpp"
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "Configure.hpp"
#include "ModelTypes.hpp"

/*
 *  CellularAutomata.h
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *  This class has the basic structure as well as transition rules
 *
 */

using namespace std;

class CellularAutomata
{

public:
    CellularAutomata();
    ~CellularAutomata();
    void initialCondition(void);
    //void finalCondition(void);
    void init(Configure *);
    double getRandom(unsigned int *seed);
    void updateObjects_X(void);
    void ruleMovement(void);
    void rule(void);
    //void addVelocity(int, int, int);
    //void addDensity(int, int, int);
    void statistic(void);
    void exec(void);
    void debug(void);
protected:
    int getGrid(int, int);
    void setGrid(int, int, int);
    void updateGrid(void);
    void getDistance(int, int, int *);
    void getDistanceAndVelocity(int, int, int *, int *);
    stObject getDistanceAndVehicle(int, int, int *);
    double betaFunction (double, double, double, double, double, double);
    double betaFunction (double *);
    int truncCurve3(double);
    float getRandom(void);
    
    int                              *mMesh,
                                      mTypeIndex,
                                      mObjects;
                                      
    stObject                         *mVechiles;
    
	Stopwatch                        mStopwatch;
    
	Configure                        *mConfigure;
    
    stVehiclesType                   *mProfile;
    
    float                           *mAVveloc,
                                    *mAVVelSa, 
                                    *mAVDensi,
                                    *mAVDenSa,
                                    mAveVeloc;
    
    fstream                         mLog;
    

};
#endif
