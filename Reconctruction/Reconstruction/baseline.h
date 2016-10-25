#ifndef BASELINE_H
#define BASELINE_H

#include <iostream>
#include <vector>

using namespace std;


///averaged signal from 0 to right_time_point (this value in points)
double Get_baseline(vector<int> data_v, const int right_time_point);

#endif // BASELINE_H
