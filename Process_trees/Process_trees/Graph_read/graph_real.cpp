void DrawGraph()
{
    const string file_name = "/home/darkside/Vlad_Programs/Physical_results/Gold_deconvolution_Real_Am_ch2.txt";

    const int n_parts = 6;
    vector< vector<double> > iter;
    vector< vector<double> > rep;
    vector< vector<double> > boost;
    vector< vector<double> > sigma;
    vector< vector<double> > time;
    vector< vector<double> > iter_rep;

    iter.resize(n_parts);
    rep.resize(n_parts);
    boost.resize(n_parts);
    sigma.resize(n_parts);
    time.resize(n_parts);
    iter_rep.resize(n_parts);

    ifstream input_file;
    input_file.open(file_name.c_str());
    if(!input_file.is_open())
    {
        cout << "error in file.open = " << file_name << endl;
        exit(1);
    }

    //verion with comments reading
    string line;
    while (std::getline(input_file, line))
    {
        if (line[0] != "#" )
        {
            std::istringstream iss(line);
            int id, iter_i, rep_i;
            double boost_d, sigma_d, time_d; // The number in the line

            //while the iss is a number
            while ((iss >> id >> iter_i >> rep_i >> boost_d >> sigma_d >> time_d))
            {
                if(id == 1)//fill part 1
                {
                    iter[0].push_back(iter_i);
                    rep[0].push_back(rep_i);
                    boost[0].push_back(boost_d);
                    sigma[0].push_back(sigma_d);
                    time[0].push_back(time_d);
                    iter_rep[0].push_back(iter_i * rep_i);
                }

//                if(id == 2)//fill part 2
//                {
//                    iter[1].push_back(iter_i);
//                    rep[1].push_back(rep_i);
//                    boost[1].push_back(boost_d);
//                    sigma[1].push_back(sigma_d);
//                    time[1].push_back(time_d);
//                    iter_rep[1].push_back(iter_i * rep_i);
//                }

//                if(id == 3)//fill part 3
//                {
//                    iter[2].push_back(iter_i);
//                    rep[2].push_back(rep_i);
//                    boost[2].push_back(boost_d);
//                    sigma[2].push_back(sigma_d);
//                    time[2].push_back(time_d);
//                    iter_rep[2].push_back(iter_i * rep_i);
//                }

//                if(id == 4)//fill part 4
//                {
//                    iter[3].push_back(iter_i);
//                    rep[3].push_back(rep_i);
//                    boost[3].push_back(boost_d);
//                    sigma[3].push_back(sigma_d);
//                    time[3].push_back(time_d);
//                    iter_rep[3].push_back(iter_i * rep_i);
//                }

//                if(id == 5)//fill part 5
//                {
//                    iter[4].push_back(iter_i);
//                    rep[4].push_back(rep_i);
//                    boost[4].push_back(boost_d);
//                    sigma[4].push_back(sigma_d);
//                    time[4].push_back(time_d);
//                    iter_rep[4].push_back(iter_i * rep_i);
//                }

//                if(id == 6)//fill part 5
//                {
//                    iter[5].push_back(iter_i);
//                    rep[5].push_back(rep_i);
//                    boost[5].push_back(boost_d);
//                    sigma[5].push_back(sigma_d);
//                    time[5].push_back(time_d);
//                    iter_rep[5].push_back(iter_i * rep_i);
//                }


            }
        }
    }


    TCanvas *c = new TCanvas("c","Real data. Run 6064 Am. ch2 (SiPM). Gold deconvolution.");
    c->Divide(2, 2);
    c->SetLogx();
    c->SetLogy();

    c->cd(1);
    TGraph *gr_cd1 = new TGraph(time[0].size(), &time[0][0], &sigma[0][0]);
    gr_cd1->SetName("gr_1");
//    gr_cd1->GetYaxis()->SetRangeUser(1,220);
    gr_cd1->SetTitle("");
    gr_cd1->GetXaxis()->SetTitle("Time / file [ms]");
    gr_cd1->GetYaxis()->SetTitle("Min(sigma)[s]");
    gr_cd1->SetMarkerStyle(20);
    gr_cd1->SetMarkerSize(1);
    gr_cd1->SetMarkerColor(kBlue);
    gr_cd1->Draw();

//    TGraph *gr_cd1_2 = new TGraph(time[1].size(), &time[1][0], &sigma[1][0]);
//    gr_cd1_2->SetName("gr_2");
//    gr_cd1_2->SetMarkerStyle(20);
//    gr_cd1_2->SetMarkerSize(1);
//    gr_cd1_2->SetMarkerColor(kRed);
//    gr_cd1_2->Draw("same pl");

//    TGraph *gr_cd1_3 = new TGraph(time[2].size(), &time[2][0], &sigma[2][0]);
//    gr_cd1_3->SetName("gr_3");
//    gr_cd1_3->SetMarkerStyle(20);
//    gr_cd1_3->SetMarkerSize(1);
//    gr_cd1_3->SetMarkerColor(kGreen);
//    gr_cd1_3->Draw("same pl");

//    TGraph *gr_cd1_4 = new TGraph(time[3].size(), &time[3][0], &sigma[3][0]);
//    gr_cd1_4->SetName("gr_4");
//    gr_cd1_4->SetMarkerStyle(20);
//    gr_cd1_4->SetMarkerSize(1);
//    gr_cd1_4->SetMarkerColor(kYellow);
//    gr_cd1_4->Draw("same pl");

//    TGraph *gr_cd1_5 = new TGraph(time[4].size(), &time[4][0], &sigma[4][0]);
//    gr_cd1_5->SetName("gr_5");
//    gr_cd1_5->SetMarkerStyle(20);
//    gr_cd1_5->SetMarkerSize(1);
//    gr_cd1_5->SetMarkerColor(28);
//    gr_cd1_5->Draw("same pl");

//    TGraph *gr_cd1_6 = new TGraph(time[5].size(), &time[5][0], &sigma[5][0]);
//    gr_cd1_6->SetName("gr_6");
//    gr_cd1_6->SetMarkerStyle(20);
//    gr_cd1_6->SetMarkerSize(1);
//    gr_cd1_6->SetMarkerColor(46);
//    gr_cd1_6->Draw("same pl");

    leg = new TLegend(0.9,0.7,1.0,0.9);
    leg->AddEntry("gr_1","it = x, rep = 1, boost = 1","lp");
//    leg->AddEntry("gr_2","it = 1, rep = x, boost = 1.1","lp");
//    leg->AddEntry("gr_3","it = 1, rep = x, boost = 1.2","lp");
//    leg->AddEntry("gr_4","it = 1, rep = x, boost = 1.3","lp");
//    leg->AddEntry("gr_5","it = 1, rep = x, boost = 1.02","lp");
//    leg->AddEntry("gr_6","it*rep = 1000, boost = 1.1","lp");
    leg->Draw();

}
