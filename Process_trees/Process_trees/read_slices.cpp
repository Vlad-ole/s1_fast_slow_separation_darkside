void read_slices()
{
    TFile *f = new TFile("/home/darkside/Vlad_Programs/Physical_results/run_6053_bg_slices.root");
    TTree *tree = (TTree*)f->Get("t_slices");

    TCanvas* canv = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv->Divide(2, 2);

    canv->cd(1);
    tree->Draw("mean:(N_ph_from+5)", "", "pl");

    canv->cd(2);
    tree->Draw("sigma:(N_ph_from+5)", "", "pl");

    canv->cd(3);
    tree->Draw("amp:(N_ph_from+5)", "", "pl");
}
