void DrawCanvas()
{
    double xv[5] = {1, 2, 3, 4, 5};
    double yv[5] = {11, 12, 13, 14, 15};
    TGraph* gr_1 = new TGraph(5, xv, yv);

    //put gr_1 in array
    TObjArray obj_array(0);
//    obj_array.SetOwner(kTRUE);
    obj_array.Add( gr_1->Clone() );

    //get gr_1 from array
    TGraph* gr_2 = obj_array[0];

    //all work correctly
    cout << "gr_2->GetN() = " << gr_2->GetN() << endl;
    for(int j = 0; j < 5; j++)
    {
        double x, y;
        gr_2->GetPoint(j, x, y);
        cout << j << " " << x << " " << y << endl;
    }

    TCanvas *canv = new TCanvas("c123", "c456", 0, 0, 1900, 1500);
//    gr_1->Draw("apl");//I see graph
    gr_2->Draw("apl");//Why I do not see graph?

    canv->ls();
}
