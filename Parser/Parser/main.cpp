#include <QCoreApplication>

#include <iostream>
#include <string>
#include <fstream>

#include <zlib.h>

using namespace std;

static const uint32_t magic_number = 0xdec0ded1;
static const uint32_t latest_global_version = 1;
static const uint32_t latest_event_version = 1;

struct event_header{
  uint32_t event_size;
  uint32_t event_id;
  uint32_t timestamp;
  uint32_t nblocks;
  event_header() : event_size(sizeof(event_header)){}
  void reset(){ event_size=0; event_id=0; timestamp=0; nblocks=0; }
};

struct global_header{
  uint32_t magic_num_check;
  uint32_t global_header_size;
  uint32_t global_header_version;
  uint32_t event_header_size;
  uint32_t event_header_version;
  uint32_t file_size;
  uint32_t start_time;
  uint32_t end_time;
  uint32_t run_id;
  uint32_t file_index;
  uint32_t nevents;
  uint32_t event_id_min;
  uint32_t event_id_max;
  global_header() : magic_num_check(magic_number),
            global_header_size(sizeof(global_header)),
            global_header_version(1),
            event_header_size(sizeof(event_header)),
            event_header_version(1) {}

};



int main(int argc, char *argv[])
{

    FILE* _fin;
    const string filename = "/home/darkside/Vlad_Programs/Run006000/Run006000.001.out";
    _fin = fopen(filename.c_str(),"rb");

    global_header _ghead; ///< header for the current file

    //read(_fin, &_ghead, _ghead.global_header_size);
    //fread(&a, sizeof(a), 1, _fin);
    fread(&_ghead, sizeof(_ghead), 1, _fin);

    cout << "magic_num_chec = " << _ghead.magic_num_check << endl;
    cout << "global_header_size = " << _ghead.global_header_size << endl;
    cout << "global_header_version = " << _ghead.global_header_version << endl;
    cout << "event_header_size = " << _ghead.event_header_size << endl;
    cout << "event_header_version = " << _ghead.event_header_version << endl;
    cout << "file_size = " << _ghead.file_size << endl;
    cout << "start_time = " << _ghead.start_time << endl;
    cout << "end_time = " << _ghead.end_time << endl;
    cout << "run_id = " << _ghead.run_id << endl;
    cout << "file_index = " << _ghead.file_index << endl;
    cout << "nevents = " << _ghead.nevents << endl;
    cout << "event_id_min = " << _ghead.event_id_min << endl;
    cout << "event_id_max = " << _ghead.event_id_max << endl;


    //cout << " = " << _ghead. << endl;

    cout << "all is ok" << endl;

    return 0;
}
