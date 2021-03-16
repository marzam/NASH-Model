/*
 *  CellularAutomata.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "CellularAutomata.hpp"
#include <iomanip>
#include <fstream>
#include <cassert>
#include <ctime>
#include <climits>
#include <omp.h>
#include <ctime>
#include <cmath>
using namespace std;

/*
 *
 */
CellularAutomata::CellularAutomata():
mConfigure(NULL),
mMesh(NULL),
mVechiles(NULL),
mProfile(NULL),
mObjects(0),
mAveVeloc(0.0)  
{  srand ( time(NULL) ); }

/*
 *
 */
CellularAutomata::~CellularAutomata(){
    //cout << endl << "END THE GAME!" << endl;
    
    mLog.close();
    if (mMesh != NULL)
        free(mMesh);
    mMesh = NULL;

    if (mVechiles != NULL)
        free(mVechiles);
    mVechiles = NULL;
    mProfile = NULL; //This pointer points to mConfigure variable. Do not free it!!!!
    
    if (mAVVelSa != NULL)
      free(mAVVelSa);
    
    if (mAVveloc != NULL)
      free(mAVveloc);
 
    if (mAVDensi != NULL)
      free(mAVDensi);
    
    if (mAVDenSa != NULL)
      free(mAVDenSa);
    
}


/**
 *  initialCondition
 *  
 */
void CellularAutomata::initialCondition(void){
    //Define how many vehicles will be on the road
/*
 * Observation 2017, March 18th
 * For each vehicle Verifying if it is in a list of log vehicles vehicleLog
 * To do this: Verifying if there is a substring with the number.
 *             Set true vehicle parameter
 *             Put in Vehicle.cpp code to log the vehicle performance
 *
 */
    int *tVehicles,
         sum = 0;
    mProfile    = mConfigure->mProfile;
    mTypeIndex  = mConfigure->mTypeIndex;

    assert(posix_memalign((void**) &tVehicles, ALIGN, mTypeIndex * sizeof(int)) == 0);
    assert(posix_memalign((void**) &mAVveloc, ALIGN, mConfigure->mCellY * sizeof(float)) == 0);
    assert(posix_memalign((void**) &mAVVelSa, ALIGN, mConfigure->mCellY * sizeof(float)) == 0);

    assert(posix_memalign((void**) &mAVDensi, ALIGN, mConfigure->mCellY * sizeof(float)) == 0);
    assert(posix_memalign((void**) &mAVDenSa, ALIGN, mConfigure->mCellY * sizeof(float)) == 0);
    
    memset(mAVveloc, 0x00, mConfigure->mCellY * sizeof(float));
    memset(mAVVelSa, 0x00, mConfigure->mCellY * sizeof(float));

    memset(mAVDensi, 0x00, mConfigure->mCellY * sizeof(float));
    memset(mAVDenSa, 0x00, mConfigure->mCellY * sizeof(float));

   

    for (int i = 0; i < mTypeIndex; i++){
        float dVehicle  = mProfile[i].percent * mConfigure->mDensity / static_cast<float> (mProfile[i].len);
        tVehicles[i]    = static_cast <int> (dVehicle * static_cast <float> (mConfigure->mCellX * mConfigure->mCellY));
        sum += tVehicles[i];
    }

    assert(posix_memalign((void**) &mVechiles, ALIGN, sum * sizeof(stObject)) == 0);
    memset(mVechiles, 0x00, sum * sizeof(stObject));
    int c = 0,
        j = 0,
        x = 0,
        y = 0;

    stVehiclesType type = mProfile[0];
    for (int i = 0; i < sum; i++){
        if (c < tVehicles[j])
            c++;
        else{
            c = 0;
            type = mProfile[++j];
        }
        mVechiles[i].ID = i+1;
        mVechiles[i].type = j;
        mVechiles[i].x = x;
        mVechiles[i].y = y;
        mVechiles[i].breaking = false;
        mVechiles[i].seed = (i + 1) * rand();
        y++;
        if (y == mConfigure->mCellY){
          y = 0;
          x += type.len;  
          if (x >= mConfigure->mCellX)
            x = x % mConfigure->mCellX;
        }
        
    }

    mObjects = sum; 
    free(tVehicles);

    tVehicles = NULL;

    updateGrid();


}


/**
 *  inicialize
 *  @param fileName: config file name
 */
