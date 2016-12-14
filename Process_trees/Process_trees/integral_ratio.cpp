double Get_integral(const vector<double> data_v, const double baseline, const int time_scale, const double time_from, const double time_to)
{
    double integral = 0;
    const int point_from = time_from / time_scale;
    const int point_to = time_to / time_scale;

    if( point_from > data_v.size() || point_to > data_v.size() )
    {
        cout << "error: incorrect integration time" << endl;
        exit(1);
    }

    for (int i = point_from; i < point_to; ++i)
    {
        integral += (data_v[i] - baseline);
    }


    return (integral * time_scale);
}

void integral_ratio()
{
    string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees/";
    string graph_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_result.root";
    const int run_id = 6064;

    const double time_scale = 4;//ns
    const int n_max = 1000;//number of files



    //integral intervals for n/g separation
    const double time_integral_to = 7900;// ns

    const double time_integral_unfold_fast_pmt_from = 1800;
    const double time_integral_unfold_fast_pmt_to = 2020;

    const double time_integral_unfold_total_pmt_from = time_integral_unfold_fast_pmt_from;
    const double time_integral_unfold_total_pmt_to = time_integral_to;

    const double time_integral_unfold_fast_sipm_from = time_integral_unfold_fast_pmt_from;
    const double time_integral_unfold_fast_sipm_to = 2200;

    const double time_integral_unfold_total_sipm_from = time_integral_unfold_fast_pmt_from;
    const double time_integral_unfold_total_sipm_to = time_integral_to;

    TH2F *h2 = new TH2F("h2","",100,0,1500,100,0,1);


    TCanvas* canv = 0;

    TChain chain("t1");// name of the tree is the argument
    for(int i = 0; i < n_max; i++)
    {
        ostringstream file_tree_oss;
        file_tree_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_block" << setfill('0') << setw(7) << i << ".root";
        chain.Add(file_tree_oss.str().c_str());

        if(i % 100 == 0) cout << "file = " << i << endl;
    }
    cout << "chain.GetEntries() = " << chain.GetEntries() << endl;

    chain.SetBranchAddress("canvas_tr", &canv);

    //set variables to read them from tree
    double integral_ch0, integral_ch1, integral_ch2;
    double baseline_ch0, baseline_ch1, baseline_ch2;
    double max_abs_amp_ch0 ,max_abs_amp_ch1, max_abs_amp_ch2;

    double min_amp_ch0, min_amp_ch1, min_amp_ch2;

    double min_amp_ch0_0_2045, min_amp_ch0_2100_5000;
    double min_amp_ch1_0_2000, min_amp_ch1_2800_5000;
    double min_amp_ch2_0_1900, min_amp_ch2_2800_5000;

    double min_amp_ch0_0_1920;
    double min_amp_ch1_0_1800, min_amp_ch1_8000_15000;
    double min_amp_ch2_0_1800, min_amp_ch2_8000_15000;

    //in general
    chain.SetBranchAddress("integral_ch0", &integral_ch0);
    chain.SetBranchAddress("integral_ch1", &integral_ch1);
    chain.SetBranchAddress("integral_ch2", &integral_ch2);

    chain.SetBranchAddress("baseline_ch0", &baseline_ch0);
    chain.SetBranchAddress("baseline_ch1", &baseline_ch1);
    chain.SetBranchAddress("baseline_ch2", &baseline_ch2);

    chain.SetBranchAddress("max_abs_amp_ch0", &max_abs_amp_ch0);
    chain.SetBranchAddress("max_abs_amp_ch1", &max_abs_amp_ch1);
    chain.SetBranchAddress("max_abs_amp_ch2", &max_abs_amp_ch2);

    chain.SetBranchAddress("min_amp_ch0", &min_amp_ch0);
    chain.SetBranchAddress("min_amp_ch1", &min_amp_ch1);
    chain.SetBranchAddress("min_amp_ch2", &min_amp_ch2);

    //for Am run
    chain.SetBranchAddress("min_amp_ch0_0_1920", &min_amp_ch0_0_1920);
    chain.SetBranchAddress("min_amp_ch1_0_1800", &min_amp_ch1_0_1800);
    chain.SetBranchAddress("min_amp_ch1_8000_15000", &min_amp_ch1_8000_15000);
    chain.SetBranchAddress("min_amp_ch2_0_1800", &min_amp_ch2_0_1800);
    chain.SetBranchAddress("min_amp_ch2_8000_15000", &min_amp_ch2_8000_15000);

    //for spe run
    chain.SetBranchAddress("min_amp_ch0_0_2045", &min_amp_ch0_0_2045);
    chain.SetBranchAddress("min_amp_ch0_2100_5000", &min_amp_ch0_2100_5000);
    chain.SetBranchAddress("min_amp_ch1_0_2000", &min_amp_ch1_0_2000);
    chain.SetBranchAddress("min_amp_ch1_2800_5000", &min_amp_ch1_2800_5000);
    chain.SetBranchAddress("min_amp_ch2_0_1900", &min_amp_ch2_0_1900);
    chain.SetBranchAddress("min_amp_ch2_2800_5000", &min_amp_ch2_2800_5000);

    const int n_entr = chain.GetEntries();
    for (int i = 0; i < n_entr; ++i)
    {
        chain.GetEntry(i);
        if(i % 100 == 0) cout << "event = " << i << endl;
//            cout << "min_amp_ch2 = " << min_amp_ch2  << endl;

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
            TPad *pad_cd = (TPad*)canv->GetListOfPrimitives()->FindObject("c_3");
            TGraph *gh_cd = (TGraph*)pad_cd->GetListOfPrimitives()->FindObject("Graph");
            if (pad_cd == NULL)
                cout << "pad_cd == NULL" << endl;
            if(gh_cd == NULL)
                cout << "gh_cd == NULL" << endl;

            vector<double> yv;
            const int n_pnt = gh_cd->GetN();
            yv.resize(n_pnt);
            for (int k = 0; k < n_pnt; ++k)
            {
                double x;
                gh_cd->GetPoint(k, x, yv[k]);
            }

            double integral_total = Get_integral(yv, 0, time_scale, time_integral_unfold_total_sipm_from, time_integral_unfold_total_sipm_to);
            double integral_fast = Get_integral(yv, 0, time_scale, time_integral_unfold_fast_sipm_from, time_integral_unfold_fast_sipm_to);

//            cout << integral_total / 1484.0 << " " << integral_fast / integral_total << endl;
            h2->Fill( - integral_total / 1484.0,  integral_fast / integral_total );
        }

    }

    TCanvas *canv_result = new TCanvas("c", "c", 0, 0, 1900, 1500);
    h2->Draw("COLz");
}

