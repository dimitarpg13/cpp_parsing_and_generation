Notes on ```std::mem_fun_t```, ```std::mem_fun1_t```, ```std::const_mem_fun_t```, 
```std::const_mem_fun_t```, ```std::mem_fun```, ```std::mem_fun_ref```, 
```boost::mem_fn```, and ```std::mem_fn```

The first six function templates were introduced in C++ 11 and were removed in C++ 17.

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


