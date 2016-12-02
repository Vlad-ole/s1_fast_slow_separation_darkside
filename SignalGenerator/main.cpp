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

#include "response.h"
#include "writedata.h"
#include "generator.h"
#include "pdfgenerator.h"

using namespace std;

const double time_scale = 4;//ns
const double time_from = 0;
const double time_to = 8000;


int main(int argc, char *argv[])
{
    TF1 *ff1_response = new TF1("ff1_response","TMath::Gaus(x, 100, 33)");
    Response rsp("/home/darkside/Vlad_Programs/Physical_results/avg_signal_1pe.txt", 1, time_scale);
//    Response rsp(ff1_response, time_from, time_to, time_scale);

    PdfGenerator pdfgen(7, 1700, 0.2308, time_from, time_to);
    Generator gnr(&rsp, &pdfgen, time_from, time_to, time_scale);
    gnr.Generate(1000);

    WriteData wrt("/home/darkside/Vlad_Programs/vlad_rawdata/tests.root");
    wrt.AddGraph(rsp.Get_xv(), rsp.Get_yv(), "response");
    wrt.AddGraph(gnr.Get_xv(), gnr.Get_yv(), "source");

    cout << endl << "SignalGenerator: all is ok" << endl;
    return 0;
}
