/******************************************************************************
 * HPTZIP --- wrapper of zlib library for manipulating with zip archivs
 ******************************************************************************
 * hptzip.c : functions for pack and unpack files from zip archive
 *
 * by Max Chernogor <mihz@mail.ru>, 2:464/108@fidonet
 *
 *****************************************************************************
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>

#ifdef unix
# include <unistd.h>
# include <utime.h>
#else
# include <direct.h>
# include <io.h>
#endif

#include "zip.h"
#include "unzip.h"

#ifdef WIN32
#define USEWIN32IOAPI
#include "iowin32.h"
#endif

#define DLLEXPORT
#include "hptzip.h"

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

char destdir[MAXFILENAME];
size_t dsLen;



/* change_file_date : change the date/time of a file
    filename : the filename of the file where date/time must be modified
    dosdate : the new date at the MSDos format (4 bytes)
    tmu_date : the SAME new date at the tm_unz format */
void change_file_date(filename,dosdate,tmu_date)
    const char *filename;
    uLong dosdate;
    tm_unz tmu_date;
{
#ifdef WIN32
  HANDLE hFile;
  FILETIME ftm,ftLocal,ftCreate,ftLastAcc,ftLastWrite;

  hFile = CreateFile(filename,GENERIC_READ | GENERIC_WRITE,
                      0,NULL,OPEN_EXISTING,0,NULL);
  GetFileTime(hFile,&ftCreate,&ftLastAcc,&ftLastWrite);
  DosDateTimeToFileTime((WORD)(dosdate>>16),(WORD)dosdate,&ftLocal);
  LocalFileTimeToFileTime(&ftLocal,&ftm);
  SetFileTime(hFile,&ftm,&ftLastAcc,&ftm);
  CloseHandle(hFile);
#else
#ifdef unix
  struct utimbuf ut;
  struct tm newdate;
  newdate.tm_sec = tmu_date.tm_sec;
  newdate.tm_min=tmu_date.tm_min;
  newdate.tm_hour=tmu_date.tm_hour;
  newdate.tm_mday=tmu_date.tm_mday;
  newdate.tm_mon=tmu_date.tm_mon;
  if (tmu_date.tm_year > 1900)
      newdate.tm_year=tmu_date.tm_year - 1900;
  else
      newdate.tm_year=tmu_date.tm_year ;
  newdate.tm_isdst=-1;

  ut.actime=ut.modtime=mktime(&newdate);
  utime(filename,&ut);
#endif
#endif
}

#ifdef WIN32
uLong filetime(f, tmzip, dt)
    char *f;                /* name of file to get info on */
    tm_zip *tmzip;             /* return value: access, modific. and creation times */
    uLong *dt;             /* dostime */
{
  int ret = 0;
  {
      FILETIME ftLocal;
      HANDLE hFind;
      WIN32_FIND_DATA  ff32;

      hFind = FindFirstFile(f,&ff32);
      if (hFind != INVALID_HANDLE_VALUE)
      {
        FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftLocal);
        FileTimeToDosDateTime(&ftLocal,((LPWORD)dt)+1,((LPWORD)dt)+0);
        FindClose(hFind);
        ret = 1;
      }
  }
  return ret;
}
#else
#ifdef unix
uLong filetime(f, tmzip, dt)
    char *f;                /* name of file to get info on */
    tm_zip *tmzip;             /* return value: access, modific. and creation times */
    uLong *dt;             /* dostime */
{
  int ret=0;
  struct stat s;        /* results of stat() */
  struct tm* filedate;
  time_t tm_t=0;

  if (strcmp(f,"-")!=0)
  {
    char name[MAXFILENAME+1];
    int len = strlen(f);

    strncpy(name, f,MAXFILENAME-1);
    /* strncpy doesnt append the trailing NULL, of the string is too long. */
    name[ MAXFILENAME ] = '\0';

    if (name[len - 1] == '/')
      name[len - 1] = '\0';
    /* not all systems allow stat'ing a file with / appended */
    if (stat(name,&s)==0)
    {
      tm_t = s.st_mtime;
      ret = 1;
    }
  }
  filedate = localtime(&tm_t);

  tmzip->tm_sec  = filedate->tm_sec;
  tmzip->tm_min  = filedate->tm_min;
  tmzip->tm_hour = filedate->tm_hour;
  tmzip->tm_mday = filedate->tm_mday;
  tmzip->tm_mon  = filedate->tm_mon ;
  tmzip->tm_year = filedate->tm_year;

  return ret;
}
#else
uLong filetime(f, tmzip, dt)
    char *f;                /* name of file to get info on */
    tm_zip *tmzip;             /* return value: access, modific. and creation times */
    uLong *dt;             /* dostime */
{
    return 0;
}
#endif
#endif




