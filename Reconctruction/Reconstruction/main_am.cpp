#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>  //for clock_gettime

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

#include "read_file.h"
#include "derivative.h"
#include "baseline.h"
#include "integral.h"
#include "individual_time_amp.h"


using namespace std;

int main(int argc, char *argv[])
{
    //read params
    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am/";
    const string trees_dir = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees/";
    const int run_id = 6064;
    const int max_files = 100;

    //processing params
    const int time_scale = 4;//ns
    const int der_param = 10; // points
    const double time_avr_baseline_to = 1600; // ns
    const double time_integral_from = 1900; // ns
    const double time_integral_to = 14500; // ns
    const double der_threshold = -10;
    const double n_points_savitzky_golay = 101;/*this number must be odd!!! */

    //write params
    const int events_per_file = 1000;


    //
    vector<double> xv_double;
    bool is_first_event = true;
    int counter_f_tree = 0;
    TFile* f_tree = NULL;
    TTree* tree = NULL;
    vector<double> C_i_der = Get_coeff_savitzky_golay(n_points_savitzky_golay);

    for(int file_i = 0; file_i < max_files; file_i++)
    {
        //create file name to read binary file
        ostringstream f_oss;
        f_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".out";
        if (file_i % 100 == 0) cout << f_oss.str() << endl;

        //read data from binary file
        vector< vector<double> > data = Get_data( f_oss.str() );
        const int nsamps = data[0].size();
        if(data[0].size() == 0 || data[1].size() == 0 || data[2].size() == 0)//some files can be empty. I do not know why
        {
            cout << "Incorrect binary file! event_id = " << file_i << endl;
            cout << "data[0].size() = " << data[0].size() << endl;
            cout << "data[1].size() = " << data[1].size() << endl;
            cout << "data[2].size() = " << data[2].size() << endl;
            continue;
        }

        //fill xv only once
        if(is_first_event)
        {
            xv_double.resize(nsamps);
            is_first_event = false;
            for(int j = 0; j < nsamps; j++)
            {
                xv_double[j] = j * time_scale;
            }

            cout << "xv was set" << endl;
        }

        //set variables to save in tree
        double integral_ch1, integral_ch2;
        double baseline_ch1, baseline_ch2;
        double max_abs_amp_ch1, max_abs_amp_ch2;

        //caculate derivative
        vector<double> ch1_der = Get_derivative(data[1], der_param);
        vector<double> ch2_der = Get_derivative(data[2], der_param);
        vector<double> ch1_der_savitzky_golay = Get_derivative_savitzky_golay(data[1], n_points_savitzky_golay, C_i_der);
        vector<double> ch2_der_savitzky_golay = Get_derivative_savitzky_golay(data[2], n_points_savitzky_golay, C_i_der);


        //calcucate baseline
        baseline_ch1 = Get_baseline(data[1], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch2 = Get_baseline(data[2], (int)(time_avr_baseline_to / time_scale) );

        //calcucate abs amp of signal
        max_abs_amp_ch1 = abs( *min_element(data[1].begin(), data[1].end()) - baseline_ch1 );
        max_abs_amp_ch2 = abs( *min_element(data[2].begin(), data[2].end()) - baseline_ch2 );

        //calculate integral
        integral_ch1 = Get_integral(data[1], baseline_ch1, time_scale, time_integral_from, time_integral_to);
        integral_ch2 = Get_integral(data[2], baseline_ch2, time_scale, time_integral_from, time_integral_to);

        //find trigg time and amp
        vector< vector<double> > individual_time_amp = get_individual_time_amp(ch1_der, der_threshold);




        TCanvas canv("c", "c", 0, 0, 1900, 1500);
        canv.Divide(2, 3);

        TGraph graph_cd1(nsamps, &xv_double[0], &data[1][0]);
        graph_cd1.SetTitle("original (Channel 1, SiPM)");
        graph_cd1.GetXaxis()->SetTitle("time [ns]");
        graph_cd1.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(1);
        graph_cd1.Draw();

        TGraph graph_ch2(nsamps, &xv_double[0], &data[2][0]);
        graph_ch2.SetTitle("original (Channel 2, SiPM)");
        graph_ch2.GetXaxis()->SetTitle("time [ns]");
        graph_ch2.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(2);
        graph_ch2.Draw();

        TGraph graph_cd3(nsamps, &xv_double[0], &ch1_der[0]);
        graph_cd3.SetTitle("derivative (Channel 1, SiPM)");
        graph_cd3.GetXaxis()->SetTitle("time [ns]");
        graph_cd3.GetYaxis()->SetTitle("derivative [channels / ns]");
        canv.cd(3);
        graph_cd3.Draw();

        TGraph graph_cd4(nsamps, &xv_double[0], &ch2_der[0]);
        graph_cd4.SetTitle("derivative (Channel 2, SiPM)");
        graph_cd4.GetXaxis()->SetTitle("time [ns]");
        graph_cd4.GetYaxis()->SetTitle("derivative [channels / ns]");
        canv.cd(4);
        graph_cd4.Draw();

        TGraph graph_cd5(nsamps, &xv_double[0], &ch1_der_savitzky_golay[0]);
        graph_cd5.SetTitle("derivative savitzky_golay (Channel 1, SiPM)");
        graph_cd5.GetXaxis()->SetTitle("time [ns]");
        graph_cd5.GetYaxis()->SetTitle("derivative [channels / ns]");
        canv.cd(5);
        graph_cd5.Draw();

        TGraph graph_cd6(nsamps, &xv_double[0], &ch2_der_savitzky_golay[0]);
        graph_cd6.SetTitle("derivative savitzky_golay (Channel 2, SiPM)");
        graph_cd6.GetXaxis()->SetTitle("time [ns]");
        graph_cd6.GetYaxis()->SetTitle("derivative [channels / ns]");
        canv.cd(6);
        graph_cd6.Draw();


        if(file_i % events_per_file == 0)
        {
            //create file name to write root tree
            ostringstream file_tree_oss;
            file_tree_oss << trees_dir << "Run" << setfill('0') << setw(6) << run_id << "_block" << setfill('0') << setw(7) << counter_f_tree << ".root";
            counter_f_tree++;

            f_tree = TFile::Open(file_tree_oss.str().c_str(), "RECREATE");
            tree = new TTree("t1", "Parser tree");

            tree->Branch("integral_ch1", &integral_ch1, "integral_ch1/D");
            tree->Branch("integral_ch2", &integral_ch2, "integral_ch2/D");
            tree->Branch("baseline_ch1", &baseline_ch1, "baseline_ch1/D");
            tree->Branch("baseline_ch2", &baseline_ch2, "baseline_ch2/D");
            tree->Branch("max_abs_amp_ch1", &max_abs_amp_ch1, "max_abs_amp_ch1/D");
            tree->Branch("max_abs_amp_ch2", &max_abs_amp_ch2, "max_abs_amp_ch2/D");

            tree->Branch("canvas_tr", "TCanvas", &canv);
        }

        tree->Fill();
        if(file_i % events_per_file == events_per_file-1) f_tree->Write();//save list of trees
        if(file_i % events_per_file == events_per_file-1)
        {
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

    cout << endl << "all is ok" << endl;
    return 0;
}
