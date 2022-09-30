typedef int Rank;
#define CAPACITY 10

template <typename T> class Vector_2{
  protected:
  Rank _size; int _capacity; T* _elem;
  void copyfrom(T const * A,Rank lo,Rank hi);
  void expand();
  
  public:
  Vector_2(int c=CAPACITY,int s=0,T v=0)
  {
    _elem=new T[_capacity=c];
    for(_size=0;_size<s;_elem[_size++]=v);
  }
  Vector_2(T const* A,Rank n)
  {
    copyform(A,0,n);
  }
  Vector_2(T const* A,Rank lo,Rank hi)
  {
    copyform(A,lo,hi);
  }

  ~Vector_2()
  {
    delete[] _elem;
  }

  Rank size() const {return _size;}
  bool empty() const {return !_size;}

  T& operator[] (Rank r) const
  {
    return _elem[r];
  }

  Vector_2<T> & operator= (Vector_2<T> const& V)
  {
    if(_elem) delete[] _elem;
    copyfrom(V._elem,0,V.size());
    return *this;
  }

  T remove (Rank r);
  int remove(Rank lo,Rank hi);
  Rank insert(Rank r,T const& e);

  Rank insert(T const& e)
  {
    return insert(_size,e);
  }
  
  void traverse (void (* ) (T&));
  template <typename VST> void traverse(VST&);
};
