/* internalZip support for HPT 
   using code from infozip
   by Dmitriy Kazimirow
   redistribution license:GPL(same as HPT)
*/
#include <cssysdef.h>
#include <archive.h>
#include <util.h>
#include <stdio.h>
//#include <fidoconf/log.h>
#include "hptzip.h"

HPTZIP_EXT int UnPackWithZlib(char * fileName, char * destDir)
{
//   printf("UnPackWithZlib(%s,%s)\n",fileName,destDir);
//  w_log( LL_SRCLINE, "%s:%d Unpacking zip archive %s to %s", __FILE__, __LINE__,fileName,destDir);
   csArchive * zipfile=new csArchive(fileName);
   if (zipfile==NULL)
   {
     return 1;//no such file or CS error
   }
   void * file=NULL;
   size_t filesize;
   char * filename;
   int i(0);
   bool stop(false);
   while (!stop)
   {
    file=zipfile->GetFile(i);
    if (file==NULL)
    {
      stop=true;
      break;
    }
    i++;
    filename=zipfile->GetFileName(file);
    filesize=zipfile->GetFileSize(file);
    char fullname[FILENAME_MAX];
    if (strlen(destDir)>MAX_PATH-11)
    {
      return 1;//destDir too long
    }
    strcpy(fullname,destDir);
    strcat(fullname,filename);
//    printf("Unpacking %s::%s to %s\n",fileName,filename,fullname);
    char * buf=zipfile->Read(filename);
    FILE * newfile=fopen(fullname,"wb+");
    if (newfile==NULL)
    {
      return 1;//new file open failed-path+name totally wrong?
    }
    if (fwrite(buf,filesize,1,newfile)<=0)
    {
       delete [] buf;
       return 2;//fwrite failed,no space left?
    }   
   fclose(newfile);
   delete [] buf;
  }
  //no errors
  delete zipfile;
  return 0;
}
HPTZIP_EXT int PackWithZlib(char * arcFile, char * fileToAdd)
{

//   FILE * log=fopen("hptzip.log","a");
//   fprintf(log,"\nPackWithZlib(%s,%s)\n",arcFile,fileToAdd);
//  w_log( LL_SRCLINE, "%s:%d Packing %s to zip archive %s", __FILE__, __LINE__, fileToAdd,arcFile);
  int res;
  int filesize;
  char * buf;
  FILE * srcfile=fopen(fileToAdd,"rb");
  if (srcfile==NULL)
  {
//    fprintf(log,"cannot open src file\n");
//    fclose(log);
    return 1;//cannot open src file
  }
  fseek(srcfile,0,SEEK_END);
  filesize=ftell(srcfile);
  buf=new char[filesize];
  if (buf==NULL)
  {
//    fprintf(log,"memory allloc fail\n");
//    fclose(log);
    fclose(srcfile);
    return 2;//memory alloc fail
  }
  fseek(srcfile,0,SEEK_SET);
  size_t read=fread(buf,filesize,1,srcfile);
  if (read!=1)
  {
//    fprintf(log,"source file read error,%d:%d\n",read,filesize);
//    fclose(log);
    fclose(srcfile);
    return 3;//source file read error
  }
  fclose(srcfile);
//  fprintf(log,"initing csArchive...\n");
  csArchive * zipfile=new csArchive(arcFile);
  if (zipfile==NULL)
  { 
//    fprintf(log,"zipfile==NULL\n");
//    fclose(log);
    fclose(srcfile);
    return 6;
  }
  char justName[FILENAME_MAX];
  char justPath[MAX_PATH];
  // E:\ftn12\out.tmp\1234.pkt -> 1234.pkt  
  csSplitPath(fileToAdd,justPath,sizeof(justPath),justName,sizeof(justName));
//  fprintf(log,"adding file %s\n",justName);
  void * arcEntry=zipfile->NewFile(justName);
  if (arcEntry==NULL)
  {
//    fprintf(log,"arcEntry==NULL\n");
//    fclose(log);
    fclose(srcfile);
    delete zipfile;
    return 7;
  }
  if (!zipfile->Write(arcEntry,buf,filesize))
  {
//    fprintf(log,"cannot write to zipfile\n");
//    fclose(log);
    delete zipfile;
    return 4;//cannot write to zip file
  }
  if (!zipfile->Flush()) 
  {
//    fprintf(log,"cannot flush zipfile\n");
//    fclose(log);
    delete zipfile;
    return 5; //real write failed
  }
  delete zipfile;
//  fprintf(log,"Packed ok\n");
//  fclose(log);  

  //no errors
  return 0;
}
