#include <fcntl.h>
#include <linux/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"

/* Code for working with a Hamming(7,4) code with extra parity bit. */
const unsigned char HAMMING[] = {
    0b00000000,
    0b11010010,
    0b01010101,
    0b10000111,
    0b10011001,
    0b01001011,
    0b11001100,
    0b00011110,
    0b11100001,
    0b00110011,
    0b10110100,
    0b01100110,
    0b01111000,
    0b10101010,
    0b00101101,
    0b11111111
};

/* Convert the lower 4 bytes of nibble into 8 bytes, representing the Hamming
 * encoding.
 */
unsigned char nibble_to_hamming(unsigned char nibble)
{
    return HAMMING[nibble];
}

/* Convert the data in the given array to its Hamming-encoded version of length
 * (2*length).
 */
unsigned char *data_to_hamming(unsigned char *data, size_t length)
{
    size_t i;
    unsigned char *hamming = calloc(length * 2, sizeof(char));

    for (i = 0; i < length; i++) {
        hamming[2*i] = (unsigned char) nibble_to_hamming(data[i] >> 4);
        hamming[2*i+1] = (unsigned char) nibble_to_hamming(data[i] & 0x0f);
    }

    return hamming;
}

/* Convert a char to a char[8], where the values are 1 or 0 corresponding to the
 * bits in the original char.
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

/* Send a message, converted to its Hamming coding and with a special prefix,
 * over GPIO.
 */
main(int argc, char **argv)
{
    unsigned char prefix[] = { 0xaa, 0xaa, 0xaa, 0xaa, 0xcc, 0xcc};
    int prefix_len = sizeof(prefix) / sizeof(char);
    unsigned char *data = argv[1];
    unsigned char *hamming, *message, *buf;
    int length = strlen(data);
    int fd, i, j;

    // Make sure to 'mknod /dev/gpio c 60 0'
    // May also need to 'chmod 666 /dev/gpio'
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

    // Each byte takes 2 bytes to transmit Hamming-coded
    // Each packet has a prefix of fixed length followed by a 1-byte size field
    hamming = data_to_hamming(data, length);
    message = malloc(sizeof(char) * (length * 2 + prefix_len + 1));
    memcpy(message, prefix, prefix_len);
    message[prefix_len] = (char) length;
    memcpy(message + prefix_len + 1, hamming, length * 2);
    buf = malloc(sizeof(char *) * 8 * (length * 2 + prefix_len + 1));

    for (i = 0; i < length * 2 + prefix_len + 1; ++i) {
        char *tmp = get_byte(message[i]);

        for (j = 0; j < 8; j++) {
            buf[i*8+j] = tmp[j];
        }

        free(tmp);
    }

    ret_val = write(fd, buf, 8 * (length * 2 + prefix_len + 1));

    free(message);
    close(fd);
}
