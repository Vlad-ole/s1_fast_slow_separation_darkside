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

#include "read_file.h"
#include "derivative.h"
#include "baseline.h"
#include "integral.h"

using namespace std;

int main(int argc, char *argv[])
{
    gROOT->SetBatch(kTRUE);

    //parameters
    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe/";
    const string trees_dir = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_trees/";
    const int run_id = 6061;
    const int time_scale = 4;//ns
    const int der_param = 10; // points

    vector<int> xv;
    bool is_first_event = true;

    for(int file_i = 0; file_i < 10; file_i++)
    {
        //create file name to read binary file
        ostringstream f_oss;
        f_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".out";
        cout << f_oss.str() << endl;

        //crerate file name to write root tree
        ostringstream file_tree_oss;
        file_tree_oss << trees_dir << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".root";
        TFile f_tree(file_tree_oss.str().c_str(), "RECREATE");
        TTree tree("t1", "Parser tree");

        //read data from binary file
        vector< vector<int> > data = Get_data( f_oss.str() );
        const int nsamps = data[0].size();
        if(is_first_event)
        {
            xv.reserve(nsamps);
            is_first_event = false;
            for(int j = 0; j < nsamps; j++)
            {
                xv[j] = j * time_scale;
            }
        }
        vector<double> xv_double;
        xv_double.resize(nsamps);
        for (int j = 0; j < nsamps; j++) xv_double[j] = xv[j]; //it is a stupid code, but TGraph don't have constructor TGraph(nsamp, int, double)

        //caculate derivative
        vector<double> ch1_der = Get_derivative(data[1], der_param);
        vector<double> ch2_der = Get_derivative(data[2], der_param);

        //set variables to save in tree
        double integral_ch1, integral_ch2;
        double baseline_ch1, baseline_ch2;
        tree.Branch("integral_ch1", &integral_ch1, "integral_ch1/D");
        tree.Branch("integral_ch2", &integral_ch2, "integral_ch2/D");
        tree.Branch("baseline_ch1", &baseline_ch1, "baseline_ch1/D");
        tree.Branch("baseline_ch2", &baseline_ch2, "baseline_ch2/D");

        //caculate baseline
        baseline_ch1 = Get_baseline(data[1], 1600 / time_scale);
        baseline_ch2 = Get_baseline(data[2], 1600 / time_scale);

        //caculate integral
        integral_ch1 = Get_integral(data[1], baseline_ch1, time_scale);
        integral_ch2 = Get_integral(data[2], baseline_ch2, time_scale);

        cout << "integral_ch1 = " << integral_ch1 << "; baseline_ch1 = " << baseline_ch1 << endl;
        cout << "integral_ch2 = " << integral_ch2 << "; baseline_c21 = " << baseline_ch2 << endl;


        //add graphs to canvas
        TCanvas canv("c", "c", 0, 0, 1900, 1000);
        canv.Divide(2, 2);
        tree.Branch("canvas_tr", "TCanvas", &canv);

        TGraph graph_ch1(nsamps, &xv[0], &data[1][0]);
        graph_ch1.SetTitle("original (Channel 1, SiPM)");
        canv.cd(1);
        graph_ch1.Draw();

        TGraph graph_ch2(nsamps, &xv[0], &data[2][0]);
        graph_ch2.SetTitle("original (Channel 2, SiPM)");
        canv.cd(2);
        graph_ch2.Draw();

        TGraph graph_ch1_processing(nsamps, &xv_double[0], &ch1_der[0]);
        graph_ch1_processing.SetTitle("derivative (Channel 1, SiPM)");
        canv.cd(3);
        graph_ch1_processing.Draw();

        TGraph graph_ch2_processing(nsamps, &xv_double[0], &ch2_der[0]);
        graph_ch2_processing.SetTitle("derivative (Channel 2, SiPM)");
        canv.cd(4);
        graph_ch2_processing.Draw();


        //save canvas and other parameters to tree
        tree.Fill();
        tree.Write();

        cout << endl;
    }


    cout << endl << "all is ok" << endl;
    return 0;
}
