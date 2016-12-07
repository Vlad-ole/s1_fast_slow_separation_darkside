void graph_q_coeff()
{

    const string file_name = "/home/darkside/Vlad_Programs/Physical_results/q_coeff.txt";

    vector<double> E;
    vector<double> q_n;
    vector<double> q_e;

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
            double pe_from, pe_to, pe_avr, E_keVee, q_nuclear, q_electronic; // The number in the line

            //while the iss is a number
            while ((iss >> pe_from >> pe_to >> pe_avr >> E_keVee >> q_nuclear >> q_electronic))
            {
                E.push_back(E_keVee);
                q_n.push_back(1 - q_nuclear);
                q_e.push_back( 1 - q_electronic);

            }
        }
    }

    TCanvas *c = new TCanvas("c","c");
    c->SetGrid();

    TGraph *gr_n = new TGraph(E.size(), &E[0], &q_n[0]);
    gr_n->GetYaxis()->SetRangeUser(0.2,0.8);
    gr_n->SetTitle("");
    gr_n->SetName("gr_n");
    gr_n->GetXaxis()->SetTitle("E [keVee]");
    gr_n->GetYaxis()->SetTitle("part fast / (fast + slow)");
    gr_n->SetMarkerStyle(20);
    gr_n->SetMarkerSize(1);
    gr_n->SetMarkerColor(kRed);
    gr_n->GetXaxis()->SetNdivisions(20);
    gr_n->GetYaxis()->SetNdivisions(20);


    TGraph *gr_e = new TGraph(E.size(), &E[0], &q_e[0]);
    gr_e->SetName("gr_e");
    gr_e->SetMarkerStyle(20);
    gr_e->SetMarkerSize(1);
    gr_e->SetMarkerColor(kBlue);

    gr_n->Draw("apl");
    gr_e->Draw("same pl");

    leg = new TLegend(0.9,0.7,1.0,0.9);
    leg->AddEntry("gr_n","A_nuclear","lp");
    leg->AddEntry("gr_e","A_electronic","lp");
    leg->Draw();

}
