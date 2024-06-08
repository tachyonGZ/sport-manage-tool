#include "../inc/pch.h"
#include "../inc/match.h"

LIST_DATA *g_pMatches = NULL;

static const char *g_szTempFileName = ".\\match.tmp";
static const char *g_szFileName = ".\\match.dat";

void clear_matches()
{
	if(g_pMatches)
	{
		clear_list_data(g_pMatches);
	}
}

bool load_matches()
{
	if (g_pMatches)
	{
		abort();
	}

	g_pMatches = load_list_data(sizeof(MATCH), g_szFileName, g_szTempFileName);

	return NULL != g_pMatches;
}

bool store_matches()
{
	if (NULL == g_pMatches)
	{
		abort();
	}

	return store_list_data(g_pMatches);
}

DATA_RES_t add_match(const MATCH *pMatch)
{
	return list_data_add(g_pMatches, pMatch->id, pMatch);
}

DATA_RES_t remove_match(const matchKey_t iMatchId)
{
	return list_data_remove(g_pMatches, iMatchId);
}

DATA_RES_t rewrite_match(const MATCH *pMatch)
{
	return list_data_rewrite(g_pMatches, pMatch->id, pMatch);
}

DATA_RES_t query_match(const matchKey_t iMatchId, const MATCH **ppMatch)
{
	return list_data_query(g_pMatches, iMatchId, ppMatch);
}

bool is_match_exist(const matchId_t matchId)
{
	return list_data_is_exist(g_pMatches, matchId);
}

DATA_RES_t enum_match(MATCH_ENUM_PROC fnEnumProc, void *pParam)
{
	return list_data_enum(g_pMatches, fnEnumProc, pParam);
}

LIST *get_matches_internal()
{
	return list_data_get_internal(g_pMatches);
}