using namespace RooFit;

const int n_photons = 300;
const int n_events = 100;


int counter(RooDataSet* data, double t_from, double t_to)
{
    int counts = 0;
    for (int i = 0; i < n_photons; ++i)
    {
        RooArgSet *RAS_obj = data->get(i);
        double value = RAS_obj->getRealValue("t");

        if(value > t_from && value < t_to) counts++;
    }

    return counts;
}

void estimate_sigma()
{
    TStopwatch t_stopwatch;
    t_stopwatch.Start();

    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

    RooRealVar t("t","t",0,8000);
    RooRealVar tau_fast("tau_fast","tau_fast parameter", 7);
    RooRealVar tau_slow("tau_slow","tau_slow parameter", 1700);
    RooGenericPdf exp_pdf_fast("exp_pdf_fast","exp_pdf_fast","( 1 / tau_fast * exp( - t / tau_fast ) )", RooArgSet(t, tau_fast));
    RooGenericPdf exp_pdf_slow("exp_pdf_slow","exp_pdf_slow","( 1 / tau_slow * exp( - t / tau_slow ) )", RooArgSet(t, tau_slow));
    RooRealVar frac("frac","fast fraction",0.70);
    RooAddPdf sum_pdf("model","model",RooArgList(exp_pdf_fast,exp_pdf_slow),frac);

    //unbinned hist
    RooRealVar ratio("ratio","ratio",0.3, 0, 1);
    RooDataSet *data_ratio = new RooDataSet("data_ratio","data_ratio",ratio);
    for (int i = 0; i < n_events; ++i)
    {
        RooDataSet* data = sum_pdf.generate(t,n_photons);
        int n_fast = counter(data, 0, 12);
        int n_total = counter(data, 0, 8000);
        ratio.setVal( (double(n_fast)) / n_total );
        data_ratio->add(ratio);
    }

    RooRealVar mean("mean","Mean of Gaussian", 0.3,0,1) ;
    RooRealVar sigma("sigma","Width of Gaussian",0.05,0,1) ;
    RooGaussian gauss("gauss","gauss(ratio,mean,sigma)",ratio,mean,sigma);

//    ratio.setRange("Rfull", 0, 0.9);
//    gauss.fitTo(*data_ratio, Range("Rfull"),Save());
     gauss.fitTo(*data_ratio);

    RooPlot* ratio_frame = ratio.frame(Title("Unbinned fit"));
    data_ratio->plotOn(ratio_frame, Binning(100));

    gauss.plotOn(ratio_frame);
    ratio_frame->Draw();

    cout << endl;
    mean.Print();
    sigma.Print();
    cout << endl;


    t_stopwatch.Stop();
    t_stopwatch.Print();
}
