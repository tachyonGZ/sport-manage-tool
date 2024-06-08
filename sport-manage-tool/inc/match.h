#pragma once

#include "pch.h"
#include "list_data.h"

typedef uint16_t matchKey_t;
typedef matchKey_t matchId_t;

typedef struct{
	matchKey_t id;
	char szName[64];
	char szGender[8];
	char szTime[64];
	char szLocation[64];
	char szStatus[64];
} MATCH;

void clear_matches();
bool load_matches();

DATA_RES_t add_match(const MATCH *pMatch);
DATA_RES_t remove_match(const matchKey_t iMatchId);
DATA_RES_t rewrite_match(const MATCH *pMatch);
DATA_RES_t query_match(const matchKey_t iMatchId, const MATCH **ppMatch);
bool is_match_exist(const matchId_t matchId);

typedef void (*MATCH_ENUM_PROC)(const MATCH *pMatch, void *pParam);
DATA_RES_t enum_match(MATCH_ENUM_PROC fnEnumProc, void *pParam);

LIST *get_matches_internal();