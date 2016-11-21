void ReadTree_unfold()
{
    //processing params
    const bool advanced_processing = false;
    const bool normal_processing = true;
    const bool simple_processing = false;

    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees_unfold/";
    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_result_unfold.root";
    const int run_id = 6064;

//    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6053_bkg_trees_unfold/";
//    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6053_bkg_result_unfold.root";
//    const int run_id = 6053;

    const double spe_integral_ch2 = 1484.0;

    TObjArray Hlist_gr(0);
    Hlist_gr.SetOwner(kTRUE);

    TChain chain("t1");

    const int n_max = 2000;//number of files
    for(int i = 0; i < n_max; i++)
    {
        ostringstream file_tree_oss;
        file_tree_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_block" << setfill('0') << setw(7) << i << ".root";
        chain.Add(file_tree_oss.str().c_str());

        if(i % 100 == 0) cout << "file = " << i << endl;
    }
    cout << "chain.GetEntries() = " << chain.GetEntries() << endl;


    //set variables to read them from tree
    TCanvas* canv = 0;
    double integral_ch0, integral_ch1, integral_ch2;
    double integral_ch0_unfold_total, integral_ch1_unfold_total, integral_ch2_unfold_total;
    double integral_ch0_unfold_fast, integral_ch1_unfold_fast, integral_ch2_unfold_fast;

    chain.SetBranchAddress("canvas_tr", &canv);

    chain.SetBranchAddress("integral_ch0", &integral_ch0);
    chain.SetBranchAddress("integral_ch1", &integral_ch1);
    chain.SetBranchAddress("integral_ch2", &integral_ch2);

    chain.SetBranchAddress("integral_ch0_unfold_total", &integral_ch0_unfold_total);
    chain.SetBranchAddress("integral_ch1_unfold_total", &integral_ch1_unfold_total);
    chain.SetBranchAddress("integral_ch2_unfold_total", &integral_ch2_unfold_total);

    chain.SetBranchAddress("integral_ch0_unfold_fast", &integral_ch0_unfold_fast);
    chain.SetBranchAddress("integral_ch1_unfold_fast", &integral_ch1_unfold_fast);
    chain.SetBranchAddress("integral_ch2_unfold_fast", &integral_ch2_unfold_fast);

    if(simple_processing)
    {
        cout << "simple_processing" << endl;
        gROOT->SetBatch(kFALSE);


        //        chain.Draw("integral_ch2_unfold_fast/integral_ch2_unfold_total>>hist(500, 0, 1)");
//        chain.Draw("integral_ch2>>hist(500, -4000000, 8000000)");
//        chain.Draw("integral_ch2");
        chain.Draw("(integral_ch2_unfold_fast / integral_ch2_unfold_total):(integral_ch2_unfold_total / 1484.0)");
    }

    if(normal_processing)
    {
        cout << "normal_processingg" << endl;
        gROOT->SetBatch(kTRUE);
//            const int n_entr = 100;
        const int n_entr = chain.GetEntries();
        for (int i = 0; i < n_entr; ++i)
        {
            chain.GetEntry(i);
            if(i % 100 == 0) cout << "event = " << i << endl;

            if( (integral_ch2_unfold_total / spe_integral_ch2) > 1800 && (integral_ch2_unfold_total / spe_integral_ch2 ) < 2000) //start cut
            {
                Hlist_gr.Add( canv->Clone() );
            }//end cut
        }

        TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
        Hlist_gr.Write();
        //        canv_result.Write();
        ofile_Hlist_gr.Close();

    }

    cout << endl << "Root cern script: all is ok" << endl;

}
