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

    const double time_scale = 4;//ns
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

        TCut cut = "(integral_ch2_unfold_total / 1484.0) > 10 && (integral_ch2_unfold_fast / integral_ch2_unfold_total) > 0.1 && (integral_ch2_unfold_fast / integral_ch2_unfold_total) < 0.6";


//        chain.Draw("integral_ch2_unfold_fast/integral_ch2_unfold_total>>hist(500, 0, 1)");
        chain.Draw("(integral_ch2_unfold_total / 1484.0)>>hist(500, -500, 8000)", cut);
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
            const int n_entr = 10;
//        const int n_entr = chain.GetEntries();
        for (int i = 0; i < n_entr; ++i)
        {
            chain.GetEntry(i);
            if(i % 100 == 0) cout << "event = " << i << endl;

            const bool condition_pe = (integral_ch2_unfold_total / 1484.0) < 1000 ;
            const bool condition_ratio = (integral_ch2_unfold_fast / integral_ch2_unfold_total) > 0.1;
//            const bool condition_total = condition_pe && condition_ratio;
            const bool condition_total = true;
            if(condition_pe) //start cut
            {
                stringstream ss;
                ss << "c_" << i;
                TCanvas *canv_result = new TCanvas(ss.str().c_str(), "c", 0, 0, 1900, 1500);

                TPad *pad_cd = (TPad*)canv->GetListOfPrimitives()->FindObject("c_4");
                TGraph *gh_cd = (TGraph*)pad_cd->GetListOfPrimitives()->FindObject("Graph");

                TPad *pad_cd2 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_7");
                TGraph *gh_cd2 = (TGraph*)pad_cd2->GetListOfPrimitives()->FindObject("Graph");

                if(gh_cd == NULL) cout << "gh_cd == NULL" << endl;

                gh_cd->Draw("apl");
                gh_cd2->Draw("same pl");

                Hlist_gr.Add( canv_result->Clone() );
//                delete canv_result;
            }//end cut
        }

        TFile ofile_Hlist_gr(graph_name.c_str(), "RECREATE");
        Hlist_gr.Write();
        //        canv_result.Write();
        ofile_Hlist_gr.Close();

    }

    if(advanced_processing)
    {
        bool is_first_time = true;
//        const int n_entr = 1000;
        const int n_entr = chain.GetEntries();

        vector<double> xv;
        vector<double> yv_cd7;
        vector<double> yv_cd8;
        vector<double> yv_cd9;
        int size;

        for (int i = 0; i < n_entr; ++i)
        {
            chain.GetEntry(i);
            if(i % 100 == 0) cout << "event = " << i << endl;

            TPad *pad_cd7 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_7");
            TGraph *gh_cd7 = (TGraph*)pad_cd7->GetListOfPrimitives()->FindObject("Graph");

            TPad *pad_cd8 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_8");
            TGraph *gh_cd8 = (TGraph*)pad_cd8->GetListOfPrimitives()->FindObject("Graph");

            TPad *pad_cd9 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_9");
            TGraph *gh_cd9 = (TGraph*)pad_cd9->GetListOfPrimitives()->FindObject("Graph");

            if(gh_cd7 == NULL || gh_cd8 == NULL || gh_cd9 == NULL)
                cout << "graph pnt in null" << endl;

            if(is_first_time)
            {
                is_first_time = false;
                size = gh_cd7->GetN();
                xv.resize(size);
                yv_cd7.resize(size);
                yv_cd8.resize(size);
                yv_cd9.resize(size);

                for (int k = 0; k < size; ++k)
                {
                    xv[k] = k*time_scale;
                }
            }

            for (int k = 0; k < size; ++k)
            {
                double x, y;
                gh_cd7->GetPoint(k, x, y);
                yv_cd7[k] += y;

                gh_cd8->GetPoint(k, x, y);
                yv_cd8[k] += y;

                gh_cd9->GetPoint(k, x, y);
                yv_cd9[k] += y;
            }
        }

        for (int k = 0; k < size; ++k)
        {
             yv_cd7[k] /= n_entr;
             yv_cd8[k] /= n_entr;
             yv_cd9[k] /= n_entr;
        }

        TCanvas *canv_result = new TCanvas("c", "c", 0, 0, 1900, 1500);
        canv_result->Divide(1, 3);

        TGraph *graph_cd7 = new TGraph(xv.size(), &xv[0], &yv_cd7[0]);
        TGraph *graph_cd8 = new TGraph(xv.size(), &xv[0], &yv_cd8[0]);
        TGraph *graph_cd9 = new TGraph(xv.size(), &xv[0], &yv_cd9[0]);

        canv_result->cd(1);
        graph_cd7->Draw("apl");

        canv_result->cd(2);
        graph_cd8->Draw("apl");

        canv_result->cd(3);
        graph_cd9->Draw("apl");

        cout << xv.size() << endl;
        cout << yv_cd7.size() << endl;
        cout << yv_cd8.size() << endl;
        cout << yv_cd9.size() << endl;
    }



    cout << endl << "Root cern script: all is ok" << endl;

}
