#ifndef FASTMATREAD_H
#define FASTMATREAD_H

#include <string>

int matread(const char *filename,bool *&outnan, double *&outdata,unsigned &sx,unsigned &sy,std::string &error);

#endif // FASTMATREAD_H
