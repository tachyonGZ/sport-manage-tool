#pragma once

#include "pch.h"
#include "file.h"

typedef struct {
	const char szPassword[32];
} PASSWORD;

bool load_password();
void clear_password();

bool is_password_right(const char *szPassword);
bool reset_password(const char *szPassword);
const size_t get_password_max_length();