int check_exist_file(filename)
    const char* filename;
{
    FILE* ftestexist;
    int ret = 1;
    ftestexist = fopen(filename,"rb");
    if (ftestexist==NULL)
        ret = 0;
    else
        fclose(ftestexist);
    return ret;
}

/* calculate the CRC32 of a file,
   because to encrypt a file, we need known the CRC32 of the file before */
int getFileCrc(const char* filenameinzip,void*buf,unsigned long size_buf,unsigned long* result_crc)
{
   unsigned long calculate_crc=0;
   int err=ZIP_OK;
   FILE * fin = fopen(filenameinzip,"rb");
   unsigned long size_read = 0;
   unsigned long total_read = 0;
   if (fin==NULL)
   {
       err = ZIP_ERRNO;
   }

    if (err == ZIP_OK)
        do
        {
            err = ZIP_OK;
            size_read = (int)fread(buf,1,size_buf,fin);
            if (size_read < size_buf)
                if (feof(fin)==0)
            {
                printf("error in reading %s\n",filenameinzip);
                err = ZIP_ERRNO;
            }

            if (size_read>0)
                calculate_crc = crc32(calculate_crc,buf,size_read);
            total_read += size_read;

        } while ((err == ZIP_OK) && (size_read>0));

    if (fin)
        fclose(fin);

    *result_crc=calculate_crc;
    printf("file %s crc %x\n",filenameinzip,calculate_crc);
    return err;
}



