#pragma once

namespace CEGUI
{

//�������ü����ķ�װ��
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
        if (*this != other)       //���ж��Ƿ�������
        {
            if (d_object)         //�ͷ���ǰ�������Ķ���
                release();

            d_object = other.d_object;
            d_count = d_object ? other.d_count : 0;

            if (d_count)
                addRef();
        }
        return *this;
    }

    //���󶨵Ķ����Ƿ����
    bool operator==(const RefCounted<T>& other) const
    {
        return d_object == other.d_object;
    }

    bool operator!=(const RefCounted<T>& other) const
    {
        return d_object != other.d_object;
    }

    //�����ò����Ƕ����󶨶���Ľ����ò���
    const T& operator*() const
    {
        return *d_object;
    }

	//��const����Ľ����ò���
    T& operator*()
    {
        return *d_object;
    }

    //->�����Ƕ����󶨶���Ĳ���
    const T* operator->() const
    {
        return d_object;
    }

	//��const����
    T* operator->()
    {
        return d_object;
    }

    //�Ƿ�Ϸ������Ƿ������Ч����
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