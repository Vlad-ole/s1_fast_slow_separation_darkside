#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <vector>

#include "TF1.h"
#include "Math/Interpolator.h"
#include "RooAddPdf.h"
#include "RooRealVar.h"
#include "RooGenericPdf.h"

#include "response.h"
#include "pdfgenerator.h"

class Generator
{
public:
    Generator(const Response* const rsp, PdfGenerator *pdfgen , const double time_from, const double time_to, const double time_scale);
    ~Generator();

    void Generate(const int N_ph);

    std::vector<double> Get_xv() const;
    std::vector<double> Get_yv() const;

private:
    std::vector<double> xv;
    std::vector<double> yv;

    const double time_scale;
    const TF1 *func;
    const double time_from;
    const double time_to;
    int size;
    const Response* rsp;
    PdfGenerator* pdfgen;
};

#endif // GENERATOR_H
