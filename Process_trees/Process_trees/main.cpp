void ReadTree()
{
//    gROOT->SetBatch(kTRUE); // it's really important to use this line if you save TCanvas in a tree!

    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_trees/";
    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_result.root";
    const int run_id = 6061;

    TObjArray Hlist_gr(0);
    Hlist_gr.SetOwner(kTRUE);

    //TGraph* graph = 0;
    TCanvas* canv = 0;

    TChain chain("t1");// name of the tree is the argument
    //const int n_max = 22426; //Am
    const int n_max = 5000; //for tests
    for(int i = 0; i < n_max; i++)
    {
        ostringstream file_tree_oss;
        file_tree_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << i << ".root";
        chain.Add(file_tree_oss.str().c_str());
    }

    chain.SetBranchAddress("canvas_tr", &canv);

    //set variables to read them from tree
    double integral_ch1, integral_ch2;
    double baseline_ch1, baseline_ch2;

    chain.SetBranchAddress("integral_ch1", &integral_ch1);
    chain.SetBranchAddress("integral_ch2", &integral_ch2);
    chain.SetBranchAddress("baseline_ch1", &baseline_ch1);
    chain.SetBranchAddress("baseline_ch2", &baseline_ch2);


     TCut total_cut = "integral_ch2 > -50000 && integral_ch2 < 50000";
//    TCut total_cut = "";

    chain.SetMarkerStyle(4);
    chain.Draw("integral_ch2", total_cut);


//    for (int i = 0; i < chain.GetEntries() ; ++i)
//    {
//        chain.GetEntry(i);
//        Hlist_gr.Add( canv->Clone() );
//    }

//    TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
//    Hlist_gr.Write();
//    ofile_Hlist_gr.Close();

    cout << endl << "Root cern script: all is ok" << endl;

}
