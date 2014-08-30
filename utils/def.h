#ifndef _UTIL_API_

#define _UTIL_API_
#ifdef _WIN32
#define UTIL_API __declspec(dllexport) 
#else
#define UTIL_API
#endif
#endif
