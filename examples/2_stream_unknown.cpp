
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <tscpp/stream.h>
#include "types.h"

using namespace std;
using namespace tscpp;

int main()
{
    //Declare some types
    Point2d p2d(1,2);
    Point3d p3d(3,4,5);
    MiscData md(p2d,p3d,12,-1);
    
    //Prepare a type pool for unserialization
    TypePoolStream tp;
    tp.registerType<Point2d>([&](Point2d& t)
    {
        cout<<"Found a Point2d"<<endl;
        assert(t==p2d);
    });
    tp.registerType<Point3d>([&](Point3d& t)
    {
        cout<<"Found a Point3d"<<endl;
        assert(t==p3d);
    });
    tp.registerType<MiscData>([&](MiscData& t)
    {
        cout<<"Found a MiscData"<<endl;
        assert(t==md);
    });
    
    //Serialize to stream
    stringstream ss;
    OutputArchive oa(ss);

    oa<<p2d<<p3d<<md;
    
    //Unserialize from stream
    UnknownInputArchive ia(ss,tp);
    ia.unserialize();
    ia.unserialize();
    ia.unserialize();
    
    cout<<"Test passed"<<endl;
}
