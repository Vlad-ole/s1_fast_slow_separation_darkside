#include <QCoreApplication>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

int main_tmp(int argc, char *argv[])
{
//    const string dir_name = "/home/darkside/Vlad_Programs/binary_test.out";
    const string dir_name = "/home/darkside/Vlad_Programs/vlad_rawdata/Run6064_Am/Run006064_event0000000.out";



//     ofstream out_file;
//     out_file.open(dir_name.c_str(), ios::binary);

//     cout << "out_file.is_open() = " << out_file.is_open() << endl;
//     if(!out_file.is_open())
//     {
//         cout << "error in out_file.open" << endl;
//         return 1;
//     }

//     int val0 = 3;
//     int val1 = 10;
//     vector<int> ch0;
//     ch0.resize(val1);

//     for(int i = 0; i < val1; i++)
//     {
//         ch0[i] = i;
//     }

//     out_file.write( (char *) &val0, sizeof(int) );
//     out_file.write( (char *) &val1, sizeof(int) );
//     out_file.write( (char *) (&ch0[0]), ch0.size() *  sizeof(int) );

//     out_file.close();




     ifstream input_file;
     input_file.open(dir_name.c_str(), ios::binary);
     cout << "input_file.is_open() = " << input_file.is_open() << endl;
     if(!input_file.is_open())
     {
         cout << "error in input_file.open" << endl;
         return 1;
     }

     int value_0 = 0;//is it important?
     int value_1 = 0;


     input_file.read( (char *) &value_0, sizeof(int) );
     input_file.read( (char *) &value_1, sizeof(int) );

     cout << "value_0 = " << value_0 << endl;
     cout << "value_1 = " << value_1 << endl;

     vector<int> ch0_read;
     ch0_read.resize(value_1);

     input_file.read( (char *) (&ch0_read[0]), ch0_read.size() * sizeof(int) );

     for(int i = 0; i < ch0_read.size(); i++)
     {
         cout << ch0_read[i] << endl;
     }


     input_file.close();

     cout << "all is ok" << endl;

     return 0;
}
