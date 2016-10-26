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
    const double time_integral_from = 1950; // ns
    const double time_integral_to = 1950 + 700; // ns
    const double time_avr_baseline_to = 1600; // ns

    //
    vector<int> xv;
    bool is_first_event = true;

    for(int file_i = 0; file_i < 1; file_i++)
    {
        //create file name to read binary file
        ostringstream f_oss;
        f_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".out";
        if (file_i % 100 == 0) cout << f_oss.str() << endl;

        //read data from binary file
        vector< vector<int> > data = Get_data( f_oss.str() );
        const int nsamps = data[0].size();
        if(data[0].size() == 0 || data[1].size() == 0 || data[2].size() == 0)//some files can be empty. I do not know why
        {
            cout << "Incorrect binary file! event_id = " << file_i << endl;
            cout << "data[0].size() = " << data[0].size() << endl;
            cout << "data[1].size() = " << data[1].size() << endl;
            cout << "data[2].size() = " << data[2].size() << endl;
            continue;
        }

        if(is_first_event)
        {
            xv.reserve(nsamps);
            is_first_event = false;
            for(int j = 0; j < nsamps; j++)
            {
                xv[j] = j * time_scale;
            }
            cout << "xv was set" << endl;
        }
        vector<double> xv_double;
        xv_double.resize(nsamps);
        for (int j = 0; j < nsamps; j++) xv_double[j] = xv[j]; //it is a stupid code, but TGraph don't have constructor TGraph(nsamp, int, double)

        //caculate derivative
        vector<double> ch1_der = Get_derivative(data[1], der_param);
        vector<double> ch2_der = Get_derivative(data[2], der_param);

        //create file name to write root tree
        ostringstream file_tree_oss;
        file_tree_oss << trees_dir << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".root";
        TFile f_tree(file_tree_oss.str().c_str(), "RECREATE");
        TTree tree("t1", "Parser tree");

        //set variables to save in tree
        double integral_ch1, integral_ch2;
        double baseline_ch1, baseline_ch2;
        double max_abs_amp_ch1, max_abs_amp_ch2;
        tree.Branch("integral_ch1", &integral_ch1, "integral_ch1/D");
        tree.Branch("integral_ch2", &integral_ch2, "integral_ch2/D");
        tree.Branch("baseline_ch1", &baseline_ch1, "baseline_ch1/D");
        tree.Branch("baseline_ch2", &baseline_ch2, "baseline_ch2/D");
        tree.Branch("max_abs_amp_ch1", &max_abs_amp_ch1, "max_abs_amp_ch1/D");
        tree.Branch("max_abs_amp_ch2", &max_abs_amp_ch2, "max_abs_amp_ch2/D");

        //just test. gererate test array
        int point_start = 2000 / time_scale ;
        int baseline_test = 3400;
        int point_stop = 3000 / time_scale ;
        int amp = 10;

        for (int i = 0; i < data[1].size(); ++i)
        {
            if(i >= point_start && i < point_stop)
            {
                data[1][i] = baseline_test - amp;
                data[2][i] = baseline_test - amp;
            }
            else
            {
                data[1][i] = baseline_test;
                data[2][i] = baseline_test;
            }
        }

        //caculate baseline
        baseline_ch1 = Get_baseline(data[1], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch2 = Get_baseline(data[2], (int)(time_avr_baseline_to / time_scale) );

        max_abs_amp_ch1 = abs( *min_element(data[1].begin(), data[1].end()) - baseline_ch1 );
        max_abs_amp_ch2 = abs( *min_element(data[2].begin(), data[2].end()) - baseline_ch2 );

        cout << "max_abs_amp_ch1 = " << max_abs_amp_ch1 << endl;
        cout << "max_abs_amp_ch2 = " << max_abs_amp_ch2 << endl;



        //caculate integral
//        integral_ch1 = Get_integral(data[1], baseline_ch1, time_scale, time_integral_from, time_integral_to);
//        integral_ch2 = Get_integral(data[2], baseline_ch2, time_scale, time_integral_from, time_integral_to);
        integral_ch1 = Get_integral(data[1], baseline_ch1, time_scale, 1000, 4000);
        integral_ch2 = Get_integral(data[2], baseline_ch2, time_scale, 2000, 3000);

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

//        cout << endl;
    }


    cout << endl << "all is ok" << endl;
    return 0;
}
