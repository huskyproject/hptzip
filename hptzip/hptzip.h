/******************************************************************************
 * HPTZIP --- wrapper of zlib library for manipulating with zip archivs
 ******************************************************************************
 * hptzip.h : functions for pack and unpack files from zip archive
 *
 * by Max Chernogor <mihz@mail.ru>, 2:464/108@fidonet
 *
 *****************************************************************************
 * $Id$
 */

#ifndef HPTZIP_H
#define HPTZIP_H

#include <huskylib/compiler.h>
#include <huskylib/huskyext.h>

HUSKYEXT int UnPackWithZlib(char * zipfilename, char * filename_to_extract, char * dest_dir);
HUSKYEXT int PackWithZlib(char * zipfilenamearg, char * filenameinzip);

#endif
