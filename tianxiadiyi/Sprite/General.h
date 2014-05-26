#ifndef __GENERAL__
#define __GENERAL__

#include <vector>

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

#include "Monster.h"
#include "Logic\Equipment.h"

#include "tinytab\DataBase.h"
#include "tinytab\DBC_Struct.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace cocos2d::extension;
using namespace gui;

class General : public Monster
{
public:

	_DBC_GENERAL attribute;

	// ��������
	int wuLiGongJi;

	// ��������
	int wuLiFangYu;

	// ��������
	int faShuGongJi;

	// ��������
	int faShuFangYu;

	// ����
	int hp;

	// �Ƿ�����
	bool isFormation;

	// ����װ���б�
	vector<Equipment*>equipmentVector;

	// ����װ������
	Equipment* equipmentArray[6];

	General(int id);
	~General();

	void creatArmature();
};

#endif