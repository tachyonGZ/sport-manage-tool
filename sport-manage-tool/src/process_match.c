#include "../inc/pch.h"

#include "../inc/process_match.h"

inline static void _send_menu_msg()
{
	send_message(M_MATCH_MENU, NULL, 0);
}

void process_match_menu()
{
	_print_split();

	fputs("��������Ҫ���еĲ���\n", stdout);

	switch(get_user_input_by_str()[0])
	{
	case '1':
		send_message(M_MATCH_ADD, NULL, 0);
		break;
	case '2':
		send_message(M_MATCH_REMOVE, NULL, 0);
		break;
	case '3':
		send_message(M_MATCH_REWRITE, NULL, 0);
		break;
	case '4':
		send_message(M_MATCH_QUERY, NULL, 0);
		break;
	case '5':
		send_message(M_MATCH_QUERY_ALL, NULL, 0);
		break;
	case '6':
		send_message(M_MATCH_FILTER, NULL, 0);
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

void process_match_menu_interface()
{
	fputs(
		"��������\n" \
		"��������Ŀ����˵�\n"\
		"��������\n" \
		"��1.��ӱ�����Ŀ\n" \
		"��2.ɾ��������Ŀ\n"	\
		"��3.�޸ı�����Ŀ\n" \
		"��4.��ѯ������Ŀ\n"	\
		"��5.��ѯ���б�����Ŀ\n" \
		"��6.ɸѡ��ѯ������Ŀ\n" \
		"��������\n" \
		"����ʾ������Q��q���Է�����һ���˵�\n" \
		"��������\n" \
		, stdout);

	_send_menu_msg();
}

static bool _print_res(const DATA_RES_t res)
{
	static const char *mapMatchRes[DATA_RES_MAX] = {
		[DATA_RES_SUCCESS] = "�������ɹ���\n",
		[DATA_RES_FAIL] = "������ʧ�ܣ�\n",
		[DATA_RES_STORE_FAIL] = "������ʧ�ܣ�\n",
		[DATA_RES_UNLOADED] = "������δ���أ�\n",
		[DATA_RES_KEY_CONFLICT] = "������ı�����Ŀ������Ѵ��ڵı�����Ŀ��ų�ͻ��\n",
		[DATA_RES_INVALID_KEY] = "����Ч�ı�����Ŀ��ţ�\n",
	};

	if (res < DATA_RES_MAX)
	{
		fputs(mapMatchRes[res], stdout);
	}

	return DATA_RES_SUCCESS == res;
}

void process_add_match()
{
	fputs( \
		"��������Ҫ��ӵı�����Ŀ��Ϣ\n" \
		"����ʽ:<���> <����> <��Ŀ�Ա�> <����ʱ��> <�����ص�>\n" \
		"������:1 ��Զ �� 10:50AM �ٳ����Ÿ���\n" \
	, stdout);

	// ����������ݳ�ʼ��
	MATCH match;

	if(5 == sscanf(get_user_input_by_str(), "%hd %s %s %s %s", &match.id, match.szName, match.szGender, match.szTime, match.szLocation))
	{
		if (NULL == strcpy(match.szStatus, "δ��"))
		{
			abort();
		}

		_print_res(add_match(&match));
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_remove_match()
{
	fputs( \
		"��������Ҫɾ���ı�����Ŀ��Ϣ��Ӧ�ı�����Ŀ���\n" \
	, stdout);

	matchKey_t iMatchId = 0;

	if(1 == sscanf(get_user_input_by_str(), "%hd", &iMatchId))
	{
		_print_res(remove_match(iMatchId));
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_rewrite_match()
{
	fputs( \
		"��������Ҫ�޸ĵı�����Ŀ��Ϣ\n" \
		"����ʽ:<���> <����> <��Ŀ�Ա�> <����ʱ��> <�����ص�> <����>\n" \
		"������:1 ��Զ �� 10:50AM �ٳ����Ÿ��� ����\n" \
	, stdout);

	// ����������ݳ�ʼ��
	MATCH match;

	if(6 == sscanf(get_user_input_by_str(), "%hd %s %s %s %s %s", &match.id, match.szName, match.szGender, match.szTime, match.szLocation, match.szStatus))
	{
		_print_res(rewrite_match(&match));
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void _print_match(const MATCH *pMatch, const void *pIgnore)
{
	// �� �� ������ �����ȩЩة�

	printf( \
		"����������\n" \
		"������ţ�%d\n" \
		"�������ƣ�%s\n" \
		"������Ŀ�Ա�: %s\n" \
		"��������ʱ�䣺%s\n" \
		"���������ص㣺%s\n" \
		"����������%s\n" \
		, pMatch->id, pMatch->szName, pMatch->szGender, pMatch->szTime, pMatch->szLocation, pMatch->szStatus
	);
}

void process_query_match()
{
	fputs( \
		"��������Ҫ��ѯ�ı�����Ŀ��Ϣ��Ӧ�ı�����Ŀ���\n" \
	, stdout);

	matchKey_t iAthleteId = 0;
	if(1 == sscanf(get_user_input_by_str(), "%hd", &iAthleteId))
	{
		const MATCH *pMatch = NULL;

		if (_print_res(query_match(iAthleteId, &pMatch)))
		{
			_print_match(pMatch, NULL);
		}
	}	
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_query_all_match()
{
	_print_res(enum_match(_print_match, NULL));
	
	_send_menu_msg();
}

typedef enum {
	_ID = 1 << 0,
	_NAME = 1 << 1,
	_GENDER = 1 << 2,
	_TIME = 1 << 3,
	_LOCATION = 1 << 4,
	_STATUS = 1 << 5
} _FILTER_MATCH_IGNORE_FLAG_MASK;

typedef uint32_t _FILTER_MATCH_IGNORE_FLAG;

inline  _FILTER_MATCH_IGNORE_FLAG_MASK _set_filter_match_ignore_mask(const _FILTER_MATCH_IGNORE_FLAG_MASK mask, const char *szInput)
{
	return ('*' != szInput[0])?(mask):(0);
}

_FILTER_MATCH_IGNORE_FLAG _make_filter_match_ignore_flag(const char *szMatchId, const MATCH *pMatch)
{
	_FILTER_MATCH_IGNORE_FLAG ignoreFlag = 0;
	ignoreFlag |= _set_filter_match_ignore_mask(_ID, szMatchId);
	ignoreFlag |= _set_filter_match_ignore_mask(_NAME, pMatch->szName);
	ignoreFlag |= _set_filter_match_ignore_mask(_GENDER, pMatch->szGender);
	ignoreFlag |= _set_filter_match_ignore_mask(_TIME, pMatch->szTime);
	ignoreFlag |= _set_filter_match_ignore_mask(_LOCATION, pMatch->szLocation);
	ignoreFlag |= _set_filter_match_ignore_mask(_STATUS, pMatch->szStatus);
	return ignoreFlag;
}

typedef struct {
	const MATCH *pMatch;
	const _FILTER_MATCH_IGNORE_FLAG ignoreFlag;
} _FILTER_MATCH_ENUM_PARAM;

void _filter_match_enum_proc(const MATCH *pMatch, const _FILTER_MATCH_ENUM_PARAM *pParam)
{
	if ( (pParam->ignoreFlag & _ID) && (pParam->pMatch->id != pMatch->id) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _NAME) && (0 != strcmp(pParam->pMatch->szName, pMatch->szName)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _GENDER) && (0 != strcmp(pParam->pMatch->szGender, pMatch->szGender)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _TIME) && (0 != strcmp(pParam->pMatch->szTime, pMatch->szTime)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _LOCATION) && (0 != strcmp(pParam->pMatch->szLocation, pMatch->szLocation)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _STATUS) && (0 != strcmp(pParam->pMatch->szStatus, pMatch->szStatus)) )
	{
		return;
	}
	else
	{
		_print_match(pMatch, NULL);
	}

	return;
}

void process_filter_match()
{
	fputs( \
		"��������ɸѡ����\n" \
		"����ʽ:<���> <����> <��Ŀ�Ա�> <����ʱ��> <�����ص�> <����>\n" \
		"�����Ե�ɸѡ�������*���\n"\
		"������:* ��Զ �� * * *\n" \
		, stdout);

	char szMatchId[8];
	MATCH match;
	if(6 != sscanf(get_user_input_by_str(), "%s %s %s %s %s %s", szMatchId, match.szName, match.szGender, &match.szTime, match.szLocation, match.szStatus))
	{
		fputs("�������ʽ����\n", stdout);
		_send_menu_msg();
		return;
	}
	_FILTER_MATCH_IGNORE_FLAG ignoreFlag = _make_filter_match_ignore_flag(szMatchId, &match);

	if (ignoreFlag & _ID)
	{
		match.id = atoi(szMatchId);

		if(errno)
		{
			fputs("����Ÿ�ʽ����\n", stdout);
			_send_menu_msg();
			return;
		}
	}

	_FILTER_MATCH_ENUM_PARAM enumParam = {
		.pMatch = &match,
		.ignoreFlag = ignoreFlag
	};

	_print_res(enum_match(_filter_match_enum_proc, &enumParam));
	_send_menu_msg();
}