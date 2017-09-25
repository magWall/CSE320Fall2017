#include "utf.h"
#include "debug.h"
#include "wrappers.h"
#include <unistd.h>

int
from_utf16le_to_utf16be(int infile, int outfile)
{
  int bom;
  utf16_glyph_t buf;
  ssize_t bytes_read;
  size_t bytes_to_write;
  int ret = 0;

  bom = UTF16BE;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  reverse_bytes(&bom, 2);
#endif
  write_to_bigendian(outfile, &bom, 2);

  while ((bytes_read = read_to_bigendian(infile, &(buf.upper_bytes), 2)) > 0) {
    bytes_to_write = 2;
    reverse_bytes(&(buf.upper_bytes), 2);
    if(is_lower_surrogate_pair(buf)) {
      if((bytes_read = read_to_bigendian(infile, &(buf.lower_bytes), 2)) < 0) {
        break;
      }
      reverse_bytes(&(buf.lower_bytes), 2);
      bytes_to_write += 2;
    }
    write_to_bigendian(outfile, &buf, bytes_to_write);
  }
  ret = bytes_read;
  return ret;
}

int
from_utf16le_to_utf8(int infile, int outfile)
{  /* TODO */
  int ret = -1;
  utf16_glyph_t utf16_buf;
  ssize_t bytes_read;
  size_t size_of_glyph;
  code_point_t code_point;
  utf8_glyph_t utf8_buf;
  utf16_glyph_t utf16_empty_buf;

  while((bytes_read = read_to_littleendian(infile, &utf16_buf.upper_bytes, 2)) > 0)
  {
    if( is_upper_surrogate_pair(utf16_buf) )
    {
      bytes_read = read_to_littleendian(infile, &utf16_buf.lower_bytes,2);
    }
    else
    {
      bytes_read = 2;
    }
    ret = bytes_read;
    code_point =utf16_glyph_to_code_point(&utf16_buf);
    size_of_glyph = utf8_glyph_size_of_code_point(code_point);
    utf8_buf = code_point_to_utf8_glyph(code_point,&size_of_glyph);
    debug("%x",code_point);
//    printf("%x,",code_point);
    write_to_littleendian(outfile,&utf8_buf,size_of_glyph);
    utf16_buf = utf16_empty_buf;
 }
  return ret;
}

utf16_glyph_t
code_point_to_utf16le_glyph(code_point_t code_point, size_t *size_of_glyph)
{
  utf16_glyph_t ret;

  memeset(&ret, 0, sizeof ret);
  if(is_code_point_surrogate(code_point)) {
    code_point -= 0x10000;
    ret.upper_bytes = (code_point >> 10) + 0xD800;
    ret.lower_bytes = (code_point & 0x3FF) + 0xDC00;
   // #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
   //   reverse_bytes(&ret.upper_bytes, 2);
   //   reverse_bytes(&ret.lower_bytes, 2);
   // #endif
    *size_of_glyph = 4;
  }
  else {
    ret.upper_bytes |= code_point;
   // #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
   //   reverse_bytes(&ret.upper_bytes, 2);
   // #endif
    *size_of_glyph = 2;
  }
  return ret;
}
