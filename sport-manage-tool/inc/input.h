#pragma once

#include "pch.h"

static char *g_pInput;
static size_t g_inputMaxCount;

void release_user_input();
bool init_user_input(const size_t maxBufferSize);
const char *get_user_input_by_str();

int _print_split();