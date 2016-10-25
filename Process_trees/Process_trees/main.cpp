void ReadTree()
{
    gROOT->SetBatch(kTRUE); // it's really important to use this line if you save TCanvas in a tree!

    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees/";
    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_result.root";
    const int run_id = 6064;

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
        file_tree_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << i << ".root";
        chain.Add(file_tree_oss.str().c_str());
    }

    chain.SetBranchAddress("canvas_tr", &canv);


//    TCut total_cut = "";

//    chain.SetMarkerStyle(4);
//    chain.Draw("amp_par_br * tau_par_br", total_cut);


    for (int i = 0; i < chain.GetEntries() ; ++i)
    {
        chain.GetEntry(i);
        Hlist_gr.Add( canv->Clone() );
    }

    TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
    Hlist_gr.Write();
    ofile_Hlist_gr.Close();

    cout << endl << "Root cern script: all is ok" << endl;

}