int do_extract_currentfile(uf,popt_extract_without_path,popt_overwrite,password)
    unzFile uf;
    const int* popt_extract_without_path;
    int* popt_overwrite;
    const char* password;
{
    char filename_inzip[MAXFILENAME];
    char* filename_withoutpath;
    char* p;
    int err=UNZ_OK;
    FILE *fout=NULL;
    void* buf;
    uInt size_buf;

    unz_file_info file_info;
    uLong ratio=0;
    err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);

    if (err!=UNZ_OK)
    {
        printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
        return err;
    }

    size_buf = WRITEBUFFERSIZE;
    buf = (void*)malloc(size_buf);
    if (buf==NULL)
    {
        printf("Error allocating memory\n");
        return UNZ_INTERNALERROR;
    }

    p = filename_withoutpath = filename_inzip;
    while ((*p) != '\0')
    {
        if (((*p)=='/') || ((*p)=='\\'))
            filename_withoutpath = p+1;
        p++;
    }

    if ((*filename_withoutpath)=='\0')
    {
        if ((*popt_extract_without_path)==0)
        {
            //printf("creating directory: %s\n",filename_inzip);
            //mymkdir(filename_inzip);
        }
    }
    else
    {
        char* write_filename;
        int skip=0;

        if ((*popt_extract_without_path)==1)
        {
            destdir[dsLen] = '\0';
            strcat(destdir,filename_withoutpath);
            write_filename = destdir;
        }
        else
        {
            write_filename = filename_inzip;
        }

        err = unzOpenCurrentFilePassword(uf,password);
        if (err!=UNZ_OK)
        {
            printf("error %d with zipfile in unzOpenCurrentFilePassword\n",err);
        }

        if (((*popt_overwrite)==0) && (err==UNZ_OK))
        {
            char rep=0;
            FILE* ftestexist;
            ftestexist = fopen(write_filename,"rb");
            if (ftestexist!=NULL)
            {
                fclose(ftestexist);
                do
                {
                    char answer[128];
                    printf("The file %s exist. Overwrite ? [y]es, [n]o, [A]ll: ",write_filename);
                    scanf("%1s",answer);
                    rep = answer[0] ;
                    if ((rep>='a') && (rep<='z'))
                        rep -= 0x20;
                }
                while ((rep!='Y') && (rep!='N') && (rep!='A'));
            }

            if (rep == 'N')
                skip = 1;

            if (rep == 'A')
                *popt_overwrite=1;
        }

        if ((skip==0) && (err==UNZ_OK))
        {
            fout=fopen(write_filename,"wb");

            /* some zipfile don't contain directory alone before file */
            if ((fout==NULL) && ((*popt_extract_without_path)==0) &&
                                (filename_withoutpath!=(char*)filename_inzip))
            {
                //char c=*(filename_withoutpath-1);
                //*(filename_withoutpath-1)='\0';
                //makedir(write_filename);
                //*(filename_withoutpath-1)=c;
                //fout=fopen(write_filename,"wb");
            }

            if (fout==NULL)
            {
                printf("error opening %s\n",write_filename);
            }
        }

        if (fout!=NULL)
        {
            /*printf(" extracting: %s\n",write_filename);*/

            do
            {
                err = unzReadCurrentFile(uf,buf,size_buf);
                if (err<0)
                {
                    printf("error %d with zipfile in unzReadCurrentFile\n",err);
                    break;
                }
                if (err>0)
                    if (fwrite(buf,err,1,fout)!=1)
                    {
                        printf("error in writing extracted file\n");
                        err=UNZ_ERRNO;
                        break;
                    }
            }
            while (err>0);
            if (fout)
                    fclose(fout);

            if (err==0)
                change_file_date(write_filename,file_info.dosDate,
                                 file_info.tmu_date);
        }

        if (err==UNZ_OK)
        {
            err = unzCloseCurrentFile (uf);
            if (err!=UNZ_OK)
            {
                printf("error %d with zipfile in unzCloseCurrentFile\n",err);
            }
        }
        else
            unzCloseCurrentFile(uf); /* don't lose the error */
    }

    free(buf);
    return err;
}


int do_extract(uf,opt_extract_without_path,opt_overwrite,password)
    unzFile uf;
    int opt_extract_without_path;
    int opt_overwrite;
    const char* password;
{
    uLong i;
    unz_global_info gi;
    int err;
    FILE* fout=NULL;

    err = unzGetGlobalInfo (uf,&gi);
    if (err!=UNZ_OK)
        printf("error %d with zipfile in unzGetGlobalInfo \n",err);

    for (i=0;i<gi.number_entry;i++)
    {
        if (do_extract_currentfile(uf,&opt_extract_without_path,
                                      &opt_overwrite,
                                      password) != UNZ_OK)
            break;

        if ((i+1)<gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err!=UNZ_OK)
            {
                printf("error %d with zipfile in unzGoToNextFile\n",err);
                break;
            }
        }
    }

    return 0;
}

int do_extract_onefile(uf,filename,opt_extract_without_path,opt_overwrite,password)
    unzFile uf;
    const char* filename;
    int opt_extract_without_path;
    int opt_overwrite;
    const char* password;
{
    int err = UNZ_OK;
    if (unzLocateFile(uf,filename,CASESENSITIVITY)!=UNZ_OK)
    {
        /*printf("file %s not found in the zipfile\n",filename);*/
        return 2;
    }

    if (do_extract_currentfile(uf,&opt_extract_without_path,
                                      &opt_overwrite,
                                      password) == UNZ_OK)
        return 0;
    else
        return 1;
}


