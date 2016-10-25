#include "integral.h"

double Get_integral(vector<int> data_v, const double baseline, const int time_scale)
{
    double integral = 0;

    for (int i = 0; i < data_v.size(); ++i)
    {
        integral += (data_v[i] - baseline);
    }


    return (integral * time_scale);
}
