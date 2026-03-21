#ifndef _API_INCLUDE
#define _API_INCLUDE

#ifdef NOVASCRIPT_EXPORTS
#define NOVASCRIPT_API __declspec(dllexport)
#else
#define NOVASCRIPT_API __declspec(dllimport)
#endif

#endif