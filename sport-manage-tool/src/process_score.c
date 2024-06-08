#include "../inc/pch.h"

#include "../inc/process_score.h"

inline static void _send_menu_msg()
{
	send_message(M_SCORE_MENU, NULL, 0);
}

void process_score_menu()
{
	_print_split();

	fputs("��������Ҫ���еĲ���\n", stdout);

	switch(get_user_input_by_str()[0])
	{
	case '1':
		send_message(M_SCORE_ADD, NULL, 0);
		break;
	case '2':
		send_message(M_SCORE_REMOVE, NULL, 0);
		break;
	case '3':
		send_message(M_SCORE_REWRITE, NULL, 0);
		break;
	case '4':
		send_message(M_SCORE_QUERY, NULL, 0);
		break;
	case '5':
		send_message(M_SCORE_QUERY_ALL, NULL, 0);
		break;
	case '6':
		send_message(M_SCORE_FILTER, NULL, 0);
		break;
	case 'q':
	case 'Q':
		send_message(M_GOBACK, NULL, 0);
		break;
	default:
		fputs("����Ч�����룡�����ԡ�\n", stdout);
		_send_menu_msg();
	}
}

void process_score_menu_interface()
{
	fputs(
		"��������\n" \
		"�������ɼ�����˵�\n"\
		"��������\n" \
		"��1.��Ӳ����ɼ�\n" \
		"��2.ɾ�������ɼ�\n"	\
		"��3.�޸Ĳ����ɼ�\n" \
		"��4.��ѯ�����ɼ�\n"	\
		"��5.��ѯ���в����ɼ�\n" \
		"��6.ɸѡ��ѯ�˶�Ա��Ϣ\n" \
		"��������\n" \
		"����ʾ������Q��q���Է�����һ���˵�\n" \
		"��������\n" \
	, stdout);

	_send_menu_msg();
}

bool _print_score_res(const DATA_RES_t res)
{
	static const char *mapScoreRes[DATA_RES_MAX] = {
		[DATA_RES_SUCCESS] = "�������ɹ���\n",
		[DATA_RES_FAIL] = "������ʧ�ܣ�\n",
		[DATA_RES_STORE_FAIL] = "������ʧ�ܣ�\n",
		[DATA_RES_UNLOADED] = "������δ���أ�\n",
		[DATA_RES_KEY_CONFLICT] = "��������˶�Ա��źͱ�����Ŀ���������Ѵ��ڵ���ϳ�ͻ��\n",
		[DATA_RES_INVALID_KEY] = "����Ч���˶�Ա��Ż������Ŀ��ţ�\n",
	};

	if (res < DATA_RES_MAX)
	{
		fputs(mapScoreRes[res], stdout);
	}

	return DATA_RES_SUCCESS == res;
}

