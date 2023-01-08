#include <string.h>
inline static char *xstrdup(const char *p)
{
  if (p)
    return strdup(p);
  else
    return NULL;
}
