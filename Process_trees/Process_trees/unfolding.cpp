void unfold()
{
    Int_t i;
    const Int_t nbins = 256;
    Double_t xmin     = 0;
    Double_t xmax     = nbins;
    Double_t source[nbins];
    Double_t response[nbins];
    gROOT->ForceStyle();
    TString dir  = gROOT->GetTutorialsDir();
    TString file = dir+"/spectrum/TSpectrum.root";
    TFile *f     = new TFile(file.Data());
    TH1F *h = (TH1F*) f->Get("decon1");
    h->SetTitle("Deconvolution");
    TH1F *d = (TH1F*) f->Get("decon_response");
    for (i = 0; i < nbins; i++) source[i]=h->GetBinContent(i + 1);
    for (i = 0; i < nbins; i++) response[i]=d->GetBinContent(i + 1);
    h->SetMaximum(30000);
    h->Draw("L");
    TSpectrum *s = new TSpectrum();
    s->Deconvolution(source,response,256,1000,1,1);
    for (i = 0; i < nbins; i++) d->SetBinContent(i + 1,source[i]);
    d->SetLineColor(kRed);
    d->Draw("SAME L");
}

void unfold_vlad()
{
    TSpectrum s;

    const double time_scale = 0.1;
    const int numberIterations = 10000;
    const int numberRepetitions = 1;
    const double boost = 1;
    const double response_rangle_from = 0;
    const Int_t ssize = 700;


    float source[ssize];
    float response[ssize];
    float xv_response[ssize];

    cout << "ssize = " << ssize << endl;
    for (int i = 0; i < ssize; ++i)
    {
        double time =  i*time_scale + response_rangle_from;
        xv_response[i] = time;
        response[i] = -TMath::Gaus(time, 20);
        source[i] = -(TMath::Gaus(time, 30) + TMath::Gaus(time, 40));
    }

    TCanvas* canv = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv->Divide(2, 2);

    TGraph *graph = new TGraph(ssize, &xv_response[0], &response[0]);
    graph->SetTitle("response");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.7);
    canv->cd(1);
    graph->Draw("AP");


    TGraph *graph_source = new TGraph(ssize, &xv_response[0], &source[0]);
    graph_source->SetTitle("original signal");
    graph_source->SetMarkerStyle(20);
    graph_source->SetMarkerSize(0.7);
    canv->cd(2);
    graph_source->Draw("AP");


    s.Deconvolution(source,response,ssize,numberIterations,numberRepetitions,boost);

    TGraph *graph_unfold = new TGraph(ssize, &xv_response[0], &source[0]);
    graph_unfold->SetTitle("unfold signal");
    graph_unfold->SetMarkerStyle(20);
    graph_unfold->SetMarkerSize(0.9);
    canv->cd(4);
    graph_unfold->Draw("AP");

}

void unfold_test()
{
    TSpectrum *s = new TSpectrum();
    const Int_t nbins = 256;
    float source[nbins];
    float response[nbins];
    s->Deconvolution(source,response,256,1000,1,1);
}

