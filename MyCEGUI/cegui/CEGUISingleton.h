#pragma once

#include "CEGUIBase.h"
#include <cassert>

namespace CEGUI
{

template <typename T> class Singleton
{
protected:
    static T* ms_Singleton;

public:
    Singleton( void )
    {
        assert( !ms_Singleton );
        ms_Singleton = static_cast<T*>(this);  //对象被创建时指向自己
    }
   ~Singleton( void )
        {  assert( ms_Singleton );  ms_Singleton = 0;  }
    static T& getSingleton( void )
        {  assert( ms_Singleton );  return ( *ms_Singleton );  }
    static T* getSingletonPtr( void )
        {  return ( ms_Singleton );  }

private:
    Singleton& operator=(const Singleton&) { return this; }
    Singleton(const Singleton&) {}
};

}