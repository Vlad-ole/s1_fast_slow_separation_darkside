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
#include "fft.h"


using namespace std;

int main_unfold(int argc, char *argv[])
{
    struct timespec timespec_str_before, timespec_str_after, timespec_str_total_before, timespec_str_total_after;
    double t_read_file;
    double t_calculate_der, t_calculate_baseline, t_calculate_integral, t_calculate_abs_amp, t_unfolding;
    double t_tree_init, t_tree_add_graphs, t_tree_fill, t_tree_write, t_tree_close;

    clock_gettime(CLOCK_REALTIME, &timespec_str_total_before);

    //read params
    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am/";
    const string trees_dir = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees_unfold/";

//    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6053_bkg/";
//    const string trees_dir = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6053_bkg_trees_unfold/";

    const string response_file_name = "/home/darkside/Vlad_Programs/Physical_results/avg_signal_1pe.txt";
    const int run_id = 6064;
    const int max_files = 100000;

    //processing params
    const int time_scale = 4;//ns
    const double time_avr_baseline_to = 1600; // ns
    const double waveform_sign = 1;

    const double spe_integral_ch2 = -1484;
    const double spe_integral_ch1 = spe_integral_ch2;
    const double spe_integral_ch0 = -588;

    const double avg_integral_ch2 = -8693.58;
    const double avg_integral_ch1 = -1122;
    const double avg_integral_ch0 = -382;//avg_integral_ch0 < spe_integral_ch0 because there are a lot of signals without PMT response

    const double time_integral_from = 0;// ns
    const double time_integral_to = 7900;// ns
    const double trigger_time = 1900; //ns

    const double time_cut = time_integral_to;

    //integral intervals for n/g separation
    //we should choose accurately this parameter to integrate correctly. 1800 ns will be better
    const double time_integral_unfold_fast_pmt_from = 1900;
    const double time_integral_unfold_fast_pmt_to = 2020;

    const double time_integral_unfold_total_pmt_from = time_integral_unfold_fast_pmt_from;
    const double time_integral_unfold_total_pmt_to = time_integral_to;

    const double time_integral_unfold_fast_sipm_from = time_integral_unfold_fast_pmt_from;
    const double time_integral_unfold_fast_sipm_to = 2200;

    const double time_integral_unfold_total_sipm_from = time_integral_unfold_fast_pmt_from;
    const double time_integral_unfold_total_sipm_to = time_integral_to;


    //fft
    const double time_fft_from = 0;
    const double time_fft_to = time_integral_to;
    const double cut_frequency = 20;//MHz

    //unfold params
    const int numberIterations = 100;
    const int numberRepetitions = 1;
    const double boost = 1.0;

    //TPad params
    const double pad_x_1 = 0.7;
    const double pad_y_1 = 0.7;
    const double pad_x_2 = 1;
    const double pad_y_2 = 1;

    //write params
    const int events_per_file = 50;

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
        if (file_i % 1 == 0 ) cout << f_oss.str() << endl;

        //read data from binary file
        vector< vector<double> > data = Get_data( f_oss.str() );
        if(data[0].size() == 0 || data[1].size() == 0 || data[2].size() == 0)//some files can be empty. I do not know why
        {
            cout << "Incorrect binary file! event_id = " << file_i << endl;
            cout << "data[0].size() = " << data[0].size() << endl;
            cout << "data[1].size() = " << data[1].size() << endl;
            cout << "data[2].size() = " << data[2].size() << endl;
            continue;
        }

        //let's add some branches in order to simplify code with cuts
        //general
        double min_amp_ch0, min_amp_ch1, min_amp_ch2;

        //Am run
        double min_amp_ch0_0_1920;
        double min_amp_ch1_0_1800, min_amp_ch1_8000_15000;
        double min_amp_ch2_0_1800, min_amp_ch2_8000_15000;

        min_amp_ch0 = *min_element(data[0].begin(), data[0].end());
        min_amp_ch1 = *min_element(data[1].begin(), data[1].end());
        min_amp_ch2 = *min_element(data[2].begin(), data[2].end());

        min_amp_ch0_0_1920 = *min_element(data[0].begin(), data[0].begin() + 1920/time_scale );;

        min_amp_ch1_0_1800 = *min_element(data[1].begin(), data[1].begin() + 1800/time_scale );
        min_amp_ch1_8000_15000 = *min_element(data[1].begin() + 8000/time_scale, data[1].end() );

        min_amp_ch2_0_1800 = *min_element(data[2].begin(), data[2].begin() + 1800/time_scale );
        min_amp_ch2_8000_15000 = *min_element(data[2].begin() + 8000/time_scale, data[2].end() );

        //if x > time_cut then stop to fill vector (in order to minimize CPU time)
        data[0] = vector_cut_time(data[0], time_cut / time_scale);
        data[1] = vector_cut_time(data[1], time_cut / time_scale);
        data[2] = vector_cut_time(data[2], time_cut / time_scale);
        const int nsamps = data[0].size();



        //it is better to work with positive waveforms
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
            response_ch0 = vector_from_double_to_float( vector_cut_by_sign( vector_multiply(response[0], -waveform_sign * spe_integral_ch0 / avg_integral_ch0) , waveform_sign) );
            response_ch1 = vector_from_double_to_float( vector_cut_by_sign( vector_multiply(response[1], -waveform_sign * spe_integral_ch1 / avg_integral_ch1) , waveform_sign) );
            response_ch2 = vector_from_double_to_float( vector_cut_by_sign( vector_multiply(response[2], -waveform_sign * spe_integral_ch2 / avg_integral_ch2) , waveform_sign) );
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

        //subtract baseline
        data[0] = vector_subtract(data[0], baseline_ch0);
        data[1] = vector_subtract(data[1], baseline_ch1);
        data[2] = vector_subtract(data[2], baseline_ch2);


//        //fft & ifft
//        vector< vector<double> > ch0_fft = Get_fft_amp_spectrum(data[0], time_fft_from, time_fft_to, time_scale);
//        vector< vector<double> > ch1_fft = Get_fft_amp_spectrum(data[1], time_fft_from, time_fft_to, time_scale);
//        vector< vector<double> > ch2_fft = Get_fft_amp_spectrum(data[2], time_fft_from, time_fft_to, time_scale);

//        vector< vector<double> > ch0_fft_cut = vector_vector_cut_x_value(ch0_fft, cut_frequency);
//        vector< vector<double> > ch1_fft_cut = vector_vector_cut_x_value(ch1_fft, cut_frequency);
//        vector< vector<double> > ch2_fft_cut = vector_vector_cut_x_value(ch2_fft, cut_frequency);

//        vector< vector<double> > ch0_ifft = Get_ifft(ch0_fft_cut, nsamps, time_scale);
//        vector< vector<double> > ch1_ifft = Get_ifft(ch1_fft_cut, nsamps, time_scale);
//        vector< vector<double> > ch2_ifft = Get_ifft(ch2_fft_cut, nsamps, time_scale);


        //unfolding part 1
        TSpectrum s0;
        TSpectrum s1;
        TSpectrum s2;

        //unfold algorithm works correctly only if all data points have the same sign. I do not know why
        vector<float> source_ch0 = vector_from_double_to_float( vector_cut_by_sign( data[0], waveform_sign ) );
        vector<float> source_ch1 = vector_from_double_to_float( vector_cut_by_sign( data[1], waveform_sign ) );
        vector<float> source_ch2 = vector_from_double_to_float( vector_cut_by_sign( data[2], waveform_sign ) );

        //if you want to use fft
        //        vector<float> source_ch0 = vector_from_double_to_float( vector_cut_by_sign(  ch0_ifft[1], waveform_sign ) );
        //        vector<float> source_ch1 = vector_from_double_to_float( vector_cut_by_sign(  ch1_ifft[1], waveform_sign ) );
        //        vector<float> source_ch2 = vector_from_double_to_float( vector_cut_by_sign(  ch2_ifft[1], waveform_sign ) );


        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //calcucate baseline for data minus baseline. Just to be sure that I found baseline correctly
        baseline_ch0 = Get_baseline( data[0], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch1 = Get_baseline( data[1], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch2 = Get_baseline( data[2], (int)(time_avr_baseline_to / time_scale) );
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
        TPaveText pt_cd4(pad_x_1,pad_y_1,pad_x_2,pad_y_2,"nbNDC");
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
        TPaveText pt_cd5(pad_x_1,pad_y_1,pad_x_2,pad_y_2,"nbNDC");
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

//        TGraph graph_cd6_2(nsamps, &ch2_ifft[0][0], &ch2_ifft[1][0]);
//        graph_cd6_2.Draw("same pl");
//        graph_cd6_2.SetLineStyle(2);
//        graph_cd6_2.SetLineColor(kBlue);
//        graph_cd6_2.SetMarkerStyle(20);
//        graph_cd6_2.SetMarkerSize(1);
//        graph_cd6_2.SetMarkerColor(kBlue);

        TF1 tf1_baseline_cd6("tf1_baseline_ch2","[0]",0,time_scale*nsamps);
        tf1_baseline_cd6.SetParameter(0,baseline_ch2);
        tf1_baseline_cd6.Draw("same");

        string text_cd6_integral = "integral[pe] = " + to_string(integral_ch2 / spe_integral_ch2);
        string text_cd6_baseline = "baseline = " + to_string(baseline_ch2);
        TPaveText pt_cd6(pad_x_1,pad_y_1,pad_x_2,pad_y_2,"nbNDC");
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

        //calculate integrals to n/g separation
        double integral_ch0_unfold_fast = Get_integral(vector_from_float_to_double(source_ch0), 0, time_scale, time_integral_unfold_fast_pmt_from, time_integral_unfold_fast_pmt_to);
        double integral_ch1_unfold_fast = Get_integral(vector_from_float_to_double(source_ch1), 0, time_scale, time_integral_unfold_fast_sipm_from, time_integral_unfold_fast_sipm_to);
        double integral_ch2_unfold_fast = Get_integral(vector_from_float_to_double(source_ch2), 0, time_scale, time_integral_unfold_fast_sipm_from, time_integral_unfold_fast_sipm_to);

        double integral_ch0_unfold_total = Get_integral(vector_from_float_to_double(source_ch0), 0, time_scale, time_integral_unfold_total_pmt_from, time_integral_unfold_total_pmt_to);
        double integral_ch1_unfold_total = Get_integral(vector_from_float_to_double(source_ch1), 0, time_scale, time_integral_unfold_total_sipm_from, time_integral_unfold_total_sipm_to);
        double integral_ch2_unfold_total = Get_integral(vector_from_float_to_double(source_ch2), 0, time_scale, time_integral_unfold_total_sipm_from, time_integral_unfold_total_sipm_to);

        //cd7
        TGraph graph_cd7(nsamps, &xv_response[0], &source_ch0[0]);
        graph_cd7.SetTitle("unfolded signal (Channel 0, PMT)");
        graph_cd7.GetXaxis()->SetTitle("time [ns]");
        graph_cd7.GetYaxis()->SetTitle("amplitude[a.u.]");
        canv.cd(7);
        graph_cd7.Draw();

        string text_cd7_integral = "integral[pe] = " + to_string(integral_ch0_unfold_total / spe_integral_ch0);
        string text_cd7_integral_delta_ratio = "#frac{int_orig - int_unfold}{int_orig}[%] = " + to_string( 100*(integral_ch0 - integral_ch0_unfold_total)/integral_ch0 );
        string text_cd7_separation_ratio = "#frac{int_fast}{int_total} = " + to_string(integral_ch0_unfold_fast / integral_ch0_unfold_total);
        TPaveText pt_cd7(pad_x_1,pad_y_1,pad_x_2,pad_y_2,"nbNDC");
        pt_cd7.AddText(text_cd7_integral.c_str());
        pt_cd7.AddText(text_cd7_integral_delta_ratio.c_str());
        pt_cd7.AddText(text_cd7_separation_ratio.c_str());
        pt_cd7.Draw();

        //cd8
        TGraph graph_cd8(nsamps, &xv_response[0], &source_ch1[0]);
        graph_cd8.SetTitle("unfolded signal (Channel 1, SiPM)");
        graph_cd8.GetXaxis()->SetTitle("time [ns]");
        graph_cd8.GetYaxis()->SetTitle("amplitude[a.u.]");
        canv.cd(8);
        graph_cd8.Draw();

        string text_cd8_integral = "integral[pe] = " + to_string(integral_ch1_unfold_total / spe_integral_ch1);
        string text_cd8_integral_delta_ratio = "#frac{int_orig - int_unfold}{int_orig}[%] = " + to_string( 100*(integral_ch1 - integral_ch1_unfold_total)/integral_ch1 );
        string text_cd8_separation_ratio = "#frac{int_fast}{int_total} = " + to_string(integral_ch1_unfold_fast / integral_ch1_unfold_total);
        TPaveText pt_cd8(pad_x_1,pad_y_1,pad_x_2,pad_y_2,"nbNDC");
        pt_cd8.AddText(text_cd8_integral.c_str());
        pt_cd8.AddText(text_cd8_integral_delta_ratio.c_str());
        pt_cd8.AddText(text_cd8_separation_ratio.c_str());
        pt_cd8.Draw();

        //cd9
        TGraph graph_cd9(nsamps, &xv_response[0], &source_ch2[0]);
        graph_cd9.SetTitle("unfolded signal (Channel 2, SiPM)");
        graph_cd9.GetXaxis()->SetTitle("time [ns]");
        graph_cd9.GetYaxis()->SetTitle("amplitude[a.u.]");
        canv.cd(9);
        graph_cd9.Draw();

        string text_cd9_integral = "integral[pe] = " + to_string(integral_ch2_unfold_total / spe_integral_ch2);
        string text_cd9_integral_delta_ratio = "#frac{int_orig - int_unfold}{int_orig}[%] = " + to_string( 100*(integral_ch2 - integral_ch2_unfold_total)/integral_ch2 );
        string text_cd9_separation_ratio = "#frac{int_fast}{int_total} = " + to_string(integral_ch2_unfold_fast / integral_ch2_unfold_total);
        TPaveText pt_cd9(pad_x_1,pad_y_1,pad_x_2,pad_y_2,"nbNDC");
        pt_cd9.AddText(text_cd9_integral.c_str());
        pt_cd9.AddText(text_cd9_integral_delta_ratio.c_str());
        pt_cd9.AddText(text_cd9_separation_ratio.c_str());
        pt_cd9.Draw();

        if(file_i % events_per_file == 0)
        {
            //create file name to write root tree
            ostringstream file_tree_oss;
            file_tree_oss << trees_dir << "Run" << setfill('0') << setw(6) << run_id << "_block" << setfill('0') << setw(7) << counter_f_tree << ".root";
            counter_f_tree++;

            f_tree = TFile::Open(file_tree_oss.str().c_str(), "RECREATE");
            tree = new TTree("t1", "Parser tree");

            tree->Branch("integral_ch0", &integral_ch0, "integral_ch0/D");
            tree->Branch("integral_ch1", &integral_ch1, "integral_ch1/D");
            tree->Branch("integral_ch2", &integral_ch2, "integral_ch2/D");

            tree->Branch("integral_ch0_unfold_total", &integral_ch0_unfold_total, "integral_ch0_unfold_total/D");
            tree->Branch("integral_ch1_unfold_total", &integral_ch1_unfold_total, "integral_ch1_unfold_total/D");
            tree->Branch("integral_ch2_unfold_total", &integral_ch2_unfold_total, "integral_ch2_unfold_total/D");

            tree->Branch("integral_ch0_unfold_fast", &integral_ch0_unfold_fast, "integral_ch0_unfold_fast/D");
            tree->Branch("integral_ch1_unfold_fast", &integral_ch1_unfold_fast, "integral_ch1_unfold_fast/D");
            tree->Branch("integral_ch2_unfold_fast", &integral_ch2_unfold_fast, "integral_ch2_unfold_fast/D");

            tree->Branch("baseline_ch0", &baseline_ch0, "baseline_ch0/D");
            tree->Branch("baseline_ch1", &baseline_ch1, "baseline_ch1/D");
            tree->Branch("baseline_ch2", &baseline_ch2, "baseline_ch2/D");

            //for cuts (in general)
            tree->Branch("min_amp_ch0", &min_amp_ch0, "min_amp_ch0/D");
            tree->Branch("min_amp_ch1", &min_amp_ch1, "min_amp_ch1/D");
            tree->Branch("min_amp_ch2", &min_amp_ch2, "min_amp_ch2/D");

            //for cuts (Am run 6064)
            tree->Branch("min_amp_ch0_0_1920", &min_amp_ch0_0_1920, "min_amp_ch0_0_1920/D");
            tree->Branch("min_amp_ch1_0_1800", &min_amp_ch1_0_1800, "min_amp_ch1_0_1800/D");
            tree->Branch("min_amp_ch1_8000_15000", &min_amp_ch1_8000_15000, "min_amp_ch1_8000_15000/D");
            tree->Branch("min_amp_ch2_0_1800", &min_amp_ch2_0_1800, "min_amp_ch2_0_1800/D");
            tree->Branch("min_amp_ch2_8000_15000", &min_amp_ch2_8000_15000, "min_amp_ch2_8000_15000/D");

            tree->Branch("canvas_tr", "TCanvas", &canv);
        }

        tree->Fill();//there was crash. memory leak may be. Or problem with empty files. I should solve...
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
