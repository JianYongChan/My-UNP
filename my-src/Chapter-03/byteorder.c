#include "unp.h"

int
main(int argc, char **argv)
{
    // 在s中存储两字节的值0x0101
    // 然后查看它的两个连续字节c[0]和c[1]以此来确定字节序
    union {
        short s;
        char  c[sizeof(short)];
    } un;

    un.s = 0x0102;
    printf("%s: ", CPU_VENDOR_OS);
    if (sizeof(short) == 2) {
        if (un.c[0] == 0x01 && un.c[1] == 0x02)
            printf("big endian\n");
        else if (un.c[0] == 0x02 && un.c[1] == 0x01)
            printf("little endian\n");
        else
            printf("unknown\n");
    } else {
        printf("sizeof(short) = %ld\n", sizeof(short));
    }
}
