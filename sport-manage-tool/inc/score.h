#pragma once

#include "pch.h"
#include "list_data.h"

#include "athlete.h"
#include "match.h"

typedef uint32_t scoreKey_t;
typedef scoreKey_t scoreId_t;

/**@struct info
* @brief 成绩结构体 \n
* 定义成绩的信息
*/
typedef struct{
	athleteId_t athleteId;	/**< 运动员id */
	matchId_t matchId; /**< 比赛项目id */
	char szResult[16]; /**< 成绩 */
	char szRank[16]; /**< 名次 */
	char szparticipation[16]; /**< 是否参赛 */
} SCORE;

/** @biref 清理已经加载的运动员数据
* @return 无返回值
***/
void clear_score_data();

/** @brief 从文件中加载成绩数据
* @return 函数执行结果
* - true 加载成功
* - false 加载失败
***/
bool load_score_data();

/** @brief 向已有的成绩数据中添加新的成绩
* @param[in] *pScore 要添加的成绩数据指针
* @return 函数执行结果
* - DATA_RES_SUECCESS 添加成功
* - DATA_RES_FAIL 添加失败
***/
DATA_RES_t add_score(const SCORE *pScore);

/** @brief 在已有的成绩数据中删除指定成绩数据
* @param[in] athleteId 要删除的成绩的运动员Id
* @param[in] matchId 要删除的成绩的比赛项目Id
* @return 函数执行结果
* - DATA_RES_SUECCESS 删除成功
* - DATA_RES_FAIL 删除失败
***/
DATA_RES_t remove_score(const athleteId_t athleteId, const matchId_t matchId);

/** @brief 在已有的成绩数据中修改指定成绩数据
* @param[in] *pScore 修改后的成绩数据指针
* @return 函数执行结果
* - DATA_RES_SUECCESS 修改成功
* - DATA_RES_FAIL 修改失败
***/
DATA_RES_t rewrite_score(const SCORE *pScore);

/** @brief 在已有的成绩数据中查询指定成绩数据
* @param[in] athleteId 要查询的参赛成绩的运动员编号
* @param[int] matchId 要查询的参赛成绩的比赛项目编号
* @param[out] **ppScore 返回查询的成绩数据
* @return 函数执行结果
* - DATA_RES_SUECCESS 查询成功
* - DATA_RES_FAIL 查询失败
***/
DATA_RES_t query_score(const athleteId_t athleteId, const matchId_t matchId, const SCORE **ppScore);

typedef void (*SCORE_ENUM_PROC)(const SCORE *pScore, void *pParam);

/** @brief 枚举已有的成绩数据
* @param[in] fnEnumProc 枚举过程函数
* @param[in] *void  枚举过程函数的调用附加参数
* @return 函数执行结果
* - DATA_RES_SUECCESS 枚举成功
* - DATA_RES_SUECCESS 枚举失败
***/
DATA_RES_t enum_score(SCORE_ENUM_PROC fnEnumProc, void *pParam);

/** @brief 获取运动员数据使用的内部链表
* @return 函数执行结果
* - NULL 获取失败
***/
LIST *get_score_data_internal();

/*
bool is_athlete_in_score(const athleteId_t athleteId);
bool is_match_in_score(const matchId_t matchId);
*/