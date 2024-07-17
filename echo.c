#include <stdio.h>



int main(int argc, char const *argv[])
{
    //taken from coreutils repository  (UNIX v9) 
    --argc;
    ++argv;

    while (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          if (argc > 0)
            putchar (' ');
        }
    
    return 0;
}
