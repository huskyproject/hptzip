/******************************************************************************
 * HPTZIP --- wrapper of zlib library for manipulating with zip archivs
 ******************************************************************************
 * hptzipdl.h : defines to implement dynamic loading
 *
 * by val khokhlov, 2:550/180@fidonet
 *
 *****************************************************************************
 * $Id$
 */

#ifndef _hptzipdl_h_
#define _hptzipdl_h_

#ifdef WIN32
# define ZCALLCONV __stdcall
#else
# define ZCALLCONV
#endif

#ifndef OF /* function prototypes */
#  ifdef STDC
#    define OF(args)  args
#  else
#    define OF(args)  ()
#  endif
#endif

typedef unsigned char  Byte;  /* 8 bits */
typedef unsigned long  uLong; /* 32 bits or more */
typedef Byte  FAR Bytef;
typedef uLong FAR uLongf;

typedef  uLong ZCALLCONV crc32_func OF((uLong crc, const Bytef *buf, uInt len));
extern crc32_func *dl_crc32;
#define crc32(crc,buf,len) (*dl_crc32)(crc,buf,len)

typedef  const uLongf * ZCALLCONV get_crc_table_func    OF((void));
extern get_crc_table_func *dl_get_crc_table;
#define get_crc_table() (*dl_get_crc_table)()

typedef  int ZCALLCONV inflateInit2__func OF((z_streamp strm, int  windowBits,
                                      const char *version, int stream_size));
extern inflateInit2__func *dl_inflateInit2_;
# define inflateInit2_(a1,a2,a3,a4) (*dl_inflateInit2_)(a1,a2,a3,a4)

typedef  int ZCALLCONV inflate_func OF((z_streamp strm, int flush));
extern inflate_func *dl_inflate;
# define inflate(a1,a2) (*dl_inflate)(a1,a2)

typedef  int ZCALLCONV inflateEnd_func OF((z_streamp strm));
extern inflateEnd_func *dl_inflateEnd;
# define inflateEnd(a1) (*dl_inflateEnd)(a1)

typedef  int ZCALLCONV deflateInit2__func OF((z_streamp strm, int  level, int  method,
                                      int windowBits, int memLevel,
                                      int strategy, const char *version,
                                      int stream_size));
extern deflateInit2__func *dl_deflateInit2_;
# define deflateInit2_(a1,a2,a3,a4,a5,a6,a7,a8) (*dl_deflateInit2_)(a1,a2,a3,a4,a5,a6,a7,a8)

typedef  int ZCALLCONV deflate_func OF((z_streamp strm, int flush));
extern deflate_func *dl_deflate;
# define deflate(a1,a2) (*dl_deflate)(a1,a2)

typedef  int ZCALLCONV deflateEnd_func OF((z_streamp strm));
extern deflateEnd_func *dl_deflateEnd;
# define deflateEnd(a1) (*dl_deflateEnd)(a1)

#endif
