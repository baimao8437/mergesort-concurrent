#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>

#include "alignAndMap.h"

#define MAX_BUFF_SIZE 100
#define MAX_LAST_NAME_SIZE 16
#define ALIGN_FILE "align.txt"

#define FILE_SIZE file_len(ALIGN_FILE)

/**
 * @brief Get the size of the file in bytes.
 */
static off_t file_len(const char *path)
{
    struct stat st;
    stat(path, &st);
    return st.st_size;
}

/**
 * @brief Pad each line of the text file _origiFileName_
 *  to _PadToLen_ characters and write to the file _alignedFileName_.
 */
static int text_align(const char *file_orig,
                      const char *file_align,
                      const int padToLen)
{
    printf("orginal file size = %ld\n", file_len(file_orig));

    FILE *fd_origin = fopen(file_orig, "r");
    if (!fd_origin) return -1;
    FILE *fd_align = fopen(file_align, "w+");
    if (!fd_align) {
        fclose(fd_origin);
        return -1;
    }

    char rbuf[MAX_BUFF_SIZE];
    int suffix;

    char *wbuf = (char *) malloc(sizeof(char) * padToLen);

    /* Read each line from input file and pad to "padToLen" characters. */
    while (fgets(rbuf, sizeof(rbuf), fd_origin)) {
        memset(wbuf, '\0', padToLen);

        suffix = padToLen - strlen(rbuf);
        /*
         * The length of input text is longer than the length to padding to.
         * Warn the user, and still write to the output file but with only
         * first "PadToLen" bytes.
         */
        if (suffix < 0)
            printf("Warning:"
                   " The length of %s is longer than %d.\n", rbuf, padToLen);
        strncpy(wbuf, rbuf, strlen(rbuf));

        fwrite(wbuf, padToLen, 1, fd_align);
    }

    fclose(fd_align);
    fclose(fd_origin);
    free(wbuf);

    return 0;
}

char *alignAndMap(char const fileName[])
{
    char *map;
    text_align(fileName, ALIGN_FILE, MAX_LAST_NAME_SIZE);
    int fd = open(ALIGN_FILE, O_RDONLY | O_NONBLOCK);

    map = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    assert(map && "mmap error");

    close(fd);
    return map;
}

int total_line()
{
    return FILE_SIZE / MAX_LAST_NAME_SIZE;
}

char *ptrLine(char *map , int line)
{
    return map + MAX_LAST_NAME_SIZE * line;
}

void release(char *map)
{
    /* Remove the align.txt file */
    assert(!remove(ALIGN_FILE) && "File remove falid");

    munmap(map, FILE_SIZE);
}

/* API gateway */
struct __MMAP_API__ MMap = {
    .total_line = total_line,
    .alignAndMap = alignAndMap,
    .ptrLine = ptrLine,
    .release = release,
};