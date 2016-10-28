#include <QCoreApplication>

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

#include "read_file.h"
#include "derivative.h"
#include "baseline.h"
#include "integral.h"

using namespace std;

double get_time_delta(struct timespec timespec_str_before, struct timespec timespec_str_after)
{
    return (timespec_str_after.tv_sec + timespec_str_after.tv_nsec*1e-9 ) - (timespec_str_before.tv_sec + timespec_str_before.tv_nsec*1e-9 );
}

int main(int argc, char *argv[])
{
    struct timespec timespec_str_before, timespec_str_after, timespec_str_total_before, timespec_str_total_after;
    double t_read_file;
    double t_calculate_der, t_calculate_baseline, t_calculate_integral, t_calculate_abs_amp;
    double t_tree_init, t_tree_add_graphs, t_tree_fill, t_tree_write, t_tree_close;

    clock_gettime(CLOCK_REALTIME, &timespec_str_total_before);


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
    const int events_per_file = 1;
    const int max_files = 500;

    //
    vector<int> xv;
    vector<double> xv_double;
    bool is_first_event = true;
    TFile f_tree;


    for(int file_i = 0; file_i < max_files; file_i++)
    {

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
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
            xv_double.resize(nsamps);
            is_first_event = false;
            for(int j = 0; j < nsamps; j++)
            {
                xv[j] = j * time_scale;
            }

            for (int j = 0; j < nsamps; j++) xv_double[j] = xv[j]; //it is a stupid code, but TGraph don't have constructor TGraph(nsamp, int, double)

            cout << "xv was set" << endl;
        }
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_read_file += get_time_delta(timespec_str_before, timespec_str_after);



        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //caculate derivative
        vector<double> ch1_der = Get_derivative(data[1], der_param);
        vector<double> ch2_der = Get_derivative(data[2], der_param);
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_der += get_time_delta(timespec_str_before, timespec_str_after);



        //create file name to write root tree
        ostringstream file_tree_oss;
        file_tree_oss << trees_dir << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".root";

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        if(file_i % events_per_file == 0)
        {
            f_tree.Open(file_tree_oss.str().c_str(), "RECREATE");
            //        f_tree.SetCompressionLevel(0); //0 - without compression (max speedof writing), 9 - max compress(max speed of reading)
        }
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_init += get_time_delta(timespec_str_before, timespec_str_after);


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



        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //caculate baseline
        baseline_ch1 = Get_baseline(data[1], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch2 = Get_baseline(data[2], (int)(time_avr_baseline_to / time_scale) );
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_baseline += get_time_delta(timespec_str_before, timespec_str_after);


        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //calucate abs amp of signal
        max_abs_amp_ch1 = abs( *min_element(data[1].begin(), data[1].end()) - baseline_ch1 );
        max_abs_amp_ch2 = abs( *min_element(data[2].begin(), data[2].end()) - baseline_ch2 );
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_abs_amp += get_time_delta(timespec_str_before, timespec_str_after);

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //caculate integral
        integral_ch1 = Get_integral(data[1], baseline_ch1, time_scale, time_integral_from, time_integral_to);
        integral_ch2 = Get_integral(data[2], baseline_ch2, time_scale, time_integral_from, time_integral_to);
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_integral += get_time_delta(timespec_str_before, timespec_str_after);


        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //add graphs to canvas
        TCanvas canv("c", "c", 0, 0, 1900, 1000);
        canv.Divide(2, 2);
        tree.Branch("canvas_tr", "TCanvas", &canv);

        //cd1
        TGraph graph_ch1(nsamps, &xv[0], &data[1][0]);
        graph_ch1.SetTitle("original (Channel 1, SiPM)");
        canv.cd(1);
        graph_ch1.Draw("apl");

        TF1 tf1_baseline_ch1("tf1_baseline_ch1","[0]",0,time_scale*nsamps);
        tf1_baseline_ch1.SetParameter(0,baseline_ch1);
        tf1_baseline_ch1.Draw("same");

        string text_cd1_integral = "integral = " + to_string(integral_ch1);
        string text_cd1_baseline = "baseline = " + to_string(baseline_ch1);
        TPaveText pt_cd1(0.8,0.8,1,1,"nbNDC");
        pt_cd1.AddText(text_cd1_integral.c_str());
        pt_cd1.AddText(text_cd1_baseline.c_str());
        pt_cd1.Draw();

        //cd2
        TGraph graph_ch2(nsamps, &xv[0], &data[2][0]);
        graph_ch2.SetTitle("original (Channel 2, SiPM)");
        canv.cd(2);
        graph_ch2.Draw();

        TF1 tf1_baseline_ch2("tf1_baseline_ch2","[0]",0,time_scale*nsamps);
        tf1_baseline_ch2.SetParameter(0,baseline_ch2);
        tf1_baseline_ch2.Draw("same");

        string text_cd2_integral = "integral = " + to_string(integral_ch2);
        string text_cd2_baseline = "baseline = " + to_string(baseline_ch2);
        TPaveText pt_cd2(0.8,0.8,1,1,"nbNDC");
        pt_cd2.AddText(text_cd2_integral.c_str());
        pt_cd2.AddText(text_cd2_baseline.c_str());
        pt_cd2.Draw();


        //cd3
        TGraph graph_ch1_processing(nsamps, &xv_double[0], &ch1_der[0]);
        graph_ch1_processing.SetTitle("derivative (Channel 1, SiPM)");
        canv.cd(3);
        graph_ch1_processing.Draw();

        //cd4
        TGraph graph_ch2_processing(nsamps, &xv_double[0], &ch2_der[0]);
        graph_ch2_processing.SetTitle("derivative (Channel 2, SiPM)");
        canv.cd(4);
        graph_ch2_processing.Draw();

        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_add_graphs += get_time_delta(timespec_str_before, timespec_str_after);


        //save canvas and other parameters to tree
        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        tree.Fill();
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_fill += get_time_delta(timespec_str_before, timespec_str_after);

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        if(file_i % events_per_file == 0)
        {
            tree.Write(); //save only current tree
//               f_tree.Write();//save list of trees
        }
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_write += get_time_delta(timespec_str_before, timespec_str_after);

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        if(file_i % events_per_file == 0)
        {
            f_tree.Close();
//            cout << "event_id = " << file_i << endl;
        }
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_close += get_time_delta(timespec_str_before, timespec_str_after);


//        cout << endl;
    }

    clock_gettime(CLOCK_REALTIME, &timespec_str_total_after);
    cout << endl << "t_total[sec] = " << get_time_delta(timespec_str_total_before, timespec_str_total_after) << endl << endl;
    cout << "t_total / file [ms] = " << get_time_delta(timespec_str_total_before, timespec_str_total_after) / max_files * 1000 << endl;
    cout << "t_read_file / file [ms] = " << t_read_file / max_files * 1000 << endl;
    cout << "t_calculate_der / file [ms] = " << t_calculate_der / max_files * 1000 << endl;
    cout << "t_tree_init / file [ms] = " << t_tree_init / max_files * 1000 << endl;
    cout << "t_calculate_baseline / file [ms] = " << t_calculate_baseline / max_files * 1000 << endl;
    cout << "t_calculate_abs_amp / file [ms] = " << t_calculate_abs_amp / max_files * 1000 << endl;
    cout << "t_calculate_integral / file [ms] = " << t_calculate_integral / max_files * 1000 << endl;
    cout << "t_tree_add_graphs / file [ms] = " << t_tree_add_graphs / max_files * 1000 << endl;
    cout << "t_tree_fill / file [ms] = " << t_tree_fill / max_files * 1000 << endl;
    cout << "t_tree_write / file [ms] = " << t_tree_write / max_files * 1000 << endl;
    cout << "t_tree_close / file [ms] = " << t_tree_close / max_files * 1000 << endl;


    cout << endl << "all is ok" << endl;
    return 0;
}
