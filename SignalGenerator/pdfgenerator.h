#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H

#include "RooAddPdf.h"
#include "RooRealVar.h"
#include "RooGenericPdf.h"

class PdfGenerator
{
public:
    PdfGenerator(const double tau_fast, const double tau_slow, const double fraction, const double time_from, const double time_to);
    std::vector<double> GetPDFValues(const int n_ph);

private:
    RooAddPdf *sum_pdf;
    RooRealVar t;

    RooRealVar tau_f;
    RooRealVar tau_s;
    RooGenericPdf exp_pdf_fast;
    RooGenericPdf exp_pdf_slow;
    RooRealVar frac;

};

#endif // PDFGENERATOR_H
