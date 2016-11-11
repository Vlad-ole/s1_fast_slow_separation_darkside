void read_tree_vlad_case1()
{
    //simple case if you do not need access to all data points

    TFile *f = new TFile("/home/darkside/Vlad_Programs/vlad_rawdata/Run16_20161104135239_all.root");
    TTree *tree = (TTree*)f->Get("ntMonFrame");

    TCut cut = "(pin1.t_day+pin1.t_secday/84600) > 0 && (pin1.t_day+pin1.t_secday/84600) < 15 && pin1.ADCVal > 7000 && pin1.ADCVal < 8000";

    TCanvas* canv = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv->Divide(2, 2);

    canv->cd(1);
    tree->Draw("pin1.ADCVal:(pin1.t_day+pin1.t_secday/84600)", cut);

    canv->cd(2);
    tree->Draw("pin1.t_day+pin1.t_secday/84600", cut);

    canv->cd(3);
    tree->Draw("pin1.ADCVal", cut);

}

#include <iostream>
#include <algorithm>    // std::min_element, std::max_element
using namespace std;

vector< vector<double> > Get_avr_data(vector<double> xv, vector<double> yv, const double bin_width)
{
    vector< vector<double> >   result;
    result.resize(2);

    double xv_min = *min_element( xv.begin(), xv.end() );
    double xv_max = *max_element( xv.begin(), xv.end() );

    const int result_size = (int)( (xv_max - xv_min) / bin_width );
    result[0].resize(result_size);
    result[1].resize(result_size);

    cout << "bins = " << result_size << endl;

    for (int i = 0; i < result_size; ++i)
    {
        result[0][i] = xv_min + i*bin_width + bin_width/2;
        double sum = 0;
        int counts = 0;
        double x_val = xv[i];
        while (x_val < xv_min + (i+1)*bin_width)
        {
            sum += yv[i+counts];
            counts++;
            x_val = xv[i+counts];
        }

        cout << "bin = " << i << endl;

        if(counts == 0) result[1][i] = 0;// you should decide what to do with empty bins
        else result[1][i] = sum / counts;

    }


    return result;
}

void read_tree_vlad_case2()
{
    //more complex case, if you want to get access to all data points

    TFile *f = new TFile("/home/darkside/Vlad_Programs/vlad_rawdata/Run16_20161104135239_all.root");
    TTree *tree = (TTree*)f->Get("ntMonFrame");

    //I think there is a better solution. I do not know.
    int array[16];
    int *pin1_NBOF = &array[0];
    int *pin1_NTimeTrgBOF = &array[1];
    int *pin1_NtrgBOF = &array[2];
    int *pin1_BoardAdr = &array[3];
    int *pin1_boardTemp = &array[4];
    int *pin1_cspTemp = &array[5];
    int *pin1_extTemp = &array[6];
    int *pin1_Vbias = &array[7];
    int *pin1_ADCVal = &array[8];
    int *pin1_PulseType = &array[9];
    int *pin1_t_year = &array[10];
    int *pin1_t_mon = &array[11];
    int *pin1_t_day = &array[12];
    int *pin1_t_secday = &array[13];
    int *pin1_fired = &array[14];
    int *pin1_ErrorVector = &array[16];

    //https://root.cern.ch/root/htmldoc/guides/users-guide/Trees.html#branches (14.15.2 Analysis)
    TBranch *b_pin1 = tree->GetBranch("pin1");
    b_pin1->SetAddress(&array[0]);

    //if you want to undestand structure of branch do this:
    //b_pin1->ls();

    vector<double> xv;
    vector<double> yv;
    const double bin_width = 0.1;//[days]

    const int n_entr = tree->GetEntries();
    cout << "there are " << n_entr << " events" << endl;
    for (int i = 0; i < n_entr; ++i) // iterate all events
    {
        b_pin1->GetEntry(i);//get information only from this branch (in common case tree->getEntry(i) )
        const bool condition_time = (*pin1_t_day + *pin1_t_secday/84600) > 0 && (*pin1_t_day + *pin1_t_secday/84600) < 15;
        const bool condition_ADC = *pin1_ADCVal > 7000 && *pin1_ADCVal < 8000;
        if(condition_time  &&  condition_ADC) // your cuts
        {
            xv.push_back(*pin1_t_day + *pin1_t_secday/84600.0);// int / int = int; int / double = double
            yv.push_back(*pin1_ADCVal);
        }
    }

    vector< vector<double> > avr_data = Get_avr_data(xv, yv, bin_width);

    //draw all graphs
    TCanvas* canv = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv->Divide(2, 2);

    TGraph *graph_1 = new TGraph(xv.size(), &xv[0], &yv[0]);
    canv->cd(1);
    graph_1->SetTitle("pin1 original graph");
    graph_1->GetXaxis()->SetTitle("time [days]");
    graph_1->GetYaxis()->SetTitle("ADCVal [channels]");
    graph_1->SetMarkerStyle(20);
    graph_1->SetMarkerSize(0.02);
    graph_1->SetMarkerColor(kRed);
    graph_1->Draw("AP");

    TGraph *graph_2 = new TGraph(avr_data[0].size(), &avr_data[0][0], &avr_data[1][0]);
    canv->cd(2);
    graph_2->SetTitle("pin1 averaged graph");
    graph_2->GetXaxis()->SetTitle("time [days]");
    graph_2->GetYaxis()->SetTitle("ADCVal [channels]");
    graph_2->SetMarkerStyle(20);
    graph_2->SetMarkerSize(0.5);
    graph_2->SetMarkerColor(kGreen);
    graph_2->Draw("AP");

    canv->cd(3);
    graph_1->SetTitle("pin1 original and averaged graphs");
    graph_1->GetXaxis()->SetTitle("time [days]");
    graph_1->GetYaxis()->SetTitle("ADCVal [channels]");
    graph_1->Draw("AP");
    graph_2->Draw("same P");
}

