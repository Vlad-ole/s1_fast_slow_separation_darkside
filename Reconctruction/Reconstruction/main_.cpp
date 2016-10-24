//#include <QCoreApplication>

//#include <iostream>
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <iomanip>

//using namespace std;

//int main(int argc, char *argv[])
//{

//    ostringstream f_oss;
//    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run006064_event0000000.out";
////    f_oss << dir_name << "Run" << setfill('0') << setw(6) << 0 << "_event" << setfill('0') << setw(7) << 1 << ".out";
////    cout << f_oss.str() << endl << endl;

//    ifstream input_file;
//    input_file.open(dir_name.c_str(), ios::binary);

//    cout << "input_file.is_open() = " << input_file.is_open() << endl;

//    vector<int> xv_ch0;//PMT
//    vector<int> xv_ch1;//SiPM 1st part
//    vector<int> xv_ch2;//SiPM 2nd part



//    int nchans = 0;
//    int nsamps = 0;

//    input_file.read( (char *) &nchans, sizeof(int) );
//    input_file.read( (char *) &nsamps, sizeof(int) );

////    xv_ch0.resize(nsamps);
////    input_file.read( (char *) (&xv_ch0[0]),  nsamps * sizeof(int) );

//    input_file.close();

//    cout << "nchans = " << nchans << endl;
//    cout << "nsamps = " << nsamps << endl;

////    cout << "xv_ch0[0] = " << xv_ch0[0] << endl;
////    cout << "xv_ch0[1] = " << xv_ch0[1] << endl;


//    cout << endl << "all is ok" << endl;
//    return 0;
//}
