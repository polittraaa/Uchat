#include "libmx.h"

void mx_printerr(const char* errortext) {
     write(2, errortext, mx_strlen(errortext));
}
