#pragma once

namespace CEGUI
{
//������װ���£�������ͬ�����Ͳ���
template<typename T>
struct FunctorReferenceBinder
{
    FunctorReferenceBinder(T& functor) :
        d_functor(functor)
    {}

    T& d_functor;
};

}