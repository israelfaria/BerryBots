/*
  Copyright (C) 2013 - Voidious

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef ZIPPER_H
#define ZIPPER_H

#include <exception>

class ZipperException : public std::exception {
  char *message_;
  public:
    ZipperException(const char *details);
    ~ZipperException();
    virtual const char* what();
};

class Zipper {
  public:
    virtual void packageFiles(const char *outputFile, const char *baseDir,
        char **filenames, int numFiles, bool binary,
        const char *absMetaFilename, const char *metaFilename) = 0;
    virtual void unpackFile(const char *zipFile, const char *outputDir) = 0;
    virtual ~Zipper() {};
};

#endif
