void combinecanvas()
{
    TFile file1("/home/darkside/Vlad_Programs/Physical_results/fit_avr_Am_signal.root", "READ");
    TCanvas* canv1 = 0;
    file1.GetObject("c", canv1);
    file1.Close();

    TPad *pad_cd1_1 = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_1");
    TPad *pad_cd2_1 = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_2");
    TPad *pad_cd3_1 = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_3");

    if(pad_cd1_1 == NULL) cout << "TPad pnt in null" << endl;

    TCanvas *canv_result = new TCanvas("c", "c", 0, 0, 1900, 1500);
    canv_result->Divide(3, 2);

    canv_result->cd(1);
    pad_cd1_1->Draw();

    canv_result->cd(2);
    pad_cd2_1->Draw();

    canv_result->cd(3);
    pad_cd3_1->Draw();
}
