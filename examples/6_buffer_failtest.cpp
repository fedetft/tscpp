
#include <iostream>
#include <sstream>
#include <cassert>
#include <tscpp/buffer.h>
#include "types.h"

using namespace std;
using namespace tscpp;

int main()
{
    //BufferTooSmall while serialize
    {
        Point3d p1(1,2,3);
        char buffer[10];
        assert(serialize(buffer,sizeof(buffer),p1)==BufferTooSmall);
    }
    
    //BufferTooSmall while unserialize
    {
        Point3d p1(1,2,3);
        char buffer[64];
        int serializedSize=serialize(buffer,sizeof(buffer),p1);
        assert(serializedSize>0);
        
        Point3d p2;
        assert(unserialize(p2,buffer,serializedSize-1)==BufferTooSmall);
        assert(p2.x==0 && p2.y==0 && p2.z==0);
    }
    
    //WrongType while unserialize
    {
        Point3d p(1,2,3);
        char buffer[64];
        int serializedSize=serialize(buffer,sizeof(buffer),p);
        assert(serializedSize>0);
        
        Point2d q;
        assert(unserialize(q,buffer,serializedSize)==WrongType);
        assert(q.x==0 && q.y==0);
    }
    
    TypePoolBuffer tp;
    tp.registerType<Point2d>([&](Point2d& t) { assert(false); });
    tp.registerType<Point3d>([&](Point3d& t) { assert(false); });

    {
        Point3d p(1,2,3);
        char buffer[64];
        int serializedSize=serialize(buffer,sizeof(buffer),p);
        assert(serializedSize>0);

        //BufferTooSmall while unserializeUnknown (name)
        assert(unserializeUnknown(tp,buffer,1)==BufferTooSmall);
        
        //BufferTooSmall while unserializeUnknown (type)
        assert(unserializeUnknown(tp,buffer,serializedSize-1)==BufferTooSmall);
    }
    
    //UnknownType while unserializeUnknown
    {
        MiscData md;
        char buffer[1024];
        int serializedSize=serialize(buffer,sizeof(buffer),md);
        assert(serializedSize>0);

        assert(unserializeUnknown(tp,buffer,serializedSize)==UnknownType);
    }
    
    cout<<"Test passed"<<endl;
}
