#pragma once

#include "pch.h"
#include "list_data.h"

typedef uint16_t athleteKey_t;
typedef athleteKey_t athleteId_t;

/**@struct info
* @brief �˶�Ա�ṹ�� \n
* �����˶�Ա����Ϣ
*/
typedef struct {
	athleteKey_t id; /**< �˶�ԱId */
	char szName[64]; /**< �˶�Ա���� */
	char szAge[8]; /**< �˶�Ա���� */
	char szGender[8]; /**< �˶�Ա�Ա� */
	char szCollege[64]; /**< �˶�Ա����ѧԺ */
	char szComment[64]; /**< �˶�Ա��ע */
} ATHLETE;

/** @biref �����Ѿ����ص��˶�Ա����
 * @return �޷���ֵ
***/
void clear_athletes();

/** @brief ���ļ��м����˶�Ա����
 * @return ����ִ�н��
 * - true ���سɹ�
 * - false ����ʧ��
***/
bool load_athletes();

/** @brief �����е��˶�Ա����������µ��˶�Ա����
 * @param[in] *pAthlete Ҫ��ӵ��˶�Ա����ָ��
 * @return ����ִ�н��
 * - DATA_RES_SUECCESS ��ӳɹ�
 * - DATA_RES_FAIL ���ʧ��
***/
DATA_RES_t add_athlete(const ATHLETE *pAthlete);

/** @brief �����е��˶�Ա������ɾ��ָ���˶�Ա����
* @param[in] athleteId Ҫɾ�����˶�Ա�Ĳ������Id
* @return ����ִ�н��
* - DATA_RES_SUECCESS ɾ���ɹ�
* - DATA_RES_FAIL ɾ��ʧ��
***/
DATA_RES_t remove_athlete(const athleteKey_t athleteId);

/** @brief �����е��˶�Ա�������޸�ָ���˶�Ա����
* @param[in] *pAthlete �޸ĺ���˶�Ա����ָ��
* @return ����ִ�н��
* - DATA_RES_SUECCESS �޸ĳɹ�
* - DATA_RES_FAIL �޸�ʧ��
***/
DATA_RES_t rewrite_athlete(const ATHLETE *pAthlete);

/** @brief �����е��˶�Ա�����в�ѯָ���˶�Ա����
* @param[in] athleteId Ҫ��ѯ���˶�Ա�Ĳ������
* @param[out] **ppAthlete ���ز�ѯ���˶�Ա����
* @return ����ִ�н��
* - DATA_RES_SUECCESS ��ѯ�ɹ�
* - DATA_RES_FAIL ��ѯʧ��
***/
DATA_RES_t query_athlete(const athleteKey_t iAthleteId, const ATHLETE **ppAthlete);

/** @brief ����Ƿ����ָ���˶�Ա
* @param[in] athleteId Ҫ������Ƿ���ڵĵ��˶�Ա�������
* @return ����ִ�н��
* - true ����
* - false ������
***/
bool is_athlete_exist(const athleteId_t athleteId);

typedef void (*ATHLETE_ENUM_PROC)(const ATHLETE *pAthlete, void *pParam);

/** @brief ö�����е��˶�Ա����
* @param[in] fnEnumProc ö�ٹ��̺���
* @param[in] *void  ö�ٹ��̺����ĵ��ø��Ӳ���
* @return ����ִ�н��
* - DATA_RES_SUECCESS ö�ٳɹ�
* - DATA_RES_SUECCESS ö��ʧ��
***/

DATA_RES_t enum_athlete(ATHLETE_ENUM_PROC fnEnumProc, void *pParam);

/** @brief ��ȡ�˶�Ա����ʹ�õ��ڲ�����
* @return ����ִ�н��
* - NULL ��ȡʧ��
***/
LIST *get_athletes_internal();