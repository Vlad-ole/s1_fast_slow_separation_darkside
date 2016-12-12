TGraph* getandavr(string canv_name)
{
//    gROOT->SetBatch(kTRUE);

    //read param
//    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_trees/";
//    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_result.root";
//    const int run_id = 6061;

    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees/";
    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_result.root";
    const int run_id = 6064;

    const double time_scale = 4;//ns
    TObjArray Hlist_gr(0);
    Hlist_gr.SetOwner(kTRUE);
    TChain chain("t1");// name of the tree is the argument

    const int n_max = 10;//number of files
    for(int i = 0; i < n_max; i++)
    {
        ostringstream file_tree_oss;
        file_tree_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_block" << setfill('0') << setw(7) << i << ".root";
        chain.Add(file_tree_oss.str().c_str());

        if(i % 100 == 0) cout << "file = " << i << endl;
    }
    cout << "chain.GetEntries() = " << chain.GetEntries() << endl;

    TCanvas* canv = 0;

    double min_amp_ch0, min_amp_ch1, min_amp_ch2;

    //run spe 6061
    double min_amp_ch0_0_2045, min_amp_ch0_2100_5000;
    double min_amp_ch1_0_2000, min_amp_ch1_2800_5000;
    double min_amp_ch2_0_1900, min_amp_ch2_2800_5000;

    //run Am 6064
    double min_amp_ch0_0_1920;
    double min_amp_ch1_0_1800, min_amp_ch1_8000_15000;
    double min_amp_ch2_0_1800, min_amp_ch2_8000_15000;


    chain.SetBranchAddress("canvas_tr", &canv);
    chain.SetBranchAddress("min_amp_ch0", &min_amp_ch0);
    chain.SetBranchAddress("min_amp_ch1", &min_amp_ch1);
    chain.SetBranchAddress("min_amp_ch2", &min_amp_ch2);

    //run spe 6061
    chain.SetBranchAddress("min_amp_ch0_0_2045", &min_amp_ch0_0_2045);
    chain.SetBranchAddress("min_amp_ch0_2100_5000", &min_amp_ch0_2100_5000);
    chain.SetBranchAddress("min_amp_ch1_0_2000", &min_amp_ch1_0_2000);
    chain.SetBranchAddress("min_amp_ch1_2800_5000", &min_amp_ch1_2800_5000);
    chain.SetBranchAddress("min_amp_ch2_0_1900", &min_amp_ch2_0_1900);
    chain.SetBranchAddress("min_amp_ch2_2800_5000", &min_amp_ch2_2800_5000);

    //run Am 6064
    chain.SetBranchAddress("min_amp_ch0_0_1920", &min_amp_ch0_0_1920);
    chain.SetBranchAddress("min_amp_ch1_0_1800", &min_amp_ch1_0_1800);
    chain.SetBranchAddress("min_amp_ch1_8000_15000", &min_amp_ch1_8000_15000);
    chain.SetBranchAddress("min_amp_ch2_0_1800", &min_amp_ch2_0_1800);
    chain.SetBranchAddress("min_amp_ch2_8000_15000", &min_amp_ch2_8000_15000);


    bool is_first_time = true;
    int cut_event_counter = 0;
    int size;

    vector<double> xv;
    vector<double> yv;

    const int n_entr = chain.GetEntries();
//    const int n_entr = 100;
    for (int i = 0; i < n_entr; ++i)
    {
        chain.GetEntry(i);
        if(i % 100 == 0) cout << "event = " << i << endl;

        //ch0
        const bool cut1_ch0_run6061_spe = (min_amp_ch0 > 3800);
        const bool cut2_ch0_run6061_spe = (min_amp_ch0_0_2045 > 4031) && (min_amp_ch0_2100_5000 > 4031);

        const bool cut1_ch0_run6064_am = (min_amp_ch0 > 200);
        const bool cut2_ch0_run6064_am = (min_amp_ch0_0_1920 > 4030) && (min_amp_ch0_0_1920 < 4050);

        //ch1
        const bool cut1_ch1_run6061_spe = (min_amp_ch1 > 3350);
        const bool cut2_ch1_run6061_spe =  (min_amp_ch1_0_2000 > 3410) && (min_amp_ch1_2800_5000 > 3410);

        const bool cut1_ch1_run6064_am = (min_amp_ch1 > 2250);
        const bool cut2_ch1_run6064_am = (min_amp_ch1_0_1800 > 3410) && (min_amp_ch1_0_1800 < 3430) && (min_amp_ch1_8000_15000 > 3300);

        //ch2
        const bool cut1_ch2_run6061_spe = (min_amp_ch2 > 3250);
        const bool cut2_ch2_run6061_spe =  (min_amp_ch2_0_1900 > 3412) && (min_amp_ch2_2800_5000 > 3412);

        const bool cut1_ch2_run6064_am = (min_amp_ch2 > 2250);
        const bool cut2_ch2_run6064_am = (min_amp_ch2_0_1800 > 3412) && (min_amp_ch2_0_1800 < 3430) && (min_amp_ch2_8000_15000 > 3300);

        //total
        const bool cut1_run6061_spe = cut1_ch0_run6061_spe && cut1_ch1_run6061_spe && cut1_ch2_run6061_spe;
        const bool cut2_run6061_spe = cut2_ch0_run6061_spe && cut2_ch1_run6061_spe && cut2_ch2_run6061_spe;
        const bool cut_run6061_spe = cut1_run6061_spe && cut2_run6061_spe;

        const bool cut1_run6064_am = cut1_ch0_run6064_am && cut1_ch1_run6064_am && cut1_ch2_run6064_am;
        const bool cut2_run6064_am = cut2_ch0_run6064_am && cut2_ch1_run6064_am && cut2_ch2_run6064_am;
        const bool cut_run6064_am = cut1_run6064_am && cut2_run6064_am;

        if(cut_run6064_am)
        {
            cut_event_counter++;

            TPad *pad_cd1 = (TPad*)canv->GetListOfPrimitives()->FindObject(canv_name.c_str());
            TGraph *gh_cd1 = (TGraph*)pad_cd1->GetListOfPrimitives()->FindObject("Graph");

            if(gh_cd1 == NULL) cout << "graph pnt in null" << endl;

            if(is_first_time)
            {
                is_first_time = false;
                size = gh_cd1->GetN();

                xv.resize(size);
                yv.resize(size);

                for (int k = 0; k < size; ++k)
                {
                    double x, y;
                    gh_cd1->GetPoint(k, xv[k], y);
                }
            }

            for (int k = 0; k < size; ++k)
            {
                double x, y;
                gh_cd1->GetPoint(k, x, y);
                yv[k] += y;
            }

        }

    }

    for (int i = 0; i < xv.size(); ++i)
    {
        yv[i] /= cut_event_counter;
    }

    cout << "cut_event_counter = " << cut_event_counter << endl;
    return ( new TGraph(xv.size(), &xv[0], &yv[0]) );

}

