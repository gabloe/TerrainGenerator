#ifndef _NOISE_API_

#define _NOISE_API_
#ifdef _WIN32
#define NOISE_API __declspec(dllexport) 
#else
#define NOISE_API
#endif
#endif
