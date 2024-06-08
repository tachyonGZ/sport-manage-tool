#pragma once

#pragma once

#include "pch.h"
#include "match.h"
#include "message.h"
#include "input.h"

void process_add_match();
void process_remove_match();
void process_rewrite_match();
void process_query_match();
void process_query_all_match();
void process_filter_match();

void process_match_menu();
void process_match_menu_interface();