void CellularAutomata::init(Configure *configure)
{

	mConfigure = new Configure(configure);
	if (mConfigure == NULL)
	    return;


    assert(posix_memalign((void**) &mMesh, ALIGN, mConfigure->mCellX *  mConfigure->mCellY *  sizeof(int)) == 0);
	assert(mMesh != NULL);
    memset(mMesh, 0x00, sizeof(int) * mConfigure->mCellX *  mConfigure->mCellY);
    strcat(mConfigure->mModelName, ".log");
    cout << "Open log file: " << mConfigure->mModelName << endl;
    cout << "      Density: " <<  fixed << setw(10) << setprecision(4) << mConfigure->mDensity << endl;
    mLog.open(mConfigure->mModelName, fstream::out | fstream::app);
    assert(mLog.is_open());
};

double CellularAutomata::getRandom(unsigned int *seed){
   return static_cast <double> (rand_r(seed) % 10000 + 1) / 10000.0f;
};


void CellularAutomata::updateObjects_X(void){
    for (int i = 0; i < mObjects; i++){
        mVechiles[i].x += mVechiles[i].vx1;
        if (mVechiles[i].x >= mConfigure->mCellX){
          mVechiles[i].x = mVechiles[i].x % mConfigure->mCellX;
        }
        mVechiles[i].vx0 = mVechiles[i].vx1;
       // addVelocity(0, mVechiles[i].y, mVechiles[i].vx0);
       // addDensity(0, mVechiles[i].y, mProfile[mVechiles[i].type].len);
    }
};
/*
void CellularAutomata::ruleMovement(void){
    for (int i = 0; i < mObjects; i++){
        stObject *vehicle = &mVechiles[i],
                 aheadV;
        stVehiclesType type = mProfile[vehicle->type];
        int iVel0 = vehicle->vx0;
        int iVel1 = iVel0;
        int iDist = 0;
        double   betapdf =  betaFunction(type.param);
        double   betapdf1 =  (1.0 - betapdf) * mConfigure->mTimePerception;
        int      iMax = min(mConfigure->mVMax, type.vMax);
        
        //getDistance(vehicle->x, vehicle->y, &iDist);
        aheadV = getDistanceAndVehicle(vehicle->x, vehicle->y, &iDist);
        int iDV = iVel0 - aheadV.vx0;
        
        if ((aheadV.breaking == true) && (iDV > 0) && (iDist <= static_cast<int>(mAveVeloc * (1 - betapdf1))) ) {
          double dDesc = static_cast<double>(type.desc);
          
          iVel1 = iVel0 - truncCurve3(dDesc * betapdf);;
          iVel1 = max(iVel1, 0);
        }else{
          iVel1 += truncCurve3(type.inc * betapdf);

          iVel1 = max(iVel1, 0);
          iVel1 = min(iVel1, iMax);
          
        }
        
        
        iVel1 = min(iVel1, iDist);
        if (iVel1 < iVel0)
          vehicle->breaking = true;
        else
          vehicle->breaking = false;
        
                
        
        vehicle->vx1 = iVel1;
    }//end-for (int i = 0; i < mObjects; i++){
};
*/
/*
void CellularAutomata::ruleMovement(void){
    for (int i = 0; i < mObjects; i++){
        stObject *vehicle = &mVechiles[i];
        stVehiclesType type = mProfile[vehicle->type];

        //bool breaking = false;
        //double   betapdf =  betaFunction(type.param);
        double   alpha = betaFunction(type.param); //betapdf ;//(1.0f - betapdf);
        double     acc = type.inc * alpha;
        int       iMax = min(mConfigure->mVMax, type.vMax);
        int iDist      = 0;
        int iAheadVel  = 0;
        int iVel       = vehicle->vx0;
        //int iOldV      = iVel;
        
        getDistanceAndVelocity(vehicle->x, vehicle->y, &iDist, &iAheadVel);
        
        
        {//init velocity adujement
          int dv = iVel - iAheadVel;  
          float beta = (1.0 - alpha) * mConfigure->mTimePerception;
          if ((iDist <= static_cast<int>(mAveVeloc * beta)) && (dv > 0) && (iDist > 0)){
              double d   =  2.0f * static_cast<double> (iDist);
              double vl  =  static_cast<double>(iAheadVel);
              double vf  =  static_cast<double>(iVel);
              double acc = ((vf * vf) - (vl * vl)) / d;
              if (acc > mConfigure->mDeceleration)
                  acc = mConfigure->mDeceleration;
              iVel = iVel - (truncCurve3(acc));
              //breaking = true;
           }else{
            //Free flow rule
              iVel += (truncCurve3(acc) - type.desc);
           }
          
        }//end velocity adujement
        
        iVel = max(iVel, 0);
        iVel = min(iVel, iMax);
        
        if (iVel > iDist){
          iVel = iDist;
          //breaking = true;
        }
        
        
        if ((iVel < iOldV) && (!breaking))
          breaking = true;
        
        //vel = min (iDist, vel);
        vehicle->vx1 = iVel;
   }//end-for (int i = 0; i < mObjects; i++){
};
*/

