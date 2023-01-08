#define LOG(level, fmt, ...)
#define info(fmt, ...) fprintf(stderr, "INFO: " fmt "\n", __VA_ARGS__)
#define error(fmt, ...) fprintf(stderr, "ERROR: " fmt "\n", __VA_ARGS__)
#define debug5(fmt, ...) fprintf(stderr, "debug5: " fmt "\n", __VA_ARGS__)
