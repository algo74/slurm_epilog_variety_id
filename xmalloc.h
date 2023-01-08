#define xmalloc(x) malloc(x)
#define xfree(x) do{ void *p = x; if (p) free(p); }while(0)