void CellularAutomata::ruleMovement(void){
    for (int i = 0; i < mObjects; i++){
        stObject *vehicle = &mVechiles[i];
        stVehiclesType type = mProfile[vehicle->type];
        int       iMax = min(mConfigure->mVMax, type.vMax);
        int       iVel = vehicle->vx0 + 1;
        
        iVel = min(iVel, iMax);
        
        
        double p = getRandom(&vehicle->seed);
        if (p < 0.25f){
            iVel = max(0, iVel - 1);
        }
        int iDist = 0;
        getDistance(vehicle->x, vehicle->y, &iDist);
                
        iVel = min (iDist, iVel);
        
        vehicle->vx1 = iVel;
    }//end-for (int i = 0; i < mObjects; i++){
};

void CellularAutomata::rule(void){
    //Inserting change lane rules
    ruleMovement();
    updateObjects_X();
    updateGrid();
    
};

/*
void CellularAutomata::addVelocity(int t, int y, int v){
  mAVveloc[y]+= static_cast<float>(v);
  mAVVelSa[y]++;
};

void CellularAutomata::addDensity(int t, int y, int l){
  mAVDensi[y]+= static_cast<float>(l);
  mAVDenSa[y]++;
};
*/
void CellularAutomata::statistic(void){
  
  for (int i = 0; i < mObjects; i++){
        mAVDensi[mVechiles[i].y] +=  mProfile[mVechiles[i].type].len;
        mAVveloc[mVechiles[i].y] +=  mVechiles[i].vx0;
        mAVVelSa[mVechiles[i].y]++;
  }
  
  mAveVeloc = 0.0;
  float count = 0.0;
  for (int y = 0; y < mConfigure->mCellY; y++){
    mAveVeloc +=  mAVveloc[y];
    count += mAVVelSa[y];
  }  
  
  mAveVeloc /= count;
  for (int y = 0; y < mConfigure->mCellY; y++){
      float velocity  = mAVveloc[y] / mAVVelSa[y];
      float density   = mAVDensi[y] / static_cast<float> (mConfigure->mCellX);
      float flow  = velocity * density;
      mLog << setw(10) << y << fixed << setw(10) << setprecision(5) << flow << fixed << setw(10) << setprecision(5) << velocity << fixed << setw(10) << setprecision(5) << density << "\t\t";
      mAVDensi[y] =  mAVDenSa[y] = mAVveloc[y] = mAVVelSa[y] = 0.0;
  }
  mLog << endl;
  mLog.flush();
};


/*
 * Running the simulation
 */

void CellularAutomata::exec(void){
    
	int mpStep   = mConfigure->mSTime / 20;
    if (mpStep == 0) mpStep = 1;
    initialCondition();
    
    
    for (int step = 1; step < mConfigure->mSTime; step++){
        if ((step % mpStep) == 0){
                cout << ".";
                cout.flush();
        }//end-if ((mStep % mpStep) == 0){
        if (step >= mConfigure->mDTime){
            if (step == mConfigure->mDTime){
                cout << "|";

                //mRules->setStatistic(true);
            }//end-if (mStep == mRules->getParam()->dTime){

        }//end-if (mStep >= mRules->getParam()->dTime){
        rule();
    
        if ((step > mConfigure->mDTime)  && ((step % mConfigure->mStTime) == 0))
          statistic();
    }
    cout << endl;

};

int CellularAutomata::getGrid(int x, int y){
   return mMesh[mConfigure->mCellX * y + x]; 
};

