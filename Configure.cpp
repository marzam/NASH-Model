#include "Configure.hpp"
#include <cassert>
#include <fstream>
using namespace std;

Configure::Configure():
mTypeModel(Configure::SINGLE_LANE),
mVMax(0),
mCellX(0), mCellY(0),
mDeltaH(0.0), mDefaultSize(0.0),
mSTime(0), mDTime(0), mStTime(0),
mDensity(0.0),
mFixed(false), mPhoto(false), mLogCluster(false), mLog(false),
mDeceleration(0), mTimePerception(0.0),
mRoadBlock(0), mLogVehicles(-1),
mProfile(NULL){
    bzero(mModelName, STRINGSIZE);
};


Configure::Configure(Configure *config){
    assert(config != NULL);

    memcpy(mModelName, config->mModelName, STRINGSIZE);
    mTypeModel = config->mTypeModel;
    mVMax = config->mVMax;
    mCellX = config->mCellX;
    mCellY = config->mCellY;
    mDeltaH = config->mDeltaH;
    mDefaultSize = config->mDefaultSize;
    mSTime  = config->mSTime;
    mDTime  = config->mDTime;
    mStTime = config->mStTime;
    mDensity = config->mDensity;
    mFixed = config->mFixed;
    mPhoto  = config->mPhoto;
    mLogCluster = config->mLogCluster;
    mLog  = config->mLog;
    mDeceleration = config->mDeceleration;
    mTimePerception  = config->mTimePerception;
    mRoadBlock  = config->mRoadBlock;
    mLogVehicles  = config->mLogVehicles;
    mTypeIndex = config->mTypeIndex;
    assert(mTypeIndex > 0);
    assert(posix_memalign(reinterpret_cast<void**> (&mProfile), ALIGN, mTypeIndex *  sizeof(stVehiclesType)) == 0);
    memcpy(mProfile, config->mProfile, mTypeIndex * sizeof(stVehiclesType));



};
Configure::~Configure(){
    clear();
};
void Configure::loadConfigFile(const string& configFile){
    string fileName = "";
    fstream input;
    fileName = configFile;

    input.open(fileName, ios::in|ios::binary);
    assert(input.is_open());
    input.read(reinterpret_cast<char*> (&mModelName), STRINGSIZE);
    input.read(reinterpret_cast<char*> (&mVMax), sizeof(int));

    input.read(reinterpret_cast<char*> (&mCellX), sizeof(int));
    input.read(reinterpret_cast<char*> (&mCellY), sizeof(int));
    input.read(reinterpret_cast<char*> (&mDeltaH), sizeof(float));
    input.read(reinterpret_cast<char*> (&mDefaultSize), sizeof(float));
    input.read(reinterpret_cast<char*> (&mSTime), sizeof(int));
    input.read(reinterpret_cast<char*> (&mDTime), sizeof(int));
    input.read(reinterpret_cast<char*> (&mStTime), sizeof(int));
    //input.read(reinterpret_cast<char*> (&mDensity), sizeof(float));

    input.read(reinterpret_cast<char*> (&mFixed), sizeof(bool));
    input.read(reinterpret_cast<char*> (&mPhoto), sizeof(bool));
    input.read(reinterpret_cast<char*> (&mLogCluster), sizeof(bool));
    input.read(reinterpret_cast<char*> (&mLog), sizeof(bool));

    input.read(reinterpret_cast<char*> (&mDeceleration), sizeof(int));
    input.read(reinterpret_cast<char*> (&mTimePerception), sizeof(float));
    input.read(reinterpret_cast<char*> (&mRoadBlock), sizeof(bool));
    input.read(reinterpret_cast<char*> (&mLogVehicles), sizeof(int));
    input.read(reinterpret_cast<char*> (&mTypeModel), sizeof(int));

    input.read(reinterpret_cast<char*> (&mTypeIndex), sizeof(int));

    assert(posix_memalign(reinterpret_cast<void**> (&mProfile), ALIGN, mTypeIndex *  sizeof(stVehiclesType)) == 0);
    input.read(reinterpret_cast<char*> (mProfile), mTypeIndex * sizeof(stVehiclesType));

    input.close();
    cout << "File loaded [" << fileName << "]" << endl;


};
void Configure::saveConfigFile(const string& configFile){
    string fileName = "";
    fstream output;
    if (configFile.compare("") != 0){
        fileName = configFile;
    }else{
        fileName = string(mModelName) + ".bin";
    }
    output.open(fileName, ios::out|ios::binary|ios::trunc);
    assert(output.is_open());
    output.write(reinterpret_cast<const char*> (&mModelName), STRINGSIZE);
    output.write(reinterpret_cast<const char*> (&mVMax), sizeof(int));

    output.write(reinterpret_cast<const char*> (&mCellX), sizeof(int));
    output.write(reinterpret_cast<const char*> (&mCellY), sizeof(int));
    output.write(reinterpret_cast<const char*> (&mDeltaH), sizeof(float));
    output.write(reinterpret_cast<const char*> (&mDefaultSize), sizeof(float));
    output.write(reinterpret_cast<const char*> (&mSTime), sizeof(int));
    output.write(reinterpret_cast<const char*> (&mDTime), sizeof(int));
    output.write(reinterpret_cast<const char*> (&mStTime), sizeof(int));
//    output.write(reinterpret_cast<const char*> (&mDensity), sizeof(float));

    output.write(reinterpret_cast<const char*> (&mFixed), sizeof(bool));
    output.write(reinterpret_cast<const char*> (&mPhoto), sizeof(bool));
    output.write(reinterpret_cast<const char*> (&mLogCluster), sizeof(bool));
    output.write(reinterpret_cast<const char*> (&mLog), sizeof(bool));

    output.write(reinterpret_cast<const char*> (&mDeceleration), sizeof(int));
    output.write(reinterpret_cast<const char*> (&mTimePerception), sizeof(float));
    output.write(reinterpret_cast<const char*> (&mRoadBlock), sizeof(bool));
    output.write(reinterpret_cast<const char*> (&mLogVehicles), sizeof(int));
    output.write(reinterpret_cast<const char*> (&mTypeModel), sizeof(int));

    output.write(reinterpret_cast<const char*> (&mTypeIndex), sizeof(int));

    output.write(reinterpret_cast<const char*> (mProfile), mTypeIndex * sizeof(stVehiclesType));

    output.close();
    cout << "File saved [" << fileName << "]" << endl;
};



