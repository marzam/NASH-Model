/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */


#include <cstdlib>
#include <string>
#include <cassert>
#include "Configure.hpp"
#include "CellularAutomata.hpp"
#include <cstring>
#include <string>
using namespace std;
int main(int argc, char *argv[])
{
   
   Configure *configure =  new Configure();
   CellularAutomata *cellularAutomata = new CellularAutomata();

   

/*
configuração - condição inicial
*/ configure->mDensity = stof(argv[1]);
   strcpy(configure->mModelName, "Exemplo-NASCH");
   configure->mTypeModel = 1;
   configure->mVMax = 25;
   configure->mCellX = 10000;
   configure->mCellY = 1;
   configure->mDeltaH = 1.5f;
   configure->mDefaultSize = 7.5f;
   configure->mSTime = 10000;
   configure->mDTime = 1000;
   configure->mStTime = 120;
   configure->mFixed = true;
   configure->mPhoto = true;
   configure->mLog = false;
   configure->mLogCluster = true;
   configure->mDeceleration = 3;
   configure->mTimePerception = 12;
   configure->mRoadBlock = false;
   configure->mLogVehicles = -1;
   configure->mTypeIndex = 1;
   assert(posix_memalign(reinterpret_cast<void**> (&configure->mProfile), ALIGN, configure->mTypeIndex *  sizeof(stVehiclesType)) == 0);
   configure->mProfile[0].percent = 1.0;
   configure->mProfile[0].len = 3;
   configure->mProfile[0].vMax = 25;
   configure->mProfile[0].param[0] = 1.0;
   configure->mProfile[0].param[0] = 1.0;
   configure->mProfile[0].inc = 4.0;
   configure->mProfile[0].desc = 1;
   configure->mProfile[0].left_p = 1.0;
   configure->mProfile[0].right_p = 1.0;
   

//Exibir configuração  
   configure->print();


   cellularAutomata->init(configure);

   cellularAutomata->exec();
   //cellularAutomata->debug();

   delete cellularAutomata;
   delete configure;
   

   return EXIT_SUCCESS;
}

/*
File loaded [SL-DEFAULT-HI.bin]
 Parameters: 
 Model = SL-DEFAULT-HI	 type: 1
 cellX  = 10000
 cellY  = 1
 DeltaH = 1.5
 sTime        = 10000
 dTime        = 1000
 stTime       = 120
 Density     = 0.10000
 vMax         = 25
 *deceleration = 3
 spc. perc.   = 12.0
 road block   = false
 Log files: 
 	fixed           = true
 	photo           = true
 	logCluster      = true
 	log             = false
 	last Vehicle(s) = -1 *[negative values indicate no log/positives indicates number of last vehicles]
=====================================================

Groups of vehicles: 1
(0) 	Percent: 1.00  Len: 3 vMax: 25 Alpha parameter ACC(6, 1)  Inc: 4 Desc: 1 Left prob.:1.00 Right prob.1.00
Open log file: SL-DEFAULT-HI.log
      Density:     0.100

      */