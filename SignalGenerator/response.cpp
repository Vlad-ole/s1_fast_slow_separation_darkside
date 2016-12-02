#include "response.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;


Response::Response(const string file_name, const int ch, const double time_scale) :
    inter(0, ROOT::Math::Interpolation::kLINEAR), time_scale(time_scale), is_TF1(false)
{
    ifstream input_file;
    input_file.open(file_name.c_str());
    if(!input_file.is_open())
    {
        cout << "error in file.open = " << file_name << endl;
        exit(1);
    }

    double ch0_d, ch1_d, ch2_d;
    while (input_file >> ch0_d >> ch1_d >> ch2_d)
    {
        if(ch == 0)
        {
            yv.push_back(ch0_d);
        }
        else if(ch == 1)
        {
            yv.push_back(ch1_d);
        }
        else if(ch == 2)
        {
            yv.push_back(ch2_d);
        }
        else
        {
            cout << "ch != 0 or 1 or 2" << endl;
            exit(1);
        }

    }

    for (int i = 0; i < yv.size(); ++i)
    {
        xv.push_back(time_scale*i);
    }

    input_file.close();

    inter.SetData(xv, yv);

}

Response::Response(const TF1 * const func, const double time_from, const double time_to, const double time_scale) :
    func(func) ,time_scale(time_scale), is_TF1(true)
{
    const int n_points = (time_to - time_from) / time_scale;

    cout << "time_scale = " << time_scale << endl;
    cout << "time_to = " << time_to << endl;
    cout << "time_from = " << time_from << endl;
    cout << "n_points = " << n_points << endl;
    for (int i = 0; i < n_points; ++i)
    {
        double time = time_from + time_scale*i;
        xv.push_back(time);
        yv.push_back( func->Eval(time) );
    }
}

std::vector<double> Response::Get_xv() const
{
    return xv;
}

std::vector<double> Response::Get_yv() const
{
    return yv;
}

ROOT::Math::Interpolator *Response::Get_response_interpolator()
{
    return &inter;
}

const TF1 *Response::Get_response_func() const
{
    return func;
}

const bool Response::GetIsTF1() const
{
    return is_TF1;
}

