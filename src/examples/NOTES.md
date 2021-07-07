Note on ```std::binder1st```, ```std::binder2nd```, ```std::bind1st```, ```std::bind2nd```, 
```boost::bind```, ```std::bind```, and ```std::bind_front```

The first 5 function templates (```std::binder1st```, ```std::binder2nd```, ```std::bind1st```,
```std::bind2nd```, ```boost::bind```) were deprecated in C++ 11 and were removed in C++17.
The last function template (```std::bind_front```) was introduced in C++20.

1. ```std::binder1st``` and ```std::binder2nd```
These are templetized function objects which bind an argument to a binary function.
The value of the parameter is passed to the object at the construction time and
stored within the object. Whenever the function object is invoked through `operator()`, the 
stored value is passed as one of the arguments, the other argument is passed as an argument
of ```operator()```. The resulting function object is an unary function.
  ```binder1st``` binds the first parameter to the value given at the construction of the
object. ```binder2nd``` binds the second parameter to the value giveb at the construction
of the object. 
 
```cpp
template < class Fn >
class binder1st : public std::unary_function<typename Fn::second_argument_type,
                                             typename Fn::result_type> {
protected:
    Fn op;
    typename Fn::first_argument_type value;

public:
    binder1st(const Fn& fn,
              const typename Fn::first_argument_type& value);

    typename Fn::result_type
        operator()(const typename Fn::second_argument_type& x) const;

    typename Fn::result_type
        operator()(typename Fn::second_argument_type& x) const;   
};

template < class Fn >
class binder2nd : public unary_function<typename Fn::first_argument_type,
                                        typename Fn::result_type> {
protected:
    Fn op;
    typename Fn::second_argument_type value;
public:
    binder2nd(const Fn& fn, 
              const typename Fn::second_argument_type& value);
    
    typename Fn::result_type
        operator()(const typename Fn::first_argument_type& x) const;

    typename Fn::result_type
        operator()(typename Fn::first_argument_type& x) const;
};
```



