#include <iostream>
#include <algorithm>    // std::min_element, std::max_element
#include <vector>
using namespace std;

void overlapping_loop()
{
    gROOT->SetBatch(kFALSE);

//    TFile file1("/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe_result.root", "READ");
    TFile file1("/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_result.root", "READ");

    const bool is_overlapping = true;
    const int n_graphs = 500;
    vector<double> xv;
    vector< vector<double> > yv;
    yv.resize(n_graphs);

    for(int i = 0; i < n_graphs; i++)
    {
        if(i % 10 == 0 ) cout << "graph " << i << endl;
        TCanvas* canv1 = 0;
        TPad *pad;
        TGraph *gh;

        ostringstream ss;
        ss << "c;" << i + 1;
        //        cout << ss.str().c_str() << endl;

        file1.GetObject(ss.str().c_str(), canv1);
        pad = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_7");
        gh = (TGraph*)pad->GetListOfPrimitives()->FindObject("Graph");

        const int n_points = gh->GetN();

        xv.resize( n_points );
        yv[i].resize( n_points );

        for(int j = 0; j < n_points; j++)
        {
            gh->GetPoint(j, xv[j], yv[i][j]);
        }

    }
    file1.Close();


    TCanvas *canv_result = new TCanvas("c", "c", 0, 0, 1900, 1500);

    //overlapping
    if(is_overlapping)
    {
        bool is_first = true;
        for (int i = 0; i < n_graphs; ++i)
        {
            TGraph *gr =  new TGraph(xv.size() ,&xv[0], &yv[i][0]);

            //        int ci = 1000 + i; // color index
            //        TColor *color = new TColor(ci, 255.0 / (i + 1), 0.2, 0.3);

            if(is_first)
            {
                gr->Draw("al");
                gr->SetTitle("");
                gr->GetYaxis()->SetRangeUser(0,5000);
                gr->GetXaxis()->SetTitle("Time [ns]");
                gr->GetYaxis()->SetTitle("Amplitude [channels]");
                //            gr->SetLineColor(color);

                is_first = false;
            }
            else
            {
                gr->Draw("same l");
            }

//            delete gr;

        }
    }

    //averaging
    if(!is_overlapping)
    {
        vector<double> yv_avr;
        const int n_points = yv[0].size();
        yv_avr.resize( n_points );
        for (int i = 0; i < n_graphs; ++i)
        {
            for (int j = 0; j < n_points; ++j)
            {
                yv_avr[j] += (yv[i][j] / n_graphs);
            }
        }



        TGraph *gr =  new TGraph(xv.size() ,&xv[0], &yv_avr[0]);
        gr->Draw("al");
        gr->SetTitle("");
//        gr->GetYaxis()->SetRangeUser(2000,3500);
        gr->GetXaxis()->SetTitle("Time [ns]");
        gr->GetYaxis()->SetTitle("Amplitude [channels]");
    }

}
