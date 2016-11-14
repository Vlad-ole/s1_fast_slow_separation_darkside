#include "fft.h"
#include "TVirtualFFT.h"
#include "TMath.h"

double phase(double x, double y)
{
    double result = 0;

    if(x > 0)result = atan(y / x);
    else if(x < 0 && y >= 0) result = atan(y / x) + TMath::Pi();
    else if(x < 0 && y < 0) result = atan(y / x) - TMath::Pi();
    else if(x == 0 && y > 0) result = TMath::Pi() / 2;
    else if(x == 0 && y < 0) result = - TMath::Pi() / 2;

    return result;
}

vector< vector<double> > Get_fft_amp_spectrum(vector<double> data, const double time_from, const double time_to, const double time_scale)
{
    const int point_start = (int)(time_from / time_scale);
    const int point_stop = (int)(time_to / time_scale);
    if(point_start < 0 || point_stop < 0 || point_start > data.size() || point_stop > data.size()
            || point_start >= point_stop)
    {
        cout << "error: incorrect time range for fft" << endl;
        exit(1);
    }
    int n_size_raw = point_stop - point_start + 1;
    const int n_size_fft = n_size_raw / 2 + 1;
    vector< vector<double> > fft_amp_spectrum;
    fft_amp_spectrum.resize(3);
    fft_amp_spectrum[0].resize(n_size_fft);//frequency
    fft_amp_spectrum[1].resize(n_size_fft);//amp
    fft_amp_spectrum[2].resize(n_size_fft);//phase

    vector<double> fft_re;
    vector<double> fft_im;
    fft_re.resize(n_size_fft);
    fft_im.resize(n_size_fft);

    TVirtualFFT *fft_own = TVirtualFFT::FFT(1, &n_size_raw, "R2C ES K");
    if (!fft_own)
    {
        cout << "error in fft!" << endl;
        exit(1);
    }

    fft_own->SetPoints(&data[point_start]);
    fft_own->Transform();
    fft_own->GetPointsComplex(&fft_re[0], &fft_im[0]);

    const double sampling_frequency = 1000.0 / time_scale;//MHz
    const double delta_frequency = sampling_frequency / n_size_raw;

    for (int i = 0; i < n_size_fft; ++i)
    {
        fft_amp_spectrum[0][i] = i * delta_frequency;
        fft_amp_spectrum[1][i] = sqrt( fft_re[i]*fft_re[i] + fft_im[i]*fft_im[i] );
        fft_amp_spectrum[2][i] = phase(fft_re[i], fft_im[i]);
    }


    delete fft_own;
    return fft_amp_spectrum;
}

vector< vector<double> > Get_ifft(vector< vector<double> > fft_spectrum, int output_size, const int time_scale)
{
    vector< vector<double> > result;
    result.resize(2);
    result[0].resize(output_size);
    result[1].resize(output_size);

    const int input_size = fft_spectrum[0].size();
    //int output_size = (fft_spectrum[0].size() - 1) * 2;//I am not sure in this line. So, I will get output_size as parameter.

    //from amp & phase to Re & Im
    vector<double> re;
    vector<double> im;
    re.resize(input_size);
    im.resize(input_size);
    for (int i = 0; i < input_size; ++i)
    {
        re[i] = fft_spectrum[1][i] * cos(fft_spectrum[2][i]);
        im[i] = fft_spectrum[1][i] * sin(fft_spectrum[2][i]);
    }


    //Now let's make a backward transform:
    TVirtualFFT *fft_back = TVirtualFFT::FFT(1, &output_size, "C2R M K");
    fft_back->SetPointsComplex(&re[0], &im[0]);
    fft_back->Transform();

    for (int i = 0; i < output_size; ++i)
    {
        result[0][i] = i * time_scale;
        result[1][i] = ( fft_back->GetPointReal(i) ) / output_size;
    }

    return result;
}
