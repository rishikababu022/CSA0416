#ifndef Q1_ENCRYPT_H
#define Q1_ENCRYPT_H
#include <stddef.h>
const char *enc_version(void);
void encrypt_buf(char *buf, size_t n);
#endif
