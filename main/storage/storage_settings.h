#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <stddef.h>

void save_machine_name(const char *name);
void load_machine_name(char *buf, size_t buf_len);

#endif
