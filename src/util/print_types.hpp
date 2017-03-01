#pragma once

#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include "rtfm/rtfm_srp.hpp"

template <class T>
std::string type_name()
{
  typedef typename std::remove_reference<T>::type TR;
  std::unique_ptr<char, void (*)(void*)> own(
#ifndef _MSC_VER
      abi::__cxa_demangle(typeid(TR).name(), nullptr, nullptr, nullptr),
#else
      nullptr,
#endif
      std::free);
  std::string r = own != nullptr ? own.get() : typeid(TR).name();
  if (std::is_const<TR>::value)
    r += " const";
  if (std::is_volatile<TR>::value)
    r += " volatile";
  if (std::is_lvalue_reference<T>::value)
    r += "&";
  else if (std::is_rvalue_reference<T>::value)
    r += "&&";
  return r;
}

template <typename T>
void print_list(std::string l, std::string pp = "");

struct name_printer
{
  template <typename U>
  void operator()(brigand::type_<U>)
  {
    std::cout << "      " << type_name<U>() << ",\n";
  }

  template <typename ID, typename... U>
  void operator()(brigand::type_<rtfm::Resource<ID, U...>>)
  {
    std::cout << "  Resource type:  " << type_name<ID>() << ",\n";
    print_list<brigand::list<U...>>("Resource Job List", "    ");
  }
};
template <typename T>
void print_list(std::string l, std::string pp)
{
  std::cout << pp << l << " = list<"
            << "\n";
  brigand::for_each<T>(name_printer{});
  std::cout << pp << ">\n" << std::endl;
}
