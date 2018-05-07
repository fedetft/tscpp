
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <tscpp/buffer.h>
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
    TypePoolBuffer tp;
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
    
    //Serialize to buffer
    char buffer[1024];
    int writeSize=0;

    int serializedSize1=serialize(buffer+writeSize,sizeof(buffer)-writeSize,p2d);
    assert(serializedSize1>0);
    writeSize+=serializedSize1;
    
    int serializedSize2=serialize(buffer+writeSize,sizeof(buffer)-writeSize,p3d);
    assert(serializedSize2>0);
    writeSize+=serializedSize2;
    
    int serializedSize3=serialize(buffer+writeSize,sizeof(buffer)-writeSize,md);
    assert(serializedSize3>0);
    writeSize+=serializedSize3;
    
    //Unserialize from buffer
    int readSize=0;

    int unserializedSize1=unserializeUnknown(tp,buffer+readSize,writeSize-readSize);
    assert(unserializedSize1>0);
    assert(unserializedSize1==serializedSize1);
    readSize+=unserializedSize1;
    
    int unserializedSize2=unserializeUnknown(tp,buffer+readSize,writeSize-readSize);
    assert(unserializedSize2>0);
    assert(unserializedSize2==serializedSize2);
    readSize+=unserializedSize2;
    
    int unserializedSize3=unserializeUnknown(tp,buffer+readSize,writeSize-readSize);
    assert(unserializedSize3>0);
    assert(unserializedSize3==serializedSize3);
    readSize+=unserializedSize3;
    
    assert(readSize==writeSize);
    
    cout<<"Test passed"<<endl;
}
