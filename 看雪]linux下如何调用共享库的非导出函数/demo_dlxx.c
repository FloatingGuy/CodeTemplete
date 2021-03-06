/*
* @Author: LK_mac
* @Date:   2016-08-18 15:24:58
* @Last Modified by:   LK_mac
* @Last Modified time: 2016-08-18 15:26:36
* @compile:
If  this program were in a file named "foo.c", you would build the pro‐
gram with the following command:
	gcc -rdynamic -o foo foo.c -ldl

Libraries exporting _init() and _fini() will want  to  be  compiled  as
follows, using bar.c as the example name:
	gcc -shared -nostartfiles -o bar bar.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char **argv)
{
   void *handle;
   double (*cosine)(double);
   char *error;

   handle = dlopen("libm.so", RTLD_LAZY);
   if (!handle) {
       fprintf(stderr, "%s\n", dlerror());
       exit(EXIT_FAILURE);
   }

   dlerror();    /* Clear any existing error */

   /* Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
      would seem more natural, but the C99 standard leaves
      casting from "void *" to a function pointer undefined.
      The assignment used below is the POSIX.1-2003 (Technical
      Corrigendum 1) workaround; see the Rationale for the
      POSIX specification of dlsym(). */

   *(void **) (&cosine) = dlsym(handle, "cos");

   if ((error = dlerror()) != NULL)  {
       fprintf(stderr, "%s\n", error);
       exit(EXIT_FAILURE);
   }

   printf("%f\n", (*cosine)(2.0));
   dlclose(handle);
   exit(EXIT_SUCCESS);
}

