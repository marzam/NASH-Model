#ifndef _MODELTYPES_HPP_
#define _MODELTYPES_HPP_
 

struct stVehiclesType
{
    float      percent;
    //int        absolute;
    int        len;
    int        vMax;
    double     param[5];
    double     inc;
    int        desc;
    float      left_p;
    float     right_p;
};


struct stObject{
    int ID,
        x, y, 
        vx0, vx1, 
        vy0, vy1,
        type;
    unsigned int seed;  
    bool breaking;
};
#endif
