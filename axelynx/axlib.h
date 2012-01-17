#pragma once

#ifdef AXELYNX_STATIC
	#define AXELYNX_API
#else
    #ifdef WIN32
		#ifdef AXELYNX_EXPORTS
			#define AXELYNX_API __declspec(dllexport)
		#else
			#define AXELYNX_API __declspec(dllimport)
		#endif
	#else
		#define AXELYNX_API
	#endif
#endif

#ifdef _MSC_VER
#define AXELYNX_MEMALIGN(a) __declspec(align(a))
#define FAST_CALL __fastcall
#else
#define AXELYNX_MEMALIGN(a)
#define FAST_CALL
#endif
