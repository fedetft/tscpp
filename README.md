Trivial serialization for C++
=============================

As you known, serialization is not supported by standard C++.
For this reason, many libraries have been made, such as cereal,
boost::serialization, flatbuffers, protocol buffers and many more to overcome
this limitation.
These libraries support the general case of serializing any type
in a portable way but all require some help from the developer.
This can range from adding a member function to serialize/unserialize the
type, tagging types and/or fields with their name, all the way up to
learning an interface definition language and dealing with a code generator.

But what if we just need to serialize some simple object? Is this boilerplate
code really needed?

TSCPP is a simple serialization library limited to flat types, that do not
contain pointers, references or virtual functions.
By restricting to this case it is possible to make a really simple
serialization library that just works, serializing objects with
zero boilerplate code, and is fast too.

## A code example

Here's an example showing how to serialize/unserialize using the TSCPP
stream API.

```C++
#include <fstream>
#include <tscpp/stream.h>

using namespace std;
using namespace tscpp;

class Bar
{
public:
    Bar(int x, int y) : x(x), y(y) {}
    int x,y;
};

class Foo
{
public:
    Foo() : b(-1,1)
    {
        for(int i=0;i<10;i++) z[i]=i;
    }
    Bar b;
    int z[10];
};

int main()
{
    Foo foo;
    
    ofstream os("serialized.dat",ios::binary);
    OutputArchive oa(os);

    oa<<foo; //How to serialize an object
    
    os.close();
    ifstream is("serialized.dat",ios::binary);
    InputArchive ia(is);
    
    Foo foo2;
    ia>>foo2; //How to unserialize an object
}
```

TSCPP also supports a buffer API to serialize to raw memory buffers instead
of std streams. Moreover, an unique TypePool concept allows to register
callbacks associated to individual types and unserialize files where the
order in which objects have been serialized is unknown.

## How does it work

TSCPP starts from the C tradition of writing raw structs to a file, or to
memcpy them to a memory buffer. To this it adds C++ RTTI support in order to
perform type checking when unserializing.

The serialization format is simply the C++ mangled name of the type to be
serialized (as returned by typeid(type).name()), followed by a '\0' string
terminator, followed by the object itself.

![Memory layout](https://github.com/fedetft/tscpp/raw/master/.readme1.png)

## What are the limitations

* Only objects with a flat memory layout, i.e. without pointers, references, virtual functions can be serialized
* The serialization format is not portable between different machines if
  * the endianness of the two machines differ
  * the C++ name mangling scheme differs (e.g: Windows has its own incompatible name mangling scheme)
  * the padding of fields differs
*  Object versioning is not supported, trying to unserialize a previous version of an object will result in wrong bits in its fields
