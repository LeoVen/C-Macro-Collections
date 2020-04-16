# futils.h

Contains common functions that can be used by Functions Table, instead of writing another one every time.

A quick list of functions present in this file:

| cmp |
| ----------- |
| `static inline int cmc_i64_cmp(int64_t x1, int64_t x2);`      |
| `static inline int cmc_i32_cmp(int32_t x1, int32_t x2);`      |
| `static inline int cmc_i16_cmp(int16_t x1, int16_t x2);`      |
| `static inline int cmc_i8_cmp(int8_t x1, int8_t x2);`         |
| `static inline int cmc_u64_cmp(uint64_t x1, uint64_t x2);`    |
| `static inline int cmc_u32_cmp(uint32_t x1, uint32_t x2);`    |
| `static inline int cmc_u16_cmp(uint16_t x1, uint16_t x2);`    |
| `static inline int cmc_u8_cmp(uint8_t x1, uint8_t x2);`       |
| `static inline int cmc_size_cmp(size_t x1, size_t x2);`       |
| `static inline int cmc_imax_cmp(intmax_t x1, intmax_t x2);`   |
| `static inline int cmc_umax_cmp(uintmax_t x1, uintmax_t x2);` |
| `static inline int cmc_float_cmp(float x1, float x2);`        |
| `static inline int cmc_double_cmp(double x1, double x2);`     |
| `static inline int cmc_str_cmp(char *ch1, char *ch2);`        |

<br>
<br>

| cpy |
| --- |
| `static inline char *cmc_str_cpy(char *str);` |

<br>
<br>

| str |
| --- |
| `static inline bool cmc_i8_str(FILE *file, int8_t element);`      |
| `static inline bool cmc_i16_str(FILE *file, int16_t element);`    |
| `static inline bool cmc_i32_str(FILE *file, int32_t element);`    |
| `static inline bool cmc_i64_str(FILE *file, int64_t element);`    |
| `static inline bool cmc_u8_str(FILE *file, uint8_t element);`     |
| `static inline bool cmc_u16_str(FILE *file, uint16_t element);`   |
| `static inline bool cmc_u32_str(FILE *file, uint32_t element);`   |
| `static inline bool cmc_u64_str(FILE *file, uint64_t element);`   |
| `static inline bool cmc_size_str(FILE *file, size_t element);`    |
| `static inline bool cmc_imax_str(FILE *file, intmax_t element);`  |
| `static inline bool cmc_umax_str(FILE *file, uintmax_t element);` |
| `static inline bool cmc_float_str(FILE *file, float element);`    |
| `static inline bool cmc_double_str(FILE *file, double element);`  |
| `static inline bool cmc_str_str(FILE *file, char *element);`      |

<br>
<br>

| free |
| --- |

<br>
<br>

| hash |
| ---- |
| `static inline size_t cmc_i64_hash(int64_t e);`                  |
| `static inline size_t cmc_i32_hash(int32_t e);`                  |
| `static inline size_t cmc_i16_hash(int16_t e);`                  |
| `static inline size_t cmc_i8_hash(int8_t e);`                    |
| `static inline size_t cmc_u64_hash(uint64_t e);`                 |
| `static inline size_t cmc_u32_hash(uint32_t e);`                 |
| `static inline size_t cmc_u16_hash(uint16_t e);`                 |
| `static inline size_t cmc_u8_hash(uint8_t e);`                   |
| `static inline size_t cmc_size_hash(size_t e);`                  |
| `static inline size_t cmc_imax_hash(intmax_t e);`                |
| `static inline size_t cmc_umax_hash(uintmax_t e);`               |
| `static inline size_t cmc_float_hash(float e);`                  |
| `static inline size_t cmc_double_hash(double e);`                |
| `static inline size_t cmc_str_hash_djb2(char *str);`             |
| `static inline size_t cmc_str_hash_sdbm(char *str);`             |
| `static inline size_t cmc_str_hash_java(char *str);`             |
| `static inline size_t cmc_str_hash_murmur3(uint64_t e);`         |
| `static inline size_t cmc_str_hash_murmur3_variant(uint64_t e);` |
| `static inline size_t cmc_i64_hash_mix(int64_t element);`        |
| `static inline size_t cmc_u64_hash_mix(uint64_t element);`       |

<br>
<br>

| pri |
| --- |
