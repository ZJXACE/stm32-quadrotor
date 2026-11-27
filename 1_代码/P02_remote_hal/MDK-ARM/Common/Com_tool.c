#include "Com_tool.h"

int16_t Com_Limit(int16_t value, int16_t min, int16_t max)
{

    if(value<min)
    {
        return min;
    }
    else if(value>max)
    {
        return max;
    }
    else
    {
        return value;
    }

}
