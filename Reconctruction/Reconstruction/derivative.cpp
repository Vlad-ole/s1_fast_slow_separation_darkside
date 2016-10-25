#include "derivative.h"

vector<double> Get_derivative(vector<int> data_v, const int der_param)
{
    vector<double> der_v;
    int nsamples = data_v.size();
    der_v.resize(nsamples);

    for (int i = 0; i < der_param; ++i)
    {
        der_v[i] = 0;
    }


    for (int i = der_param; i < nsamples; ++i)
    {
        der_v[i] = data_v[i] - data_v[i-der_param];
    }


    return der_v;
}
