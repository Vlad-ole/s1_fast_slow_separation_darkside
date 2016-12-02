#include "wunfold.h"

WUnfold::WUnfold()
{
    cout << "constructor WUnfold()" << endl;
}

WUnfold::~WUnfold()
{
    cout << "destructor WUnfold()" << endl;
}

vector<float> WUnfold::GetUnfold()
{
//    unfold_source = Get_ifft( G() );


    return unfold_source;
}

WUnfold::WUnfold(const vector<float> &source, const vector<float> &response)
{
    this->response = response;
    this->source = source;
}
