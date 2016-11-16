#pragma once

namespace CEGUI
{

//具有引用计数的封装类
template<typename T>
class RefCounted
{
public:
    RefCounted() :
        d_object(0),
        d_count(0)
    {
    }

    RefCounted(T* ob) :
        d_object(ob),
        d_count((ob != 0) ? new unsigned int(1) : 0)
    {
    }

    RefCounted(const RefCounted<T>& other) :
        d_object(other.d_object),
        d_count(other.d_count)
    {
        if (d_count)
            addRef();
    }

    ~RefCounted()
    {
        if (d_object)
            release();
    }

    RefCounted<T>& operator=(const RefCounted<T>& other)
    {
        if (*this != other)       //先判断是否是自身
        {
            if (d_object)         //释放以前所关联的对象
                release();

            d_object = other.d_object;
            d_count = d_object ? other.d_count : 0;

            if (d_count)
                addRef();
        }
        return *this;
    }

    //所绑定的对象是否相等
    bool operator==(const RefCounted<T>& other) const
    {
        return d_object == other.d_object;
    }

    bool operator!=(const RefCounted<T>& other) const
    {
        return d_object != other.d_object;
    }

    //解引用操作是对所绑定对象的解引用操作
    const T& operator*() const
    {
        return *d_object;
    }

	//非const对象的解引用操作
    T& operator*()
    {
        return *d_object;
    }

    //->操作是对所绑定对象的操作
    const T* operator->() const
    {
        return d_object;
    }

	//非const对象
    T* operator->()
    {
        return d_object;
    }

    //是否合法，即是否绑定了有效对象
    bool isValid() const
    {
        return d_object != 0;
    }

private:
    void addRef()
    {
        ++*d_count;
    }
    void release()
    {
        if (!--*d_count)
        {
            delete d_object;
            delete d_count;
            d_object = 0;
            d_count = 0;
        }
    }

    T* d_object;            //! pointer to the object.
    unsigned int* d_count;  //! pointer to the shared counter object.
};

}