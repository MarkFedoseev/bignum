# bignum
Long arithmetic library for C++

# About
bignum.h is a long arithmetic library for integers for C++

Number main class:
```
class bigint
```
## Avalable operations:
- comparison
- sum
- difference
- multiplication
- division
- exponentiation

# How to use:

## Initialization:
Here are constructors prototypes:
```
bigint();
```
initialyzes a ```zero``` ```bigint``` type number

```
bigint(const int& num, char sgn = 1);
```
initialyzes a ```bigint``` from ```int``` with a sign (default sigh is +)
positive sgn argument for positive sigh, negative - for negative

```
bigint(const string& num, int rev = 1);
```
initialyzes a positive ```bigint``` from ```string``` with a **space** as delimiter

```rev``` argument by default equals ```1```, which means string will be parsed naturally (right number will be parsed with lesser exponent). Class stores numbers reversed

## Getting numbers:
Here are method's prototypes allows you to get a number stored in class
> ! all methods do not return sign
```
vector<int> get_number(int rev = 1);
```
returns a ```std::vector<int>``` with a stored number

```rev``` argument ```0``` value allows you to get number as it stores

```
string get_string(int rev = 1);
```
returns a ```string``` with stores number with a **space** delimeter

```rev``` argument ```0``` value allows you to get number as it stores

```
vector<int> get_binary();
```
returns a ```std::vector<int>``` with a binary representation of stored number

## Other methods:

```
int get_size();
```
returns a number of "digits" in ```bigint```

```
int get_sign();
```
returns a sign of ```bigint```

## Operations:
### sum
```
bigint operator + (bigint bi1, bigint bi2);
```
returns a ```bigint``` equals **sum** of passed ```bigints```pair<bigint, bigint> operator / (bigint bi1, bigint bi2);

sample:
```
bigint bi1(2), bi2(3);
(bi1 + bi2).print_me(); //output: 5
```

### difference
```
bigint operator - (bigint bi1, bigint bi2);
```
returns a ```bigint``` equals **difference** of passed ```bigints```

sample:
```
bigint bi1(2), bi2(3);
(bi1 - bi2).print_me(); //output: -1
```

### multiplication
```
bigint operator * (bigint bi1, bigint bi2);
```
returns a ```bigint``` equals **product** of passed ```bigints```

sample:
```
bigint bi1(2), bi2(3);
(bi1 * bi2).print_me(); //output: 6
```

### division
```
pair<bigint, bigint> operator / (bigint bi1, bigint bi2);
```
returns a pair of ```bigints```:

  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**first** ```bigint``` equals ```bi1``` **div** ```bi2```
  
  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**second** ```bigint``` equals ```bi1``` **mod** ```bi2``` 

sample:
```
bigint bi1(2), bi2(3);
std::pair<bigint, bigint> res = bi1 / bi2;
res.first.print_me();   //output: 0
res.second.print_me();  //output: 2
```

### exponentiation
> ! Only non-negative exponent supported
```
bigint operator ^ (bigint bi, int exp);
bigint operator ^ (bigint bi, long long exp);
bigint operator ^ (bigint bi, bigint exp);
```
returns a ```bigint``` equals passed ```bigint``` exponentiated to passed ```int```, ```long long``` and ```bigint``` respectively

sample:
```
bigint bi1(2), bi2(3);
int a = 3;
long long b = 3;
(bi1 ^ bi2).print_me(); //output: 8
(bi1 ^ a).print_me();   //output: 8
(bi1 ^ b).print_me();   //output: 8
```

### comparison
```
bool operator > (bigint bi1, bigint bi2);
bool operator < (bigint bi1, bigint bi2);
bool operator == (bigint bi1, bigint bi2);
bool operator != (bigint bi1, bigint bi2);
bool operator >= (bigint bi1, bigint bi2);
bool operator <= (bigint bi1, bigint bi2);
```
return ```bool``` value according to comparison result

sample:
```
bigint bi1(2), bi2(3), bi3(3);
std::cout << (bi1 < bi2);   //output: 1
std::cout << (bi1 > bi2);   //output: 0
std::cout << (bi2 <= bi3);  //output: 1
std::cout << (bi2 >= bi3);  //output: 1
std::cout << (bi2 == bi3);  //output: 1
```
