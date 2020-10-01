#include <stdio.h>
#include <argp.h>
#include "wavhandler.h"

int main( int argc, char** argv ) {
    return argp_parse( NULL, argc, argv, 0, 0, 0 );
}
