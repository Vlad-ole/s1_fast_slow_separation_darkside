void test_vector()
{
    TSpectrum s;

    const int size = 100;
    vector<float> response;
    vector<float> source;
    response.resize(size);
    source.resize(size);

    for (int i = 0; i < size; ++i)
    {
        response[i] = TMath::Gaus(i, 20);
        source[i] = ( TMath::Gaus(i, 30)  + 0.2*TMath::Gaus(i, 33) );
    }

    s.Deconvolution(&source[0],&response[0],size,100,1,1);
}
