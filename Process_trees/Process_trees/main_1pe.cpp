#include <fstream>

void ReadTree()
{
    gROOT->SetBatch(kTRUE); // it's really important to use this line if you save TCanvas in a tree!

    //read param
    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_trees/";
    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_result.root";
    const int run_id = 6061;

//    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees/";
//    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_result.root";
//    const int run_id = 6064;


    //processing params
    const bool advanced_processing = false;
    const bool normal_processing = true;
    const double time_scale = 4;//ns

    TObjArray Hlist_gr(0);
    Hlist_gr.SetOwner(kTRUE);

    //TGraph* graph = 0;
    TCanvas* canv = 0;

    TChain chain("t1");// name of the tree is the argument
    //const int n_max = 22426; //Am
    const int n_max = 1;//number of files
    for(int i = 0; i < n_max; i++)
    {
        ostringstream file_tree_oss;
        file_tree_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_block" << setfill('0') << setw(7) << i << ".root";
        chain.Add(file_tree_oss.str().c_str());

        if(i % 100 == 0) cout << "file = " << i << endl;
    }
    cout << "chain.GetEntries() = " << chain.GetEntries() << endl;

    chain.SetBranchAddress("canvas_tr", &canv);

    //set variables to read them from tree
    double integral_ch0, integral_ch1, integral_ch2;
    double baseline_ch0, baseline_ch1, baseline_ch2;
    double max_abs_amp_ch0 ,max_abs_amp_ch1, max_abs_amp_ch2;

    chain.SetBranchAddress("integral_ch0", &integral_ch0);
    chain.SetBranchAddress("integral_ch1", &integral_ch1);
    chain.SetBranchAddress("integral_ch2", &integral_ch2);

    chain.SetBranchAddress("baseline_ch0", &baseline_ch0);
    chain.SetBranchAddress("baseline_ch1", &baseline_ch1);
    chain.SetBranchAddress("baseline_ch2", &baseline_ch2);

    chain.SetBranchAddress("max_abs_amp_ch0", &max_abs_amp_ch0);
    chain.SetBranchAddress("max_abs_amp_ch1", &max_abs_amp_ch1);
    chain.SetBranchAddress("max_abs_amp_ch2", &max_abs_amp_ch2);


    //TCut total_cut = "integral_ch2 > -50000 && integral_ch2 < 50000";
    //    TCut total_cut = "max_abs_amp_ch2 > 9 && max_abs_amp_ch2 < 14";
    //    TCut total_cut = "max_abs_amp_ch2 < 9";
    TCut total_cut = "";

    chain.SetMarkerStyle(4);

//    chain.Draw("integral_ch0>>h2(500,-3500,200)", total_cut);
    //    chain.Draw("integral_ch2>>h2(300,-35000,5000)", total_cut);

    //       chain.Draw("integral_ch2>>h2(500,-20000,5000)", total_cut); // >>hsqrt(bins, min, max)
    //        chain.Draw("integral_ch2>>h1(2000,-20000,5000)", total_cut);

    //   TH1F *h2 = (TH1F*)gDirectory->Get("h2");
    //   TH1F *h1 = (TH1F*)gDirectory->Get("h1");

    //   h1->SetFillStyle(3001);
    //   h2->SetFillStyle(3001);


    //   h1->SetFillColor(kBlue);
    //   h2->SetFillColor(kRed);


    //   h2->Draw();
    //   h1->Draw("same");

    //advanced processing
    if(advanced_processing)
    {

        bool is_first_time = true;
        int cut_event_counter = 0;
        int size_cd1_2_3_4_5_6;
        int size_cd7;
        int size_cd8_9;
        int size_cd10;
        int size_cd11_12;
        vector<double> xv_cd1_2_3_4_5_6;
        vector<double> xv_cd7;
        vector<double> xv_cd8_9;
        vector<double> xv_cd10;
        vector<double> xv_cd11_12;

        vector<double> yv_cd1;
        vector<double> yv_cd2;
        vector<double> yv_cd3;
        vector<double> yv_cd4;
        vector<double> yv_cd5;
        vector<double> yv_cd6;
        vector<double> yv_cd7;
        vector<double> yv_cd8;
        vector<double> yv_cd9;
        vector<double> yv_cd10;
        vector<double> yv_cd11;
        vector<double> yv_cd12;

        double avr_integral_ch0 = 0;
        double avr_integral_ch1 = 0;
        double avr_integral_ch2 = 0;

        double avr_baseline_ch0 = 0;
        double avr_baseline_ch1 = 0;
        double avr_baseline_ch2 = 0;


        //    const int n_entr = 100;
        const int n_entr = chain.GetEntries();
        for (int i = 0; i < n_entr; ++i)
        {
            chain.GetEntry(i);
            if(i % 100 == 0) cout << "event = " << i << endl;

            if(/*max_abs_amp_ch2 > 9 && max_abs_amp_ch2 < 15*/ 1)//start cut
            {
                //                //            cout << "cut_event_counter = " << cut_event_counter << endl;
                cut_event_counter++;

                avr_integral_ch0 += integral_ch0;
                avr_integral_ch1 += integral_ch1;
                avr_integral_ch2 += integral_ch2;

                avr_baseline_ch0 += baseline_ch0;
                avr_baseline_ch1 += baseline_ch1;
                avr_baseline_ch2 += baseline_ch2;

                TPad *pad_cd1 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_1");
                TGraph *gh_cd1 = (TGraph*)pad_cd1->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd2 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_2");
                TGraph *gh_cd2 = (TGraph*)pad_cd2->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd3 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_3");
                TGraph *gh_cd3 = (TGraph*)pad_cd3->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd4 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_4");
                TGraph *gh_cd4 = (TGraph*)pad_cd4->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd5 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_5");
                TGraph *gh_cd5 = (TGraph*)pad_cd5->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd6 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_6");
                TGraph *gh_cd6 = (TGraph*)pad_cd6->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd7 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_7");
                TGraph *gh_cd7 = (TGraph*)pad_cd7->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd8 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_8");
                TGraph *gh_cd8 = (TGraph*)pad_cd8->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd9 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_9");
                TGraph *gh_cd9 = (TGraph*)pad_cd9->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd10 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_10");
                TGraph *gh_cd10 = (TGraph*)pad_cd10->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd11 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_11");
                TGraph *gh_cd11 = (TGraph*)pad_cd11->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd12 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_12");
                TGraph *gh_cd12 = (TGraph*)pad_cd12->GetListOfPrimitives()->FindObject("Graph");

                if(gh_cd1 == NULL || gh_cd2 == NULL || gh_cd3 == NULL || gh_cd4 == NULL
                        || gh_cd5 == NULL || gh_cd6 == NULL || gh_cd7 == NULL || gh_cd8 == NULL
                        || gh_cd9 == NULL || gh_cd10 == NULL || gh_cd11 == NULL || gh_cd12 == NULL) cout << "graph pnt in null" << endl;


                if(is_first_time)
                {
                    is_first_time = false;
                    size_cd1_2_3_4_5_6 = gh_cd1->GetN();
                    size_cd7 = gh_cd7->GetN();
                    size_cd8_9 = gh_cd8->GetN();
                    size_cd10 = gh_cd10->GetN();
                    size_cd11_12 = gh_cd11->GetN();

                    xv_cd1_2_3_4_5_6.resize(size_cd1_2_3_4_5_6);
                    xv_cd7.resize(size_cd7);
                    xv_cd8_9.resize(size_cd8_9);
                    xv_cd10.resize(size_cd10);
                    xv_cd11_12.resize(size_cd11_12);


                    yv_cd1.resize(size_cd1_2_3_4_5_6);
                    yv_cd2.resize(size_cd1_2_3_4_5_6);
                    yv_cd3.resize(size_cd1_2_3_4_5_6);
                    yv_cd4.resize(size_cd1_2_3_4_5_6);
                    yv_cd5.resize(size_cd1_2_3_4_5_6);
                    yv_cd6.resize(size_cd1_2_3_4_5_6);


                    yv_cd7.resize(size_cd7);
                    yv_cd8.resize(size_cd8_9);
                    yv_cd9.resize(size_cd8_9);


                    yv_cd10.resize(size_cd10);
                    yv_cd11.resize(size_cd11_12);
                    yv_cd12.resize(size_cd11_12);

                    for (int k = 0; k < size_cd1_2_3_4_5_6; ++k)
                    {
                        double x, y;
                        gh_cd1->GetPoint(k, xv_cd1_2_3_4_5_6[k], y);
                    }

                    for (int k = 0; k < size_cd7; ++k)
                    {
                        double x, y;
                        gh_cd7->GetPoint(k, xv_cd7[k], y);
                    }

                    for (int k = 0; k < size_cd8_9; ++k)
                    {
                        double x, y;
                        gh_cd8->GetPoint(k, xv_cd8_9[k], y);
                    }

                    for (int k = 0; k < size_cd10; ++k)
                    {
                        double x, y;
                        gh_cd10->GetPoint(k, xv_cd10[k], y);
                    }

                    for (int k = 0; k < size_cd11_12; ++k)
                    {
                        double x, y;
                        gh_cd11->GetPoint(k, xv_cd11_12[k], y);
                    }

                }

                for (int k = 0; k < size_cd1_2_3_4_5_6; ++k)
                {
                    double x, y;

                    gh_cd1->GetPoint(k, x, y);
                    yv_cd1[k] += y;

                    gh_cd2->GetPoint(k, x, y);
                    yv_cd2[k] += y;

                    gh_cd3->GetPoint(k, x, y);
                    yv_cd3[k] += y;

                    gh_cd4->GetPoint(k, x, y);
                    yv_cd4[k] += y;

                    gh_cd5->GetPoint(k, x, y);
                    yv_cd5[k] += y;

                    gh_cd6->GetPoint(k, x, y);
                    yv_cd6[k] += y;
                }

                for (int k = 0; k < size_cd7; ++k)
                {
                    double x, y;
                    gh_cd7->GetPoint(k, x, y);
                    yv_cd7[k] += y;
                }

                for (int k = 0; k < size_cd8_9; ++k)
                {
                    double x, y;
                    gh_cd8->GetPoint(k, x, y);
                    yv_cd8[k] += y;

                    gh_cd9->GetPoint(k, x, y);
                    yv_cd9[k] += y;
                }

                for (int k = 0; k < size_cd10; ++k)
                {
                    double x, y;
                    gh_cd10->GetPoint(k, x, y);
                    yv_cd10[k] += y;
                }

                for (int k = 0; k < size_cd11_12; ++k)
                {
                    double x, y;
                    gh_cd11->GetPoint(k, x, y);
                    yv_cd11[k] += y;

                    gh_cd12->GetPoint(k, x, y);
                    yv_cd12[k] += y;
                }


                //                //        Hlist_gr.Add( gh_cd1->Clone() );
                //                //                    Hlist_gr.Add( canv->Clone() );
            }//end cut
        }

        //divide summ by cut_event_counter
        avr_integral_ch0 /= cut_event_counter;
        avr_integral_ch1 /= cut_event_counter;
        avr_integral_ch2 /= cut_event_counter;

        avr_baseline_ch0 /= cut_event_counter;
        avr_baseline_ch1 /= cut_event_counter;
        avr_baseline_ch2 /= cut_event_counter;


        for (int i = 0; i < xv_cd1_2_3_4_5_6.size(); ++i)
        {
            yv_cd1[i] /= cut_event_counter;
            yv_cd2[i] /= cut_event_counter;
            yv_cd3[i] /= cut_event_counter;
            yv_cd4[i] /= cut_event_counter;
            yv_cd5[i] /= cut_event_counter;
            yv_cd6[i] /= cut_event_counter;
        }

        for (int i = 0; i < xv_cd7.size(); ++i)
        {
            yv_cd7[i] /= cut_event_counter;
        }

        for (int i = 0; i < xv_cd8_9.size(); ++i)
        {
            yv_cd8[i] /= cut_event_counter;
            yv_cd9[i] /= cut_event_counter;
        }


        for (int i = 0; i < xv_cd10.size(); ++i)
        {
            yv_cd10[i] /= cut_event_counter;
        }

        for (int i = 0; i < xv_cd11_12.size(); ++i)
        {
            yv_cd11[i] /= cut_event_counter;
            yv_cd12[i] /= cut_event_counter;
        }

        TCanvas canv_result("c", "c", 0, 0, 1900, 1500);
        canv_result.Divide(3, 4);

        if(cut_event_counter == 0)
        {
            cout << "cut_event_counter == 0 !" << endl;
        }
        else
        {
            cout << cut_event_counter << " events passed through cuts" << endl;

            //create graphs
            TGraph graph_cd1(xv_cd1_2_3_4_5_6.size(), &xv_cd1_2_3_4_5_6[0], &yv_cd1[0]);
            TGraph graph_cd2(xv_cd1_2_3_4_5_6.size(), &xv_cd1_2_3_4_5_6[0], &yv_cd2[0]);
            TGraph graph_cd3(xv_cd1_2_3_4_5_6.size(), &xv_cd1_2_3_4_5_6[0], &yv_cd3[0]);
            TGraph graph_cd4(xv_cd1_2_3_4_5_6.size(), &xv_cd1_2_3_4_5_6[0], &yv_cd4[0]);
            TGraph graph_cd5(xv_cd1_2_3_4_5_6.size(), &xv_cd1_2_3_4_5_6[0], &yv_cd5[0]);
            TGraph graph_cd6(xv_cd1_2_3_4_5_6.size(), &xv_cd1_2_3_4_5_6[0], &yv_cd6[0]);

            TGraph graph_cd7(xv_cd7.size(), &xv_cd7[0], &yv_cd7[0]);

            TGraph graph_cd8(xv_cd8_9.size(), &xv_cd8_9[0], &yv_cd8[0]);
            TGraph graph_cd9(xv_cd8_9.size(), &xv_cd8_9[0], &yv_cd9[0]);

            TGraph graph_cd10(xv_cd10.size(), &xv_cd10[0], &yv_cd10[0]);

            TGraph graph_cd11(xv_cd11_12.size(), &xv_cd11_12[0], &yv_cd11[0]);
            TGraph graph_cd12(xv_cd11_12.size(), &xv_cd11_12[0], &yv_cd12[0]);

            //cd1
            graph_cd1.SetTitle("original (Channel 0, PMT)");
            graph_cd1.GetXaxis()->SetTitle("time [ns]");
            graph_cd1.GetYaxis()->SetTitle("amplitude[channels]");
            canv_result.cd(1);
            graph_cd1.Draw("apl");

            TF1 tf1_baseline_cd1("tf1_baseline_cd1","[0]",0,time_scale*xv_cd1_2_3_4_5_6.size());
            tf1_baseline_cd1.SetParameter(0,avr_baseline_ch0);
            tf1_baseline_cd1.Draw("same");


            ostringstream sstream_text_cd1_integral;
            sstream_text_cd1_integral << "integral = " << avr_integral_ch0;
            ostringstream sstream_text_cd1_baseline;
            sstream_text_cd1_baseline << "baseline = " << avr_baseline_ch0;

            TPaveText pt_cd1(0.8,0.8,1,1,"nbNDC");
            pt_cd1.AddText(sstream_text_cd1_integral.str().c_str());
            pt_cd1.AddText(sstream_text_cd1_baseline.str().c_str());
            pt_cd1.Draw();

            //cd2
            graph_cd2.SetTitle("original (Channel 1, SiPM)");
            graph_cd2.GetXaxis()->SetTitle("time [ns]");
            graph_cd2.GetYaxis()->SetTitle("amplitude[channels]");
            canv_result.cd(2);
            graph_cd2.Draw("apl");


            TF1 tf1_baseline_cd2("tf1_baseline_cd2","[0]",0,time_scale*xv_cd1_2_3_4_5_6.size());
            tf1_baseline_cd2.SetParameter(0,avr_baseline_ch1);
            tf1_baseline_cd2.Draw("same");

            ostringstream sstream_text_cd2_integral;
            sstream_text_cd2_integral << "integral = " << avr_integral_ch1;
            ostringstream sstream_text_cd2_baseline;
            sstream_text_cd2_baseline << "baseline = " << avr_baseline_ch1;

            TPaveText pt_cd2(0.8,0.8,1,1,"nbNDC");
            pt_cd2.AddText(sstream_text_cd2_integral.str().c_str());
            pt_cd2.AddText(sstream_text_cd2_baseline.str().c_str());
            pt_cd2.Draw();

            //cd3
            graph_cd3.SetTitle("original (Channel 2, SiPM)");
            graph_cd3.GetXaxis()->SetTitle("time [ns]");
            graph_cd3.GetYaxis()->SetTitle("amplitude[channels]");
            canv_result.cd(3);
            graph_cd3.Draw();

            TF1 tf1_baseline_cd3("tf1_baseline_cd3","[0]",0,time_scale*xv_cd1_2_3_4_5_6.size());
            tf1_baseline_cd3.SetParameter(0,avr_baseline_ch2);
            tf1_baseline_cd3.Draw("same");

            ostringstream sstream_text_cd3_integral;
            sstream_text_cd3_integral << "integral = " << avr_integral_ch2;
            ostringstream sstream_text_cd3_baseline;
            sstream_text_cd3_baseline << "baseline = " << avr_baseline_ch2;

            TPaveText pt_cd3(0.8,0.8,1,1,"nbNDC");
            pt_cd3.AddText(sstream_text_cd3_integral.str().c_str());
            pt_cd3.AddText(sstream_text_cd3_baseline.str().c_str());
            pt_cd3.Draw();

            //cd4
            graph_cd4.SetTitle("derivative (Channel 0, PMT)");
            graph_cd4.GetXaxis()->SetTitle("time [ns]");
            graph_cd4.GetYaxis()->SetTitle("derivative [channels / ns]");
            canv_result.cd(4);
            graph_cd4.Draw();

            //cd5
            graph_cd5.SetTitle("derivative (Channel 1, SiPM)");
            graph_cd5.GetXaxis()->SetTitle("time [ns]");
            graph_cd5.GetYaxis()->SetTitle("derivative [channels / ns]");
            canv_result.cd(5);
            graph_cd5.Draw();

            //cd6
            graph_cd6.SetTitle("derivative (Channel 2, SiPM)");
            graph_cd6.GetXaxis()->SetTitle("time [ns]");
            graph_cd6.GetYaxis()->SetTitle("derivative [channels / ns]");
            canv_result.cd(6);
            graph_cd6.Draw();

            //cd7
            graph_cd7.SetTitle("Amplitude spectrum of noise signal (Channel 0, PMT)");
            graph_cd7.GetXaxis()->SetTitle("frequensy [MHz]");
            graph_cd7.GetYaxis()->SetTitle("amplitude [a.u.]");
            canv_result.cd(7);
            graph_cd7.Draw("AP");
            graph_cd7.SetMarkerStyle(20);
            graph_cd7.SetMarkerSize(0.5);
            graph_cd7.SetMarkerColor(kRed);

            //cd8
            graph_cd8.SetTitle("Amplitude spectrum of noise signal (Channel 1, SiPM)");
            graph_cd8.GetXaxis()->SetTitle("frequensy [MHz]");
            graph_cd8.GetYaxis()->SetTitle("amplitude [a.u.]");
            canv_result.cd(8);
            graph_cd8.Draw("AP");
            graph_cd8.SetMarkerStyle(20);
            graph_cd8.SetMarkerSize(0.5);
            graph_cd8.SetMarkerColor(kRed);

            //cd9
            graph_cd9.SetTitle("Amplitude spectrum of noise signal (Channel 2, SiPM)");
            graph_cd9.GetXaxis()->SetTitle("frequensy [MHz]");
            graph_cd9.GetYaxis()->SetTitle("amplitude [a.u.]");
            canv_result.cd(9);
            graph_cd9.Draw("AP");
            graph_cd9.SetMarkerStyle(20);
            graph_cd9.SetMarkerSize(0.5);
            graph_cd9.SetMarkerColor(kRed);

            //cd10
            graph_cd10.SetTitle("Amplitude spectrum of signal (Channel 0, PMT)");
            graph_cd10.GetXaxis()->SetTitle("frequensy [MHz]");
            graph_cd10.GetYaxis()->SetTitle("amplitude [a.u.]");
            canv_result.cd(10);
            graph_cd10.Draw("AP");
            graph_cd10.SetMarkerStyle(20);
            graph_cd10.SetMarkerSize(0.5);
            graph_cd10.SetMarkerColor(kRed);

            //cd11
            graph_cd11.SetTitle("Amplitude spectrum of signal (Channel 1, SiPM)");
            graph_cd11.GetXaxis()->SetTitle("frequensy [MHz]");
            graph_cd11.GetYaxis()->SetTitle("amplitude [a.u.]");
            canv_result.cd(11);
            graph_cd11.Draw("AP");
            graph_cd11.SetMarkerStyle(20);
            graph_cd11.SetMarkerSize(0.5);
            graph_cd11.SetMarkerColor(kRed);

            //cd12
            graph_cd12.SetTitle("Amplitude spectrum of signal (Channel 2, SiPM)");
            graph_cd12.GetXaxis()->SetTitle("frequensy [MHz]");
            graph_cd12.GetYaxis()->SetTitle("amplitude [a.u.]");
            canv_result.cd(12);
            graph_cd12.Draw("AP");
            graph_cd12.SetMarkerStyle(20);
            graph_cd12.SetMarkerSize(0.5);
            graph_cd12.SetMarkerColor(kRed);

        }



        TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
        //        Hlist_gr.Write();
        canv_result.Write();
        ofile_Hlist_gr.Close();


//        //save averaged response
//        ofstream out_file;
//        out_file.open("/home/darkside/Vlad_Programs/Physical_results/avg_signal_1pe.txt");
//        for (int j = 0; j < xv_cd1_2_3_4_5_6.size(); ++j)
//        {
//            out_file << (yv_cd1[j] - avr_baseline_ch0) << " " << (yv_cd2[j] - avr_baseline_ch1) << " " << (yv_cd3[j] - avr_baseline_ch2) << endl;
//        }
//        out_file.close();

    }


    if(normal_processing)
    {
        const int n_entr = chain.GetEntries();
        for (int i = 0; i < n_entr; ++i)
        {
            chain.GetEntry(i);
            if(i % 100 == 0) cout << "event = " << i << endl;
            if(integral_ch0 > -100)
            {
                Hlist_gr.Add( canv->Clone() );
            }

        }

        TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
        Hlist_gr.Write();
        ofile_Hlist_gr.Close();
    }




    cout << endl << "Root cern script: all is ok" << endl;

}
