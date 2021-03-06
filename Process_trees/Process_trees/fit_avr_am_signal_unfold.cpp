Double_t fit_exp(Double_t *x,Double_t *par)
{
    const double time = x[0];

    const double baseline = par[0];
    const double time_0 = par[1];

    const double amp_1 = par[2];
    const double tau_1 = par[3];

    const double amp_2 = par[4];
    const double tau_2 = par[5];

    double fitval;
    if(time < time_0)
    {
        fitval = baseline;
    }
    else
    {
        fitval = baseline + amp_1*TMath::Exp(- (time - time_0)/tau_1 ) + amp_2*TMath::Exp(- (time - time_0)/tau_2 );
    }

    return fitval;
}

void fit_signal()
{
    TFile file("/home/darkside/Vlad_Programs/Physical_results/avr_Am_signal_unfold.root", "READ");
    const double time_scale = 4;//ns

    TCanvas* canv = 0;
    file.GetObject("c", canv);
    file.Close();

    TPad *pad_cd1 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_1");
    TGraph *gh_cd1 = (TGraph*)pad_cd1->GetListOfPrimitives()->FindObject("Graph");

    TPad *pad_cd2 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_2");
    TGraph *gh_cd2 = (TGraph*)pad_cd2->GetListOfPrimitives()->FindObject("Graph");

    TPad *pad_cd3 = (TPad*)canv->GetListOfPrimitives()->FindObject("c_3");
    TGraph *gh_cd3 = (TGraph*)pad_cd3->GetListOfPrimitives()->FindObject("Graph");

    if(gh_cd1 == NULL || gh_cd2 == NULL || gh_cd3 == NULL) cout << "graph pnt in null" << endl;

    const int size_cd1 = gh_cd1->GetN();
    vector<double> xv;
    vector<double> yv_cd1;
    vector<double> yv_cd2;
    vector<double> yv_cd3;
    xv.resize(size_cd1);
    yv_cd1.resize(size_cd1);
    yv_cd2.resize(size_cd1);
    yv_cd3.resize(size_cd1);

    for (int k = 0; k < size_cd1; ++k)
    {
        double x, y;
        gh_cd1->GetPoint(k, xv[k], yv_cd1[k]);
        gh_cd2->GetPoint(k, x, yv_cd2[k]);
        gh_cd3->GetPoint(k, x, yv_cd3[k]);
    }


    TF1 *fitFcn_1 = new TF1("fitFcn_1", fit_exp, 0, 4500, 6);
    fitFcn_1->SetNpx(10000);
    fitFcn_1->SetLineColor(kBlue);  // sets the color to red
    fitFcn_1->SetParNames("baseline","time_0", "amp_1", "tau_1", "amp_2", "tau_2");
    fitFcn_1->FixParameter(0, 0);//baseline
    fitFcn_1->SetParameter(1, 1994);//time_0
    fitFcn_1->SetParLimits(1, 1993, 2000);
    fitFcn_1->SetParameter(2, 2060.58);//amp_1
    fitFcn_1->SetParameter(3, 15.0554);//tau_1
    fitFcn_1->SetParameter(4, 131.142);//amp_2
    fitFcn_1->SetParameter(5, 883.509);//tau_2

    TF1 *fitFcn_2 = new TF1("fitFcn_2", fit_exp, 0, 4500, 6);
    fitFcn_2->SetNpx(10000);
    fitFcn_2->SetLineColor(kBlue);  // sets the color to red
    fitFcn_2->SetParNames("baseline","time_0", "amp_1", "tau_1", "amp_2", "tau_2");
    fitFcn_2->FixParameter(0, 0);//baseline
    fitFcn_2->SetParameter(1, 2085);//time_0
    fitFcn_2->SetParameter(2, 300);//amp_1
    fitFcn_2->SetParameter(3, 20);//tau_1
    fitFcn_2->SetParameter(4, 100);//amp_2
    fitFcn_2->SetParameter(5, 1000);//tau_2

    TF1 *fitFcn_3 = new TF1("fitFcn_3", fit_exp, 0, 4500, 6);
    fitFcn_3->SetNpx(10000);
    fitFcn_3->SetLineColor(kBlue);  // sets the color to red
    fitFcn_3->SetParNames("baseline","time_0", "amp_1", "tau_1", "amp_2", "tau_2");
    fitFcn_3->FixParameter(0, 0);//baseline
    fitFcn_3->SetParameter(1, 2100);//time_0
    fitFcn_3->SetParLimits(1, 2050, 2090);
    fitFcn_3->SetParameter(2, 350);//amp_1
    fitFcn_3->SetParameter(3, 20);//tau_1
    fitFcn_3->SetParameter(4, 150);//amp_2
    fitFcn_3->SetParameter(5, 1000);//tau_2

    TCanvas *canv_res = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv_res->Divide(1, 3);

    TGraph *graph_cd1 = new TGraph(xv.size(), &xv[0], &yv_cd1[0]);
    graph_cd1->SetTitle("avr unfold signal Am(Channel 0, PMT)");
    graph_cd1->GetXaxis()->SetTitle("time [ns]");
    graph_cd1->GetYaxis()->SetTitle("amplitude[a.u.]");
    canv_res->cd(1);
    graph_cd1->Fit("fitFcn_1", "R");
    graph_cd1->Draw();


    TGraph *graph_cd2 = new TGraph(xv.size(), &xv[0], &yv_cd2[0]);
    graph_cd2->SetTitle("avr unfold signal Am(Channel 1, SiPM)");
    graph_cd2->GetXaxis()->SetTitle("time [ns]");
    graph_cd2->GetYaxis()->SetTitle("amplitude[a.u.]");
    canv_res->cd(2);
    graph_cd2->Fit("fitFcn_2", "R");
    graph_cd2->Draw();


    TGraph *graph_cd3 = new TGraph(xv.size(), &xv[0], &yv_cd3[0]);
    graph_cd3->SetTitle("avr unfold signal Am(Channel 2, SiPM)");
    graph_cd3->GetXaxis()->SetTitle("time [ns]");
    graph_cd3->GetYaxis()->SetTitle("amplitude[a.u.]");
    canv_res->cd(3);
    graph_cd3->Fit("fitFcn_3", "R");
    graph_cd3->Draw();


    cout << endl << "Root cern script fit_avr_am_signal_undold.cpp: all is ok" << endl;
}
