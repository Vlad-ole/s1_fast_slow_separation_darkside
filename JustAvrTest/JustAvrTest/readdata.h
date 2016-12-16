#ifndef READDATA_H
#define READDATA_H

#include <iostream>
#include <fstream>
#include <vector>

class ReadData
{
public:
    ReadData(const int file_number);
    ~ReadData();
    const std::vector< std::vector<int> > GetDataInt() const;
    const bool GetStatusIsOk() const;
private:
    std::vector< std::vector<int> > yv;
    std::ifstream input_file;
    bool is_ok;
};

#endif // READDATA_H
