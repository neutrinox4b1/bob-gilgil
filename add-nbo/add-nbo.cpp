#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <stdlib.h>

uint32_t func(char *filename)
{
    FILE *fp;
    uint32_t res;

    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "invalid file\n");
        exit(1);
    }

    fread(&res, sizeof(uint32_t), 1, fp);
    fclose(fp);

    return htonl(res);
}


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "syntax: add-nbo <file1> <file2>\n");
        fprintf(stderr, "sample: add-nbo a.bin b.bin\n");
        exit(1);
    }
    uint32_t arg1 = func(argv[1]);
    uint32_t arg2 = func(argv[2]);
    uint32_t res = arg1 + arg2;

    printf("%d(0x%x) + %d(0x%x) = %d(0x%x)", arg1, arg1, arg2, arg2, res, res);

    return 0;
}
