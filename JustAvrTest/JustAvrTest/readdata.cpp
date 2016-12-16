#include "readdata.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

using namespace std;

ReadData::ReadData(const int file_number) : is_ok(true)
{
    ostringstream f_oss;

//    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6061_1pe/";
//    const int run_id = 6061;

    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6157_generator/";
    const int run_id = 6157;

    f_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_number << ".out";

    input_file.open(f_oss.str().c_str(), ios::binary);

    if(!input_file.is_open())
    {
        cout << "error in file.open = " << f_oss.str().c_str() << endl;
        exit(1);
    }

    int nchans = 0;
    int nsamps = 0;

    input_file.read( (char *) &nchans, sizeof(int) );
    input_file.read( (char *) &nsamps, sizeof(int) );

    if( (nchans != 1) || (nsamps != 1248) )
    {
        is_ok = false;
        cout << endl << "is_ok status = " << is_ok << endl;
        cout << "file_number = " << file_number << endl;
        cout << "nchans = " << nchans << endl;
        cout << "nsamps = " << nsamps << endl << endl;
    }

    if (is_ok)
    {
        yv.resize(nchans);
        for (int i = 0; i < nchans; ++i)
        {
            yv[i].resize(nsamps);
            input_file.read( (char *) (&yv[i][0]), nsamps * sizeof(int) );
        }

            //test
//        for (int i = 0; i < nsamps; ++i)
//        {
//            cout << "yv[0][" << i << "] = " << yv[0][i] << endl;
//        }
    }

}

ReadData::~ReadData()
{
    input_file.close();
}

const std::vector<std::vector<int> > ReadData::GetDataInt() const
{
    return yv;
}

const bool ReadData::GetStatusIsOk() const
{
    return is_ok;
}
