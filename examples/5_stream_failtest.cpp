
#include <iostream>
#include <sstream>
#include <cassert>
#include <tscpp/stream.h>
#include "types.h"

using namespace std;
using namespace tscpp;

int main()
{
    //eof during type name in InputArchive
    {
        stringstream ss;
        InputArchive ia(ss);
        Point3d p;
        try {
            ia>>p;
            assert(false);
        } catch(TscppException& ex) {
            assert(string(ex.what())=="eof");
            assert(ex.name()=="");
            assert(p.x==0 && p.y==0 && p.z==0);
        }
    }
    
    //wrong type in InputArchive
    {
        stringstream ss;
        OutputArchive oa(ss);
        Point3d p(1,2,3);
        oa<<p;
        
        InputArchive ia(ss);
        Point2d q;
        try {
            ia>>q;
            assert(false);
        } catch(TscppException& ex) {
            assert(ss.tellg()==0);
            assert(string(ex.what())=="wrong type");
            assert(ex.name()==typeid(p).name());
            assert(q.x==0 && q.y==0);
        }
    }
    
    //eof after type name in InputArchive
    {
        stringstream ss;
        OutputArchive oa(ss);
        Point3d p(1,2,3);
        oa<<p;
        
        string s=ss.str();
        s.pop_back(); //Remove one char
        ss.str(s);
        
        InputArchive ia(ss);
        Point3d q;
        try {
            ia>>q;
            assert(false);
        } catch(TscppException& ex) {
            assert(string(ex.what())=="eof");
            assert(ex.name()=="");
        }
    }
    
    TypePoolStream tp;
    tp.registerType<Point2d>([&](Point2d& t) { assert(false); });
    tp.registerType<Point3d>([&](Point3d& t) { assert(false); });

    //eof during type name in UnknownInputArchive
    {
        
        stringstream ss;
        UnknownInputArchive ia(ss,tp);
        try {
            ia.unserialize();
            assert(false);
        } catch(TscppException& ex) {
            assert(string(ex.what())=="eof");
            assert(ex.name()=="");
        }
    }
    
    //unknown type in UnknownInputArchive
    {
        stringstream ss;
        OutputArchive oa(ss);
        MiscData md;
        oa<<md;

        UnknownInputArchive ia(ss,tp);
        try {
            ia.unserialize();
            assert(false);
        } catch(TscppException& ex) {
            assert(ss.tellg()==0);
            assert(string(ex.what())=="unknown type");
            assert(ex.name()==typeid(md).name());
        }
    }
    
    //eof after type name in UnknownInputArchive
    {
        stringstream ss;
        OutputArchive oa(ss);
        Point3d p(1,2,3);
        oa<<p;
        
        string s=ss.str();
        s.pop_back(); //Remove one char
        ss.str(s);
        
        UnknownInputArchive ia(ss,tp);
        try {
            ia.unserialize();
            assert(false);
        } catch(TscppException& ex) {
            assert(string(ex.what())=="eof");
            assert(ex.name()=="");
        }
    }
    
    cout<<"Test passed"<<endl;
}
