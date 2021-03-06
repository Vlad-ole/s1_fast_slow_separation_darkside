#include "read_file.h"
#include <fstream>

vector< vector<double> > Get_data(string file_name)
{

    ifstream input_file;
    input_file.open(file_name.c_str(), ios::binary);

    //    cout << "input_file.is_open() = " << input_file.is_open() << endl;
    if(!input_file.is_open())
    {
        cout << "error in file.open = " << file_name << endl;
        exit(1);
    }

    int nchans = 0;
    int nsamps = 0;

    input_file.read( (char *) &nchans, sizeof(int) );
    input_file.read( (char *) &nsamps, sizeof(int) );

    //    cout << "nchans = " << nchans << endl;
    //    cout << "nsamps = " << nsamps << endl;

    vector<int> ch0_read;//PMT
    vector<int> ch1_read;//SiPM 1st part
    vector<int> ch2_read;//SiPM 2nd part

    ch0_read.resize(nsamps);
    ch1_read.resize(nsamps);
    ch2_read.resize(nsamps);

    input_file.read( (char *) (&ch0_read[0]), nsamps * sizeof(int) );
    input_file.read( (char *) (&ch1_read[0]), nsamps * sizeof(int) );
    input_file.read( (char *) (&ch2_read[0]), nsamps * sizeof(int) );

    input_file.close();

    vector< vector<double> > data;

    data.push_back( vector_from_int_to_double(ch0_read) );
    data.push_back( vector_from_int_to_double(ch1_read) );
    data.push_back( vector_from_int_to_double(ch2_read) );

    return data;
}

vector< vector<double> > Get_response(string file_name, const double trigger_time, const double time_scale, const int response_size)
{
    vector<double> ch0_read;//PMT
    vector<double> ch1_read;//SiPM 1st part
    vector<double> ch2_read;//SiPM 2nd part

    ifstream input_file;
    input_file.open(file_name.c_str());
    if(!input_file.is_open())
    {
        cout << "error in file.open = " << file_name << endl;
        exit(1);
    }


    double ch0, ch1, ch2;
    int counter = 0;
    while (input_file >> ch0 >> ch1 >> ch2)
    {
        if(counter > (int)(trigger_time/time_scale) )
        {
            ch0_read.push_back(ch0);
            ch1_read.push_back(ch1);
            ch2_read.push_back(ch2);
        }
        counter++;
    }

    const int ch_size = ch0_read.size();

    if(ch_size < response_size)
    {
        for (int i = 0; i < (response_size - ch_size); ++i)
        {
            ch0_read.push_back(0);
            ch1_read.push_back(0);
            ch2_read.push_back(0);
        }
    }

    if(ch0_read.size() != response_size)
    {
        cout << "ch0_read.size() != response_size" << endl;
        cout << ch0_read.size() << " " << response_size << endl;
    }


    vector< vector<double> > response;
    response.push_back(ch0_read);
    response.push_back(ch1_read);
    response.push_back(ch2_read);

    return response;
}


vector<double> vector_from_int_to_double(vector<int> vector_int)
{
    vector<double> vector_double;
    vector_double.resize( vector_int.size() );
    for (int i = 0; i < vector_int.size(); ++i)
    {
        vector_double[i] = vector_int[i];
    }

    return vector_double;
}

vector<float> vector_from_double_to_float(vector<double> vector_double)
{
    vector<float> vector_float;
    vector_float.resize( vector_double.size() );
    for (int i = 0; i < vector_double.size(); ++i)
    {
        vector_float[i] = vector_double[i];
    }

    return vector_float;
}

vector<double> vector_multiply(vector<double> v, const double value)
{
    vector<double> result;
    result.resize(v.size());
    for (int i = 0; i < v.size(); ++i)
    {
        result[i] = v[i] * value;
    }

    return result;
}

vector<double> vector_subtract(vector<double> v, const double value)
{
    vector<double> result;
    result.resize(v.size());
    for (int i = 0; i < v.size(); ++i)
    {
        result[i] = (v[i] - value);
    }

    return result;
}

vector<double> vector_cut_by_sign(vector<double> v_origin, const int sign)
{
    if(sign == 0)
    {
        cout << "incorrect use of vector_cut_by_sign function" << endl;
        cout << "sign can't be zero" << endl;
        exit(1);
    }

    vector<double> result;
    result.resize(v_origin.size());
    for (int i = 0; i < v_origin.size(); ++i)
    {
        if(sign < 0)
        {
            if(v_origin[i] > 0)
            {
                result[i] = 0;
            }
            else
            {
                result[i] = v_origin[i];
            }
        }
        else
        {
            if(v_origin[i] < 0)
            {
                result[i] = 0;
            }
            else
            {
                result[i] = v_origin[i];
            }
        }
    }

    return result;
}


vector<double> vector_cut_time(vector<double> v_origin, const int n_points)
{
    vector<double> result;
    result.resize(n_points);
    for (int i = 0; i < n_points; ++i)
    {
        result[i] = v_origin[i];
    }

    return result;
}


vector<double> vector_cut_right_value(vector<double> v_origin, const double value)
{
    vector<double> result;

    for(int i = 0; i < v_origin.size(); ++i)
    {
        if(v_origin[i] < value)
        {
            result.push_back(v_origin[i]);
        }
    }

    return result;
}

vector< vector<double> > vector_vector_cut_x_value(vector< vector<double> > v_origin, const double value)
{
    vector< vector<double> > result;
    result.resize(3);

    for(int i = 0; i < v_origin[0].size(); ++i)
    {
        result[0].push_back(v_origin[0][i]);
        result[2].push_back(v_origin[2][i]);

        if(v_origin[0][i] < value)
        {

            result[1].push_back(v_origin[1][i]);
        }
        else
        {
            result[1].push_back(0);
        }
    }

    return result;
}


vector<double> vector_from_float_to_double(vector<float> vector_float)
{
    vector<double> vector_double;
    vector_double.resize( vector_float.size() );
    for (int i = 0; i < vector_float.size(); ++i)
    {
        vector_double[i] = vector_float[i];
    }

    return vector_double;
}
