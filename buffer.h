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

#pragma once

#include <type_traits>
#include <functional>
#include <cstring>
#include <string>
#include <map>

/**
 * \file buffer.h
 * TSCPP buffer API. This file contains functions to serialize types to raw
 * memory buffers. These classes provide a low level API to serialization.
 * Error reporting is performed through return codes.
 * 
 * NOTE: the serialization format between the buffer and stream API is
 * interchangeable, so you can for example serialize using the buffer API and
 * unserialize using the stream API.
 */

namespace tscpp {

/**
 * Error codes returned by the buffer API of tscpp
 */
enum TscppError
{
    BufferTooSmall = -1, ///< Buffer is too small for the given type
    WrongType      = -2, ///< While unserializing a different type was found
    UnknownType    = -3  ///< While unserializing the type was not found in the pool
};

/**
 * Type pool for the TSCPP buffer API.
 * A type pool is a class where you can register types and associate callbacks
 * to them. It is used to unserialize types when you don't know the exact type
 * or order in which types have been serialized.
 */
class TypePoolBuffer
{
public:
    /**
     * Register a type and the associated callback
     * \tparam T type to be registered
     * \param callback callabck to be called whan the given type is unserialized
     */
    template<typename T>
    void registerType(std::function<void (T& t)> callback)
    {
        #ifndef _MIOSIX
        static_assert(std::is_trivially_copyable<T>::value,"Type is not trivially copyable");
        #endif
        types[typeid(T).name()]=UnserializerImpl(sizeof(T),[=](const void *buffer) {
            //NOTE: We copy the buffer to respect alignment requirements.
            //The buffer may not be suitably aligned for the unserialized type
            //TODO: support classes without default constructor
            //NOTE: we are writing on top of a constructed type without callingits
            //destructor. However, since it is trivially copyable, we at least aren't
            //overwriting pointers to allocated memory.
            T t;
            memcpy(&t,buffer,sizeof(T));
            callback(t);
        });
    }

    /**
     * \internal
     */
    int unserializeUnknownImpl(const char *name, const void *buffer, int bufSize) const;
    
private:
    /**
     * \internal
     */
    class UnserializerImpl
    {
    public:
        UnserializerImpl() : size(0) {}
        UnserializerImpl(int size, std::function<void (const void*)> usc) : size(size), usc(usc) {}
        int size;
        std::function<void (const void*)> usc;
    };

    std::map<std::string,UnserializerImpl> types; ///< Registered types
};

/**
 * \internal
 */
int serializeImpl(void *buffer, int bufSize, const char *name, const void *data, int size);

/**
 * Serialize a type to a memory buffer
 * \param buffer ponter to the memory buffer where to serialize the type
 * \param bufSize buffer size
 * \param t type to serialize
 * \return the size of the serialized type (which is larger than sizeof(T) due
 * to serialization overhead), or TscppError::BufferTooSmall if the given
 * buffer is too small
 */
template<typename T>
int serialize(void *buffer, int bufSize, const T& t)
{
    #ifndef _MIOSIX
    static_assert(std::is_trivially_copyable<T>::value,"Type is not trivially copyable");
    #endif
    return serializeImpl(buffer,bufSize,typeid(t).name(),&t,sizeof(t));
}

/**
 * \internal
 */
int unserializeImpl(const char *name, void *data, int size, const void *buffer, int bufSize);

/**
 * Unserialize a known type from a memory buffer
 * \param t type to unserialize
 * \param buffer pointer to buffer where the serialized type is
 * \param bufSize buffer size
 * \return the size of the unserialized type (which is larger than sizeof(T) due
 * to serialization overhead), or TscppError::WrongType if the buffer does
 * not contain the given type or TscppError::BufferTooSmall if the type is
 * truncated, i.e the buffer is smaller tah the serialized type size
 */
template<typename T>
int unserialize(T& t, const void *buffer, int bufSize)
{
    #ifndef _MIOSIX
    static_assert(std::is_trivially_copyable<T>::value,"Type is not trivially copyable");
    #endif
    return unserializeImpl(typeid(t).name(),&t,sizeof(t),buffer,bufSize);
}

/**
 * Unserialize an unknown type from a memory buffer
 * \param tp type pool where possible serialized types are registered
 * \param buffer pointer to buffer where the serialized type is
 * \param bufSize buffer size
 * \return the size of the unserialized type (which is larger than sizeof(T) due
 * to serialization overhead), or TscppError::UnknownType if the pool does
 * not contain the type found in the buffer or TscppError::BufferTooSmall if the 
 * type is truncated, i.e the buffer is smaller tah the serialized type size
 */
int unserializeUnknown(const TypePoolBuffer& tp, const void *buffer, int bufSize);

/**
 * Given a buffer where a type has been serialized, return the C++ mangled
 * name of the serialized type.
 * It is useful when unserialize returns TscppError::WrongType to print an
 * error message with the name of the type in the buffer
 * \code
 * Foo f;
 * auto result=unserialize(f,buffer,size);
 * if(result==WrongType)
 * {
 *     cerr<<"While unserializing Foo, "<<demangle(peekTypeName(buffer,size))<<" was found\n";
 * }
 * \endcode
 * \param buffer pointer to buffer where the serialized type is
 * \param bufSize buffer size
 * \return the serialized type name, or "" if the buffer does not contain a type
 * name
 */
std::string peekTypeName(const void *buffer, int bufSize);

} // namespace tscpp
