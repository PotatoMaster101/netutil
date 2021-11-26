///////////////////////////////////////////////////////////////////////////////
// Helper functions for IO.
// https://github.com/PotatoMaster101/netutil
///////////////////////////////////////////////////////////////////////////////

#include "ioutil.h"

// Determines whether the specified file exists.
//
// PARAMS:
// f - the file to check
//
// RETURN:
// 1 if the file exists, 0 otherwise.
_Bool file_exist(const char *f) {
    FILE *fp = fopen(f, "r");
    _Bool ret = false;
    if (fp != NULL) {
        fclose(fp);
        ret = true;
    }
    return ret;
}

// Returns the file size in bytes.
//
// PARAMS:
// f - the file to retrieve the size
//
// RETURN:
// The file size in bytes. On error, -1 will be returned.
long file_size(const char *f) {
    FILE *fp = fopen(f, "r");
    long ret = -1;
    if (fp != NULL) {
        fseek(fp, 0, SEEK_END);
        ret = ftell(fp);
        fclose(fp);
    }
    return ret;
}

// Wipes the specified file.
//
// PARAMS:
// f - the file to wipe
void file_wipe(const char *f) {
    FILE *fp = fopen(f, "w");
    if (fp != NULL)
        fclose(fp);
}

// Promps user for an integer between the given lower and upper bounds.
//
// PARAMS:
// msg - the message to display for the user
// low - the lower bound to use
// up  - the upper bound to use
//
// RETURN:
// An integer between the lower and upper bound.
long prompt_int(const char *msg, long low, long up) {
    if (up < low) {
        up = up + low;
        low = up - low;
        up = up - low;
    }

    long ret = low - 1;
    fprintf(stdout, "%s", msg);
    fscanf(stdin, "%ld", &ret);
    while (ret < low || ret > up) {
        fprintf(stdout, "Invalid range (%ld to %ld).\n", low, up);
        clear_stdin();
        fscanf(stdin, "%ld", &ret);
    }
    return ret;
}

// Prompts user for a number between the given lower and upper bounds.
//
// PARAMS:
// msg - the message to display for the user
// low - the lower bound to use
// up  - the upper bound to use
//
// RETURN:
// A number between the lower and upper bound.
double prompt_double(const char *msg, double low, double up) {
    if (up < low) {
        up = up + low;
        low = up - low;
        up = up - low;
    }

    double ret = low - 1;
    fprintf(stdout, "%s", msg);
    fscanf(stdin, "%lf", &ret);
    while (ret < low || ret > up) {
        fprintf(stdout, "Invalid range (%lf to %lf).\n", low, up);
        clear_stdin();
        fscanf(stdin, "%lf", &ret);
    }
    return ret;
}

// Clears the stdin buffer.
void clear_stdin() {
    int c = 0;
    while ((c = fgetc(stdin)) != '\n' && c != EOF);
}
