void get_graph()
{
    TFile file1("/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am_trees_unfold/", "READ");
    TCanvas* canv1 = 0;
    file1.ls();
    file1.GetObject("c", canv1);
    file1.Close();

    TPad *pad_cd = (TPad*)canv1->GetListOfPrimitives()->FindObject("c_4");
    TGraph *gh_cd = (TGraph*)pad_cd->GetListOfPrimitives()->FindObject("Graph");
//    TGraph *gh_cd = (TGraph*)pad_cd->GetListOfPrimitives()->At(1);
//    TGraph *gh_2 = (TGraph*)pad_cd->GetListOfPrimitives()->At(2);

//    TIter next(GetListOfPrimitives());
//    while ((TObject *obj = next()))
//       obj->Draw(next.GetOption());


    if (pad_cd == NULL)
        cout << "pad_cd == NULL" << endl;
    if (gh_cd == NULL)
        cout << "gh_cd == NULL" << endl;

    pad_cd->ls();

//    double baseline = Get_baseline(gh_cd);
//    TF1 *tf1 = new TF1("tf1","[0]", 0, 16000);
//    tf1->SetParameter(0,baseline);

    TCanvas *canv_result = new TCanvas("c", "c", 0, 0, 1900, 1500);
    pad_cd->Draw();
//    gh_2->Draw("same");
////    tf1->Draw("same");

    TObjArray Hlist_gr(0);
    Hlist_gr.SetOwner(kTRUE);

    Hlist_gr.Add( canv_result->Clone() );

    TFile ofile_Hlist_gr("/home/darkside/Vlad_Programs/vlad_rawdata/test.root", "RECREATE");
    Hlist_gr.Write();
    ofile_Hlist_gr.Close();
}

double Get_baseline(TGraph *gh_cd)
{
    double baseline = 0;
    const double time_scale = 4;
    const int size = 400.0 / time_scale;

    for (int i = 0; i < size; ++i)
    {
        double x, y;
        gh_cd->GetPoint(i, x, y);
        baseline += y;
    }

    return baseline / size;
}
