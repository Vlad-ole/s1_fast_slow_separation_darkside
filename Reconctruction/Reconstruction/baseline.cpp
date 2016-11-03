#include "baseline.h"


double Get_baseline(const vector<double> data_v, const int right_time_point)
{
    double baseline = 0;

    for (int i = 0; i < right_time_point; ++i)
    {
        baseline += data_v[i];
    }

    return (baseline / right_time_point);
}
