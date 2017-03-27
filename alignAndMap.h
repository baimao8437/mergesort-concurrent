#ifndef __TEXT_ALIGN_
#define __TEXT_ALIGN_

extern struct __MMAP_API__ {
    int (*total_line)();
    char *(*alignAndMap)(char const fileName[]);
    char *(*ptrLine)(char *map, int line);
    void (*release)(char *map);
} MMap;

#endif // __TEXT_ALIGN_
