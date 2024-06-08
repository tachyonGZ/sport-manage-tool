#pragma once

#include "pch.h"
#include "score.h"
#include "message.h"
#include "input.h"

void process_add_score();
void process_remove_score();
void process_rewrite_score();
void process_query_score();
void process_query_all_score();
void process_filter_score();

void process_score_menu();
void process_score_menu_interface();