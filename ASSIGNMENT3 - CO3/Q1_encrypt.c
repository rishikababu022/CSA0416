/* Encryption library, version 1 (original). Built as libencrypt.so or linked statically. */
#include "Q1_encrypt.h"
const char *enc_version(void) { return "v1 (original, XOR 0x5A)"; }
void encrypt_buf(char *buf, size_t n)
{
    for (size_t i = 0; i < n; i++) buf[i] ^= 0x5A;
}
