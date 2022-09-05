#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <string>	// std::string
#include <sstream>  // std:ostringstream


// Used to get the current directory, can use later for something?
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#define abs(x) ((x)<0 ? -(x) : (x))
#endif

const char kPathSeparator =
#ifdef _WIN32
	'\\';
#else
	'/';
#endif

inline std::string& rtrim(std::string& s, const char t);

std::string join_path();

template <typename... Args>
std::string join_path(std::string& first, Args... args);


#endif