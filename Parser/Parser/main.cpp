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

struct event_header_v0{
    uint32_t event_size;
    uint32_t event_id;
    uint32_t timestamp;
};

struct datablock{
    uint32_t datasize;
    uint32_t type;
    std::vector<unsigned char> data;
    datablock(uint32_t Type, uint32_t Size=0) : datasize(Size), type(Type)
    {data.resize(Size); }
};

struct datablock_header{
  uint32_t total_blocksize_disk;
  uint32_t datasize;
  uint32_t type;
};

/// //////////////////////////////////
/// \brief The RawEvent class
//header
class RawEvent
{
public:
    /// constructor, automatically increments event_id counter by default
    RawEvent(bool increment_id_counter = true);

    /// Add new block at end of event, return index of that block (-1 if error)
    int AddDataBlock(uint32_t blocktype, uint32_t datasize=0);

    /// Get a pointer to the block of raw data number block_n
    unsigned char* GetRawDataBlock(size_t block_n);

    std::vector<datablock> _datablocks;
    uint32_t _buffer_size;

    static long _total_buffer_size;
    static uint32_t _event_count;
};


//cpp

//initialize all the statics
long RawEvent::_total_buffer_size = 0;
uint32_t RawEvent::_event_count = 0;


RawEvent::RawEvent(bool increment_id_counter)
{

}

int RawEvent::AddDataBlock(uint32_t blocktype, uint32_t datasize)
{
    _datablocks.push_back(datablock(blocktype, datasize));
    _buffer_size += datasize;
    _total_buffer_size += datasize;
    return _datablocks.size() - 1;
}


unsigned char* RawEvent::GetRawDataBlock(size_t block_n)
{
    return &(_datablocks.at(block_n).data[0]);
}


/// //////////////////////////////////


/// Search for an event with internal id <id>
//RawEventPtr GetEventWithID(uint32_t id);









int main(int argc, char *argv[])
{


    const string filename = "/home/darkside/Vlad_Programs/Run006000/Run006000.003.out";
    //FILE* _fin;
    //_fin = fopen(filename.c_str(),"rb");

    gzFile _fin; ///< gzip file that we are reading from
    _fin = gzopen(filename.c_str(),"rb");

    global_header _ghead; ///< header for the current file

    //fread(&_ghead, sizeof(_ghead), 1, _fin);

    //read in the global file header
    //assume we're using the latest header, then check to make sure
    gzread(_fin, &_ghead, _ghead.global_header_size);



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


//    {
//        //work
//        RawEvent next(new RawEvent);
//        event_header _ehead; ///< header for the current event

//        ////event data in this generation of file is not internally zipped
//        ////consists only of V172X blocks after the legacy header
//        uint32_t evsize = _ehead.event_size - sizeof(event_header_v0);
//        int blockn = next.AddDataBlock(0, evsize);
//        //int bytes_read = gzread(_fin, next->GetRawDataBlock(blockn), evsize);
//        //fread(tmp, evsize, 1, _fin);
//        int bytes_read = fread(next.GetRawDataBlock(blockn), evsize, 1, _fin);
//    }



    {
        //this event structure has individually zipped data blocks
        datablock_header bh;
        uint32_t thisblock = 0;
        int bytes_read = 0;

//        while(thisblock < _ehead.nblocks && (uint32_t)bytes_read<_ehead.event_size-sizeof(event_header))
//        {
//            int head_read = gzread(_fin, &bh, sizeof(bh));

//            if(ErrorCheck(head_read, sizeof(bh)))
//                return RawEventPtr();

//            //create datablock in the raw event
//            int blockn = next->AddDataBlock(bh.type, bh.datasize);

//            //read the compressed block into a temporary buffer
//            std::vector<char> buf(bh.total_blocksize_disk);
//            int block_read = gzread(_fin, &(buf[0]), bh.total_blocksize_disk-sizeof(bh));

//            if(ErrorCheck(block_read, bh.total_blocksize_disk-sizeof(bh))){
//                Message(ERROR)<<"Incorrect blocksize for block "<<thisblock<<" in event "
//                             <<_ehead.event_id;
//                return RawEventPtr();
//            }

//            //unzip the buffer into the RawEvent
//            uLongf decomp = bh.datasize;
//            int err = uncompress(next->GetRawDataBlock(blockn), &decomp, (Bytef*)(&(buf[0])), block_read);

//            if(err != Z_OK)
//            {
//                Message(ERROR)<<"uncompress function returned "<< err <<" while reading event!\n";
//                return RawEventPtr();
//            }

//            next->SetDataBlockSize(blockn,decomp);

//            //done with this block
//            thisblock++;
//            bytes_read += head_read + block_read;
//        }

//        //make sure everything got read
//        if((uint32_t)bytes_read != _ehead.event_size-sizeof(event_header) || thisblock != _ehead.nblocks)
//        {
//            Message(ERROR)<<"The event with id "<<_ehead.event_id << " was not fully read out!\n";
//            return RawEventPtr();
//        }
    }












    cout << endl << "all is ok" << endl;

    return 0;
}
