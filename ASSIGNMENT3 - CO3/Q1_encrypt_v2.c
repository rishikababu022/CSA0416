/* Encryption library, version 2 = the "security fix". Same interface, new behaviour. */
#include "Q1_encrypt.h"
const char *enc_version(void) { return "v2 (SECURITY FIX, XOR 0xA5)"; }
void encrypt_buf(char *buf, size_t n)
{
    for (size_t i = 0; i < n; i++) buf[i] ^= 0xA5;
}
