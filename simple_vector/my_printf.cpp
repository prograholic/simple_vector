#include <stdio.h>

extern "C"
{

int __cdecl my_printf(char const* const format, ...)
{
    int result;
    va_list argList;
    __crt_va_start(argList, format);
    result = _vfprintf_l(stdout, format, NULL, argList);
    __crt_va_end(argList);
    return result;
}

}
