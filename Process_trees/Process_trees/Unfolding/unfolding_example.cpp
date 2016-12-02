#include <iostream>
#include <algorithm>    // std::min_element, std::max_element
#include <vector>
using namespace std;

void unfolding_example()
{
    TSpectrum s;
    TSpectrum s_unfold_response;

    const double min_time = 0;
    const double max_time = 70;
    const double time_scale = 0.1;
    const int ssize = (max_time - min_time) / time_scale;

    const int numberIterations = 100;
    const int numberRepetitions = 1;
    const double boost = 1.0;

    vector<float> source;
    vector<float> response;
    vector<float> xv;

    source.resize(ssize);
    response.resize(ssize);
    xv.resize(ssize);

    TF1 *ff1_response = new TF1("ff1_response","TMath::Gaus(x, 20) - 0.2*TMath::Gaus(x, 23)",min_time,max_time);

    for (int i = 0; i < ssize; ++i)
    {
        double time =  i*time_scale + min_time;
        xv[i] = time;

        response[i] = ff1_response->Eval(time);
        source[i] = ff1_response->Eval(time - 10) + ff1_response->Eval(time - 20);
    }


    TCanvas* canv = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv->Divide(2, 2);

    TGraph *graph = new TGraph(ssize, &xv[0], &response[0]);
    graph->SetTitle("response");
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.7);
    canv->cd(1);
    graph->Draw("AP");


    TGraph *graph_source = new TGraph(ssize, &xv[0], &source[0]);
    graph_source->SetTitle("original signal");
    graph_source->SetMarkerStyle(20);
    graph_source->SetMarkerSize(0.7);
    canv->cd(2);
    graph_source->Draw("AP");


    s.Deconvolution(&source[0],&response[0],ssize,numberIterations,numberRepetitions,boost);
    s_unfold_response.Deconvolution(&response[0],&response[0],ssize,numberIterations,numberRepetitions,boost);


    TGraph *graph_unfold_response = new TGraph(ssize, &xv[0], &response[0]);
    graph_unfold_response->SetTitle("unfold response");
    graph_unfold_response->SetMarkerStyle(20);
    graph_unfold_response->SetMarkerSize(0.9);
    canv->cd(3);
    graph_unfold_response->Draw("AP");

    TGraph *graph_unfold_source = new TGraph(ssize, &xv[0], &source[0]);
    graph_unfold_source->SetTitle("unfold signal");
    graph_unfold_source->SetMarkerStyle(20);
    graph_unfold_source->SetMarkerSize(0.9);
    canv->cd(4);
    graph_unfold_source->Draw("AP");

}
