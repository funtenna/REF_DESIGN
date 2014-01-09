#include "gpio.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/ioctl.h>

/* Convert a char into a char[8], where each char in the array is a 1 or 0
 * corresponding to the bits in the given char.
 */
char *get_byte(char byte)
{
    int i;
    char *buf = malloc(sizeof(char) * 8);

    for (i = 0; i < 8; i++) {
        buf[i] = byte >> (8 - 1 - i) & 0x1;
    }

    return buf;
}

/* Broadcast a repeated 10101010 pattern from GPIO pin 38. */
main(int argc, char **argv)
{
    char byte = -86; // 10101010
    char *buf;
    int length = 10;
    int fd, i, j;

    // Make sure to 'mknod /dev/gpio c 60 0'
    // may also need to 'chmod 666 /dev/gpio'
    fd = open("/dev/gpio", O_RDWR | O_SYNC);

    if (fd < 0) {
        printf("Can't open device file\n");
        exit(-1);
    }

    int ret_val = ioctl(fd, IOC_SET, 200000);
    if (ret_val < 0) {
        printf("set cycles failed:%d\n", ret_val);
        exit(-1);
    }

    buf = malloc(sizeof(char *) * 8 * length);
    for (i = 0; i < length + 2; ++i) {
        char *tmp = get_byte(byte);

        for (j = 0; j < 8; j++) {
            buf[i*8+j] = tmp[j];
        }

        free(tmp);
    }

    for(;;) {
        ret_val = write(fd, buf, 8 * length);
    }

    free(buf);
    close(fd);
}
