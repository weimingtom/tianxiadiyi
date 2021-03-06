#include "UICountry.h"
#include "UICountryLand.h"

#include "..\TianXiaDiYi.h"

UICountry::UICountry()
{
	countryManager = CountryManager::getTheOnlyInstance();
	multiStringPanelTemp = NULL;
}

UICountry::~UICountry()
{
}

bool UICountry::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	uiLayer = UILayer::create();

	UILayout* countryRoot = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/UICountry.ExportJson"));
	uiLayer->addWidget(countryRoot);

	UIButton* closeButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("CloseButton"));
	closeButton->addTouchEventListener(this, toucheventselector(UICountry::closeButtonClicked));

	UIButton* verifyButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("VerifyButton"));
	verifyButton->addTouchEventListener(this, toucheventselector(UICountry::verifyButtonClicked));

	UIButton* modifyButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("ModifyButton"));
	modifyButton->addTouchEventListener(this, toucheventselector(UICountry::modifyButtonClicked));

	// UIButton* appointmentButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("AppointmentButton"));
	// appointmentButton->addTouchEventListener(this, toucheventselector(UICountry::appointmentButtonClicked));

	UIButton* quitButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("QuitButton"));
	quitButton->addTouchEventListener(this, toucheventselector(UICountry::quitButtonClicked));

	UIButton* kickButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("KickButton"));
	kickButton->addTouchEventListener(this, toucheventselector(UICountry::kickButtonClicked));

	UIButton* countryRankButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("CountryRankButton"));
	countryRankButton->addTouchEventListener(this, toucheventselector(UICountry::countryRankButtonClicked));

	UIButton* countryLand = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("CountryLandButton"));
	countryLand->addTouchEventListener(this, toucheventselector(UICountry::countryLandButtonClicked));

	for (int i = 0; i < 4; i++)
	{
		const char* s = CCString::createWithFormat("AttributeLabel_%d", i+1)->getCString();
		attributeLabel[i] = dynamic_cast<UILabel*>(uiLayer->getWidgetByName(s));
	}

	multiStringPanel = dynamic_cast<UIPanel*>(uiLayer->getWidgetByName("MultiStringPanel"));

	coutryTestButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("CountryTestButton"));
	memberPanel = dynamic_cast<UIPanel*>(uiLayer->getWidgetByName("MemberPanel"));

	// 国家成员列表
	UIPanel* tableViewPanel = dynamic_cast<UIPanel*>(uiLayer->getWidgetByName("TableViewPanel"));

	memberTableView = CCTableView::create(this, CCSizeMake(tableViewPanel->getContentSize().width, tableViewPanel->getContentSize().height));
	memberTableView->setDirection(kCCScrollViewDirectionVertical);
	memberTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

	CCPoint coutryTestButtonPosition = coutryTestButton->getPosition();
	memberTableView->setPosition(ccp(coutryTestButtonPosition.x, coutryTestButtonPosition.y - tableViewPanel->getContentSize().height));
	memberTableView->setDelegate(this);
	memberTableView->reloadData();

	addChild(uiLayer);
	addChild(memberTableView);

	setVisible(false);
	refresh();
	return true;
}

void UICountry::onEnter()
{
	CCLayer::onEnter();
	setTouchEnabled(true);
}

void UICountry::clear()
{
	for (int i = 0; i < 4; i++)
	{
		attributeLabel[i]->setText(" ");
	}

	if (multiStringPanelTemp != NULL)
	{
		multiStringPanel->removeChild(multiStringPanelTemp, true);
	}
}

void UICountry::refresh()
{
	clear();

	// 国号
	const char* name = TianXiaDiYi::getTheOnlyInstance()->ansi2utf8(countryManager->name);
	// 人物
	const char* renWu = CCString::createWithFormat("%d/%d", countryManager->onlineNum, countryManager->totalNum)->getCString();
	// 等级
	const char* lv = CCString::createWithFormat("%d", countryManager->lv)->getCString();
	// 经验
	const char* exp = CCString::createWithFormat("%d", countryManager->exp)->getCString();

	const char* attribute[4] = {name, renWu, lv, exp,};

	for (int i = 0; i < 4; i++)
	{
		attributeLabel[i]->setText(attribute[i]);
	}

	multiStringPanelTemp = TianXiaDiYi::getTheOnlyInstance()->getMultiString(countryManager->notice, multiStringPanel->getContentSize().width);
	multiStringPanel->addChild(multiStringPanelTemp);

	delete name;
}

void UICountry::scrollViewDidScroll( CCScrollView* view )
{

}

void UICountry::scrollViewDidZoom( CCScrollView* view )
{
}

void UICountry::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	CCLOG("cell touched at index: %i", cell->getIdx());
	countryManager->selectId = cell->getIdx();
}

cocos2d::CCSize UICountry::cellSizeForTable( CCTableView* table )
{
	return  CCSizeMake(memberPanel->getContentSize().width, memberPanel->getContentSize().height);
}

