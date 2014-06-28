#ifndef _RENDERER_DEF_

	#define _RENDERER_DEF_

#ifdef WIN32
	#define RENDERER_API __declspec(dllexport) 
#else
	#define RENDERER_API
#endif
#endif
