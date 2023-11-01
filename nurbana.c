/* Author: Justin Shumaker
 * Date: April 15, 2002
 * Desc: Lib Path Wrapper for Nurbana
 */
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
  putenv("LD_LIBRARY_PATH=/usr/lib;.");
//  printf("env: %s\n",getenv("LD_LIBRARY_PATH"));
  execlp("./.nurbanabin","./.nurbanabin",argv[1],argv[2],argv[3],0);
} //eof main()
