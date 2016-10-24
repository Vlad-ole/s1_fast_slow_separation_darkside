#include <QCoreApplication>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

int main(int argc, char *argv[])
{

    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am/";
    const int run_id = 6064;

    for(int file_i = 0; file_i < 1; file_i++)
    {
        ostringstream f_oss;
        f_oss << dir_name << "Run" << setfill('0') << setw(6) << run_id << "_event" << setfill('0') << setw(7) << file_i << ".out";
        cout << f_oss.str() << endl << endl;


        ifstream input_file;
        input_file.open(f_oss.str().c_str(), ios::binary);

        cout << "input_file.is_open() = " << input_file.is_open() << endl;
        if(!input_file.is_open())
        {
            cout << "error in input_file.open" << endl;
            return 1;
        }

        int nchans = 0;
        int nsamps = 0;

        input_file.read( (char *) &nchans, sizeof(int) );
        input_file.read( (char *) &nsamps, sizeof(int) );

        cout << "nchans = " << nchans << endl;
        cout << "nsamps = " << nsamps << endl;

        vector<int> xv_ch0;//PMT
        vector<int> xv_ch1;//SiPM 1st part
        vector<int> xv_ch2;//SiPM 2nd part

        vector<int> ch0_read;
        vector<int> ch1_read;
        vector<int> ch2_read;
        ch0_read.resize(nsamps);
        ch1_read.resize(nsamps);
        ch2_read.resize(nsamps);

        input_file.read( (char *) (&ch0_read[0]), nsamps * sizeof(int) );
        input_file.read( (char *) (&ch1_read[0]), nsamps * sizeof(int) );
        input_file.read( (char *) (&ch2_read[0]), nsamps * sizeof(int) );

        input_file.close();

    }

    cout << endl << "all is ok" << endl;
    return 0;
}
