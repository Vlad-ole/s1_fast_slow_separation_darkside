#ifndef DERIVATIVE_H
#define DERIVATIVE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<double> Get_derivative(const vector<double>, const int der_param);
vector<double> Get_derivative_savitzky_golay(const vector<double>, const int der_param, vector<double> C_i_der);
vector<double> Get_coeff_savitzky_golay(int points /*this number must be odd!!! */);


#endif // DERIVATIVE_H
