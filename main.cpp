#include <iostream>

int main(
         int argc,
         char **argv
         )
{
    /* Instructions */
    char header[] =
{"\n\n\
----------------------------------------------------------------------\n\
    SPDB -- Single Protein Bownian Dynamics\n\
    \n\
    Kherim Willems (kherim@kher.im)\n\
    \n\"};
    
    char *usage =
{"\n\n\
----------------------------------------------------------------------\n\
    This program performs a 1D brownian dynamics simulation on a\n\
    single particle trapped inside a force profile.\n\
        Usage:\n\n\
        spdb [options] spdb.inv\n\n\
        where spdb.in is a formatted input file and [options] are:\n\n\
--output-file=<name>     Enables output logging to the path\n\
    listed in <name>.  Uses flat-file format.\n\
--help                   Display this help information.\n\
--version                Display the current APBS version.\n\
----------------------------------------------------------------------\n\n"};
    
	printf("hello world\n");
	return 0;
}
