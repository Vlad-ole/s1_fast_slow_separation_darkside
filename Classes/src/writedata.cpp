#include "writedata.h"

WriteData::WriteData(string file_name) : file(file_name.c_str(), "RECREATE"), canv("c", "c", 0, 0, 1900, 1500)
{
    canv.Divide(2, 2);
}

WriteData::~WriteData()
{
    for (int i = 1; i <= gr_v.size(); ++i)
    {
        canv.cd(i);
        gr_v[i-1].Draw("AP");
    }

    canv.Write();
    file.Close();
}

void WriteData::AddGraph(vector<float> xv, vector<float> yv, string graph_name)
{
    TGraph graph(xv.size(), &xv[0], &yv[0]);
    graph.SetTitle(graph_name.c_str());
    graph.SetMarkerStyle(20);
    graph.SetMarkerSize(0.7);
    gr_v.push_back(graph);
}

void WriteData::AddGraph(vector<double> xv, vector<double> yv, string graph_name)
{
    TGraph graph(xv.size(), &xv[0], &yv[0]);
    graph.SetTitle(graph_name.c_str());
    graph.SetMarkerStyle(20);
    graph.SetMarkerSize(0.7);
    gr_v.push_back(graph);
}


