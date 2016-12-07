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
    TCanvas* canv = 0;
    TChain chain("t1");// name of the tree is the argument

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

    bool is_first_time = true;
    int cut_event_counter = 0;
    int size;

    vector<double> xv;
    vector<double> yv;

//    const int n_entr = chain.GetEntries();
    const int n_entr = 123;
    for (int i = 0; i < n_entr; ++i)
    {
        chain.GetEntry(i);
        if(i % 100 == 0) cout << "event = " << i << endl;

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

    for (int i = 0; i < xv.size(); ++i)
    {
        yv[i] /= cut_event_counter;
    }

    return ( new TGraph(xv.size(), &xv[0], &yv[0]) );

}

void DrawGraph()
{
    TCanvas *canv_result = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv_result->SetLogy();

    TGraph* graph_result = getandavr("c_4");
//    graph_result->SetTitle("Power spectral density");
//    graph_result->GetXaxis()->SetTitle("frequency [MHz]");
//    graph_result->GetYaxis()->SetTitle("amplitude[channels^2 / GHz]");

//    graph_result->SetName("gr_1");
//    graph_result->SetMarkerStyle(20);
//    graph_result->SetMarkerSize(1);
//    graph_result->SetMarkerColor(kRed);
//    graph_result->Draw("apl");

//    TGraph* graph_result_2 = getandavr("c_7");
//    graph_result_2->SetName("gr_2");
//    graph_result_2->SetMarkerStyle(20);
//    graph_result_2->SetMarkerSize(1);
//    graph_result_2->SetMarkerColor(kGreen);
//    graph_result_2->Draw("same pl");

//    TGraph* graph_result_3 = getandavr("c_5");
//    graph_result_3->SetName("gr_3");
//    graph_result_3->SetMarkerStyle(34);
//    graph_result_3->SetMarkerSize(1);
//    graph_result_3->SetMarkerColor(kBlue);
//    graph_result_3->Draw("same pl");

//    TGraph* graph_result_4 = getandavr("c_8");
//    graph_result_4->SetName("gr_4");
//    graph_result_4->SetMarkerStyle(34);
//    graph_result_4->SetMarkerSize(1);
//    graph_result_4->SetMarkerColor(7);
//    graph_result_4->Draw("same pl");

    TGraph* graph_result_5 = getandavr("c_6");
    graph_result_5->SetName("gr_5");
    graph_result_5->SetMarkerStyle(20);
    graph_result_5->SetMarkerSize(1);
    graph_result_5->SetMarkerColor(46);
    graph_result_5->Draw("apl");

//    TGraph* graph_result_6 = getandavr("c_9");
//    graph_result_6->SetName("gr_6");
//    graph_result_6->SetMarkerStyle(20);
//    graph_result_6->SetMarkerSize(1);
//    graph_result_6->SetMarkerColor(9);
//    graph_result_6->Draw("same pl");


    leg = new TLegend(0.9,0.7,1.0,0.9);

    //1
//    leg->AddEntry("gr_1","ch0 (PMT), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_2","ch0 (PMT), signal (2050,2100)ns","lp");
//    leg->AddEntry("gr_3","ch1 (SiPM), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_4","ch1 (SiPM), signal (1950,2650)ns","lp");
//    leg->AddEntry("gr_5","ch2 (SiPM), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_6","ch2 (SiPM), signal (1950,2650)ns","lp");

    //2
//    leg->AddEntry("gr_1","ch0 (PMT), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_2","ch0 (PMT), signal (1900,14900)ns","lp");
//    leg->AddEntry("gr_3","ch1 (SiPM), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_4","ch1 (SiPM), signal (1900,14900)ns","lp");
    leg->AddEntry("gr_5","ch2 (SiPM), noise (0,1600)ns","lp");
//    leg->AddEntry("gr_6","ch2 (SiPM), signal (1900,14900)ns","lp");


    leg->Draw();
}
