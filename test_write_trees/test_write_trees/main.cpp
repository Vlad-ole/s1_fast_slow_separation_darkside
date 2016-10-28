#include <QCoreApplication>

#include <iostream>
#include <string>
#include <iomanip>


#include "TGraph.h"
#include "TFile.h"
#include "TRandom.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TRandom.h"

using namespace std;

int main(int argc, char *argv[])
{
    string s_output_file = "/home/darkside/Vlad_Programs/test.root";

    const int events_per_file = 2;
    const int n_events = 2;
    const int samp_per_event = 100;

    vector<double> xv;
    for (int i = 0; i < samp_per_event; ++i)
    {
        xv.push_back(i);
    }

    TRandom rnd;


    TFile* f_tree = NULL;
    TTree* tree = NULL;

    for(int file_i = 0; file_i < n_events; file_i++)
    {
        double just_some_value = file_i;
        vector<double> yv;
        for (int i = 0; i < samp_per_event; ++i)
        {
            yv.push_back( rnd.Rndm() );
        }
        TCanvas canv("c", "c", 0, 0, 1900, 1000);
        TGraph graph_ch1(samp_per_event, &xv[0], &yv[0]);
        graph_ch1.Draw();

        if(file_i % events_per_file == 0)
        {
            f_tree = TFile::Open(s_output_file.c_str(), "RECREATE");
            tree = new TTree("t1", "Tree with result");
            tree->Branch("just_some_value", &just_some_value, "just_some_value/D");
            tree->Branch("canvas_tr", "TCanvas", &canv);
        }


        //I want to save "events_per_file" events per one file
        tree->Fill();

        if(file_i % events_per_file == events_per_file-1)
        {
            f_tree->Write();
            f_tree->Close();
            delete f_tree;
            f_tree = NULL;
            tree = NULL;
        }
    }

    if(f_tree)
    {
        f_tree->Write();
        f_tree->Close();
        delete f_tree;
        f_tree = NULL;
        tree = NULL;
    }



    cout << "all is ok" << endl;
    return 0;
}
