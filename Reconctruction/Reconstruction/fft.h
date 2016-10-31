#ifndef FFT_H
#define FFT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector< vector<double> > Get_fft_amp_spectrum(vector<double> data, const double time_fft_noise_from, const double time_fft_noise_to, const double time_scale);

#endif // FFT_H
