#ifndef KEYBOARD_H__
#define KEYBOARD_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define KEYB_DEV_PATH                       "/dev/keyboard"

int button_read_value(uint8_t *value);

#endif