CCTableViewCell* UICountry::tableCellAtIndex( CCTableView* table, unsigned int idx )
{
	CCTableViewCell* cell = table->cellAtIndex(idx);

	if (!cell)
	{
		cell = new CCTableViewCell();
		cell->autorelease();

		UIPanel* panel = UIPanel::create();
		panel->setTag(8);
		panel->setContentSize(memberPanel->getContentSize());

		UIImageView* memberBGImageView = UIImageView::create();

		if (idx % 2 == 0)
		{
			memberBGImageView->loadTexture("png/MemberBG2.png");
		}
		else
		{
			memberBGImageView->loadTexture("png/MemberBG1.png");
		}
		
		UIImageView* memberBGImageViewEXT = dynamic_cast<UIImageView*>(uiLayer->getWidgetByName("MemberBGImageView"));
		memberBGImageView->setPosition(memberBGImageViewEXT->getPosition());
		panel->addChild(memberBGImageView);

		CountryMember countryMember = countryManager->countryMemberVector[idx];

		// 玩家
		const char* name = TianXiaDiYi::getTheOnlyInstance()->ansi2utf8(countryMember.name);
		// 职务
		const char* job = TianXiaDiYi::getTheOnlyInstance()->ansi2utf8(countryManager->getJobName(countryMember.job));
		// 等级
		const char* lv = CCString::createWithFormat("%d", countryMember.lv)->getCString();
		// 贡献度
		const char* contribute = CCString::createWithFormat("%d", countryMember.contribute)->getCString();
		// 登录时间
		const char* login = CCString::createWithFormat("%d小时%d分钟", countryMember.hour, countryMember.minute)->getCString();
		login = TianXiaDiYi::getTheOnlyInstance()->ansi2utf8(login);

		const char* attribute[5] = {name, job, lv, contribute, login};

		for (int i = 0; i < 5; i++)
		{
			UILabel* memberLabel = UILabel::create();
			memberLabel->setZOrder(5);
			memberLabel->setColor(ccRED);
			memberLabel->setText(attribute[i]);

			const char* s = CCString::createWithFormat("MemberLabel_%d", i+1)->getCString();
			UILabel* memberLableEXT = dynamic_cast<UILabel*>(uiLayer->getWidgetByName(s));
			memberLabel->setPosition(memberLableEXT->getPosition());

			panel->addChild(memberLabel);
		}

		delete[] name;
		delete[] job;
		delete[] login;

		UIImageView* selectCoutryFrameImageView = UIImageView::create();
		selectCoutryFrameImageView->loadTexture("png/SelectCountryFrame.png");
		selectCoutryFrameImageView->setVisible(false);
		selectCoutryFrameImageView->setTag(8);
		UIImageView* selectCoutryFrameImageViewEXT = dynamic_cast<UIImageView*>(uiLayer->getWidgetByName("SelectCoutryFrameImageView"));
		selectCoutryFrameImageView->setPosition(selectCoutryFrameImageViewEXT->getPosition());
		panel->addChild(selectCoutryFrameImageView);

		cell->addChild(panel);

		tableViewSpriteVector.push_back(selectCoutryFrameImageView);
	}

	return cell;
}

unsigned int UICountry::numberOfCellsInTableView( cocos2d::extension::CCTableView *table )
{
	return countryManager->countryMemberVector.size();
}

void UICountry::tableCellHighlight( CCTableView* table, extension::CCTableViewCell* cell )
{
	countryManager->selectId = cell->getIdx();

	for (int i = 0; i < tableViewSpriteVector.size(); i++)
	{
		tableViewSpriteVector[i]->setVisible(false);
	}

	UIPanel* panel = (UIPanel*)cell->getChildByTag(8);
	UIImageView* selectCoutryFrameImageView = (UIImageView*)panel->getChildByTag(8);
	selectCoutryFrameImageView->setVisible(true);
}

void UICountry::tableCellUnhighlight( CCTableView* table, extension::CCTableViewCell* cell )
{
}

void UICountry::closeButtonClicked( CCObject* sender, TouchEventType type )
{
	TianXiaDiYi::getTheOnlyInstance()->removeChild(TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountry, true);
	TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountry->release();
	TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountry = NULL;
}

void UICountry::verifyButtonClicked( CCObject* sender, TouchEventType type )
{
	if (type == CCTOUCHBEGAN)
	{
		TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryVerify = UICountryVerify::create();
		TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryVerify->retain();
		TianXiaDiYi::getTheOnlyInstance()->addChild(TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryVerify);
	}
}

void UICountry::modifyButtonClicked( CCObject* sender, TouchEventType type )
{
	countryManager->modify();
}

void UICountry::appointmentButtonClicked( CCObject* sender, TouchEventType type )
{
	countryManager->appointment();
}

void UICountry::quitButtonClicked( CCObject* sender, TouchEventType type )
{
	countryManager->quit();
}

void UICountry::kickButtonClicked( CCObject* sender, TouchEventType type )
{
	if (type == CCTOUCHBEGAN)
	{
		countryManager->kick();
		memberTableView->reloadData();
	}
}

void UICountry::countryRankButtonClicked( CCObject* sender, TouchEventType type )
{
	if (type == CCTOUCHBEGAN)
	{
		TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryRank = UICountryRank::create();
		TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryRank->retain();
		TianXiaDiYi::getTheOnlyInstance()->addChild(TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryRank);
	}
}

void UICountry::countryLandButtonClicked( CCObject* sender, TouchEventType type )
{
	if (type == CCTOUCHBEGAN)
	{
		TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryLand = UICountryLand::create();
		TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryLand->retain();
		TianXiaDiYi::getTheOnlyInstance()->addChild(TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiCountryLand);
	}
}