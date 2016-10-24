#include <QCoreApplication>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

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

using namespace std;

int main(int argc, char *argv[])
{
    gROOT->SetBatch(kTRUE);

    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am/";
    const string trees_dir = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees/";
    const int run_id = 6064;
    bool is_first_event = true;
    const int time_scale = 4;//ns
    vector<int> xv;

    for(int file_i = 0; file_i < 1; file_i++)
    {
        ostringstream f_oss;
        f_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".out";
        cout << f_oss.str() << endl << endl;


        ifstream input_file;
        input_file.open(f_oss.str().c_str(), ios::binary);

        cout << "input_file.is_open() = " << input_file.is_open() << endl;
        if(!input_file.is_open())
        {
            cout << "error in input_file.open" << endl;
            return 1;
        }

        int nchans = 0;
        int nsamps = 0;

        input_file.read( (char *) &nchans, sizeof(int) );
        input_file.read( (char *) &nsamps, sizeof(int) );

        cout << "nchans = " << nchans << endl;
        cout << "nsamps = " << nsamps << endl;

        if(is_first_event)
        {
            xv.reserve(nsamps);
            is_first_event = false;
            for(int j = 0; j < nsamps; j++)
            {
                xv[j] = j * time_scale;
            }
        }

        vector<int> ch0_read;//PMT
        vector<int> ch1_read;//SiPM 1st part
        vector<int> ch2_read;//SiPM 2nd part

        ch0_read.resize(nsamps);
        ch1_read.resize(nsamps);
        ch2_read.resize(nsamps);

        input_file.read( (char *) (&ch0_read[0]), nsamps * sizeof(int) );
        input_file.read( (char *) (&ch1_read[0]), nsamps * sizeof(int) );
        input_file.read( (char *) (&ch2_read[0]), nsamps * sizeof(int) );

        input_file.close();


        ostringstream file_tree_oss;
        file_tree_oss << trees_dir << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".root";
        TFile f_tree(file_tree_oss.str().c_str(), "RECREATE");
        TTree tree("t1", "Parser tree");

        TCanvas canv("c", "c", 0, 0, 1900, 1000);
        canv.Divide(2, 2);
        tree.Branch("canvas_tr", "TCanvas", &canv);

        TGraph graph_ch1(ch1_read.size(), &xv[0], &ch1_read[0]);
        graph_ch1.SetTitle("original Chanel 1 (SiPM)");
        canv.cd(1);
        graph_ch1.Draw();

        tree.Fill();
        tree.Write();


    }


//    TTree tree("t1", "Parser tree");

    cout << endl << "all is ok" << endl;
    return 0;
}
