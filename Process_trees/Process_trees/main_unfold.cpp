void ReadTree_unfold()
{
    gROOT->SetBatch(kTRUE);

    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees_unfold/";
    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_result_unfold.root";
    const int run_id = 6064;

    TObjArray Hlist_gr(0);
    Hlist_gr.SetOwner(kTRUE);

    TCanvas* canv = 0;
    TChain chain("t1");

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

    //advanced processing
    if(1)
    {
        //    const int n_entr = 100;
        const int n_entr = chain.GetEntries();
        for (int i = 0; i < n_entr; ++i)
        {
            chain.GetEntry(i);
            if(i % 100 == 0) cout << "event = " << i << endl;

            if(1)//start cut
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