int UnPackWithZlib(char * zipfilename, char * filename_to_extract, char * dest_dir)
{
    unzFile uf=NULL;
    int nRet  = 0;

    if (zipfilename!=NULL)
    {

        #ifdef USEWIN32IOAPI
        zlib_filefunc_def ffunc;
        #endif

        #ifdef USEWIN32IOAPI
        fill_win32_filefunc(&ffunc);
        uf = unzOpen2(zipfilename,&ffunc);
        #else
        uf = unzOpen(zipfilename);
        #endif
    }

    if (uf==NULL)
    {
        printf("Cannot open %s\n",zipfilename);
        return 1;
    }

    /*printf("%s opened\n",zipfilename);*/

    strcpy(destdir,dest_dir);
    dsLen = strlen( dest_dir );

    if (filename_to_extract == NULL)
        nRet = do_extract(uf,1,1,NULL);
    else
        nRet = do_extract_onefile(uf,filename_to_extract,1,1,NULL);

    unzCloseCurrentFile(uf);
    unzClose(uf);
    return nRet;
}

int PackWithZlib(char * zipfilenamearg, char * filenameinzip)
{
    
    int opt_overwrite=2;
    int opt_compress_level=Z_DEFAULT_COMPRESSION;
    int err=0;
    int size_buf=0;
    void* buf=NULL;
    const char* password=NULL;

    size_buf = WRITEBUFFERSIZE;

    buf = (void*)malloc(size_buf);
    if (buf==NULL)
    {
        printf("Error allocating memory\n");
        return ZIP_INTERNALERROR;
    }


    /* if the file don't exist, we not append file */
    if (check_exist_file(zipfilenamearg)==0)
        opt_overwrite=1;

    if(filenameinzip)
    {
        char *basename = strrchr(filenameinzip,PATH_DELIM);
        zipFile zf;
#ifdef USEWIN32IOAPI
        zlib_filefunc_def ffunc;
        fill_win32_filefunc(&ffunc);
        zf = zipOpen2(zipfilenamearg,(opt_overwrite==2) ? 2 : 0,NULL,&ffunc);
#else
        zf = zipOpen(zipfilenamearg,(opt_overwrite==2) ? 2 : 0);
#endif

        if (zf == NULL)
        {
            printf("error opening %s\n",zipfilenamearg);
            err= ZIP_ERRNO;
        }
/*
        else
            printf("creating %s\n",zipfilenamearg);
*/
        {
            FILE * fin;
            int size_read;
            zip_fileinfo zi;
            unsigned long crcFile=0;

            zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
                zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
            zi.dosDate = 0;
            zi.internal_fa = 0;
            zi.external_fa = 0;
            filetime(filenameinzip,&zi.tmz_date,&zi.dosDate);

            if ((password != NULL) && (err==ZIP_OK))
                err = getFileCrc(filenameinzip,buf,size_buf,&crcFile);
            
            basename++; /* skip pathdelim */
            err = zipOpenNewFileInZip3(zf,basename,&zi,
                NULL,0,NULL,0,NULL /* comment*/,
                (opt_compress_level != 0) ? Z_DEFLATED : 0,
                opt_compress_level,0,
                /* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
                -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                password,crcFile);

            if (err != ZIP_OK)
                printf("error in opening %s in zipfile\n",basename);
            else
            {
                fin = fopen(filenameinzip,"rb");
                if (fin==NULL)
                {
                    err=ZIP_ERRNO;
                    printf("error in opening %s for reading\n",filenameinzip);
                }
            }

            if (err == ZIP_OK)
            {
                do
                {
                    err = ZIP_OK;
                    size_read = (int)fread(buf,1,size_buf,fin);
                    if (size_read < size_buf)
                        if (feof(fin)==0)
                        {
                            printf("error in reading %s\n",filenameinzip);
                            err = ZIP_ERRNO;
                        }

                        if (size_read>0)
                        {
                            err = zipWriteInFileInZip (zf,buf,size_read);
                            if (err<0)
                            {
                                printf("error in writing %s in the zipfile\n",
                                    basename);
                            }

                        }
                } while ((err == ZIP_OK) && (size_read>0));
            }
            if (fin)
                fclose(fin);

            if (err<0)
                err=ZIP_ERRNO;
            else
            {
                err = zipCloseFileInZip(zf);
                if (err!=ZIP_OK)
                    printf("error in closing %s in the zipfile\n",
                    basename);
            }
        }
        if (zipClose(zf,NULL) != ZIP_OK)
            printf("error in closing %s\n",zipfilenamearg);
    }

    free(buf);
    return 0;
}


