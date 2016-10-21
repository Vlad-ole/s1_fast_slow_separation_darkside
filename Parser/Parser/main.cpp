#include <QCoreApplication>

#include <iostream>
#include <string>
#include <fstream>

#include <zlib.h>

#include "RawEvent.hh"
#include "Reader.hh"

using namespace std;



//int main(int argc, char *argv[])
//{

//    const string filename = "/home/darkside/Vlad_Programs/Run006000/Run006000.005.out";
//    //FILE* _fin;
//    //_fin = fopen(filename.c_str(),"rb");

//    gzFile _fin; ///< gzip file that we are reading from
//    _fin = gzopen(filename.c_str(),"rb");

//    global_header _ghead; ///< header for the current file

//    //fread(&_ghead, sizeof(_ghead), 1, _fin);

//    //read in the global file header
//    //assume we're using the latest header, then check to make sure
//    gzread(_fin, &_ghead, _ghead.global_header_size);



//    cout << "magic_num_chec = " << _ghead.magic_num_check << endl;
//    cout << "global_header_size = " << _ghead.global_header_size << endl;
//    cout << "global_header_version = " << _ghead.global_header_version << endl;
//    cout << "event_header_size = " << _ghead.event_header_size << endl;
//    cout << "event_header_version = " << _ghead.event_header_version << endl;
//    cout << "file_size = " << _ghead.file_size << endl;
//    cout << "start_time = " << _ghead.start_time << endl;
//    cout << "end_time = " << _ghead.end_time << endl;
//    cout << "run_id = " << _ghead.run_id << endl;
//    cout << "file_index = " << _ghead.file_index << endl;
//    cout << "nevents = " << _ghead.nevents << endl;
//    cout << "event_id_min = " << _ghead.event_id_min << endl;
//    cout << "event_id_max = " << _ghead.event_id_max << endl;

//    event_header _ehead; ///< header for the current event

//    RawEvent next(new RawEvent);

//    //    {
//    //        //work
//    //        ;
//    //        event_header _ehead; ///< header for the current event

//    //        ////event data in this generation of file is not internally zipped
//    //        ////consists only of V172X blocks after the legacy header
//    //        uint32_t evsize = _ehead.event_size - sizeof(event_header_v0);
//    //        int blockn = next.AddDataBlock(0, evsize);
//    //        //int bytes_read = gzread(_fin, next->GetRawDataBlock(blockn), evsize);
//    //        //fread(tmp, evsize, 1, _fin);
//    //        int bytes_read = fread(next.GetRawDataBlock(blockn), evsize, 1, _fin);
//    //    }



//    /////////////////////////////////

//        //this event structure has individually zipped data blocks
//        datablock_header bh;
//        uint32_t thisblock = 0;
//        int bytes_read = 0;



////        while(thisblock < _ehead.nblocks && (uint32_t)bytes_read<_ehead.event_size-sizeof(event_header))
//        {
////            cout << endl <<"line 215" << endl;

//            int head_read = gzread(_fin, &bh, sizeof(bh));

//            //            if(ErrorCheck(head_read, sizeof(bh)))
//            //                return RawEventPtr();

//            cout << endl << "datablock_header.datasize = " << bh.datasize << endl;
//            cout << "datablock_header.total_blocksize_disk = " << bh.total_blocksize_disk << endl;
//            cout << "datablock_header.type = " << bh.type << endl;
//            cout<< endl;

//            //create datablock in the raw event
//            int blockn = next.AddDataBlock(bh.type, bh.datasize);

//            //read the compressed block into a temporary buffer
//            std::vector<char> buf(bh.total_blocksize_disk);
//            int block_read = gzread(_fin, &(buf[0]), bh.total_blocksize_disk-sizeof(bh));

//            //            if(ErrorCheck(block_read, bh.total_blocksize_disk-sizeof(bh))){
//            //                Message(ERROR)<<"Incorrect blocksize for block "<<thisblock<<" in event "
//            //                             <<_ehead.event_id;
//            //                return RawEventPtr();
//            //            }

//            //unzip the buffer into the RawEvent
//            uLongf decomp = bh.datasize;
//            int err = uncompress(next.GetRawDataBlock(blockn), &decomp, (Bytef*)(&(buf[0])), block_read);

//            if(err != Z_OK)
//            {
//                //                        Message(ERROR)<<"uncompress function returned "<< err <<" while reading event!\n";
//                //                return RawEventPtr();
//            }



//            cout << endl;
//            cout << "next._datablocks.size() = " << next._datablocks.size() << endl;
//            cout << "next._datablocks[0].data.size() = " << next._datablocks[0].data.size() << endl;

//            for(int i = 0; i < 5000; i+=100)
//            {
//                cout << "next._datablocks.at(0).data[" << i << "] = " << (int)(next._datablocks.at(0).data[i]) << endl;
//            }

//            next.SetDataBlockSize(blockn,decomp);

//            cout << "thisblock = " << thisblock << endl;

//            //done with this block
//            thisblock++;
//            bytes_read += head_read + block_read;


//        }

//        //        //make sure everything got read
////                if((uint32_t)bytes_read != _ehead.event_size-sizeof(event_header) || thisblock != _ehead.nblocks)
////                {
////                    Message(ERROR)<<"The event with id "<<_ehead.event_id << " was not fully read out!\n";
////                    cout << "The event with id "<<_ehead.event_id << " was not fully read out!\n";
////                    return RawEvent();
////                }


////        //if we get here everything seems ok
////        next.SetID(_ehead.event_id);
////        next.SetTimestamp(_ehead.timestamp);
////        if(_ghead.global_header_version > 0)
////          next.SetRunID(_ghead.run_id);

////        _current_index++;
////        _current_event = next;


//           /////////////////////////////////












//    cout << endl << "all is ok" << endl;

//    return 0;
//}

int main(int argc, char *argv[])
{
//    Reader reader(fname.c_str());
    cout << endl << "all is ok" << endl;

    return 0;
}
