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
