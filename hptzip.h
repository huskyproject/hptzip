/* internalZip support for HPT 
   by Dmitriy Kazimirow
*/
#ifdef _MAKE_DLL
#   if defined(_MSC_VER) && (_MSC_VER >= 1200)
#               define _MAKE_DLL_MVC_
#       ifndef _HPTZIP_EXT
#           define HPTZIP_EXT __declspec(dllimport)
#       else
#           define HPTZIP_EXT __declspec(dllexport)
#       endif //HPTZIP_EXT
#   else
#       define _HPTZIP_EXT
#   endif
#else
#   define HPTZIP_EXT
#endif
#ifdef __cplusplus
extern "C" {
#endif
HPTZIP_EXT int UnPackWithZlib(char * fileName, char * destDir);
HPTZIP_EXT int PackWithZlib(char * arcFile, char * fileToAdd);
#define PATH_DELIM '\\' 
#ifdef __cplusplus
}
#endif
