#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "keyboard.h"

int button_read_value(uint8_t *value)
{
    int ret;
    int fd;

    fd = open(KEYB_DEV_PATH, O_RDWR);
    if (fd < 0)
        printf("Open error\n");

    ret = read(fd, value, 1);
    close(fd);
    return ret;
}