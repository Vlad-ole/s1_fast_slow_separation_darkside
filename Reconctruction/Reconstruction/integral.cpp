#include "integral.h"

double Get_integral(const vector<int> data_v, const double baseline, const int time_scale, const double time_from, const double time_to)
{
    double integral = 0;
    const int point_from = time_from / time_scale;
    const int point_to = time_to / time_scale;

    if( point_from > data_v.size() || point_to > data_v.size() )
    {
        cout << "error: incorrect integration time" << endl;
        exit(1);
    }

    for (int i = point_from; i < point_to; ++i)
    {
        integral += (data_v[i] - baseline);
    }


    return (integral * time_scale);
}
