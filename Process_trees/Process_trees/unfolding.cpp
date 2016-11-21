#include <iostream>
#include <algorithm>    // std::min_element, std::max_element
using namespace std;

double Get_integral(vector<float> data_v, const int size, const double baseline, const double time_scale, const double time_from, const double time_to)
{
    double integral = 0;
    const int point_from = time_from / time_scale;
    const int point_to = time_to / time_scale;

    if( point_from > size || point_to > size )
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
    TSpectrum s_unfold_response;

    const double response_rangle_from = 0;
    const double max_time = 70;
    const double time_scale = 0.1;
    const int ssize = max_time / time_scale;
//    const int ssize = 700;

    const int numberIterations = 8;
    const int numberRepetitions = 125;
    const double boost = 1.1;
    const bool is_RL = false;

    const double time_from = 0;
    const double time_to = ssize * time_scale;

    const int n_fake_repetitions = 100;
//    float source[ssize];
//    float response[ssize];
//    float unfold_source[ssize];
//    float unfold_response[ssize];
//    float xv_response[ssize];

    vector<float> source;
    vector<float> response;
    vector<float> unfold_source;
    vector<float> unfold_response;
    vector<float> xv_response;

    source.resize(ssize);
    response.resize(ssize);
    unfold_source.resize(ssize);
    unfold_response.resize(ssize);
    xv_response.resize(ssize);

    cout << "ssize = " << ssize << endl;
    cout << "time_scale = " << time_scale << endl;
    TRandom rnd;
    for (int i = 0; i < ssize; ++i)
    {
        double time =  i*time_scale + response_rangle_from;
        xv_response[i] = time;
        response[i] = TMath::Gaus(time, 20);
        unfold_response[i] = response[i];

        //        double noise = 0.01*TMath::Sin(time);
        double noise = 0.00 * rnd.Uniform(0, 1);
        source[i] =  TMath::Gaus(time, 30)  + 0.2*TMath::Gaus(time, 33);
//                      + 0.1*TMath::Gaus(time, 36) + 0.05*TMath::Gaus(time, 39) + 0.5*TMath::Gaus(time, 42) ) + noise;
        unfold_source[i] = source[i];
    }


    double integral_response = Get_integral(response, ssize, 0, time_scale, time_from, time_to);
    double integral_source = Get_integral(source, ssize, 0, time_scale, time_from, time_to);


    TCanvas* canv = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv->Divide(2, 2);

    TGraph *graph = new TGraph(ssize, &xv_response[0], &response[0]);
    graph->SetTitle("response");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.7);
    canv->cd(1);
    graph->Draw("AP");
    ostringstream sstream_text_cd1_integral;
    sstream_text_cd1_integral << "integral = " << integral_response;
    TPaveText *pt_cd1 = new TPaveText(0.8,0.8,1,1,"nbNDC");
    pt_cd1->AddText(sstream_text_cd1_integral.str().c_str());
    pt_cd1->Draw();


    TGraph *graph_source = new TGraph(ssize, &xv_response[0], &source[0]);
    graph_source->SetTitle("original signal");
    graph_source->SetMarkerStyle(20);
    graph_source->SetMarkerSize(0.7);
    canv->cd(2);
    graph_source->Draw("AP");
    ostringstream sstream_text_cd2_integral;
    sstream_text_cd2_integral << "integral = " << integral_source;
    TPaveText *pt_cd2 = new TPaveText(0.8,0.8,1,1,"nbNDC");
    pt_cd2->AddText(sstream_text_cd2_integral.str().c_str());
    pt_cd2->Draw();

    TStopwatch t;
    t.Start();
    for (int k = 0; k < n_fake_repetitions; ++k)
    {
        for (int i = 0; i < ssize; ++i)
        {
            unfold_source[i] = source[i];
            unfold_response[i] = response[i];
        }

        if(!is_RL)
        {
            s.Deconvolution(&unfold_source[0],&response[0],ssize,numberIterations,numberRepetitions,boost);
            s_unfold_response.Deconvolution(&unfold_response[0],&response[0],ssize,numberIterations,numberRepetitions,boost);
        }
        else
        {
            s.DeconvolutionRL(&unfold_source[0],&response[0],ssize,numberIterations,numberRepetitions,boost);
            s_unfold_response.DeconvolutionRL(&unfold_response[0],&response[0],ssize,numberIterations,numberRepetitions,boost);
        }

    }
    t.Stop();
    t.Print();

    double integral_unfold_response = Get_integral(unfold_response, ssize, 0, time_scale, time_from, time_to);
    double integral_unfold_source = Get_integral(unfold_source, ssize, 0, time_scale, time_from, time_to);


    TGraph *graph_unfold_response = new TGraph(ssize, &xv_response[0], &unfold_response[0]);
    graph_unfold_response->SetTitle("unfold response");
    graph_unfold_response->SetMarkerStyle(20);
    graph_unfold_response->SetMarkerSize(0.9);
    canv->cd(3);
    graph_unfold_response->Draw("AP");
    ostringstream sstream_text_cd3_integral;
    sstream_text_cd3_integral << "integral = " << integral_unfold_response;
    TPaveText *pt_cd3 = new TPaveText(0.8,0.8,1,1,"nbNDC");
    pt_cd3->AddText(sstream_text_cd3_integral.str().c_str());
    pt_cd3->Draw();

    TGraph *graph_unfold_source = new TGraph(ssize, &xv_response[0], &unfold_source[0]);
    graph_unfold_source->SetTitle("unfold signal");
    graph_unfold_source->SetMarkerStyle(20);
    graph_unfold_source->SetMarkerSize(0.9);
    canv->cd(4);
    graph_unfold_source->Draw("AP");
    ostringstream sstream_text_cd4_integral;
    sstream_text_cd4_integral << "integral = " << integral_unfold_source;
    TPaveText *pt_cd4 = new TPaveText(0.8,0.8,1,1,"nbNDC");
    pt_cd4->AddText(sstream_text_cd4_integral.str().c_str());
    pt_cd4->Draw();

//    for (int i = 0; i < ssize; ++i)
//    {
//        cout << xv_response[i] << " " << source[i] << endl;
//    }

}

void unfold_test()
{
    TSpectrum *s = new TSpectrum();
    const Int_t nbins = 256;
    float source[nbins];
    float response[nbins];
    s->Deconvolution(source,response,256,1000,1,1);
}

