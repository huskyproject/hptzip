#include "hptzip.h"
#ifdef ZLIBDL
#include "hptzipdl.h"
#endif
int main(int argc, char *argv[]) {
  int rc;
  rc = init_hptzip();
  printf("init_hptzip(): %d\n", rc);
#ifdef ZLIBDL
  printf(" > crc32()=%p\n", dl_crc32);
  printf(" > get_crc_table()=%p\n", dl_get_crc_table);
  printf(" > inflateInit2_()=%p\n", dl_inflateInit2_);
  printf(" > inflate()=%p\n", dl_inflate);
  printf(" > inflateEnd()=%p\n", dl_inflateEnd);
  printf(" > deflateInit2_()=%p\n", dl_deflateInit2_);
  printf(" > deflate()=%p\n", dl_deflate);
  printf(" > deflateEnd()=%p\n", dl_deflateEnd);
#endif
  {
    uLong crc = crc32(0L, NULL, 0);;
    crc = crc32(0L, "val khokhlov", 12);
    printf("testing crc32(): %s\n", crc == 0xae16c6abL ? "OK" : "ERR");
  }
  {
    rc = UnPackWithZlib("test.zip", NULL, ".\\");
    printf("testing UnPackWithZlib(): %d\n", rc);
  }
}
