#include "generator.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include <iostream>
using namespace std;

Generator::Generator(const Response * const rsp, PdfGenerator * pdfgen, const double time_from, const double time_to, const double time_scale):
    rsp(rsp), pdfgen(pdfgen),time_from(time_from), time_to(time_to), time_scale(time_scale)
{
    size = (time_to - time_from) / time_scale;
    xv.resize(size);
    yv.resize(size);

    for (int i = 0; i < size; ++i)
    {
        xv[i] = time_from + time_scale*i;
    }

}

Generator::~Generator()
{

}

void Generator::Generate(const int N_ph)
{
    vector<double> time_shift = pdfgen->GetPDFValues(N_ph);

    //too slow!
//    if(rsp->GetIsTF1())
    {
        for (int k = 0; k < N_ph; ++k)
        {
            cout << "time_shift (" << k << ") = " << time_shift[k] << endl;
            for (int i = 0; i < size; ++i)
            {
                double time = time_from + time_scale*i;

                if(rsp->GetIsTF1())
                    yv[i] += rsp->Get_response_func()->Eval(time - time_shift[k]);
                else
                    yv[i] += rsp->Get_response_interpolator()->Eval(time - time_shift[k]);
            }
        }

    }
}

std::vector<double> Generator::Get_xv() const
{
    return xv;
}

std::vector<double> Generator::Get_yv() const
{
    return yv;
}


