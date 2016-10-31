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

    const int n_entr = 5;
//    const int n_entr = chain.GetEntries();
    for (int i = 0; i < n_entr ; ++i)
    {
        chain.GetEntry(i);
        //       TGraph *gh_cd1 = (TGraph*)canv->GetListOfPrimitives()->FindObject("Graph");

        //       if(integral_ch1 > 2000)
        //       {
        Hlist_gr.Add( canv->Clone() );
        //       }
    }

       TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
       Hlist_gr.Write();
       ofile_Hlist_gr.Close();

    cout << endl << "Root cern script: all is ok" << endl;

}
