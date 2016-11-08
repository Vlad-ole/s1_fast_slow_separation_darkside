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
#include "TText.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TAxis.h"

#include "read_file.h"
#include "derivative.h"
#include "baseline.h"
#include "integral.h"
#include "fft.h"
#include "time_measure.h"

using namespace std;


int main(int argc, char *argv[])
{
    struct timespec timespec_str_before, timespec_str_after, timespec_str_total_before, timespec_str_total_after;
    double t_read_file = 0;
    double t_calculate_der = 0, t_calculate_baseline = 0, t_calculate_integral = 0, t_calculate_abs_amp = 0, t_fft = 0;
    double t_tree_init = 0, t_tree_add_graphs = 0, t_tree_fill = 0, t_tree_write = 0, t_tree_close = 0;

    clock_gettime(CLOCK_REALTIME, &timespec_str_total_before);


    gROOT->SetBatch(kTRUE);

    //parameters
    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe/";
    const string trees_dir = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_trees/";
    const int run_id = 6061;
    const int time_scale = 4;//ns
    const int der_param = 10; // points
    const double time_integral_ch0_from = 2050; // ns
    const double time_integral_ch0_to = 2100; // ns
    const double time_integral_from = 1950; // ns
    const double time_integral_to = 1950 + 700; // ns
    const double time_avr_baseline_to = 1600; // ns
    const int events_per_file = 1000;
    const int max_files = 100;


//    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am/";
//    const string trees_dir = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees/";
//    const int run_id = 6064;
//    const int time_scale = 4;//ns
//    const int der_param = 10; //points
//    const double time_integral_from = 1900; // ns
//    const double time_integral_to = 14900; // ns
//    const double time_avr_baseline_to = 1600; // ns
//    const int events_per_file = 1000;
//    const int max_files = 100;




    //fft
    const double time_fft_noise_ch0_from = 0;//ns
    const double time_fft_noise_ch0_to = 2000;//ns
    const double time_fft_signal_ch0_from = time_integral_ch0_from;
    const double time_fft_signal_ch0_to = time_integral_ch0_to;

    const double time_fft_noise_from = 0;//ns
    const double time_fft_noise_to = time_avr_baseline_to;//ns
    const double time_fft_signal_from = time_integral_from;
    const double time_fft_signal_to = time_integral_to;

    //
    vector<double> xv_double;
    bool is_first_event = true;
    TFile* f_tree = NULL;
    TTree* tree = NULL;
    int counter_f_tree = 0;


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
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_read_file += get_time_delta(timespec_str_before, timespec_str_after);



        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //caculate derivative
        vector<double> ch0_der = Get_derivative(data[0], der_param);
        vector<double> ch1_der = Get_derivative(data[1], der_param);
        vector<double> ch2_der = Get_derivative(data[2], der_param);
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_der += get_time_delta(timespec_str_before, timespec_str_after);



        //set variables to save in tree
        double integral_ch0, integral_ch1, integral_ch2;
        double baseline_ch0, baseline_ch1, baseline_ch2;
        double max_abs_amp_ch0, max_abs_amp_ch1, max_abs_amp_ch2;


        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //calcucate baseline
        baseline_ch0 = Get_baseline(data[0], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch1 = Get_baseline(data[1], (int)(time_avr_baseline_to / time_scale) );
        baseline_ch2 = Get_baseline(data[2], (int)(time_avr_baseline_to / time_scale) );
        vector< vector<double> > data_minus_baseline;
        data_minus_baseline.resize(3);
        data_minus_baseline[0] = vector_subtract(data[0], baseline_ch0);
        data_minus_baseline[1] = vector_subtract(data[1], baseline_ch1);
        data_minus_baseline[2] = vector_subtract(data[2], baseline_ch2);
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_baseline += get_time_delta(timespec_str_before, timespec_str_after);


        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //calcucate abs amp of signal
        max_abs_amp_ch0 = abs( *min_element(data[0].begin(), data[0].end()) - baseline_ch0 );
        max_abs_amp_ch1 = abs( *min_element(data[1].begin(), data[1].end()) - baseline_ch1 );
        max_abs_amp_ch2 = abs( *min_element(data[2].begin(), data[2].end()) - baseline_ch2 );
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_abs_amp += get_time_delta(timespec_str_before, timespec_str_after);

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //calculate integral
        integral_ch0 = Get_integral(data[0], baseline_ch0, time_scale, time_integral_ch0_from, time_integral_ch0_to);
        integral_ch1 = Get_integral(data[1], baseline_ch1, time_scale, time_integral_from, time_integral_to);
        integral_ch2 = Get_integral(data[2], baseline_ch2, time_scale, time_integral_from, time_integral_to);
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_calculate_integral += get_time_delta(timespec_str_before, timespec_str_after);


//        //test data for fft
//        TF1 *f_ff1 = new TF1("fcos", "0.5*cos(2*3.1416*10*x - 3.1416*1/2.0) + sin(2*3.1416*35*x) + 1", 0, 1);
//        const double time = 20.0 / 10.0;
//        const double sampling_frequency = 100;//Hz
//        const int N_raw = time * sampling_frequency;
//        const int N_fft = N_raw/2 + 1;

//        const double delta_frequency = sampling_frequency / N_raw;
//        vector<double> test_data_y;
//        vector<double> test_data_x;
//        for (int i = 0; i < N_raw; ++i)
//        {
//            test_data_x.push_back( i/sampling_frequency );
//            test_data_y.push_back( f_ff1->Eval(i/sampling_frequency) );
//        }


        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        //calculate fft. Output in MHz
//        vector< vector<double> > ch1_fft_amp_spectum_noise = Get_fft_amp_spectrum(test_data_y, time_fft_noise_from, time_fft_noise_to, 1.0/sampling_frequency);

        vector< vector<double> > ch0_fft_amp_spectum_noise = Get_fft_amp_spectrum(data_minus_baseline[0], time_fft_noise_ch0_from, time_fft_noise_ch0_to, time_scale);
        vector< vector<double> > ch0_fft_amp_spectum_signal = Get_fft_amp_spectrum(data_minus_baseline[0], time_fft_signal_ch0_from, time_fft_signal_ch0_to, time_scale);

        vector< vector<double> > ch1_fft_amp_spectum_noise = Get_fft_amp_spectrum(data_minus_baseline[1], time_fft_noise_from, time_fft_noise_to, time_scale);
        vector< vector<double> > ch1_fft_amp_spectum_signal = Get_fft_amp_spectrum(data_minus_baseline[1], time_fft_signal_from, time_fft_signal_to, time_scale);

        vector< vector<double> > ch2_fft_amp_spectum_noise = Get_fft_amp_spectrum(data_minus_baseline[2], time_fft_noise_from, time_fft_noise_to, time_scale);
        vector< vector<double> > ch2_fft_amp_spectum_signal = Get_fft_amp_spectrum(data_minus_baseline[2], time_fft_signal_from, time_fft_signal_to, time_scale);
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_fft = get_time_delta(timespec_str_before, timespec_str_after);


        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        TCanvas canv("c", "c", 0, 0, 1900, 1500);
//        canv.SetCanvasSize(1900, 2000);
        //canv.SetWindowSize(100, 100);
        canv.Divide(3, 4);


        //cd1
        TGraph graph_cd1(nsamps, &xv_double[0], &data[0][0]);
        graph_cd1.SetTitle("original (Channel 0, PMT)");
        graph_cd1.GetXaxis()->SetTitle("time [ns]");
        graph_cd1.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(1);
        graph_cd1.Draw("apl");


        TF1 tf1_baseline_cd1("tf1_baseline_cd1","[0]",0,time_scale*nsamps);
        tf1_baseline_cd1.SetParameter(0,baseline_ch0);
        tf1_baseline_cd1.Draw("same");

        string text_cd1_integral = "integral = " + to_string(integral_ch0);
        string text_cd1_baseline = "baseline = " + to_string(baseline_ch0);
        TPaveText pt_cd1(0.8,0.8,1,1,"nbNDC");
        pt_cd1.AddText(text_cd1_integral.c_str());
        pt_cd1.AddText(text_cd1_baseline.c_str());
        pt_cd1.Draw();

        //cd2
        TGraph graph_cd2(nsamps, &xv_double[0], &data[1][0]);
        graph_cd2.SetTitle("original (Channel 1, SiPM)");
        graph_cd2.GetXaxis()->SetTitle("time [ns]");
        graph_cd2.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(2);
        graph_cd2.Draw("apl");


        TF1 tf1_baseline_cd2("tf1_baseline_cd2","[0]",0,time_scale*nsamps);
        tf1_baseline_cd2.SetParameter(0,baseline_ch1);
        tf1_baseline_cd2.Draw("same");

        string text_cd2_integral = "integral = " + to_string(integral_ch1);
        string text_cd2_baseline = "baseline = " + to_string(baseline_ch1);
        TPaveText pt_cd2(0.8,0.8,1,1,"nbNDC");
        pt_cd2.AddText(text_cd2_integral.c_str());
        pt_cd2.AddText(text_cd2_baseline.c_str());
        pt_cd2.Draw();

        //cd3
        TGraph graph_cd3(nsamps, &xv_double[0], &data[2][0]);
        graph_cd3.SetTitle("original (Channel 2, SiPM)");
        graph_cd3.GetXaxis()->SetTitle("time [ns]");
        graph_cd3.GetYaxis()->SetTitle("amplitude[channels]");
        canv.cd(3);
        graph_cd3.Draw();

        TF1 tf1_baseline_cd3("tf1_baseline_cd3","[0]",0,time_scale*nsamps);
        tf1_baseline_cd3.SetParameter(0,baseline_ch2);
        tf1_baseline_cd3.Draw("same");

        string text_cd3_integral = "integral = " + to_string(integral_ch2);
        string text_cd3_baseline = "baseline = " + to_string(baseline_ch2);
        TPaveText pt_cd3(0.8,0.8,1,1,"nbNDC");
        pt_cd3.AddText(text_cd3_integral.c_str());
        pt_cd3.AddText(text_cd3_baseline.c_str());
        pt_cd3.Draw();


        //cd4
        TGraph graph_cd4(nsamps, &xv_double[0], &ch0_der[0]);
        graph_cd4.SetTitle("derivative (Channel 0, PMT)");
        graph_cd4.GetXaxis()->SetTitle("time [ns]");
        graph_cd4.GetYaxis()->SetTitle("derivative [channels / ns]");
        canv.cd(4);
        graph_cd4.Draw();

        //cd5
        TGraph graph_cd5(nsamps, &xv_double[0], &ch1_der[0]);
        graph_cd5.SetTitle("derivative (Channel 1, SiPM)");
        graph_cd5.GetXaxis()->SetTitle("time [ns]");
        graph_cd5.GetYaxis()->SetTitle("derivative [channels / ns]");
        canv.cd(5);
        graph_cd5.Draw();

        //cd6
        TGraph graph_cd6(nsamps, &xv_double[0], &ch2_der[0]);
        graph_cd6.SetTitle("derivative (Channel 2, SiPM)");
        graph_cd6.GetXaxis()->SetTitle("time [ns]");
        graph_cd6.GetYaxis()->SetTitle("derivative [channels / ns]");
        canv.cd(6);
        graph_cd6.Draw();

        //cd7
        TGraph graph_cd7(ch0_fft_amp_spectum_noise[0].size(), &ch0_fft_amp_spectum_noise[0][0], &ch0_fft_amp_spectum_noise[1][0]);
        graph_cd7.SetTitle("Amplitude spectrum of noise signal (Channel 0, PMT)");
        graph_cd7.GetXaxis()->SetTitle("frequensy [MHz]");
        graph_cd7.GetYaxis()->SetTitle("amplitude [a.u.]");
        canv.cd(7);
        graph_cd7.Draw("AP");
        graph_cd7.SetMarkerStyle(20);
        graph_cd7.SetMarkerSize(0.5);
        graph_cd7.SetMarkerColor(kRed);


        //cd8
        TGraph graph_cd8(ch1_fft_amp_spectum_noise[0].size(), &ch1_fft_amp_spectum_noise[0][0], &ch1_fft_amp_spectum_noise[1][0]);
        graph_cd8.SetTitle("Amplitude spectrum of noise signal (Channel 1, SiPM)");
        graph_cd8.GetXaxis()->SetTitle("frequensy [MHz]");
        graph_cd8.GetYaxis()->SetTitle("amplitude [a.u.]");
        canv.cd(8);
        graph_cd8.Draw("AP");
        graph_cd8.SetMarkerStyle(20);
        graph_cd8.SetMarkerSize(0.5);
        graph_cd8.SetMarkerColor(kRed);

        //cd9
        TGraph graph_cd9(ch2_fft_amp_spectum_noise[0].size(), &ch2_fft_amp_spectum_noise[0][0], &ch2_fft_amp_spectum_noise[1][0]);
        graph_cd9.SetTitle("Amplitude spectrum of noise signal (Channel 2, SiPM)");
        graph_cd9.GetXaxis()->SetTitle("frequensy [MHz]");
        graph_cd9.GetYaxis()->SetTitle("amplitude [a.u.]");
        canv.cd(9);
        graph_cd9.Draw("AP");
        graph_cd9.SetMarkerStyle(20);
        graph_cd9.SetMarkerSize(0.5);
        graph_cd9.SetMarkerColor(kRed);

        //cd10
        TGraph graph_cd10(ch0_fft_amp_spectum_signal[0].size(), &ch0_fft_amp_spectum_signal[0][0], &ch0_fft_amp_spectum_signal[1][0]);
        graph_cd10.SetTitle("Amplitude spectrum of signal (Channel 0, PMT)");
        graph_cd10.GetXaxis()->SetTitle("frequensy [MHz]");
        graph_cd10.GetYaxis()->SetTitle("amplitude [a.u.]");
        canv.cd(10);
        graph_cd10.Draw("AP");
        graph_cd10.SetMarkerStyle(20);
        graph_cd10.SetMarkerSize(0.5);
        graph_cd10.SetMarkerColor(kRed);

        //cd11
        TGraph graph_cd11(ch1_fft_amp_spectum_signal[0].size(), &ch1_fft_amp_spectum_signal[0][0], &ch1_fft_amp_spectum_signal[1][0]);
        graph_cd11.SetTitle("Amplitude spectrum of signal (Channel 1, SiPM)");
        graph_cd11.GetXaxis()->SetTitle("frequensy [MHz]");
        graph_cd11.GetYaxis()->SetTitle("amplitude [a.u.]");
        canv.cd(11);
        graph_cd11.Draw("AP");
        graph_cd11.SetMarkerStyle(20);
        graph_cd11.SetMarkerSize(0.5);
        graph_cd11.SetMarkerColor(kRed);

        //cd12
        TGraph graph_cd12(ch2_fft_amp_spectum_signal[0].size(), &ch2_fft_amp_spectum_signal[0][0], &ch2_fft_amp_spectum_signal[1][0]);
        graph_cd12.SetTitle("Amplitude spectrum of signal (Channel 2, SiPM)");
        graph_cd12.GetXaxis()->SetTitle("frequensy [MHz]");
        graph_cd12.GetYaxis()->SetTitle("amplitude [a.u.]");
        canv.cd(12);
        graph_cd12.Draw("AP");
        graph_cd12.SetMarkerStyle(20);
        graph_cd12.SetMarkerSize(0.5);
        graph_cd12.SetMarkerColor(kRed);


        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_add_graphs += get_time_delta(timespec_str_before, timespec_str_after);

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        if(file_i % events_per_file == 0)
        {
            //create file name to write root tree
            ostringstream file_tree_oss;
            file_tree_oss << trees_dir << "Run" << setfill('0') << setw(6) << run_id << "_block" << setfill('0') << setw(7) << counter_f_tree << ".root";
            counter_f_tree++;

//            f_tree.Open(file_tree_oss.str().c_str(), "RECREATE");
            //        f_tree.SetCompressionLevel(0); //0 - without compression (max speedof writing), 9 - max compress(max speed of reading)
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
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_init += get_time_delta(timespec_str_before, timespec_str_after);


        //save canvas and other parameters to tree
        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        tree->Fill();
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_fill += get_time_delta(timespec_str_before, timespec_str_after);

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        if(file_i % events_per_file == events_per_file-1) f_tree->Write();//save list of trees
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_write += get_time_delta(timespec_str_before, timespec_str_after);

        clock_gettime(CLOCK_REALTIME, &timespec_str_before);
        if(file_i % events_per_file == events_per_file-1)
        {
            f_tree->Close();
            delete f_tree;
            f_tree = NULL;
            tree = NULL;
        }
        clock_gettime(CLOCK_REALTIME, &timespec_str_after);
        t_tree_close += get_time_delta(timespec_str_before, timespec_str_after);


//        cout << endl;
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
    cout << "t_tree_init / file [ms] = " << t_tree_init / max_files * 1000 << endl;
    cout << "t_calculate_baseline / file [ms] = " << t_calculate_baseline / max_files * 1000 << endl;
    cout << "t_calculate_abs_amp / file [ms] = " << t_calculate_abs_amp / max_files * 1000 << endl;
    cout << "t_calculate_integral / file [ms] = " << t_calculate_integral / max_files * 1000 << endl;
    cout << "t_fft / file [ms] = " << t_fft / max_files * 1000 << endl;
    cout << "t_tree_add_graphs / file [ms] = " << t_tree_add_graphs / max_files * 1000 << endl;
    cout << "t_tree_fill / file [ms] = " << t_tree_fill / max_files * 1000 << endl;
    cout << "t_tree_write / file [ms] = " << t_tree_write / max_files * 1000 << endl;
    cout << "t_tree_close / file [ms] = " << t_tree_close / max_files * 1000 << endl;


    cout << endl << "all is ok" << endl;
    return 0;
}
