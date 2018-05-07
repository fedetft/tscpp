/***************************************************************************
 *   Copyright (C) 2018 by Terraneo Federico                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include "buffer.h"

using namespace std;

namespace tscpp {

int TypePoolBuffer::unserializeUnknownImpl(const char *name, const void *buffer, int bufSize) const
{
    auto it=types.find(name);
    if(it==types.end()) return UnknownType;

    if(it->second.size>bufSize) return BufferTooSmall;

    it->second.usc(buffer);
    return it->second.size;
}

int serializeImpl(void *buffer, int bufSize, const char *name, const void *data, int size)
{
    int nameSize=strlen(name);
    int serializedSize=nameSize+1+size;
    if(serializedSize>bufSize) return BufferTooSmall;

    char *buf=reinterpret_cast<char*>(buffer);
    memcpy(buf,name,nameSize+1); //Copy also the \0
    memcpy(buf+nameSize+1,data,size);
    return serializedSize;
}

int unserializeImpl(const char *name, void *data, int size, const void *buffer, int bufSize)
{
    int nameSize=strlen(name);
    int serializedSize=nameSize+1+size;
    if(serializedSize>bufSize) return BufferTooSmall;

    const char *buf=reinterpret_cast<const char*>(buffer);
    if(memcmp(buf,name,nameSize+1)) return WrongType;

    //NOTE: we are writing on top of a constructed type without calling its
    //destructor. However, since it is trivially copyable, we at least aren't
    //overwriting pointers to allocated memory.
    memcpy(data,buf+nameSize+1,size);
    return serializedSize;
}

int unserializeUnknown(const TypePoolBuffer& tp, const void *buffer, int bufSize)
{
    const char *buf=reinterpret_cast<const char*>(buffer);
    int nameSize=strnlen(buf,bufSize);
    if(nameSize>=bufSize) return BufferTooSmall;

    const char *name=buf;
    buf+=nameSize+1;
    bufSize-=nameSize+1;
    auto result=tp.unserializeUnknownImpl(name,buf,bufSize);
    if(result<0) return result;
    return result+nameSize+1;
}

string peekTypeName(const void *buffer, int bufSize)
{
    const char *buf=reinterpret_cast<const char*>(buffer);
    int nameSize=strnlen(buf,bufSize);
    if(nameSize>=bufSize) return "";
    return buf;
}

} //namespace tscpp