void Configure::setProfiles(int i){
    mTypeIndex = i;
    assert(posix_memalign(reinterpret_cast<void**> (&mProfile), ALIGN, mTypeIndex *  sizeof(stVehiclesType)) == 0);
};

void Configure::setProfile(int index,
                           float percent,
                           int len,
                           int vMax,
                           double alpha,
                           double beta,
                           double inc,
                           int desc,
                           float left_p,
                           float right_p){

    assert(index < mTypeIndex);
    mProfile[index].percent = percent;
    mProfile[index].len = len;
    mProfile[index].vMax = vMax;
    mProfile[index].param[0] = alpha;
    mProfile[index].param[1] = beta;
    mProfile[index].inc = inc;
    mProfile[index].desc = desc;
    mProfile[index].left_p = left_p;
    mProfile[index].right_p = right_p;



}
/**
 *  inicialize
 *  @param fileName: config file name
 */
void Configure::print(void){
    cout << " Parameters: \n";
    cout << " Model = " << mModelName << "\t type: " << mTypeModel << endl;
    cout << " cellX  = " << mCellX << endl;
    cout << " cellY  = " << mCellY << endl;
    cout << " DeltaH = " << mDeltaH << endl;
    cout << " sTime        = " << mSTime << endl;
    cout << " dTime        = " << mDTime << endl;
    cout << " stTime       = " << mStTime << endl;
    //cout << " deltaTime    = " << mParam.deltaTime << endl;
    cout << " Density     = " << fixed << setprecision(5) << mDensity << endl;
    cout << " vMax         = " << mVMax << endl;
    cout << " *deceleration = " << mDeceleration << endl;

    cout << " spc. perc.   = " << fixed << setprecision(1) << mTimePerception << endl;
    cout << " road block   = " << boolalpha  <<  mRoadBlock << endl;

    cout << " Log files: " << endl;
    cout << " \tfixed           = " << boolalpha  << mFixed << endl;
    cout << " \tphoto           = " << boolalpha  <<  mPhoto << endl;
    cout << " \tlogCluster      = " << boolalpha  << mLogCluster << endl;
    cout << " \tlog             = " << boolalpha  << mLog << endl;
    cout << " \tlast Vehicle(s) = " << noboolalpha  << mLogVehicles << " *[negative values indicate no log/positives indicates number of last vehicles]" << endl;



    cout << "=====================================================" << endl << endl;
    cout << fixed << setprecision(0);
    cout << "Groups of vehicles: " <<mTypeIndex   << endl;
    for (int i = 0; i < mTypeIndex; i++){
        stVehiclesType *type =  &mProfile[i];
        cout << "("  << i << ") \t";
        cout << "Percent: " << fixed << setprecision(2) << type->percent \
        << "  Len: " << type->len \
        << " vMax: " << type->vMax \
        << " Alpha parameter ACC(" << setprecision(0) << type->param[0] << ", " << type->param[1] << ") "
        << " Inc: " << setprecision(0) << type->inc \
        << " Desc: " << type->desc \
        << " Left prob.:" <<   setprecision(2) << type->left_p << " Right prob." << type->right_p \
        << endl;
    }
    cout.flush();



};
void Configure::clear(void){
    mTypeIndex = 0;
    if (mProfile != NULL)
        free(mProfile);

    mProfile = NULL;
}
