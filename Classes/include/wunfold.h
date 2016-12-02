#ifndef WUNFOLD_H
#define WUNFOLD_H

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class WUnfold
{
public:
    WUnfold();
    WUnfold(const vector<double> &source, const vector<double> &response);
    WUnfold(const vector<float> &source, const vector<float> &response);

    ~WUnfold();

    vector<float> GetUnfold();

private:
    vector<float> response;
    vector<float> source;
    vector<float> unfold_source;
};

#endif // WUNFOLD_H
