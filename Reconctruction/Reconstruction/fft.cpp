#include "fft.h"
#include "TVirtualFFT.h"

vector< vector<double> > Get_fft_amp_spectrum(vector<double> data, const double time_fft_noise_from, const double time_fft_noise_to, const double time_scale)
{
    int n_size_raw = data.size();
    const int n_size_fft = n_size_raw / 2 + 1;
    vector< vector<double> > fft_amp_spectrum;
    fft_amp_spectrum.resize(2);
    fft_amp_spectrum[0].resize(n_size_fft);
    fft_amp_spectrum[1].resize(n_size_fft);
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

    fft_own->SetPoints(&data[0]);
    fft_own->Transform();
    fft_own->GetPointsComplex(&fft_re[0], &fft_im[0]);

    const double sampling_frequency = 1 / time_scale;//Hz
    const double delta_frequency = sampling_frequency / n_size_raw;

    for (int i = 0; i < n_size_fft; ++i)
    {
        fft_amp_spectrum[0][i] = i * delta_frequency;
        fft_amp_spectrum[1][i] = sqrt( fft_re[i]*fft_re[i] + fft_im[i]*fft_im[i] );
    }

    delete fft_own;
    return fft_amp_spectrum;
}
