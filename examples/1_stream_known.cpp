
#include <iostream>
#include <sstream>
#include <cassert>
#include <tscpp/stream.h>
#include "types.h"

using namespace std;
using namespace tscpp;

int main()
{
    //Serialize to stream
    Point3d p1(1,2,3);
    stringstream ss;
    OutputArchive oa(ss);
    oa<<p1;
    
    //Unserialize from stream
    Point3d p2;
    InputArchive ia(ss);
    ia>>p2;
    assert(p1==p2);
    
    cout<<"Test passed"<<endl;
}
