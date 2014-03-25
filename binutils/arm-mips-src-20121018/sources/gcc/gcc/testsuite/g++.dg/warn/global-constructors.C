/* { dg-do compile } */
/* { dg-options "-Wglobal-constructors" } */


#include <iostream>
#include <vector>
class foo
{
public:
  foo ()
    {
      std::cout << "foo::foo" << std::endl;
    }
};

foo a_foo;  /* { dg-warning "Object with static initialization 'a_foo'" } */
std::vector<int> array; /* { dg-warning "Object with static initialization 'array'" } */

int
main ()
{
  foo local_foo;
  return 0;
}

