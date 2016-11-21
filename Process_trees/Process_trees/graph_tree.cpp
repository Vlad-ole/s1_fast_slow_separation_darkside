void DrawGraphTree()
{
    //open file
    const string file_name = "/home/darkside/Vlad_Programs/Physical_results/Gold_deconvolution_Simulation.txt";
    ifstream input_file;
    input_file.open(file_name.c_str());
    if(!input_file.is_open())
    {
        cout << "error in file.open = " << file_name << endl;
        exit(1);
    }

    TTree tree;
    int iter, rep;
    double boost, sigma, time;
    tree.Branch("iter", &iter, "iter/I");
    tree.Branch("rep", &rep, "rep/I");
    tree.Branch("boost", &boost, "boost/D");
    tree.Branch("sigma", &sigma, "sigma/D");
    tree.Branch("time", &time, "time/D");

    //verion with comments reading
    string line;
    while (std::getline(input_file, line))
    {
        if (line[0] != "#" )
        {
            std::istringstream iss(line);

            //while the iss is a number
            while ((iss >> iter >> rep >> boost >> sigma >> time))
            {
                //look at the number
                tree.Fill();
            }
        }
    }


    TCanvas *c = new TCanvas("c","Simulation. Gaus(u = 30,sigma = 1,amp = 1) + Gaus(u = 33,sigma = 1,amp = 0.2). Gold deconvolution. No noise.");
    c->Divide(2, 2);

    c->cd(1);
    tree.SetMarkerColor(kRed);
    tree.SetMarkerStyle(20);
    tree.SetMarkerSize(1);
    tree.SetLineStyle(1);
    tree.SetLineColor(kBlack);
    tree.Draw("sigma:time", "boost == 1 && rep == 1", " lp");
    tree.Draw("sigma:time", "iter == 1", "same lp");

//    h2->GetXaxis()->SetTitle("my new X title");
//    h2->GetYaxis()->SetTitle("my new Y title");
//    h2->Draw();
//    gPad->Update();
//    tree.SetMarkerColor(kBlue);
//    tree.SetLineStyle(1);
//    tree.SetLineColor(kBlack);
//    tree.Draw("sigma:time", "boost == 1 && rep == 1", "same lp");


//    TGraph *gr_cd1 = new TGraph(time.size(), &time[0], &sigma[0]);
//    gr_cd1->SetTitle("");
//    gr_cd1->GetXaxis()->SetTitle("Time / file [ms]");
//    gr_cd1->GetYaxis()->SetTitle("Min(sigma)[s]");
//    gr_cd1->SetMarkerStyle(20);
//    gr_cd1->SetMarkerSize(1);
//    gr_cd1->SetMarkerColor(kBlue);
//    gr_cd1->Draw();

//    c->cd(2);
//    TGraph *gr_cd2 = new TGraph(time.size(), &iter[0], &sigma[0]);
//    gr_cd2->SetTitle("");
//    gr_cd2->GetXaxis()->SetTitle("numberIterations * numberRepetitions");
//    gr_cd2->GetYaxis()->SetTitle("Min(sigma)[s]");
//    gr_cd2->SetMarkerStyle(20);
//    gr_cd2->SetMarkerSize(1);
//    gr_cd2->SetMarkerColor(kBlue);
//    gr_cd2->Draw();

//    c->cd(3);
//    TGraph *gr_cd3 = new TGraph(time.size(), &iter[0], &time[0]);
//    gr_cd3->SetTitle("");
//    gr_cd3->GetXaxis()->SetTitle("numberIterations * numberRepetitions");
//    gr_cd3->GetYaxis()->SetTitle("Time / file [ms]");
//    gr_cd3->SetMarkerStyle(20);
//    gr_cd3->SetMarkerSize(1);
//    gr_cd3->SetMarkerColor(kBlue);
//    gr_cd3->Draw();


}
