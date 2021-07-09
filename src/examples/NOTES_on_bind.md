Note on ```std::binder1st```, ```std::binder2nd```, ```std::bind1st```, ```std::bind2nd```, 
```boost::bind```, ```std::bind```, and ```std::bind_front```

The first four function templates (```std::binder1st```, ```std::binder2nd```, ```std::bind1st```,
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

2. ```std::bind1st``` and ```std::bind2nd```

Two templates which bind a given parameter ```x``` to a first or second parameter
of the given binary function object ```f```. That is, the template stores ```x```
within the resulting wrapper, which, if called, passes ```x``` as the first or the 
second parameter of ```f```.

Parameters: 
  ```f``` - pointer to a function to bind an argument to
  ```x``` - argument to bind to ```f```

Return value:
  A function object wrapping ```f``` and ```x```


```cpp
template< class F, class T >
std::binder1st<F> bind1st( const F& f, const T& x );
```

binds the first argument of ```f``` to ```x```. Effectivelly calls

```cpp

std::binder1st<F>(f, typename F::first_argument_type(x))

```



```cpp
template< class F, class T >
std::binder2nd<F> bind2nd( const F& f, const T& x );
```
binds the second argument of ```f``` to ```x```. Effectively calls 

```cpp

std::binder2nd<F>(f, typename F::second_argument_type(x))

```

3. ```boost::bind```

```boost::bind``` is a generalization of the standard functions ```std::bind1st``` and ```std::bind2nd```.
It supports arbitrary function objects, functions, function pointers, and member function pointers, and 
is able to bind any argument to a specific value or route input arguments into arbitrary positions.
Unlike the ```bind``` functions in the standard library mentioned previously ```boost::bind``` does
not place any requirements on the function object. In particular, it does not need the ```result_type```,
```first_argument_type``` and ```second_argument_type``` standard typedefs.

Using ```boost::bind``` with functions and function pointers

Given these definitions:

```cpp
int f(int a, int b) 
{
   return a + b;
}

int g(int a, int b, int c)
{
   return a + b + c;
}
```

```boost::bind(f, 1, 2)``` will produce a "nullary" function object that takes no arguments and 
returns ```f(1, 2)```. Similarly, ```boost::bind(g, 1, 2, 3)()``` is equivalent to ```g(1, 2, 3)```.
It is possible to selectively bind only some of the arguments. ```boost::bind(f, _1, 5)(x)``` is
equivalent to ```f(x, 5)```; here ```_1``` is a _placeholder_ argument that means "substitute with
the first input argument". For comparison, here is the same operation expressed with standard 
library functions :
```cpp
std::bind2nd(std::ptr_fun(f), 5)(x);  // f(x, 5) equivalent to boost::bind(f, _1, 5)
std::bind1st(std::ptr_fun(f), 5)(x);  // f(5, x) equivalent to boost::bind(f, 5, _1)
```
```boost::bind``` can handle functions with more than two arguments, and its argument substitution
mechanism is more general:
```cpp
boost::bind(f, _2, _1) (x, y);        // f(y, x)
boost::bind(g, _1, 9, _1) (x);        // g(x, 9, x)
boost::bind(g, _3, _3, _3) (x, y, z); // g(z, z, z)
boost::bind(g, _1, _1, _1) (x, y, z); // g(x, x, x)
```
Note that, in the last example, the function object produced by ```boost::bind(g, _1, _1, _1)```
does not contain references to any arguments beyond the first, but it can still be used with 
more than one argument. Any extra arguments are silently ignoredjust like the first and second
argument are ignored in the third example.

The arguments that ```boost::bind``` takes are copied and held internally by the returned function object.
For example, in the following code:
```cpp
int i = 5;
boost::bind(f, i, _1);
``` 
a copy of the value of ```i``` is stored into the function object. ```boost::ref``` and ```boost::cref```
can be used to make the function obejct store a reference to an object, rather than a copy:
```cpp
int i = 5;
boost::bind(f, ref(i), _1);
boost::bind(f, cref(i), _1);
```

Using ```boost::bind``` with function objects

```boost::bind``` is not limited to functions; it accepts arbitrary function objects. In the general
case, the return type of the generated function object's ```operator()``` has to be specified explicitly
(without a ```typeof``` operator the return type cannot be inferred)

```cpp
struct F
{
  int operator()(int a, int b) { return a - b; }
  bool operator()(long a, long b) { return a == b; }
};
F f;
int x = 104;
boost::bind<int>(f, _1, _1)(x);  // f(x, x) i.e. zero
```

Some compilers have trouble with the ```bind<R>(f, ...)``` syntax. For portability reasons, an alternative
to express the above is supported:
```cpp
boost::bind(boost::type<int>(), f, _1, _1)(x);
``` 
Note, however, that the alternative syntax is provided only as a workaround. It is not part of the interface.
When the function object exposes a nested type named ```result_type```, the explicit return type can be 
omitted:

```cpp
int x = 8;
boost::bind(std::less<int>(), _1, 9) (x);  // x < 9
```
_[Note: the ability to omit the return type is not available on all compilers.]_

By default, ```boost::bind``` makes a copy of the provided function object. ```boost::ref``` and ```boost::cref```
can be used to make it store a reference to the function object, rather than a copy. This can be useful when
the function object is non-copyable, expensive to copy, or contains state; of course in this case the
programmer is expected to ensure that the function object is not destroyed while it's still being used.

```cpp
struct F2
{
   int s;

   typedef void result_type;
   void operator()(int x) { s += x; }
};

F2 f2 = { 0 };
int a[] = { 1, 2, 3 };

std::for_each(a, a+3, boost::bind(ref(f2), _1));

assert(f2.s == 6);
```

Using ```boost::bind``` with pointers to members

Pointers to member functions and pointers to data members are not function objects, because they do 
not support ```operator()```. For convenience, ```boost::bind``` accepts member pointers as its
first argument, and the behavior is as if ```boost::mem_fn``` has been used to convert the member
pointer into a function object. In other words, the expression
```cpp
boost::bind(&x::f, args)
```
is equivalent to 
```cpp
boost::bind<R>(mem_fn(&X::f), args)
```
where ```R``` is the return type of ```X::f``` (for member functions) or the type of the member
(for data members).





