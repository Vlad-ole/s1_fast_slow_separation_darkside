#ifndef TYPECONVERT_H
#define TYPECONVERT_H

#include <iostream>
#include <vector>
using namespace std;

class TypeConvert
{
public:
    static vector<double> vector_from_float_to_double();
    static vector<float> vector_from_double_to_float(vector<double> v_double);
private:
    TypeConvert(){}
};

#endif // TYPECONVERT_H