void CellularAutomata::setGrid(int x, int y, int index){
   mMesh[mConfigure->mCellX * y + x] = index; 
};
void CellularAutomata::updateGrid(void){


   //Updating all vehicles
  
   memset(mMesh, 0x00, mConfigure->mCellX * mConfigure->mCellY * sizeof(int));
   for (int i = 0; i < mObjects; i++){
       stVehiclesType type = mProfile[mVechiles[i].type];
       int y = mVechiles[i].y;
       
       for (int k = 0 ; k <   type.len ; k++){
            int x  =  mVechiles[i].x - k;
            if (x < 0)
               x = mConfigure->mCellX + x;

            if (getGrid(x, y) != 0){
                    std::cerr << "\t [ERROR] " << __FILE__ << " at " << __LINE__  <<  std::endl ;

                    // v.mBreakForeseenForeseenLight = true;
                    std::cerr << "V1(ID= " << mVechiles[i].ID << ",X= "
                    <<  mVechiles[i].x  << ",Y= "
                    <<  mVechiles[i].y  << ")" << std::endl;


                    int j = getGrid(x, y);
                    std::cerr << "V2(ID= " <<  mVechiles[j].ID << ",X= "
                    <<  mVechiles[j].x  << ",Y= "
                    <<  mVechiles[j].y  << ")" << std::endl;
                    exit(-1);

                }
                setGrid(x, y, i);
        }//end-for (int k = 0 ; k <   pEntity->lg; k++){
       
   }//end-for (int i = 0; i < mObjects; i++){
   
   
};


void CellularAutomata::debug(void){
    cout << "Debug" << endl;
    for (int y = 0; y < mConfigure->mCellY; y++){
        for (int x = 0; x < mConfigure->mCellX; x++){
            cout << getGrid(x, y) << " ";
        }
        cout << endl;
    }
    
};

void CellularAutomata::getDistanceAndVelocity(int xv, int yv, int *dist, int *aheadvel){
   //rule == true then solve cluster
   //otherwise movement rule
   //tpVehiclesType *type = getGrid()->getVehicles(idx).type;
  
   int x = xv,
       y = yv,
       iDist = 0,
       index = 0;
   

   do
   {
      x++;
      iDist++;
      if (x == mConfigure->mCellX)
         x = 0;
      index = getGrid(x, y);

   } while (index == 0); // && (iDist <= mConfigure->mVMax));


   iDist--;
   if (iDist < 0)
      iDist = 0;

   *dist = iDist;
   *aheadvel = mVechiles[index].vx0;
};

stObject CellularAutomata::getDistanceAndVehicle(int xv, int yv, int *dist){
   //rule == true then solve cluster
   //otherwise movement rule
   //tpVehiclesType *type = getGrid()->getVehicles(idx).type;
  
   int x = xv,
       y = yv,
       iDist = 0,
       index = 0;
   

   do
   {
      x++;
      iDist++;
      if (x == mConfigure->mCellX)
         x = 0;
      index = getGrid(x, y);

   } while (index == 0); // && (iDist <= mConfigure->mVMax));


   iDist--;
   if (iDist < 0)
      iDist = 0;

   *dist = iDist;
   return mVechiles[index];
};




void CellularAutomata::getDistance(int xv, int yv, int *dist){
   //rule == true then solve cluster
   //otherwise movement rule
   //tpVehiclesType *type = getGrid()->getVehicles(idx).type;
  
   int x = xv,
       y = yv,
       iDist = 0,
       index = 0;
   

   do
   {
      x++;
      iDist++;
      if (x == mConfigure->mCellX)
         x = 0;
      index = getGrid(x, y);

   } while ((index == 0) && (iDist <= mConfigure->mVMax));


   iDist--;
   if (iDist < 0)
      iDist = 0;

   *dist = iDist;

};


double  CellularAutomata::betaFunction (double gamaAlpha,
                                        double gamaBeta,
                                        double gamaAlphaBeta,
                                        double alpha,
                                        double beta,
                                        double x){


	double  xalpha = pow(x, (alpha-1.0f)),
            xbeta  = pow((1.0f - x), (beta-1.0f));

	double ret = gamaAlphaBeta / (gamaAlpha * gamaBeta);

    ret *= xalpha * xbeta;

    return ret;
};

double  CellularAutomata::betaFunction (double *vet){

	double x, y, y1;
    do{
        x  = getRandom();
        y1 = getRandom();

        y = betaFunction( vet[2],
                         vet[3],
                         vet[4],
                         vet[0],
                         vet[1],
                         x);


    }while (y1 > y);

    if (x < 0.0f)
        x = 0.0f;

    if (x > 1.0f)
        x = 1.0f;

    return x;

};


int CellularAutomata::truncCurve3(double v)
{ return static_cast<int> (floor(v+0.5f)); };

float CellularAutomata::getRandom(void)
{ return static_cast <float> (rand() % 10000 + 1) / 10000.0f;};