void process_add_score()
{
	fputs( \
		"��������Ҫ��ӵı����ɼ�\n" \
		"����ʽ:<�˶�Ա���> <������Ŀ���> <�ɼ�> <����> <�Ƿ����>\n" \
		"������:001 002 3��45�� �ڶ��� �Ѳ���\n" \
	, stdout);

	// ����������ݳ�ʼ��
	SCORE score;
	if(5 == sscanf(get_user_input_by_str(), "%hd %hd %s %s %s", &score.athleteId, &score.matchId, score.szResult, score.szRank, score.szparticipation))
	{
		if(_print_score_res(add_score(&score)))
		{
			fputs("��¼������ɼ��ɹ����ñ�����Ŀ���������޸ĳɹ���\n", stdout);
		}
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_remove_score()
{
	fputs( \
		"��������Ҫɾ���ı����ɼ���Ӧ���˶�Ա��źͱ�����Ŀ��ŵ����\n" \
		"����ʽ��<�˶�Ա���> <������Ŀ���>\n" \
	, stdout);


	athleteId_t athleteId = 0;
	matchId_t matchId= 0;
	if(2 == sscanf(get_user_input_by_str(), "%hd %hd", &athleteId, &matchId))
	{
		_print_score_res(remove_score(athleteId, matchId));
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_rewrite_score()
{
	fputs( \
		"��������Ҫ�޸ĵı����ɼ�\n" \
		"����ʽ:<�˶�Ա���> <������Ŀ���> <�ɼ�> <����> <�Ƿ����>\n" \
		"������:001 002 3��45�� �ڶ��� �Ѳ���\n" \
	, stdout);

	SCORE score;
	if(5 == sscanf(get_user_input_by_str(), "%hd %hd %s %s %s", &score.athleteId, &score.matchId, score.szResult, score.szRank, score.szparticipation))
	{
		_print_score_res(rewrite_score(&score));
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void _print_score(const SCORE *pScore, void *pIgnore)
{
	// �� �� ������ �����ȩЩة�

	printf( \
		"����������\n" \
		"�����˶�Ա��ţ�%hd\n" \
		"����������Ŀ��ţ�%hd\n" \
		"�����ɼ�: %s\n" \
		"�������Σ�%s\n" \
		"�����Ƿ������%s\n" \
		, pScore->athleteId, pScore->matchId, pScore->szResult, pScore->szRank, pScore->szparticipation
	);
}

void process_query_score()
{
	fputs( \
		"��������Ҫ��ѯ�ı����ɼ���Ӧ���˶�Ա��źͱ�����Ŀ��ŵ����\n" \
		"����ʽ��<�˶�Ա���> <������Ŀ���>\n" \
	, stdout);

	athleteId_t athleteId = 0;
	matchId_t matchId = 0;
	if(2 == sscanf(get_user_input_by_str(), "%hd %hd", &athleteId, &matchId))
	{
		const SCORE *pScore = NULL;

		if (_print_score_res(query_score(athleteId, matchId, &pScore)))
		{
			_print_score(pScore, NULL);
		}
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_query_all_score()
{
	_print_score_res(enum_score(_print_score, NULL));

	_send_menu_msg();
}

typedef enum {
	_ATHLETE_ID = 1 << 0,
	_MATCH_ID = 1 << 1,
	_RESULT = 1 << 2,
	_RANK = 1 << 3,
	_PARTICIPATION = 1 << 4,
} _FILTER_SCORE_IGNORE_FLAG_MASK;

typedef uint32_t _FILTER_SCORE_IGNORE_FLAG;

inline  _FILTER_SCORE_IGNORE_FLAG_MASK _set_filter_score_ignore_mask(const _FILTER_SCORE_IGNORE_FLAG_MASK mask, const char *szInput)
{
	return ('*' != szInput[0])?(mask):(0);
}

_FILTER_SCORE_IGNORE_FLAG _make_filter_score_ignore_flag(const char *szAthleteId, const char *szMatchId, const SCORE *pScore)
{
	_FILTER_SCORE_IGNORE_FLAG ignoreFlag = 0;
	ignoreFlag |= _set_filter_score_ignore_mask(_ATHLETE_ID, szAthleteId);
	ignoreFlag |= _set_filter_score_ignore_mask(_MATCH_ID, szMatchId);
	ignoreFlag |= _set_filter_score_ignore_mask(_RESULT, pScore->szResult);
	ignoreFlag |= _set_filter_score_ignore_mask(_RANK, pScore->szRank);
	ignoreFlag |= _set_filter_score_ignore_mask(_PARTICIPATION, pScore->szparticipation);
	return ignoreFlag;
}


typedef struct {
	const SCORE *pScore;
	const _FILTER_SCORE_IGNORE_FLAG ignoreFlag;
} _FILTER_SCORE_ENUM_PARAM;

void _filter_score_enum_proc(const SCORE *pScore, const _FILTER_SCORE_ENUM_PARAM *pParam)
{
	if ( (pParam->ignoreFlag & _ATHLETE_ID) && (pParam->pScore->athleteId != pScore->athleteId) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _MATCH_ID) && (pParam->pScore->matchId != pScore->matchId) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _RESULT) && (0 != strcmp(pParam->pScore->szResult, pScore->szResult)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _RANK) && (0 != strcmp(pParam->pScore->szRank, pScore->szRank)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _PARTICIPATION) && (0 != strcmp(pParam->pScore->szparticipation, pScore->szparticipation)) )
	{
		return;
	}
	else
	{
		_print_score(pScore, NULL);
	}

	return;
}

void process_filter_score()
{
	fputs( \
		"��������ɸѡ����\n" \
		"����ʽ:<�˶�Ա���> <������Ŀ���> <�ɼ�> <����> <�Ƿ����>\n" \
		"�����Ե�ɸѡ�������*���\n"\
		"������:* 001 * ��һ�� *\n" \
	, stdout);

	char szAthleteId[8];
	char szMatchId[8];
	SCORE score;
	if(5 != sscanf(get_user_input_by_str(), "%s %s %s %s %s", szAthleteId, szMatchId, score.szResult, score.szRank, score.szparticipation))
	{
		fputs("�������ʽ����\n", stdout);
		_send_menu_msg();
		return;
	}

	_FILTER_SCORE_IGNORE_FLAG ignoreFlag = _make_filter_score_ignore_flag(szAthleteId, szMatchId, &score);

	if (ignoreFlag & _ATHLETE_ID)
	{
		score.athleteId = atoi(szAthleteId);

		if(errno)
		{
			fputs("���˶�Ա��Ÿ�ʽ����\n", stdout);
			_send_menu_msg();
			return;
		}
	}

	if (ignoreFlag & _MATCH_ID)
	{
		score.matchId = atoi(szMatchId);

		if(errno)
		{
			fputs("��������Ŀ��Ÿ�ʽ����\n", stdout);
			_send_menu_msg();
			return;
		}
	}


	_FILTER_SCORE_ENUM_PARAM enumParam = {
		.pScore = &score,
		.ignoreFlag = ignoreFlag
	};

	_print_score_res(enum_score(_filter_score_enum_proc, &enumParam));
	_send_menu_msg();
}