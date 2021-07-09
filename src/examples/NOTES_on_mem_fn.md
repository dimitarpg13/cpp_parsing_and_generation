Notes on ```std::mem_fun_t```, ```std::mem_fun1_t```, ```std::const_mem_fun_t```, 
```std::const_mem_fun_t```, ```std::mem_fun```, ```std::mem_fun_ref```, 
```boost::mem_fn```, and ```std::mem_fn```

The first six function templates were introduced in C++ 11 and were removed in C++ 17.

```std::mem_fun_t```, ```std::mem_fun1_t```, ```std::const_mem_fun_t```, ```std::const_mem_fun1_t```
wrapper around a member function pointer. The class instance whose member function to
call is passed as a pointer to the operator. 
```cpp
template< class S, class T > 
class mem_fun_ : public unary_function<T*, S> {
public:
   explicit mem_fun_t(S (T::*p)());
   S operator()(T* p) const;
};
```
wraps a non-const member function with no parameters.

```cpp
template< class S, class T >
class const_mem_fun_t : public unary_function<const T*,S> {
public:
   explicit const_mem_fun_t(S (T::*p)() const);
   S operator()(const T* p) const;
};
```
wraps a const member function with no parameters.

```cpp
template< class S, class T, class A >
class mem_fun1_t : public binary_function<T*,A,S> {
public:
   explicit mem_fun1_t(S (T::*p)(A));
   S operator()(T* p, A x) const;
}
```
wraps a non-const member function with a single parameter.

```cpp
template< class S, class T, class A >
class const_mem_fun1_t : public binary_function<const T*,A,S> {
public:
   ex
}
```


```std::mem_fun```
creates a member function wrapper object, deducing the target type from the template
arguments. The wrapper object expects a pointer to an object of type T as the first
parameter to its ```operator()```.
We have the following signatures:
```cpp
template < class Res, class T >
std::mem_fun_t<Res, T> mem_fun( Res (T::*f)() );
``` 
which effectively 


```boost::mem_fn``` 
```boost::mem_fn``` is a generalization of the standard functions ```std::mem_fun``` and 
```std::mem_fun_ref```. It supports member function pointers with more than one argument, and
the returned function object can take a pointer, a reference, or a smart pointer to an object
instance as its first argument. ```mem_fn``` also supports pointers to data members by treating
them as functions taking no arguments and returning a (const) reference to the member.


