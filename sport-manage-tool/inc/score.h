#pragma once

#include "pch.h"
#include "list_data.h"

#include "athlete.h"
#include "match.h"

typedef uint32_t scoreKey_t;
typedef scoreKey_t scoreId_t;

/**@struct info
* @brief �ɼ��ṹ�� \n
* ����ɼ�����Ϣ
*/
typedef struct{
	athleteId_t athleteId;	/**< �˶�Աid */
	matchId_t matchId; /**< ������Ŀid */
	char szResult[16]; /**< �ɼ� */
	char szRank[16]; /**< ���� */
	char szparticipation[16]; /**< �Ƿ���� */
} SCORE;

/** @biref �����Ѿ����ص��˶�Ա����
* @return �޷���ֵ
***/
void clear_score_data();

/** @brief ���ļ��м��سɼ�����
* @return ����ִ�н��
* - true ���سɹ�
* - false ����ʧ��
***/
bool load_score_data();

/** @brief �����еĳɼ�����������µĳɼ�
* @param[in] *pScore Ҫ��ӵĳɼ�����ָ��
* @return ����ִ�н��
* - DATA_RES_SUECCESS ��ӳɹ�
* - DATA_RES_FAIL ���ʧ��
***/
DATA_RES_t add_score(const SCORE *pScore);

/** @brief �����еĳɼ�������ɾ��ָ���ɼ�����
* @param[in] athleteId Ҫɾ���ĳɼ����˶�ԱId
* @param[in] matchId Ҫɾ���ĳɼ��ı�����ĿId
* @return ����ִ�н��
* - DATA_RES_SUECCESS ɾ���ɹ�
* - DATA_RES_FAIL ɾ��ʧ��
***/
DATA_RES_t remove_score(const athleteId_t athleteId, const matchId_t matchId);

/** @brief �����еĳɼ��������޸�ָ���ɼ�����
* @param[in] *pScore �޸ĺ�ĳɼ�����ָ��
* @return ����ִ�н��
* - DATA_RES_SUECCESS �޸ĳɹ�
* - DATA_RES_FAIL �޸�ʧ��
***/
DATA_RES_t rewrite_score(const SCORE *pScore);

/** @brief �����еĳɼ������в�ѯָ���ɼ�����
* @param[in] athleteId Ҫ��ѯ�Ĳ����ɼ����˶�Ա���
* @param[int] matchId Ҫ��ѯ�Ĳ����ɼ��ı�����Ŀ���
* @param[out] **ppScore ���ز�ѯ�ĳɼ�����
* @return ����ִ�н��
* - DATA_RES_SUECCESS ��ѯ�ɹ�
* - DATA_RES_FAIL ��ѯʧ��
***/
DATA_RES_t query_score(const athleteId_t athleteId, const matchId_t matchId, const SCORE **ppScore);

typedef void (*SCORE_ENUM_PROC)(const SCORE *pScore, void *pParam);

/** @brief ö�����еĳɼ�����
* @param[in] fnEnumProc ö�ٹ��̺���
* @param[in] *void  ö�ٹ��̺����ĵ��ø��Ӳ���
* @return ����ִ�н��
* - DATA_RES_SUECCESS ö�ٳɹ�
* - DATA_RES_SUECCESS ö��ʧ��
***/
DATA_RES_t enum_score(SCORE_ENUM_PROC fnEnumProc, void *pParam);

/** @brief ��ȡ�˶�Ա����ʹ�õ��ڲ�����
* @return ����ִ�н��
* - NULL ��ȡʧ��
***/
LIST *get_score_data_internal();

/*
bool is_athlete_in_score(const athleteId_t athleteId);
bool is_match_in_score(const matchId_t matchId);
*/