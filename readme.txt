встроенный zip для hpt(zipInternal)
реализован на базе csArchive из CrystalSpace
(http://sourceforge.net/projects/crystal)
для сборки требуется исходники самого CS,собранная libcsutil
и соотвествующий комплилятору/OS комплект внешних библиотек(zlib например)
(см.документацию по CS)
для сборки hpt с этим кодом-достаточно только hptzip.h,hptzip.dll/lib
для работы надо еще zlib.dll из CS