void DrawGraph()
{
    const string file_name = "/home/darkside/Vlad_Programs/Physical_results/Gold_deconvolution_sigma_vs_time.txt";

    vector<double> iter;
    vector<double> sigma;
    vector<double> time;

    ifstream input_file;
    input_file.open(file_name.c_str());
    if(!input_file.is_open())
    {
        cout << "error in file.open = " << file_name << endl;
        exit(1);
    }

    double x, y, z;
    while (input_file >> x >> y >> z)
    {
        iter.push_back(x);
        sigma.push_back(y);
        time.push_back(z);
    }

    TCanvas *c = new TCanvas("c","Simulation. Gaus(sigma = 1, amp = 1). Gold deconvolution. No noise. NRep = 1, boost = 1.");
    c->Divide(2, 2);

    c->cd(1);
    TGraph *gr_cd1 = new TGraph(time.size(), &time[0], &sigma[0]);
    gr_cd1->SetTitle("");
    gr_cd1->GetXaxis()->SetTitle("Time / file [ms]");
    gr_cd1->GetYaxis()->SetTitle("Min(sigma)[s]");
    gr_cd1->SetMarkerStyle(20);
    gr_cd1->SetMarkerSize(1);
    gr_cd1->SetMarkerColor(kBlue);
    gr_cd1->Draw();

    c->cd(2);
    TGraph *gr_cd2 = new TGraph(time.size(), &iter[0], &sigma[0]);
    gr_cd2->SetTitle("");
    gr_cd2->GetXaxis()->SetTitle("Number of iterations");
    gr_cd2->GetYaxis()->SetTitle("Min(sigma)[s]");
    gr_cd2->SetMarkerStyle(20);
    gr_cd2->SetMarkerSize(1);
    gr_cd2->SetMarkerColor(kBlue);
    gr_cd2->Draw();

    c->cd(3);
    TGraph *gr_cd3 = new TGraph(time.size(), &iter[0], &time[0]);
    gr_cd3->SetTitle("");
    gr_cd3->GetXaxis()->SetTitle("Number of iterations");
    gr_cd3->GetYaxis()->SetTitle("Time / file [ms]");
    gr_cd3->SetMarkerStyle(20);
    gr_cd3->SetMarkerSize(1);
    gr_cd3->SetMarkerColor(kBlue);
    gr_cd3->Draw();


}
