#ifndef RESPONSE_H
#define RESPONSE_H

#include <iostream>
#include <vector>
#include "Math/Interpolator.h"

#include "TF1.h"

class Response
{
public:
    Response(const std::string file_name, const int ch, const double time_scale);
    Response(const TF1 * const func, const double time_from, const double time_to, const double time_scale);

    std::vector<double> Get_xv() const;
    std::vector<double> Get_yv() const;
    ROOT::Math::Interpolator *Get_response_interpolator();
    const TF1* Get_response_func() const;
    const bool GetIsTF1() const;

private:
    std::vector<double> xv;
    std::vector<double> yv;
    const double time_scale;
    ROOT::Math::Interpolator inter;
    const TF1 *func;
    const bool is_TF1;
};

#endif // RESPONSE_H
