void overlapping()
{
    TFile file1("/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_result.root", "READ");
    TCanvas* canv1 = 0;
//    file1.ls();

    file1.GetObject("c;196", canv1);
    TPad *pad = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_12");
    TGraph *gh_1 = (TGraph*)pad->GetListOfPrimitives()->FindObject("Graph");

    file1.GetObject("c;123", canv1);
    pad = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_12");
    TGraph *gh_2 = (TGraph*)pad->GetListOfPrimitives()->FindObject("Graph");

    file1.GetObject("c;8", canv1);
    pad = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_12");
    TGraph *gh_3 = (TGraph*)pad->GetListOfPrimitives()->FindObject("Graph");

    file1.GetObject("c;84", canv1);
    pad = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_12");
    TGraph *gh_4 = (TGraph*)pad->GetListOfPrimitives()->FindObject("Graph");

    file1.GetObject("c;130", canv1);
    pad = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_12");
    TGraph *gh_5 = (TGraph*)pad->GetListOfPrimitives()->FindObject("Graph");

    file1.GetObject("c;90", canv1);
    pad = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_12");
    TGraph *gh_6 = (TGraph*)pad->GetListOfPrimitives()->FindObject("Graph");


    TCanvas *canv_result = new TCanvas("c", "c", 0, 0, 1900, 1500);
    gh_1->Draw("al");

    gh_2->Draw("same l");
    gh_2->SetLineColor(kRed);

    gh_3->Draw("same l");
    gh_3->SetLineColor(kGreen);

    gh_4->Draw("same l");
    gh_4->SetLineColor(kBlue);

    gh_5->Draw("same l");
    gh_5->SetLineColor(kMagenta);

    gh_6->Draw("same l");
    gh_6->SetLineColor(8);

    file1.Close();
}
