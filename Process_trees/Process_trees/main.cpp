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
    vector<double> xv_time;
    vector<double> yv_cd1_ch1_amp;
    vector<double> yv_cd2_ch2_amp;
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
                    || gh_cd5 == NULL || gh_cd6 == NULL || gh_cd7 == NULL || gh_cd8 == NULL)


            if(is_first_time)
            {
                is_first_time = false;
                size_cd1 = gh_cd1->GetN();
                xv_time.resize(size_cd1);
                yv_cd1_ch1_amp.resize(size_cd1);
            }

            for (int k = 0; k < size_cd1; ++k)
            {
                double y;
                gh_cd1->GetPoint(k, xv_time[k], y);
                yv_cd1_ch1_amp[k] += y;
            }

            //        Hlist_gr.Add( gh_cd1->Clone() );
//                    Hlist_gr.Add( canv->Clone() );
        }//end cut
    }

    for (int i = 0; i < xv_time.size(); ++i)
    {
        yv_cd1_ch1_amp[i] /= cut_event_counter;
    }
    TGraph graph_ch1(xv_time.size(), &xv_time[0], &yv_cd1_ch1_amp[0]);


    TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
//    Hlist_gr.Write();
    graph_ch1.Write();
    ofile_Hlist_gr.Close();

    cout << endl << "Root cern script: all is ok" << endl;

}
