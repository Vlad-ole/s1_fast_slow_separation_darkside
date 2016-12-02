#include <iostream>
#include <algorithm>
#include <vector>

#include "TF1.h"
#include "TGraph.h"
#include "TFile.h"
#include "TRandom.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TGraphErrors.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TText.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TAxis.h"
#include "TSpectrum.h"

#include "wunfold.h"
#include "writedata.h"
using namespace std;

int main(int argc, char *argv[])
{
    WriteData wrt("/home/darkside/Vlad_Programs/vlad_rawdata/unfolding_tests.root");


    const double min_time = 0;
    const double max_time = 70;
    const double time_scale = 0.1;
    const int ssize = (max_time - min_time) / time_scale;

    vector<float> source;
    vector<float> response;
    vector<float> xv;

    source.resize(ssize);
    response.resize(ssize);
    xv.resize(ssize);

    TF1 *ff1_response = new TF1("ff1_response","TMath::Gaus(x, 20) - 0.2*TMath::Gaus(x, 23)",min_time,max_time);

    for (int i = 0; i < ssize; ++i)
    {
        double time =  i*time_scale + min_time;
        xv[i] = time;

        response[i] = ff1_response->Eval(time);
        source[i] = ff1_response->Eval(time - 10) + ff1_response->Eval(time - 20);
    }

    WUnfold unf(source, response);

    wrt.AddGraph(xv, response, "response");
    wrt.AddGraph(xv, source, "source");
    wrt.AddGraph(xv, unf.GetUnfold(), "unfold source");

    cout << endl << "all is ok" << endl;
    return 0;
}
