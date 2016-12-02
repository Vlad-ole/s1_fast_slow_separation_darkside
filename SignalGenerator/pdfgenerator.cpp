#include "pdfgenerator.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

using namespace std;

PdfGenerator::PdfGenerator(const double tau_fast, const double tau_slow, const double fraction, const double time_from, const double time_to) :
    t("t","t", time_from, time_to), tau_f("tau_fast","tau_fast parameter", tau_fast), tau_s("tau_slow","tau_slow parameter", tau_slow),
    exp_pdf_fast("exp_pdf_fast","exp_pdf_fast","( 1 / tau_fast * exp( - t / tau_fast ) )", RooArgSet(t, tau_f)),
    exp_pdf_slow("exp_pdf_slow","exp_pdf_slow","( 1 / tau_slow * exp( - t / tau_slow ) )", RooArgSet(t, tau_s)),
    frac("frac","fast fraction",fraction)
{
}

vector<double> PdfGenerator::GetPDFValues(const int N_ph)
{

    vector<double> pdf_values;
    pdf_values.resize(N_ph);


    sum_pdf = new RooAddPdf("model","model",RooArgList(exp_pdf_fast,exp_pdf_slow),frac);
    RooDataSet* data = sum_pdf->generate(t,N_ph);

    for (int i = 0; i < N_ph; ++i)
    {
        const RooArgSet *RAS_obj = data->get(i);
        pdf_values[i] = RAS_obj->getRealValue("t");
//        cout << "pdf_values (" << i << ") = " << pdf_values[i] << endl;

    }

    return pdf_values;
}
