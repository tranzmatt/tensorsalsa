#include <string>
const char* tf_git_version() {return "2.0.0";}
const char* tf_compiler_version() {
#ifdef _MSC_VER
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
  return "MSVC " TOSTRING(_MSC_FULL_VER);
#else
  return __VERSION__;
#endif
}
int tf_cxx11_abi_flag() {
#ifdef _GLIBCXX_USE_CXX11_ABI
  return _GLIBCXX_USE_CXX11_ABI;
#else
  return 0;
#endif
}
int tf_monolithic_build() {
#ifdef TENSORFLOW_MONOLITHIC_BUILD
  return 1;
#else
  return 0;
#endif
}
