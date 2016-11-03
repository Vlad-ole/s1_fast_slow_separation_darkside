#include "derivative.h"
#include <math.h>

vector<double> Get_derivative(const vector<double> data_v, const int der_param)
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


//Calculate Filter Coeff for (Savitzky–Golay filter)
vector<double> Get_coeff_savitzky_golay(int points /*this number must be odd!!! */)
{
    //cout << endl << "start Calculate filter coefficients" << endl;

    //Savitzky–Golay filter
    //order = 3

    vector<double> C_i_der;
    int m = points;//

    //C_i
    for (int i = (1 - m) / 2.0; i <= (m - 1) / 2.0; i++)
    {
        double numerator = 5 * (3 * pow(m, 4.0) - 18 * pow(m, 2.0) + 31)*i - 28 * (3 * pow(m, 2.0) - 7)*pow(i, 3.0);
        double denominator = m * (pow(m, 2.0) - 1) * (3 * pow(m, 4.0) - 39 * pow(m, 2.0) + 108) / 15.0;
        C_i_der.push_back(numerator / denominator);
    }
    //cout << endl << "stop Calculate filter coefficients" << endl;
    return C_i_der;
}


//Calculate Derivative (Savitzky–Golay filter)
vector<double> Get_derivative_savitzky_golay(vector<double> yv, const int points, vector<double> C_i_der)
{
    //cout << endl << "start Calculate Derivatives" << endl;

    vector<double> yv_der;
    const int point_half = (points - 1) / 2.0;
    yv_der.resize(yv.size());

#pragma omp parallel for num_threads(2)
    for (int i = 0; i < yv.size(); i++)
    {

        if (i < point_half || i >(yv.size() - point_half - 1))
        {
            yv_der[i] = 0;
        }
        else
        {
            double value = 0;
            for (int j = 0; j < C_i_der.size(); j++)
            {
                value += C_i_der[j] * yv[i - point_half + j];
            }
            yv_der[i] = value;

        }

    }

    //cout << endl << "stop Calculate Derivatives" << endl;

    return yv_der;
}
