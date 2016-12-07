using namespace RooFit;

const int n_photons = 500;
const int n_events = 100;

const double step = 4;//sampling rate in simulation
const double t_w = 150;
const double fraction = 0.2308;
const double sigma_response = 33;

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

double Get_integral(vector< vector<double> > signal, const double t_from, const double t_to)
{
    double integral = 0;
    int counts = 0;

    for (int i = 0; i < signal[0].size(); ++i)
    {
        if(signal[0][i] > t_from && signal[0][i] < t_to)
        {
            integral += signal[1][i];
            counts++;
        }
    }

    return integral * step;
}

vector< vector<double> > Get_signal(RooDataSet* data)
{
    const double t_from = -500;
    const double t_to = 8000;
    const double n_sigma_cut = 3.5;//generate gauss func only inside (mean - n_sigma_cut*sigma; mean + n_sigma_cut*sigma) region

    const int n_entries = data->numEntries();
    const int n_points = static_cast<int>( (t_to - t_from) / step );

    vector< vector<double> > signal;
    signal.resize(2);
    signal[0].resize(n_points);
    signal[1].resize(n_points);

    for (int j = 0; j < n_points; ++j)
    {
        signal[0][j] = t_from + step*j;
    }

    for (int i = 0; i < n_entries; ++i)
    {
        RooArgSet *RAS_obj = data->get(i);
        double mean = RAS_obj->getRealValue("t");

        //to slow!
        //        for (int j = 0; j < n_points; ++j)
        //        {
        //            signal[1][j] += TMath::Gaus(signal[0][j], mean, sigma);
        //        }

        //a better variant
        int j_start = static_cast<int>( ((mean - n_sigma_cut*sigma_response) > t_from)?   (mean - n_sigma_cut*sigma_response - t_from) / step :  0 );
        int j_stop =  static_cast<int>( ((mean + n_sigma_cut*sigma_response) < t_to)?   (mean + n_sigma_cut*sigma_response - t_from) / step :  (n_points - 1)  );
        for (int j = j_start; j < j_stop; ++j)
        {
            signal[1][j] += TMath::Gaus(signal[0][j], mean, sigma_response, kTRUE);
        }


    }

    return signal;
}

void estimate_sigma()
{
    TStopwatch t_stopwatch;
    t_stopwatch.Start();

    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

    //define sum_pdf
    RooRealVar t("t","t",0,8000);
    RooRealVar tau_fast("tau_fast","tau_fast parameter", 7);
    RooRealVar tau_slow("tau_slow","tau_slow parameter", 1700);
    RooGenericPdf exp_pdf_fast("exp_pdf_fast","exp_pdf_fast","( 1 / tau_fast * exp( - t / tau_fast ) )", RooArgSet(t, tau_fast));
    RooGenericPdf exp_pdf_slow("exp_pdf_slow","exp_pdf_slow","( 1 / tau_slow * exp( - t / tau_slow ) )", RooArgSet(t, tau_slow));
    RooRealVar frac("frac","fast fraction",fraction);// <------  choose fraction of fast component
    RooAddPdf sum_pdf("model","model",RooArgList(exp_pdf_fast,exp_pdf_slow),frac);


    TStopwatch t_stopwatch_Get_signal;
    TStopwatch t_stopwatch_generate;


    //unbinned hist
    RooRealVar ratio("ratio","ratio",0.3, 0, 1);
    RooDataSet *data_ratio = new RooDataSet("data_ratio","data_ratio",ratio);
    for (int i = 0; i < n_events; ++i)
    {
        if(i % 100 == 0) cout << "event =" << i << endl;

        t_stopwatch_generate.Start(kFALSE);
        RooDataSet* data = sum_pdf.generate(t,n_photons);
        t_stopwatch_generate.Stop();

//        //delta func response
//        int n_fast = counter(data, 0, t_w);
//        int n_total = counter(data, 0, 8000);
//        ratio.setVal( (double(n_fast)) / n_total );

        //gauss response
        t_stopwatch_Get_signal.Start(kFALSE);
        vector< vector<double> > signal = Get_signal(data);
        t_stopwatch_Get_signal.Stop();

        double integral_fast = Get_integral(signal, -500, t_w);
        double integral_total = Get_integral(signal, -500, 8000);
        ratio.setVal( integral_fast / integral_total );

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



    {
//        //convolution delta functions with gauss response
//        RooDataSet* data = sum_pdf.generate(t,n_photons);
//        vector< vector<double> > signal = Get_signal(data);
//        TGraph *gr = new TGraph(signal[0].size(), &signal[0][0], &signal[1][0]);
//        gr->Draw();

//        double integral_fast = Get_integral(signal, -500, 200);
//        double integral_total = Get_integral(signal, -500, 8000);
//        cout << "integral_fast = " << integral_fast << endl;
//        cout << "integral_total = " << integral_total << endl;
//        cout << "ratio = " << integral_fast / integral_total << endl;
    }


    t_stopwatch.Stop();
    cout << "total Get_signal() time = " << t_stopwatch_Get_signal.RealTime() << endl;
    cout << "total generate time = " << t_stopwatch_generate.RealTime() << endl << endl;
    t_stopwatch.Print();
}
