#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include"minmax.h"

int main(int argc, char **argv){
  if( argc == 3 && atoi(argv[1]) > 0 && atoi(argv[2]) > 0) {
    int n1 = atoi(argv[1]);
    int n2 = atoi(argv[2]);
    printf("Max(%d,%d)=%d\n", n1, n2, max(n1,n2));
    printf("Min(%d,%d)=%d\n", n1, n2, min(n1,n2));

  }
  else {
    printf("USAGE: main n1 n2 (n1 and n2 are integers).\n");
    exit(1);
  }
}
