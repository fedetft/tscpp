
#include <iostream>
#include <cassert>
#include <tscpp/buffer.h>
#include "types.h"

using namespace std;
using namespace tscpp;

int main()
{
    //Serialize to buffer
    Point3d p1(1,2,3);
    char buffer[64];
    int serializedSize=serialize(buffer,sizeof(buffer),p1);
    assert(serializedSize>0);
    
    //Unserialize from buffer
    Point3d p2;
    int unserializedSize=unserialize(p2,buffer,serializedSize);
    assert(unserializedSize>0);
    assert(unserializedSize==serializedSize);
    assert(p1==p2);
    
    cout<<"Test passed"<<endl;
}
