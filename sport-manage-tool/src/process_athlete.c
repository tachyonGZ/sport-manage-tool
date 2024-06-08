#include "../inc/pch.h"

#include "../inc/process_athlete.h"

inline static void _send_menu_msg()
{
	send_message(M_ATHLETE_MENU, NULL, 0);
}

void process_athlete_menu()
{
	_print_split();

	fputs("��������Ҫ���еĲ���\n", stdout);
	
	switch(get_user_input_by_str()[0])
	{
	case '1':
		send_message(M_ATHLETE_ADD, NULL, 0);
		break;
	case '2':
		send_message(M_ATHLETE_REMOVE, NULL, 0);
		break;
	case '3':
		send_message(M_ATHLETE_REWRITE, NULL, 0);
		break;
	case '4':
		send_message(M_ATHLETE_QUERY, NULL, 0);
		break;
	case '5':
		send_message(M_ATHLETE_QUERY_ALL, NULL, 0);
		break;
	case '6':
		send_message(M_ATHLETE_FILTER, NULL, 0);
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

void process_athlete_menu_interface()
{
	fputs(
		"��������\n" \
		"���˶�Ա��Ϣ����˵�\n"\
		"��������\n" \
		"��1.����˶�Ա��Ϣ\n" \
		"��2.ɾ���˶�Ա��Ϣ\n"	\
		"��3.�޸��˶�Ա��Ϣ\n" \
		"��4.��ѯ�˶�Ա��Ϣ\n"	\
		"��5.��ѯȫ���˶�Ա��Ϣ\n" \
		"��6.ɸѡ��ѯ�˶�Ա��Ϣ\n" \
		"��������\n" \
		"����ʾ������Q��q���Է�����һ���˵�\n" \
		"��������\n" \
	, stdout);

	_send_menu_msg();
}



bool _print_athlete_res(const DATA_RES_t res)
{
	static const char *mapAthleteRes[DATA_RES_MAX] = {
		[DATA_RES_SUCCESS] = "�������ɹ���\n",
		[DATA_RES_FAIL] = "������ʧ�ܣ�\n",
		[DATA_RES_STORE_FAIL] = "������ʧ�ܣ�\n",
		[DATA_RES_UNLOADED] = "������δ���أ�\n",
		[DATA_RES_KEY_CONFLICT] = "��������˶�Ա������Ѵ��ڵ��˶�Ա��ų�ͻ��\n",
		[DATA_RES_INVALID_KEY] = "����Ч���˶�Ա��ţ�\n",
	};

	if (res < DATA_RES_MAX)
	{
		fputs(mapAthleteRes[res], stdout);
	}

	return DATA_RES_SUCCESS == res;
}

void process_add_athlete()
{
	fputs( \
		"��������Ҫ��ӵ��˶�Ա��Ϣ\n" \
		"����ʽ:<���> <����> <�Ա�> <����> <����ѧԺ> <��ע>\n" \
		"������:1 ���� �� 18 ��ϢѧԺ ��\n" \
	, stdout);
	
	// ����������ݳ�ʼ��
	ATHLETE athlete;
	if(6 == sscanf(get_user_input_by_str(), "%hd %s %s %s %s %s", &athlete.id, athlete.szName, athlete.szGender, &athlete.szAge, athlete.szCollege, athlete.szComment))
	{
		_print_athlete_res(add_athlete(&athlete));
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_remove_athlete()
{
	fputs( \
		"��������Ҫɾ�����˶�Ա��Ϣ��Ӧ���˶�Ա���\n" \
	, stdout);


	athleteKey_t iAthleteId = 0;
	if(1 == sscanf(get_user_input_by_str(), "%hd", &iAthleteId))
	{
		_print_athlete_res(remove_athlete(iAthleteId));
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_rewrite_athlete()
{
	fputs( \
		"��������Ҫ�޸ĵ��˶�Ա��Ϣ\n" \
		"����ʽ:<���> <����> <�Ա�> <����> <����ѧԺ> <��ע>\n" \
		"������:1 ���� �� 18 ��ϢѧԺ ��\n" \
	, stdout);

	ATHLETE athlete;
	if(6 == sscanf(get_user_input_by_str(), "%hd %s %s %s %s %s", &athlete.id, athlete.szName, athlete.szGender, &athlete.szAge, athlete.szCollege, athlete.szComment))
	{
		_print_athlete_res(rewrite_athlete(&athlete));
	}
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void _print_athlete(const ATHLETE *pAthlete, const *pIgnore)
{
	// �� �� ������ �����ȩЩة�
	
	printf( \
		"����������\n" \
		"������ţ�%d\n" \
		"����������%s\n" \
		"�����Ա�: %s\n" \
		"�������䣺%s\n" \
		"����ѧԺ��%s\n" \
		"������ע��%s\n" \
		, pAthlete->id, pAthlete->szName, pAthlete->szGender, pAthlete->szAge, pAthlete->szCollege, pAthlete->szComment
	);
}

void process_query_athlete()
{
	fputs( \
		"��������Ҫ��ѯ���˶�Ա��Ϣ��Ӧ���˶�Ա���\n" \
	, stdout);

	athleteKey_t iAthleteId = 0;
	if(1 == sscanf(get_user_input_by_str(), "%hd", &iAthleteId))
	{
		const ATHLETE *pAthlete = NULL;

		if (_print_athlete_res(query_athlete(iAthleteId, &pAthlete)))
		{
			_print_athlete(pAthlete, NULL);
		}
	}	
	else
	{
		fputs("�������ʽ����\n", stdout);
	}

	_send_menu_msg();
}

void process_query_all_athlete()
{
	_print_athlete_res(enum_athlete(_print_athlete, NULL));

	_send_menu_msg();
}

typedef enum {
	_ID = 1 << 0,
	_NAME = 1 << 1,
	_GENDER = 1 << 2,
	_AGE = 1 << 3,
	_COLLEGE = 1 << 4,
	_COMMENT = 1 << 5
} _FILTER_ATHLETE_IGNORE_FLAG_MASK;

typedef uint32_t _FILTER_ATHLETE_IGNORE_FLAG;

inline  _FILTER_ATHLETE_IGNORE_FLAG_MASK _set_filter_match_ignore_mask(const _FILTER_ATHLETE_IGNORE_FLAG_MASK mask, const char *szInput)
{
	return ('*' != szInput[0])?(mask):(0);
}

_FILTER_ATHLETE_IGNORE_FLAG _make_filter_ahtlete_ignore_flag(const char *szMatchId, const ATHLETE *pAthlete)
{
	_FILTER_ATHLETE_IGNORE_FLAG ignoreFlag = 0;
	ignoreFlag |= _set_filter_match_ignore_mask(_ID, szMatchId);
	ignoreFlag |= _set_filter_match_ignore_mask(_NAME, pAthlete->szName);
	ignoreFlag |= _set_filter_match_ignore_mask(_GENDER, pAthlete->szGender);
	ignoreFlag |= _set_filter_match_ignore_mask(_AGE, pAthlete->szAge);
	ignoreFlag |= _set_filter_match_ignore_mask(_COLLEGE, pAthlete->szCollege);
	ignoreFlag |= _set_filter_match_ignore_mask(_COMMENT, pAthlete->szComment);
	return ignoreFlag;
}

typedef struct {
	const ATHLETE *pAthlete;
	const _FILTER_ATHLETE_IGNORE_FLAG ignoreFlag;
} _FILTER_ATHLETE_ENUM_PARAM;

void _filter_athlete_enum_proc(const ATHLETE *pAthlete, const _FILTER_ATHLETE_ENUM_PARAM *pParam)
{
	if ( (pParam->ignoreFlag & _ID) && (pParam->pAthlete->id != pAthlete->id) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _NAME) && (0 != strcmp(pParam->pAthlete->szName, pAthlete->szName)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _GENDER) && (0 != strcmp(pParam->pAthlete->szGender, pAthlete->szGender)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _AGE) && (0 != strcmp(pParam->pAthlete->szAge, pAthlete->szAge)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _COLLEGE) && (0 != strcmp(pParam->pAthlete->szCollege, pAthlete->szCollege)) )
	{
		return;
	}
	else if ( (pParam->ignoreFlag & _COMMENT) && (0 != strcmp(pParam->pAthlete->szComment, pAthlete->szComment)) )
	{
		return;
	}
	else
	{
		_print_athlete(pAthlete, NULL);
	}

	return;
}

void process_filter_athlete()
{
	fputs( \
		"��������ɸѡ����\n" \
		"����ʽ:<���> <����> <�Ա�> <����> <����ѧԺ> <��ע>\n" \
		"�����Ե�ɸѡ�������*���\n"\
		"������:* * �� 18 * *\n" \
	, stdout);

	char szAthleteId[8];
	ATHLETE athlete;
	if(6 != sscanf(get_user_input_by_str(), "%s %s %s %s %s %s", szAthleteId, athlete.szName, athlete.szGender, &athlete.szAge, athlete.szCollege, athlete.szComment))
	{
		fputs("�������ʽ����\n", stdout);
		_send_menu_msg();
		return;
	}

	_FILTER_ATHLETE_IGNORE_FLAG ignoreFlag = _make_filter_ahtlete_ignore_flag(szAthleteId, &athlete);

	if (ignoreFlag & _ID)
	{
		athlete.id = atoi(szAthleteId);

		if(errno)
		{
			fputs("����Ÿ�ʽ����\n", stdout);
			_send_menu_msg();
			return;
		}
	}


	_FILTER_ATHLETE_ENUM_PARAM enumParam = {
		.pAthlete = &athlete,
		.ignoreFlag = ignoreFlag
	};

	_print_athlete_res(enum_athlete(_filter_athlete_enum_proc, &enumParam));
	_send_menu_msg();
}