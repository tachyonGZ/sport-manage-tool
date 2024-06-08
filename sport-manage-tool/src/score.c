#include "../inc/pch.h"
#include "../inc/score.h"

LIST_DATA *g_pScoreData = NULL;

static const char *g_szTempFileName = ".\\score.tmp";
static const char *g_szFileName = ".\\score.dat";

void clear_score_data()
{
	if(g_pScoreData)
	{
		clear_list_data(g_pScoreData);
	}
}

bool load_score_data()
{
	if (g_pScoreData)
	{
		abort();
	}

	g_pScoreData = load_list_data(sizeof(SCORE), g_szFileName, g_szTempFileName);

	return NULL != g_pScoreData;
}

bool store_score_data()
{
	if (NULL == g_pScoreData)
	{
		abort();
	}

	return store_list_data(g_pScoreData);
}

scoreKey_t _make_score_key(athleteId_t athleteId, matchId_t matchId)
{
	return (athleteId << 16) | (matchId);
}

DATA_RES_t add_score(const SCORE *pScore)
{	
	const MATCH *pMatchSrc = NULL;
	if (!is_athlete_exist(pScore->athleteId) || DATA_RES_SUCCESS != query_match(pScore->matchId, &pMatchSrc))
	{
		return DATA_RES_INVALID_KEY;
	}

	scoreKey_t scoreKey = _make_score_key(pScore->athleteId, pScore->matchId);

	DATA_RES_t res = list_data_add(g_pScoreData, scoreKey, pScore);	

	if (DATA_RES_SUCCESS != res)
	{
		return res;	
	}

	if (0 == strcmp("Î´Èü", pMatchSrc->szStatus))
	{
		MATCH matchDst;
		if (NULL == memcpy(&matchDst, pMatchSrc, sizeof(MATCH)))
		{
			abort();
		}

		if (NULL == strcpy(matchDst.szStatus, "ÒÑÈü"))
		{
			abort();
		}

		if (DATA_RES_SUCCESS != rewrite_match(&matchDst))
		{
			abort();
		}
	}

	return res;
}


DATA_RES_t remove_score(const athleteId_t athleteId, const matchId_t matchId)
{
	scoreKey_t scoreKey = _make_score_key(athleteId, matchId);
	return list_data_remove(g_pScoreData, scoreKey);
}

DATA_RES_t rewrite_score(const SCORE *pScore)
{
	return list_data_rewrite(g_pScoreData, _make_score_key(pScore->athleteId, pScore->matchId), pScore);
}

DATA_RES_t query_score(const athleteKey_t athleteId, const matchId_t matchId, const SCORE **ppScore)
{
	return list_data_query(g_pScoreData, _make_score_key(athleteId, matchId), ppScore);
}

DATA_RES_t enum_score(SCORE_ENUM_PROC fnEnumProc, void *pParam)
{
	return list_data_enum(g_pScoreData, fnEnumProc, pParam);
}

LIST *get_score_data_internal()
{
	return list_data_get_internal(g_pScoreData);
}


bool is_athlete_in_score(const athleteId_t athleteId)
{
	LIST_ITERATOR *pIter = get_list_begin(get_score_data_internal());
	LIST_ITERATOR *pLast = get_list_last(get_score_data_internal());

	for (; !is_list_iterator_equal(pIter, pLast); forward_list_iterator(pIter))
	{
		uint32_t iKey = get_list_iterator_key(pIter);
		if (athleteId == iKey >> 16)
		{
			return true;
		}
	}

	free_list_iterator(pIter);
	free_list_iterator(pLast);

	return false;
}


bool is_match_in_score(const matchId_t matchId)
{
	LIST_ITERATOR *pIter = get_list_begin(g_pScoreData->pList);
	LIST_ITERATOR *pLast = get_list_last(g_pScoreData->pList);

	for (; !is_list_iterator_equal(pIter, pLast); forward_list_iterator(pIter))
	{
		uint32_t iKey = get_list_iterator_key(pIter);
		if (matchId == ((iKey << 16) >> 16))
		{
			return true;
		}
	}

	free_list_iterator(pIter);
	free_list_iterator(pLast);

	return false;
}