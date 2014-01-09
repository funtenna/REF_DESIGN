#include <linux/ioctl.h>

/* A non-conflicting number. */
#define IOC_MAGIC 60

/* Set the number of cycles per bit. */
#define IOC_SET _IOW(IOC_MAGIC, 1, int)

/* Only one ioctl option. */
#define IOC_MAXNR 1
