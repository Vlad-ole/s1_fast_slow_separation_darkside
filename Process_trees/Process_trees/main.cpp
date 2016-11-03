void ReadTree()
{
    gROOT->SetBatch(kTRUE); // it's really important to use this line if you save TCanvas in a tree!

    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_trees/";
    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_result.root";
    const int run_id = 6061;

    TObjArray Hlist_gr(0);
    Hlist_gr.SetOwner(kTRUE);

    //TGraph* graph = 0;
    TCanvas* canv = 0;

    TChain chain("t1");// name of the tree is the argument
    //const int n_max = 22426; //Am
    const int n_max = 1; //for tests
    for(int i = 0; i < n_max; i++)
    {
        ostringstream file_tree_oss;
        file_tree_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_block" << setfill('0') << setw(7) << i << ".root";
        chain.Add(file_tree_oss.str().c_str());

        if(i % 100 == 0) cout << "event = " << i << endl;
    }
    cout << "chain.GetEntries() = " << chain.GetEntries() << endl;

    chain.SetBranchAddress("canvas_tr", &canv);

    //set variables to read them from tree
    double integral_ch1, integral_ch2;
    double baseline_ch1, baseline_ch2;
    double max_abs_amp_ch1, max_abs_amp_ch2;

    chain.SetBranchAddress("integral_ch1", &integral_ch1);
    chain.SetBranchAddress("integral_ch2", &integral_ch2);
    chain.SetBranchAddress("baseline_ch1", &baseline_ch1);
    chain.SetBranchAddress("baseline_ch2", &baseline_ch2);
    chain.SetBranchAddress("max_abs_amp_ch1", &max_abs_amp_ch1);
    chain.SetBranchAddress("max_abs_amp_ch2", &max_abs_amp_ch2);


    //TCut total_cut = "integral_ch2 > -50000 && integral_ch2 < 50000";
    //    TCut total_cut = "max_abs_amp_ch2 > 9 && max_abs_amp_ch2 < 14";
    //    TCut total_cut = "max_abs_amp_ch2 < 9";
    TCut total_cut = "";

    chain.SetMarkerStyle(4);

    //    chain.Draw("max_abs_amp_ch1>>h1(300,0,100)", total_cut);
    chain.Draw("integral_ch2>>h2(300,-35000,5000)", total_cut);

    //   chain.Draw("integral_ch2>>h2(500,-20000,5000)", total_cut); // >>hsqrt(bins, min, max)
    //    chain.Draw("integral_ch1>>h1(500,-20000,5000)", total_cut);

    //   TH1F *h2 = (TH1F*)gDirectory->Get("h2");
    //   TH1F *h1 = (TH1F*)gDirectory->Get("h1");

    //   h1->SetFillStyle(3001);
    //   h2->SetFillStyle(3001);


    //   h1->SetFillColor(kBlue);
    //   h2->SetFillColor(kRed);


    //   h2->Draw();
    //   h1->Draw("same");

    bool is_first_time = true;
    int cut_event_counter = 0;
    int size_cd1;
    int size_cd5;
    int size_cd7;
    vector<double> xv_cd1;
    vector<double> xv_cd5;
    vector<double> xv_cd7;

    vector<double> yv_cd1;
    vector<double> yv_cd2;
    vector<double> yv_cd3;
    vector<double> yv_cd4;
    vector<double> yv_cd5;
    vector<double> yv_cd6;
    vector<double> yv_cd7;
    vector<double> yv_cd8;
    //    xv_time.resize();

    const int n_entr = 100;
    //    const int n_entr = chain.GetEntries();
    for (int i = 0; i < n_entr; ++i)
    {
        chain.GetEntry(i);


        if(1)//start cut
        {
//            cout << "cut_event_counter = " << cut_event_counter << endl;
            cut_event_counter++;
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

            if(gh_cd1 == NULL || gh_cd2 == NULL || gh_cd3 == NULL || gh_cd4 == NULL
                    || gh_cd5 == NULL || gh_cd6 == NULL || gh_cd7 == NULL || gh_cd8 == NULL) cout << "graph pnt in null" << endl;


            if(is_first_time)
            {
                is_first_time = false;
                size_cd1 = gh_cd1->GetN();
                size_cd5 = gh_cd5->GetN();
                size_cd7 = gh_cd7->GetN();

                xv_cd1.resize(size_cd1);
                yv_cd1.resize(size_cd1);
                yv_cd2.resize(size_cd1);
                yv_cd3.resize(size_cd1);
                yv_cd4.resize(size_cd1);

                xv_cd5.resize(size_cd5);
                yv_cd5.resize(size_cd5);
                yv_cd6.resize(size_cd5);

                xv_cd7.resize(size_cd7);
                yv_cd7.resize(size_cd7);
                yv_cd8.resize(size_cd7);

                for (int k = 0; k < size_cd1; ++k)
                {
                    double x, y;
                    gh_cd1->GetPoint(k, xv_cd1[k], y);
                }

                for (int k = 0; k < size_cd5; ++k)
                {
                    double x, y;
                    gh_cd5->GetPoint(k, xv_cd5[k], y);
                }

                for (int k = 0; k < size_cd7; ++k)
                {
                    double x, y;
                    gh_cd7->GetPoint(k, xv_cd7[k], y);
                }

            }

            for (int k = 0; k < size_cd1; ++k)
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
            }

            for (int k = 0; k < size_cd5; ++k)
            {
                double x, y;
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

                gh_cd8->GetPoint(k, x, y);
                yv_cd8[k] += y;
            }

            //        Hlist_gr.Add( gh_cd1->Clone() );
//                    Hlist_gr.Add( canv->Clone() );
        }//end cut
    }

    //divide summ by cut_event_counter
    for (int i = 0; i < xv_cd1.size(); ++i)
    {
        yv_cd1[i] /= cut_event_counter;
        yv_cd2[i] /= cut_event_counter;
        yv_cd3[i] /= cut_event_counter;
        yv_cd4[i] /= cut_event_counter;
    }

    for (int i = 0; i < xv_cd5.size(); ++i)
    {
        yv_cd5[i] /= cut_event_counter;
        yv_cd6[i] /= cut_event_counter;
    }

    for (int i = 0; i < xv_cd7.size(); ++i)
    {
        yv_cd7[i] /= cut_event_counter;
        yv_cd8[i] /= cut_event_counter;
    }


    //create graphs
    TGraph graph_cd1(xv_cd1.size(), &xv_cd1[0], &yv_cd1[0]);
    TGraph graph_cd2(xv_cd1.size(), &xv_cd1[0], &yv_cd2[0]);
    TGraph graph_cd3(xv_cd1.size(), &xv_cd1[0], &yv_cd3[0]);
    TGraph graph_cd4(xv_cd1.size(), &xv_cd1[0], &yv_cd4[0]);

    TGraph graph_cd5(xv_cd5.size(), &xv_cd5[0], &yv_cd5[0]);
    TGraph graph_cd6(xv_cd5.size(), &xv_cd5[0], &yv_cd6[0]);

    TGraph graph_cd7(xv_cd7.size(), &xv_cd7[0], &yv_cd7[0]);
    TGraph graph_cd8(xv_cd7.size(), &xv_cd7[0], &yv_cd8[0]);

    TCanvas canv_result("c", "c", 0, 0, 1900, 1500);
    canv_result.Divide(2, 4);

    canv_result.cd(1);
    graph_cd1.SetTitle("original (Channel 1, SiPM)");
    graph_cd1.GetXaxis()->SetTitle("time [ns]");
    graph_cd1.GetYaxis()->SetTitle("amplitude[channels]");
    graph_cd1.Draw();

    canv_result.cd(2);
    graph_cd2.SetTitle("original (Channel 2, SiPM)");
    graph_cd2.GetXaxis()->SetTitle("time [ns]");
    graph_cd2.GetYaxis()->SetTitle("amplitude[channels]");
    graph_cd2.Draw();

    canv_result.cd(3);
    graph_cd3.SetTitle("derivative (Channel 1, SiPM)");
    graph_cd3.GetXaxis()->SetTitle("time [ns]");
    graph_cd3.GetYaxis()->SetTitle("derivative [channels / ns]");
    graph_cd3.Draw();

    canv_result.cd(4);
    graph_cd4.SetTitle("derivative (Channel 2, SiPM)");
    graph_cd4.GetXaxis()->SetTitle("time [ns]");
    graph_cd4.GetYaxis()->SetTitle("derivative [channels / ns]");
    graph_cd4.Draw();

    canv_result.cd(5);
    graph_cd5.SetTitle("Amplitude spectrum of noise signal (Channel 1, SiPM)");
    graph_cd5.GetXaxis()->SetTitle("frequensy [MHz]");
    graph_cd5.GetYaxis()->SetTitle("amplitude [a.u.]");
    graph_cd5.Draw("APL");
    graph_cd5.SetMarkerStyle(20);
    graph_cd5.SetMarkerSize(0.5);
    graph_cd5.SetMarkerColor(kRed);

    canv_result.cd(6);
    graph_cd6.SetTitle("Amplitude spectrum of noise signal (Channel 2, SiPM)");
    graph_cd6.GetXaxis()->SetTitle("frequensy [MHz]");
    graph_cd6.GetYaxis()->SetTitle("amplitude [a.u.]");
    graph_cd6.Draw("APL");
    graph_cd6.SetMarkerStyle(20);
    graph_cd6.SetMarkerSize(0.5);
    graph_cd6.SetMarkerColor(kRed);

    canv_result.cd(7);
    graph_cd7.SetTitle("Amplitude spectrum of signal (Channel 1, SiPM)");
    graph_cd7.GetXaxis()->SetTitle("frequensy [MHz]");
    graph_cd7.GetYaxis()->SetTitle("amplitude [a.u.]");
    graph_cd7.Draw("APL");
    graph_cd7.SetMarkerStyle(20);
    graph_cd7.SetMarkerSize(0.5);
    graph_cd7.SetMarkerColor(kRed);

    canv_result.cd(8);
    graph_cd8.SetTitle("Amplitude spectrum of signal (Channel 2, SiPM)");
    graph_cd8.GetXaxis()->SetTitle("frequensy [MHz]");
    graph_cd8.GetYaxis()->SetTitle("amplitude [a.u.]");
    graph_cd8.Draw("APL");
    graph_cd8.SetMarkerStyle(20);
    graph_cd8.SetMarkerSize(0.5);
    graph_cd8.SetMarkerColor(kRed);



    TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
//    Hlist_gr.Write();
    canv_result.Write();
    ofile_Hlist_gr.Close();

    cout << endl << "Root cern script: all is ok" << endl;

}
