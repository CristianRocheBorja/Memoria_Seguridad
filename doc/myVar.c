/* myVar.c */

#include <string.h>
#include <stdio.h>

void foo (char * bar){

   char  c[28];
   float myVar = 10.5;

   printf("myVar value = %f\n", myVar);

   memcpy(c, bar, strlen(bar));

   printf("myVar value = %f\n", myVar);
}

int main (int argc, char ** argv){
   foo("foo text");
   return 0;
}