void DrawGraph()
{
    TCanvas *canv_result = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv_result->SetLogy();

    TGraph* graph_result = getandavr("c_4");
    graph_result->SetName("gr_1");
    graph_result->SetTitle("Power spectral density");
    graph_result->GetXaxis()->SetTitle("frequency [MHz]");
    graph_result->GetYaxis()->SetTitle("amplitude[channels^2 / GHz]");//
    graph_result->SetMarkerStyle(20);
    graph_result->SetMarkerSize(1);
    graph_result->SetMarkerColor(kRed);
    graph_result->Draw("apl");

    TGraph* graph_result_2 = getandavr("c_7");
    graph_result_2->SetName("gr_2");
    graph_result_2->SetMarkerStyle(20);
    graph_result_2->SetMarkerSize(1);
    graph_result_2->SetMarkerColor(kGreen);
    graph_result_2->Draw("same pl");

    TGraph* graph_result_3 = getandavr("c_5");
    graph_result_3->SetName("gr_3");
    graph_result_3->SetMarkerStyle(34);
    graph_result_3->SetMarkerSize(1);
    graph_result_3->SetMarkerColor(kBlue);
    graph_result_3->Draw("same pl");

    TGraph* graph_result_4 = getandavr("c_8");
    graph_result_4->SetName("gr_4");
    graph_result_4->SetMarkerStyle(34);
    graph_result_4->SetMarkerSize(1);
    graph_result_4->SetMarkerColor(7);
    graph_result_4->Draw("same pl");

    TGraph* graph_result_5 = getandavr("c_6");
    graph_result_5->SetName("gr_5");
    graph_result_5->SetMarkerStyle(20);
    graph_result_5->SetMarkerSize(1);
    graph_result_5->SetMarkerColor(46);
    graph_result_5->Draw("same pl");

    TGraph* graph_result_6 = getandavr("c_9");
    graph_result_6->SetName("gr_6");
    graph_result_6->SetMarkerStyle(20);
    graph_result_6->SetMarkerSize(1);
    graph_result_6->SetMarkerColor(9);
    graph_result_6->Draw("same pl");


    leg = new TLegend(0.9,0.7,1.0,0.9);

    //1
//    leg->AddEntry("gr_1","ch0 (PMT), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_2","ch0 (PMT), signal (2050,2100)ns","lp");
//    leg->AddEntry("gr_3","ch1 (SiPM), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_4","ch1 (SiPM), signal (1900,3000)ns","lp");
//    leg->AddEntry("gr_5","ch2 (SiPM), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_6","ch2 (SiPM), signal (1900,3000)ns","lp");

    //2
    leg->AddEntry("gr_1","ch0 (PMT), noise (0,1600)ns","lp");
    leg->AddEntry("gr_2","ch0 (PMT), signal (0,14900)ns","lp");
    leg->AddEntry("gr_3","ch1 (SiPM), noise (0,1600)ns","lp");
    leg->AddEntry("gr_4","ch1 (SiPM), signal (0,14900)ns","lp");
    leg->AddEntry("gr_5","ch2 (SiPM), noise (0,1600)ns","lp");
    leg->AddEntry("gr_6","ch2 (SiPM), signal (0,14900)ns","lp");


    leg->Draw();
}
