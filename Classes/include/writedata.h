#ifndef WRITEDATA_H
#define WRITEDATA_H

#include <iostream>
#include <vector>
#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
using namespace std;

class WriteData
{
public:
    WriteData(string file_name);

    void AddGraph(vector<float> xv, vector<float> yv, string graph_name);
    void AddGraph(vector<double> xv, vector<double> yv, string graph_name);

    ~WriteData();
private:
    TFile file;
    TCanvas canv;
    vector<TGraph> gr_v;
};

#endif // WRITEDATA_H
