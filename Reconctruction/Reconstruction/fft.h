#ifndef FFT_H
#define FFT_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;


///output in MHz
vector< vector<double> > Get_fft_amp_spectrum(vector<double> data, const double time_fft_noise_from, const double time_fft_noise_to,
                                              const double time_scale, const bool is_nomalizied = true);

//output in ns
vector< vector<double> > Get_ifft(vector< vector<double> > fft_spectrum, int output_size, const int time_scale);

#endif // FFT_H
