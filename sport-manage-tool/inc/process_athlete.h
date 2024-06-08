#pragma once

#include "pch.h"
#include "athlete.h"
#include "message.h"
#include "input.h"

void process_add_athlete();
void process_remove_athlete();
void process_rewrite_athlete();
void process_query_athlete();
void process_query_all_athlete();
void process_filter_athlete();

void process_athlete_menu();
void process_athlete_menu_interface();