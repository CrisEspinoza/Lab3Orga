#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv) {
  char *avalue = NULL;
  int bflag = 0;
  int cvalue = 0;
  int index;
  int c;

  opterr = 0;


  while ((c = getopt (argc, argv, "a:bc:")) != -1)
    switch (c)
      {
      case 'a':
        avalue = optarg;
        break;
      case 'b':
        bflag = 1;
        break;
      case 'c':
	      sscanf(optarg, "%d", &cvalue);
        break;
      case '?':
        if (optopt == 'a' || optopt == 'b' || optopt == 'c')
          fprintf (stderr, "Opcion -%c requiere un argumento.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Opcion desconocida `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Opcion con caracter desconocido `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }


  printf ("avalue = %s, bflag = %d, cvalue = %d\n",
          avalue, bflag, cvalue);

  for (index = optind; index < argc; index++)
    printf ("No existe opcion para agumento: %s\n", argv[index]);
  return 0;
}
