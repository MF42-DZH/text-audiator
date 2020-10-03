#ifndef TEXTPARSEOPERATIONS_H_
#define TEXTPARSEOPERATIONS_H_

#define  _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELAY_NANOSECONDS 'n'
#define DELAY_SAMPLES     's'

#define IO_SUCCESS 0
#define IO_FAILURE 1

static const char WN32_LINE_TERMINATOR[ 3 ] = { '\r', '\n', '\0' };
static const char UNIX_LINE_TERMINATOR[ 3 ] = { '\n', '\0', '\0' };

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    static char LINE_TERMINATOR[ 3 ] = { '\r', '\n', '\0' };
#else
    static char LINE_TERMINATOR[ 3 ] = { '\n', '\0', '\0' };
#endif

// Verify line terminator
static void DETECT_LINE_TERMINATOR( char* line_of_text ) {
    int len = strlen( line_of_text );

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    if ( line_of_text[ len - 3 ] != '\r' ) {
        LINE_TERMINATOR[ 0 ] = '\n';
        LINE_TERMINATOR[ 1 ] = '\0';
    }
#else
    if ( line_of_text[ len - 3 ] == '\r' ) {
        LINE_TERMINATOR[ 0 ] = '\r';
        LINE_TERMINATOR[ 1 ] = '\n';
    }
#endif
}

/*
 * Look, I have no idea how large your text file lines are or how many characters
 * you are trying to fit inside one line of a text box, but goddamn if you somehow
 * break this limit you are either insane, doing something unique or are just sh##
 * at game design and / or writing dialogue for dialogue boxes.
 */
static long unsigned int MAX_LINE_SIZE = 1024;
#define __MAX_LINE_SIZE 1024

// Lack of argp.h
typedef int error_t;

// Line and info storage
typedef struct {
    char** lines;
    uint32_t line_count;

    uint32_t delay_length_for_letter;
    uint32_t delay_length_for_punctuation;
    uint32_t delay_length_for_newline;
    char delay_mode;
} text_info_t;

// Parse text file
int parse_text_file( FILE* file, text_info_t* info );

// Free file memory
static void free_text_file( text_info_t* info ) {
    if ( info->lines != NULL ) free( info->lines );
}

#endif // TEXTPARSEOPERATIONS_H_
