#include"vector_2.h"

template <typename T>
void Vector_2<T>::copyfrom(T const* A,Rank lo,Rank hi)
{
    _elem =new T[_capacity=2*(hi-lo)];
    _size=0;
    while(lo<hi)
    _elem[_size++]=A[lo++];
}


template <typename T> void Vector_2<T>::expand()
{
    if(_size<_capacity) return ;
    if(_capacity <CAPACITY)
    _capacity = CAPACITY;
    T* oldelem =_elem; _elem=new T[_capacity<<=1];
    for(int i=0;i<_size;i++)
      _elem[i]=oldelem[i];
    delete[] oldelem;
}

template<typename T> int Vector_2<T>::remove(Rank lo,Rank hi)
{
    if(lo==hi) return 0;
    while(hi<_size) _elem[lo++]=_elem[hi++];
    _size=lo;
    return hi-lo;
}

template<typename T> T Vector_2<T>::remove(Rank r)
{
    T e = _elem[r];
    remove(r,r+1);
    return e;
}

template<typename T>
Rank Vector_2<T>::insert(Rank r,T const& e)
{
    expand();
    for(int i=_size;i>r;i--)
    {
        _elem[i]=_elem[i-1];
    }
    _elem[r]=e;
    _size++;
    return r;
}

template <typename T> void Vector_2<T>::traverse(void (*visit )(T&))
{
    for(int i=0;i<_size;i++) visit(_elem[i]);
}

template <typename T> template <typename VST>
void Vector_2<T>::traverse(VST& visit)
{
    for(int i=0;i<_size;i++) visit(_elem[i]);
}