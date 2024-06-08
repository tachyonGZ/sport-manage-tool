#pragma once

#include "pch.h"

typedef enum {
	M_EXIT = 0,
	M_EXIT_PRE = 1,
	M_STARTUP,
	M_GOBACK,

	M_WELCOME_INTERFACE,

	M_PASSWORD_MENU_INTERFACE,
	M_PASSWORD_MENU,
	M_PASSWORD_LOGIN,
	M_PASSWORD_CHANGE,

	M_MAIN_MENU_INTERFACE,
	M_MAIN_MENU,
	
	M_ATHLETE_MENU_INTERFACE,
	M_ATHLETE_MENU,
	M_ATHLETE_ADD,
	M_ATHLETE_REMOVE,
	M_ATHLETE_REWRITE,
	M_ATHLETE_QUERY,
	M_ATHLETE_QUERY_ALL,
	M_ATHLETE_FILTER,

	M_MATCH_MENU_INTERFACE,
	M_MATCH_MENU,
	M_MATCH_ADD,
	M_MATCH_REMOVE,
	M_MATCH_REWRITE,
	M_MATCH_QUERY,
	M_MATCH_QUERY_ALL,
	M_MATCH_FILTER,

	M_SCORE_MENU_INTERFACE,
	M_SCORE_MENU,
	M_SCORE_ADD,
	M_SCORE_REMOVE,
	M_SCORE_REWRITE,
	M_SCORE_QUERY,
	M_SCORE_QUERY_ALL,
	M_SCORE_FILTER,

	M_STATISTICS_MENU_INTERFACE,
	M_STATISTICS_MENU,
	M_STATISTICS_ATHLETE,
	M_STATISTICS_MATCH,
	
	M_TABLE_MENU_INTERFACE,
	M_TABLE_MENU,
	M_TABLE_ATHLETE,
	M_TABLE_SCORE,

	MAX_MESSAGE_TYPE_NUM,
} message_t;

typedef int (*MSG_PROC)(const message_t msg, const void *pParam, const size_t paramSize);

typedef struct {
	bool bAlive;
	MSG_PROC fnMsgProc;
	message_t msg;
} MESSAGE_QUEUE;

static MESSAGE_QUEUE g_msgQueue;

bool starup_message_queue(MSG_PROC fnMsgProc);

void exit_message_queue();

bool get_message(message_t *pMsg);

bool send_message(const message_t msg, void *pParam, const size_t paramSize);

bool dispatch_message(const message_t msg);