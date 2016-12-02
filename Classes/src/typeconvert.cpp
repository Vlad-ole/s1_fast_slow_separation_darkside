#include "typeconvert.h"


vector<float> TypeConvert::vector_from_double_to_float(vector<double> v_double)
{
    vector<float> v_float;
    for (int i = 0; i < v_double.size(); ++i)
    {
        v_float[i] = v_double[i];
    }

    return v_float;
}
