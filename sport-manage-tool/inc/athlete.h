#pragma once

#include "pch.h"
#include "list_data.h"

typedef uint16_t athleteKey_t;
typedef athleteKey_t athleteId_t;

/**@struct info
* @brief 运动员结构体 \n
* 定义运动员的信息
*/
typedef struct {
	athleteKey_t id; /**< 运动员Id */
	char szName[64]; /**< 运动员名称 */
	char szAge[8]; /**< 运动员年龄 */
	char szGender[8]; /**< 运动员性别 */
	char szCollege[64]; /**< 运动员所属学院 */
	char szComment[64]; /**< 运动员备注 */
} ATHLETE;

/** @biref 清理已经加载的运动员数据
 * @return 无返回值
***/
void clear_athletes();

/** @brief 从文件中加载运动员数据
 * @return 函数执行结果
 * - true 加载成功
 * - false 加载失败
***/
bool load_athletes();

/** @brief 向已有的运动员数据中添加新的运动员数据
 * @param[in] *pAthlete 要添加的运动员数据指针
 * @return 函数执行结果
 * - DATA_RES_SUECCESS 添加成功
 * - DATA_RES_FAIL 添加失败
***/
DATA_RES_t add_athlete(const ATHLETE *pAthlete);

/** @brief 在已有的运动员数据中删除指定运动员数据
* @param[in] athleteId 要删除的运动员的参赛编号Id
* @return 函数执行结果
* - DATA_RES_SUECCESS 删除成功
* - DATA_RES_FAIL 删除失败
***/
DATA_RES_t remove_athlete(const athleteKey_t athleteId);

/** @brief 在已有的运动员数据中修改指定运动员数据
* @param[in] *pAthlete 修改后的运动员数据指针
* @return 函数执行结果
* - DATA_RES_SUECCESS 修改成功
* - DATA_RES_FAIL 修改失败
***/
DATA_RES_t rewrite_athlete(const ATHLETE *pAthlete);

/** @brief 在已有的运动员数据中查询指定运动员数据
* @param[in] athleteId 要查询的运动员的参赛编号
* @param[out] **ppAthlete 返回查询的运动员数据
* @return 函数执行结果
* - DATA_RES_SUECCESS 查询成功
* - DATA_RES_FAIL 查询失败
***/
DATA_RES_t query_athlete(const athleteKey_t iAthleteId, const ATHLETE **ppAthlete);

/** @brief 检查是否存在指定运动员
* @param[in] athleteId 要检查是是否存在的的运动员参赛编号
* @return 函数执行结果
* - true 存在
* - false 不存在
***/
bool is_athlete_exist(const athleteId_t athleteId);

typedef void (*ATHLETE_ENUM_PROC)(const ATHLETE *pAthlete, void *pParam);

/** @brief 枚举已有的运动员数据
* @param[in] fnEnumProc 枚举过程函数
* @param[in] *void  枚举过程函数的调用附加参数
* @return 函数执行结果
* - DATA_RES_SUECCESS 枚举成功
* - DATA_RES_SUECCESS 枚举失败
***/

DATA_RES_t enum_athlete(ATHLETE_ENUM_PROC fnEnumProc, void *pParam);

/** @brief 获取运动员数据使用的内部链表
* @return 函数执行结果
* - NULL 获取失败
***/
LIST *get_athletes_internal();