#pragma once

namespace CEGUI
{
//仅仅包装了下，区别相同的类型参数
template<typename T>
struct FunctorReferenceBinder
{
    FunctorReferenceBinder(T& functor) :
        d_functor(functor)
    {}

    T& d_functor;
};

}