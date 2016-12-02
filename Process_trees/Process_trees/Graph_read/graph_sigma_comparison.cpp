vector< vector<double> > Get_sigma_npe(const int identificator)
{
    ifstream input_file;
    input_file.open("/home/darkside/Vlad_Programs/Physical_results/sep_prob_simulation.txt");
    if(!input_file.is_open())
    {
        cout << "error in file.open = " << file_name << endl;
        exit(1);
    }


    vector< vector<double> > result;
    result.resize(2);

    //verion with comments reading
    string line;
    while (std::getline(input_file, line))
    {
        if (line[0] != "#" )
        {
            std::istringstream iss(line);
            int id, N_ph_i;
            double time_of_right_edge_d, mean_d, sigma_d;

            //while the iss is a number
            while ((iss >> id >> time_of_right_edge_d >> mean_d >> sigma_d >> N_ph_i))
            {
                if(id == identificator)
                {
                    result[0].push_back(N_ph_i);
                    result[1].push_back(sigma_d);
                }
            }
        }
    }

    return result;
}

void graph_sigma_comparison()
{
    TFile f("/home/darkside/Vlad_Programs/Physical_results/run_6053_bg_slices.root");
    TFile f2("/home/darkside/Vlad_Programs/Physical_results/run_6064_Am_slices.root");
    TTree *tree = (TTree*)f.Get("t_slices");
    TTree *tree2 = (TTree*)f2.Get("t_slices");

    double mean_d, sigma_d, amp_d, N_ph_from_d;
    double mean_err, sigma_err, amp_err;
    TH1F *h;
    double sigma_d2, N_ph_from_d2;

    tree->SetBranchAddress("sigma", &sigma_d);
    tree->SetBranchAddress("N_ph_from", &N_ph_from_d);
    tree2->SetBranchAddress("sigma", &sigma_d2);
    tree2->SetBranchAddress("N_ph_from", &N_ph_from_d2);


    const int n_cases = 3;
    vector< vector <double> > sigma;
    vector< vector <double> > n_pe;
    sigma.resize(n_cases);
    n_pe.resize(n_cases);


    const int n_entr = tree->GetEntries();
    sigma[0].resize(n_entr);
    n_pe[0].resize(n_entr);
    cout << "there are " << n_entr << " events" << endl;
    for (int i = 0; i < n_entr; ++i) // iterate all events
    {
        tree->GetEntry(i);
        sigma[0][i] = sigma_d;
        n_pe[0][i] = (N_ph_from_d + 5);//because step = 10
    }


    const int n_entr2 = tree2->GetEntries();
    sigma[1].resize(n_entr2);
    n_pe[1].resize(n_entr2);
    cout << "there are " << n_entr2 << " events" << endl;
    for (int i = 0; i < n_entr2; ++i) // iterate all events
    {
        tree2->GetEntry(i);
        sigma[1][i] = sigma_d2;
        n_pe[1][i] = (N_ph_from_d2 + 5);//because step = 10
    }

//    vector< vector<double> > sim_data_tw20_res0 = Get_sigma_npe(0);
//    vector< vector<double> > sim_data_20 = Get_sigma_npe(5);
//    vector< vector<double> > sim_data_200 = Get_sigma_npe(6);
//    vector< vector<double> > sim_data_500 = Get_sigma_npe(7);
//    vector< vector<double> > sim_data_tw20_res10 = Get_sigma_npe(8);
    vector< vector<double> > sim_data_tw200_res33 = Get_sigma_npe(9);


//    TCanvas* canv = new TCanvas("c", "c", 0, 0, 1900, 1500);
//    tree->Draw("sigma:(N_ph_from+5)", "", "pl");

    TGraph *gr_cd1 = new TGraph(sigma[0].size(), &n_pe[0][0], &sigma[0][0]);
    gr_cd1->SetName("gr_cd1");
//    gr_cd1->GetYaxis()->SetRangeUser(1E-3,1);
    gr_cd1->SetTitle("Comparison of real and ideal sigma");
    gr_cd1->GetXaxis()->SetTitle("n_pe");
    gr_cd1->GetYaxis()->SetTitle("sigma");
    gr_cd1->SetMarkerStyle(20);
    gr_cd1->SetMarkerSize(1);
    gr_cd1->SetMarkerColor(kBlue);
    gr_cd1->Draw();

    TGraph *gr_cd1_2 = new TGraph(sigma[1].size(), &n_pe[1][0], &sigma[1][0]);
    gr_cd1_2->SetName("gr_cd1_2");
    gr_cd1_2->SetMarkerStyle(20);
    gr_cd1_2->SetMarkerSize(1);
    gr_cd1_2->SetMarkerColor(kRed);
    gr_cd1_2->Draw("same pl");

    TGraph *gr_cd1_3 = new TGraph(sim_data_tw200_res33[0].size(), &sim_data_tw200_res33[0][0], &sim_data_tw200_res33[1][0]);
    gr_cd1_3->SetName("gr_cd1_3");
    gr_cd1_3->SetMarkerStyle(20);
    gr_cd1_3->SetMarkerSize(1);
    gr_cd1_3->SetMarkerColor(kGreen);
    gr_cd1_3->Draw("same pl");

//    TGraph *gr_cd1_4 = new TGraph(sim_data_200[0].size(), &sim_data_200[0][0], &sim_data_200[1][0]);
//    gr_cd1_4->SetName("gr_cd1_4");
//    gr_cd1_4->SetMarkerStyle(20);
//    gr_cd1_4->SetMarkerSize(1);
//    gr_cd1_4->SetMarkerColor(kYellow);
//    gr_cd1_4->Draw("same pl");

//    TGraph *gr_cd1_5 = new TGraph(sim_data_500[0].size(), &sim_data_500[0][0], &sim_data_500[1][0]);
//    gr_cd1_5->SetName("gr_cd1_5");
//    gr_cd1_5->SetMarkerStyle(20);
//    gr_cd1_5->SetMarkerSize(1);
//    gr_cd1_5->SetMarkerColor(46);
//    gr_cd1_5->Draw("same pl");

//    TGraph *gr_cd1_6 = new TGraph(sim_data_tw20_res0[0].size(), &sim_data_tw20_res0[0][0], &sim_data_tw20_res0[1][0]);
//    gr_cd1_6->SetName("gr_cd1_6");
//    gr_cd1_6->SetMarkerStyle(20);
//    gr_cd1_6->SetMarkerSize(1);
//    gr_cd1_6->SetMarkerColor(7);
//    gr_cd1_6->Draw("same pl");

//    TGraph *gr_cd1_7 = new TGraph(sim_data_tw20_res10[0].size(), &sim_data_tw20_res10[0][0], &sim_data_tw20_res10[1][0]);
//    gr_cd1_7->SetName("gr_cd1_7");
//    gr_cd1_7->SetMarkerStyle(20);
//    gr_cd1_7->SetMarkerSize(1);
//    gr_cd1_7->SetMarkerColor(8);
//    gr_cd1_7->Draw("same pl");


    leg = new TLegend(0.9,0.7,1.0,0.9);
    leg->AddEntry("gr_cd1","Run 6053 bg ch2, t_w 200ns, 100it ~ 33ns sigma","lp");
    leg->AddEntry("gr_cd1_2","Run 6064 Am ch2, t_w 200ns, 100it ~ 33ns sigma","lp");
    leg->AddEntry("gr_cd1_3","Sim, gauss resp=33ns, t_w 200ns. maen_exp = mean_sim = 0.315","lp");
//    leg->AddEntry("gr_cd1_4","Sim, gauss resp=33ns, t_w 200ns","lp");
//    leg->AddEntry("gr_cd1_5","Sim, gauss resp=33ns, t_w 500ns","lp");
//    leg->AddEntry("gr_cd1_6","Sim, delta, t_w 20ns","lp");
//    leg->AddEntry("gr_cd1_7","Sim, gauss resp=10ns, t_w 20ns","lp");
    leg->Draw();

}
