#ifndef READ_FILE_H
#define READ_FILE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector< vector<double> > Get_data(string file_name);
vector< vector<double> > Get_response(string file_name, const double trigger_time, const double time_scale, const int response_size);
vector<double> vector_from_int_to_double(vector<int>);
vector<double> vector_multiply(vector<double> v, const double value);
vector<double> vector_subtract(vector<double> v, const double value);
vector<float> vector_from_double_to_float(vector<double>);
vector<double> vector_cut_by_sign(vector<double> v_origin, const int sign);
vector<double> vector_cut_time(vector<double> v_origin, const int n_points);
vector<double> vector_cut_right_value(vector<double> v_origin, const double value);
vector< vector<double> > vector_vector_cut_x_value(vector< vector<double> > v_origin, const double value);

#endif // READ_FILE_H
