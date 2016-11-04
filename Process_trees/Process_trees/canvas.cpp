void canvasscrollbars_create ()
{
   //good view - no problems

   TCanvas *c = new TCanvas("c","c");
   c->Divide(2, 4);
   c->SetCanvasSize(1500, 1500);
   c->SetWindowSize(800, 800);
   TF1 *f = new TF1 ("f","x*sin(x)",-10,10);

   c->cd(1);
   f->Draw();

   c->cd(2);
   f->Draw();

   c->cd(3);
   f->Draw();

   c->cd(4);
   f->Draw();

   c->cd(5);
   f->Draw();

   c->cd(6);
   f->Draw();

   c->cd(7);
   f->Draw();

   c->cd(8);
   f->Draw();


   TTree tree("t1", "Parser tree");
   tree.Branch("canvas_tr", "TCanvas", &c);
   tree.Fill();


   TFile file("/home/darkside/Vlad_Programs/test.root", "RECREATE");
   tree.Write();
   file.Close();
}

void canvasscrollbars_process ()
{
    TObjArray Hlist(0);
    Hlist.SetOwner(kTRUE);

    TCanvas* canv = 0;

    TChain chain("t1");
    chain.Add("/home/darkside/Vlad_Programs/test.root");
    chain.SetBranchAddress("canvas_tr", &canv);

    for (int i = 0; i < chain.GetEntries(); ++i)
    {
        chain.GetEntry(i);

        if(true)//some cuts
        {
            Hlist.Add( canv->Clone() );
        }
    }

    TFile file("/home/darkside/Vlad_Programs/test_result.root", "RECREATE");
    Hlist.Write();
    file.Close();
}


