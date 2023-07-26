#ifdef __cplusplus
extern "C" {
#endif

#define CRC_USE_PRECALCULATED_TABLE

#if !defined(CRC_USE_PRECALCULATED_TABLE)
void crc32_make_table(void);
#endif

unsigned int crc32_init();
unsigned int crc32_update(unsigned int crc, unsigned char *buf, size_t len);
unsigned int crc32_finalize(unsigned int crc);
unsigned int crc32_calc(unsigned char *buf, size_t len);

#ifdef __cplusplus
}
#endif
