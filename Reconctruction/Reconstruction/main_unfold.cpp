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
#include "TText.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TAxis.h"
#include "TSpectrum.h"

#include "read_file.h"
#include "derivative.h"
#include "baseline.h"
#include "integral.h"
#include "individual_time_amp.h"
#include "time_measure.h"


using namespace std;

int main(int argc, char *argv[])
{
    struct timespec timespec_str_before, timespec_str_after, timespec_str_total_before, timespec_str_total_after;
    double t_read_file;
    double t_calculate_der, t_calculate_baseline, t_calculate_integral, t_calculate_abs_amp, t_unfolding;
    double t_tree_init, t_tree_add_graphs, t_tree_fill, t_tree_write, t_tree_close;

    clock_gettime(CLOCK_REALTIME, &timespec_str_total_before);

    //read params
    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am/";
    const string trees_dir = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees_unfold/";
    const string response_file_name = "/home/darkside/Vlad_Programs/Physical_results/avg_signal_1pe.txt";
    const int run_id = 6064;
    const int max_files = 10;

    //processing params
    const int time_scale = 4;//ns
    const double time_avr_baseline_to = 1600; // ns
    const double waveform_sign = +1;

    const double spe_integral_ch2 = -1484;
    const double spe_integral_ch1 = spe_integral_ch2;
    const double spe_integral_ch0 = -588;

    const double avg_integral_ch2 = -8693.58;
    const double avg_integral_ch1 = -1122;
    const double avg_integral_ch0 = -382;//avg_integral_ch0 < spe_integral_ch0 because there are a lot of signals without PMT response

    const double time_integral_from = 0;// ns
    const double time_integral_to = 14900;// ns
    const double trigger_time = 1900; //ns

    //unfold params
    const int numberIterations = 1000;
    const int numberRepetitions = 1;
    const double boost = 1;

    //write params
    const int events_per_file = 1000;

    //
    vector<double> xv_double;
    vector<float> xv_response;
    bool is_first_event = true;
    int counter_f_tree = 0;
    TFile* f_tree = NULL;
    TTree* tree = NULL;
    vector< vector<double> > response;
    vector<float> response_ch0;
    vector<float> response_ch1;
    vector<float> response_ch2;

    for(int file_i = 0; file_i < max_files; file_i++)
    {
        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
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

        if(waveform_sign > 0)
        {
            data[0] = vector_multiply(data[0], -waveform_sign);
            data[1] = vector_multiply(data[1], -waveform_sign);
            data[2] = vector_multiply(data[2], -waveform_sign);
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

            xv_response = vector_from_double_to_float(xv_double);
            cout << "xv was set" << endl;

            response = Get_response(response_file_name, trigger_time, time_scale, nsamps);
            response_ch0 = vector_from_double_to_float( vector_multiply(response[0], -waveform_sign * spe_integral_ch0 / avg_integral_ch0) );
            response_ch1 = vector_from_double_to_float( vector_multiply(response[1], -waveform_sign * spe_integral_ch1 / avg_integral_ch1) );
            response_ch2 = vector_from_double_to_float( vector_multiply(response[2], -waveform_sign * spe_integral_ch2 / avg_integral_ch2) );
        }
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_read_file += get_time_delta(timespec_str_before, timespec_str_after);

        //set variables to save in tree
        double integral_ch0, integral_ch1, integral_ch2;
        double baseline_ch0, baseline_ch1, baseline_ch2;

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //calcucate baseline
        baseline_ch0 = Get_baseline(data[0], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch1 = Get_baseline(data[1], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch2 = Get_baseline(data[2], (int)(time_avr_baseline_to / time_scale) );
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_baseline += get_time_delta(timespec_str_before, timespec_str_after);

        //calculate integral
        integral_ch0 = Get_integral(data[0], baseline_ch0, time_scale, time_integral_from, time_integral_to);
        integral_ch1 = Get_integral(data[1], baseline_ch1, time_scale, time_integral_from, time_integral_to);
        integral_ch2 = Get_integral(data[2], baseline_ch2, time_scale, time_integral_from, time_integral_to);

        //unfolding part 1
        TSpectrum s0;
        TSpectrum s1;
        TSpectrum s2;

        vector<float> source_ch0 = vector_from_double_to_float( vector_subtract(data[0], baseline_ch0) );
        vector<float> source_ch1 = vector_from_double_to_float( vector_subtract(data[1], baseline_ch1) );
        vector<float> source_ch2 = vector_from_double_to_float( vector_subtract(data[2], baseline_ch2) );


        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //calcucate baseline for data minus baseline
        baseline_ch0 = Get_baseline( vector_subtract(data[0], baseline_ch0), (int)(time_avr_baseline_to / time_scale) );
        baseline_ch1 = Get_baseline( vector_subtract(data[1], baseline_ch1), (int)(time_avr_baseline_to / time_scale) );
        baseline_ch2 = Get_baseline( vector_subtract(data[2], baseline_ch2), (int)(time_avr_baseline_to / time_scale) );
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_baseline += get_time_delta(timespec_str_before, timespec_str_after);

        //fill canvas
        TCanvas canv("c", "c", 0, 0, 1900, 1500);
        canv.Divide(3, 3);


        //cd1
        TGraph graph_cd1(nsamps, &xv_response[0], &response_ch0[0]);
        graph_cd1.SetTitle("response (Channel 0, PMT)");
        graph_cd1.GetXaxis()->SetTitle("time [ns]");
        graph_cd1.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(1);
        graph_cd1.Draw();

        //cd2
        TGraph graph_cd2(nsamps, &xv_response[0], &response_ch1[0]);
        graph_cd2.SetTitle("response (Channel 1, SiPM)");
        graph_cd2.GetXaxis()->SetTitle("time [ns]");
        graph_cd2.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(2);
        graph_cd2.Draw();

        //cd3
        TGraph graph_cd3(nsamps, &xv_response[0], &response_ch2[0]);
        graph_cd3.SetTitle("response (Channel 2, SiPM)");
        graph_cd3.GetXaxis()->SetTitle("time [ns]");
        graph_cd3.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(3);
        graph_cd3.Draw();


        //cd4
        TGraph graph_cd4(nsamps, &xv_response[0], &source_ch0[0]);
        graph_cd4.SetTitle("original - baseline (Channel 0, PMT)");
        graph_cd4.GetXaxis()->SetTitle("time [ns]");
        graph_cd4.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(4);
        graph_cd4.Draw();

        TF1 tf1_baseline_cd4("tf1_baseline_ch0","[0]",0,time_scale*nsamps);
        tf1_baseline_cd4.SetParameter(0,baseline_ch0);
        tf1_baseline_cd4.Draw("same");

        string text_cd4_integral = "integral[pe] = " + to_string(integral_ch0 / spe_integral_ch0);
        string text_cd4_baseline = "baseline = " + to_string(baseline_ch0);
        TPaveText pt_cd4(0.8,0.8,1,1,"nbNDC");
        pt_cd4.AddText(text_cd4_integral.c_str());
        pt_cd4.AddText(text_cd4_baseline.c_str());
        pt_cd4.Draw();


        //cd5
        TGraph graph_cd5(nsamps, &xv_response[0], &source_ch1[0]);
        graph_cd5.SetTitle("original - baseline (Channel 1, SiPM)");
        graph_cd5.GetXaxis()->SetTitle("time [ns]");
        graph_cd5.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(5);
        graph_cd5.Draw();

        TF1 tf1_baseline_cd5("tf1_baseline_ch1","[0]",0,time_scale*nsamps);
        tf1_baseline_cd5.SetParameter(0,baseline_ch1);
        tf1_baseline_cd5.Draw("same");

        string text_cd5_integral = "integral[pe] = " + to_string(integral_ch1 / spe_integral_ch1);
        string text_cd5_baseline = "baseline = " + to_string(baseline_ch1);
        TPaveText pt_cd5(0.8,0.8,1,1,"nbNDC");
        pt_cd5.AddText(text_cd5_integral.c_str());
        pt_cd5.AddText(text_cd5_baseline.c_str());
        pt_cd5.Draw();

        //cd6
        TGraph graph_cd6(nsamps, &xv_response[0], &source_ch2[0]);
        graph_cd6.SetTitle("original - baseline (Channel 2, SiPM)");
        graph_cd6.GetXaxis()->SetTitle("time [ns]");
        graph_cd6.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(6);
        graph_cd6.Draw();

        TF1 tf1_baseline_cd6("tf1_baseline_ch2","[0]",0,time_scale*nsamps);
        tf1_baseline_cd6.SetParameter(0,baseline_ch2);
        tf1_baseline_cd6.Draw("same");

        string text_cd6_integral = "integral[pe] = " + to_string(integral_ch2 / spe_integral_ch2);
        string text_cd6_baseline = "baseline = " + to_string(baseline_ch2);
        TPaveText pt_cd6(0.8,0.8,1,1,"nbNDC");
        pt_cd6.AddText(text_cd6_integral.c_str());
        pt_cd6.AddText(text_cd6_baseline.c_str());
        pt_cd6.Draw();

        //unfolding part 2
        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        s0.Deconvolution(&source_ch0[0],&response_ch0[0],nsamps,numberIterations,numberRepetitions,boost);
        s1.Deconvolution(&source_ch1[0],&response_ch1[0],nsamps,numberIterations,numberRepetitions,boost);
        s2.Deconvolution(&source_ch2[0],&response_ch2[0],nsamps,numberIterations,numberRepetitions,boost);
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_unfolding += get_time_delta(timespec_str_before, timespec_str_after);

        //cd7
        TGraph graph_cd7(nsamps, &xv_response[0], &source_ch0[0]);
        graph_cd7.SetTitle("unfolded signal (Channel 0, PMT)");
        graph_cd7.GetXaxis()->SetTitle("time [ns]");
        graph_cd7.GetYaxis()->SetTitle("amplitude[a.u.]");
        canv.cd(7);
        graph_cd7.Draw();

        //cd8
        TGraph graph_cd8(nsamps, &xv_response[0], &source_ch1[0]);
        graph_cd8.SetTitle("unfolded signal (Channel 1, SiPM)");
        graph_cd8.GetXaxis()->SetTitle("time [ns]");
        graph_cd8.GetYaxis()->SetTitle("amplitude[a.u.]");
        canv.cd(8);
        graph_cd8.Draw();

        //cd9
        TGraph graph_cd9(nsamps, &xv_response[0], &source_ch2[0]);
        graph_cd9.SetTitle("unfolded signal (Channel 2, SiPM)");
        graph_cd9.GetXaxis()->SetTitle("time [ns]");
        graph_cd9.GetYaxis()->SetTitle("amplitude[a.u.]");
        canv.cd(9);
        graph_cd9.Draw();

//        //cd2
//        TGraph graph_ch2(nsamps, &xv_double[0], &data[2][0]);
//        graph_ch2.SetTitle("original (Channel 2, SiPM)");
//        graph_ch2.GetXaxis()->SetTitle("time [ns]");
//        graph_ch2.GetYaxis()->SetTitle("amplitude[channels]");
//        canv.cd(2);
//        graph_ch2.Draw();

//        TF1 tf1_baseline_ch2("tf1_baseline_ch2","[0]",0,time_scale*nsamps);
//        tf1_baseline_ch2.SetParameter(0,baseline_ch2);
//        tf1_baseline_ch2.Draw("same");

//        string text_cd2_integral = "integral[pe] = " + to_string(integral_ch2 / spe_integral_ch2);
//        string text_cd2_baseline = "baseline = " + to_string(baseline_ch2);
//        TPaveText pt_cd2(0.8,0.8,1,1,"nbNDC");
//        pt_cd2.AddText(text_cd2_integral.c_str());
//        pt_cd2.AddText(text_cd2_baseline.c_str());
//        pt_cd2.Draw();




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

    clock_gettime(CLOCK_REALTIME, &timespec_str_total_after);
    cout << endl << "t_total[sec] = " << get_time_delta(timespec_str_total_before, timespec_str_total_after) << endl << endl;
    cout << "t_total / file [ms] = " << get_time_delta(timespec_str_total_before, timespec_str_total_after) / max_files * 1000 << endl;
    cout << "t_read_file / file [ms] = " << t_read_file / max_files * 1000 << endl;
    cout << "t_calculate_der / file [ms] = " << t_calculate_der / max_files * 1000 << endl;
    cout << "t_calculate_baseline / file [ms] = " << t_calculate_baseline / max_files * 1000 << endl;
    cout << "t_unfolding / file [ms] = " << t_unfolding / max_files * 1000 << endl;


    cout << endl << "all is ok" << endl;
    return 0;
}
