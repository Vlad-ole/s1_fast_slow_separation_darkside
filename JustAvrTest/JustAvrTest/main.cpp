#include <iostream>
#include "readdata.h"

#include "TObjArray.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TFile.h"
#include "TAxis.h"

using namespace std;

int main(int argc, char *argv[])
{
    const int n_points = 1248;
    const int channel_id = 0;

    vector<double> yv_avr;
    yv_avr.resize(n_points);

    int counter = 0;
    for (int i = 0; i < 100000; ++i)
    {
        if(i % 1000 == 0) cout << "event = " << i << endl;

        ReadData rdt(i);
        const vector<vector<int> > data = rdt.GetDataInt();

        if( rdt.GetStatusIsOk() )
        {
            counter++;
            for (int j = 0; j < n_points; ++j)
            {
                yv_avr[j] += data[channel_id][j];
//                cout << "data[0][" << j <<"] = " << data[0][j] << endl;
            }
        }
    }

    cout << "counter = " << counter << endl;
    if(counter == 0)
    {
        cout << endl << "Error: nothing to avr!" << endl;
        exit(0);
    }
    for (int i = 0; i < n_points; ++i)
    {
         yv_avr[i] = yv_avr[i] / static_cast<double>(counter);
         cout << i << " " << yv_avr[i] << endl;
    }




    //let's plot avr graph and save in .root file
    vector<double> xv;
    xv.resize(n_points);
    for (int i = 0; i < n_points; ++i)
    {
        xv[i] = i * 4;
    }

    TGraph gr(xv.size(), &xv[0], &yv_avr[0]);
    gr.SetTitle("Signal from generator. 100000 avr signals");
    gr.GetXaxis()->SetTitle("time [ns]");
    gr.GetYaxis()->SetTitle("amplitude [channels]");

    TFile ofile("/home/darkside/Vlad_Programs/vlad_rawdata/JustAvrTest.root", "RECREATE");
    gr.Write();
    ofile.Close();

    cout << "JustAvrTest: all is ok" << endl;
    return 0;
}
