#include <algorithm>

double Get_overlapped_area(const double m1, const double s1, const double m2, const double s2)
{
    double overlapped_area = 0;
    const double range_l = 0;
    const double range_r = 1;
    const double step = 0.001;
    int n_points = (range_r - range_l) / step;

    for (int i = 0; i < n_points; ++i)
    {
        double time = range_l + step*i;
        overlapped_area += std::min( TMath::Gaus(time, m1, s1, kTRUE), TMath::Gaus(time, m2, s2, kTRUE) );
    }

    overlapped_area /= n_points;

    return overlapped_area;
}


void DrawGraph()
{
    const string file_name = "/home/darkside/Vlad_Programs/Physical_results/sep_prob_simulation.txt";
    const bool is_log_scale = true;

    const int n_parts = 3;
    vector< vector<double> > time_of_right_edge;
    vector< vector<double> > mean;
    vector< vector<double> > sigma;
    vector< vector<double> > N_ph;

    time_of_right_edge.resize(n_parts);
    mean.resize(n_parts);
    sigma.resize(n_parts);
    N_ph.resize(n_parts);

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
            int id, N_ph_i;
            double time_of_right_edge_d, mean_d, sigma_d; // The number in the line

            //while the iss is a number
            while ((iss >> id >> time_of_right_edge_d >> mean_d >> sigma_d >> N_ph_i))
            {
                if(id == 3)
                {
                    time_of_right_edge[0].push_back(time_of_right_edge_d);
                    mean[0].push_back(mean_d);
                    sigma[0].push_back(sigma_d);
                    N_ph[0].push_back(N_ph_i);
                }

                if(id == 4)
                {
                    time_of_right_edge[1].push_back(time_of_right_edge_d);
                    mean[1].push_back(mean_d);
                    sigma[1].push_back(sigma_d);
                    N_ph[1].push_back(N_ph_i);
                }

                if(id == 5)
                {
                    time_of_right_edge[2].push_back(time_of_right_edge_d);
                    mean[2].push_back(mean_d);
                    sigma[2].push_back(sigma_d);
                    N_ph[2].push_back(N_ph_i);
                }



            }
        }
    }

    vector<double> overlapped_area;
    vector<double> xv;
    xv.resize(time_of_right_edge[0].size());
    overlapped_area.resize(time_of_right_edge[0].size());

    for (int i = 0; i < time_of_right_edge[0].size(); ++i)
    {
        overlapped_area[i] = Get_overlapped_area(mean[0][i], sigma[0][i], mean[1][i], sigma[1][i]);
//        overlapped_area[i] = Get_overlapped_area(0.30, 0.02, 0.32, 0.02);
        cout << "overlapped_area " << i << " = " << overlapped_area[i] << endl;
    }

    TCanvas *c = new TCanvas("c","simulation. PDF = a*pdf_fast(7ns) + (1 - a)*pdf_slow(1700ns). Guass(sigma = 33 ns) response.");
    c->Divide(2,2);

    c->cd(1);
    if(is_log_scale)
    {
        TPad *pad_cd1 = (TPad*)c->GetListOfPrimitives()->FindObject("c_1");
        pad_cd1->SetLogx();
    }
    TGraph *gr_cd1 = new TGraph(time_of_right_edge[0].size(), &time_of_right_edge[0][0], &mean[0][0]);
    gr_cd1->SetName("gr_cd1");
//    gr_cd1->GetYaxis()->SetRangeUser(1E-3,1);
    gr_cd1->SetTitle("");
    gr_cd1->GetXaxis()->SetTitle("Time[ns]");
    gr_cd1->GetYaxis()->SetTitle("mean");
    gr_cd1->SetMarkerStyle(20);
    gr_cd1->SetMarkerSize(1);
    gr_cd1->SetMarkerColor(kBlue);
    gr_cd1->Draw();

    TGraph *gr_cd1_2 = new TGraph(time_of_right_edge[1].size(), &time_of_right_edge[1][0], &mean[1][0]);
    gr_cd1_2->SetName("gr_cd1_2");
    gr_cd1_2->SetMarkerStyle(20);
    gr_cd1_2->SetMarkerSize(1);
    gr_cd1_2->SetMarkerColor(kRed);
    gr_cd1_2->Draw("same pl");

    leg = new TLegend(0.9,0.7,1.0,0.9);
    leg->AddEntry("gr_cd1","a = 0.3","lp");
    leg->AddEntry("gr_cd1_2","a = 0.7","lp");
    leg->Draw();


    c->cd(2);
    if(is_log_scale)
    {
        TPad *pad_cd2 = (TPad*)c->GetListOfPrimitives()->FindObject("c_2");
        pad_cd2->SetLogx();
    }
    TGraph *gr_cd2 = new TGraph(time_of_right_edge[0].size(), &time_of_right_edge[0][0], &sigma[0][0]);
    gr_cd2->SetName("gr_cd2");
//    gr_cd1->GetYaxis()->SetRangeUser(1E-3,1);
    gr_cd2->SetTitle("");
    gr_cd2->GetXaxis()->SetTitle("Time[ns]");
    gr_cd2->GetYaxis()->SetTitle("sigma");
    gr_cd2->SetMarkerStyle(20);
    gr_cd2->SetMarkerSize(1);
    gr_cd2->SetMarkerColor(kBlue);
    gr_cd2->Draw();

    TGraph *gr_cd2_2 = new TGraph(time_of_right_edge[1].size(), &time_of_right_edge[1][0], &sigma[1][0]);
    gr_cd2_2->SetName("gr_cd2_2");
    gr_cd2_2->SetMarkerStyle(20);
    gr_cd2_2->SetMarkerSize(1);
    gr_cd2_2->SetMarkerColor(kRed);
    gr_cd2_2->Draw("same pl");

    c->cd(3);
    if(is_log_scale)
    {
        TPad *pad_cd3 = (TPad*)c->GetListOfPrimitives()->FindObject("c_3");
        pad_cd3->SetLogx();
        pad_cd3->SetLogy();
    }
    TGraph *gr_cd3 = new TGraph(time_of_right_edge[0].size(), &time_of_right_edge[0][0], &overlapped_area[0]);
    gr_cd3->SetName("gr_cd3");
//    gr_cd1->GetYaxis()->SetRangeUser(1E-3,1);
    gr_cd3->SetTitle("overlapped_area (1 = full overlapping, 0 = full separation)");
    gr_cd3->GetXaxis()->SetTitle("Time[ns]");
    gr_cd3->GetYaxis()->SetTitle("overlapped_area");
    gr_cd3->SetMarkerStyle(20);
    gr_cd3->SetMarkerSize(1);
    gr_cd3->SetMarkerColor(kGreen);
    gr_cd3->Draw();

    c->cd(4);
    if(is_log_scale)
    {
        TPad *pad_cd4 = (TPad*)c->GetListOfPrimitives()->FindObject("c_4");
        pad_cd4->SetLogx();
    }
    TGraph *gr_cd4 = new TGraph(time_of_right_edge[2].size(), &N_ph[2][0], &sigma[2][0]);
    gr_cd4->SetName("gr_cd4");
//    gr_cd1->GetYaxis()->SetRangeUser(1E-3,1);
    gr_cd4->SetTitle("A = 0.3, t_window = (-500, 20). 20 ns is optimum");
    gr_cd4->GetXaxis()->SetTitle("N_ph");
    gr_cd4->GetYaxis()->SetTitle("sigma");
    gr_cd4->SetMarkerStyle(20);
    gr_cd4->SetMarkerSize(1);
    gr_cd4->SetMarkerColor(kGreen);
    gr_cd4->Draw();


}
