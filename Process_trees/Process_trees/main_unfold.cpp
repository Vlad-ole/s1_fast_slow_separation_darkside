void ReadTree_unfold()
{
    //processing params
    const bool advanced_processing = false;
    const bool normal_processing = false;
    const bool simple_processing = true;

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

    const int n_max = 1900;//number of files
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
//        gROOT->SetBatch(kTRUE);

//        TCut cut = "(integral_ch2_unfold_total / 1484.0) < 5 && (integral_ch2_unfold_fast / integral_ch2_unfold_total) < 0.1";


        chain.Draw("integral_ch2_unfold_fast/integral_ch2_unfold_total>>hist(500, 0, 1)");
//        chain.Draw("integral_ch2>>hist(500, -4000000, 8000000)");
//        chain.Draw("integral_ch2");


//        chain.Draw("(integral_ch2_unfold_fast / integral_ch2_unfold_total):(integral_ch2_unfold_total / 1484.0) >>hist2(200, 0, 1200, 200, 0, 1)", "(integral_ch2_unfold_total / 1484.0) < 1200", "COLz");
//        Double_t levels[] = {0, 2, 6, 10, 15, 20, 25, 30, 60, 100, 200};
//        hist2->SetContour((sizeof(levels)/sizeof(Double_t)), levels);
//        c1->SetLogz();









//        TFile f_tree("/home/darkside/Vlad_Programs/Physical_results/run_6053_bg_slices.root", "RECREATE");
//        TTree *tree = new TTree("t_slices", "run_6053_bg_slices");

//        TCanvas *cnv = new TCanvas("c", "c", 0, 0, 1900, 1500);

//        double mean, sigma, amp, N_ph_from;
//        double mean_err, sigma_err, amp_err;
//        TH1F *h;

//        tree->Branch("h", "TH1F", &h);

//        tree->Branch("mean", &mean, "mean/D");
//        tree->Branch("sigma", &sigma, "sigma/D");
//        tree->Branch("amp", &amp, "amp/D");

//        tree->Branch("mean_err", &mean_err, "mean_err/D");
//        tree->Branch("sigma_err", &sigma_err, "sigma_err/D");
//        tree->Branch("amp_err", &amp_err, "amp_err/D");

//        tree->Branch("N_ph_from", &N_ph_from, "N_ph_from/D");

//        const double step = 10;
//        for (int j = 0; j < 120; ++j)
//        {
//            ostringstream cut_slice_nph_oss;
//            cut_slice_nph_oss << "(integral_ch2_unfold_total / 1484.0) > " << j*step << " && ";
//            cut_slice_nph_oss << "(integral_ch2_unfold_total / 1484.0) < " << (j + 1)*step;
//            cout << cut_slice_nph_oss.str() << endl;
//            TCut cut_slice_nph = cut_slice_nph_oss.str().c_str();

//            chain.Draw("(integral_ch2_unfold_fast / integral_ch2_unfold_total) >>hist(500, 0, 1)", cut_slice_nph);
//            TH1F *hist = (TH1F*)gDirectory->Get("hist");

//            cout << "hist->GetNbinsX() " << hist->GetNbinsX() << endl;

//            TF1 f1("f1","gaus", 0.01, 0.8);
//            f1.SetParLimits(0,0,1E6);
//            f1.SetParLimits(1,0,1);
//            f1.SetParLimits(2,0,2);
//            hist->Fit("f1","R");

//            amp = f1.GetParameter(0);
//            amp_err = f1.GetParError(0);

//            mean = f1.GetParameter(1);
//            mean_err = f1.GetParError(1);

//            sigma = f1.GetParameter(2);
//            sigma_err = f1.GetParError(2);

//            N_ph_from = j*step;

//            h = hist;
//            ostringstream hist_name;
//            hist_name << "hist_from_" << j*step << "__to_" << (j + 1)*step << "[pe]";
//            h->SetName( hist_name.str().c_str() );
//            h->Write();
//            tree->Fill();
//        }

//        tree->Write();

//        f_tree.Close();
    }

    if(normal_processing)
    {
        cout << "normal_processingg" << endl;
        gROOT->SetBatch(kTRUE);
            const int n_entr = 1000;
//        const int n_entr = chain.GetEntries();
        for (int i = 0; i < n_entr; ++i)
        {
            chain.GetEntry(i);
            if(i % 100 == 0) cout << "event = " << i << endl;

            if( (integral_ch2_unfold_total / 1484.0) < 5 && (integral_ch2_unfold_fast / integral_ch2_unfold_total) < 0.1  ) //start cut
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
