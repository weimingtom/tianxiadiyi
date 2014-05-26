#include "UIStrengthen.h"

#include "..\TianXiaDiYi.h"

UIStrengthen::UIStrengthen()
{
	itemManager = ItemManager::getTheOnlyInstance();
	formationManager = FormationManager::getTheOnlyInstance();
	strengthenManager = StrengthenManager::getTheOnlyInstance();
}

UIStrengthen::~UIStrengthen()
{
}

bool UIStrengthen::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	uiLayer = UILayer::create();

	UILayout* strengthenRoot = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("ui/UIStrengthen.ExportJson"));
	uiLayer->addWidget(strengthenRoot);

	UIButton* closeButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("CloseButton"));
	closeButton->addTouchEventListener(this, toucheventselector(UIStrengthen::closeButtonClicked));

	UIButton* pageLeftButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("PageLeftButton"));
	pageLeftButton->addTouchEventListener(this, toucheventselector(UIStrengthen::pageLeftButtonClicked));

	UIButton* pageRightButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("PageRightButton"));
	pageRightButton->addTouchEventListener(this, toucheventselector(UIStrengthen::pageRightButtonClicked));

	for (int i = 0; i < 4; i++)
	{
		const char* s = CCString::createWithFormat("EquipmentImageView_%d", i+1)->getCString();
		equipmentImageView[i] = dynamic_cast<UIImageView*>(uiLayer->getWidgetByName(s));
	}

	for (int i = 0; i < 3; i++)
	{
		const char* s = CCString::createWithFormat("GemImageView_%d", i+1)->getCString();
		gemImageView[i] = dynamic_cast<UIImageView*>(uiLayer->getWidgetByName(s));
	}

	featureImageView = dynamic_cast<UIImageView*>(uiLayer->getWidgetByName("FeatureImageView"));

	strengthenTestButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("StrengthenTestButton"));

	UIPanel* tableViewPanel = dynamic_cast<UIPanel*>(uiLayer->getWidgetByName("TableViewPanel"));

	CCTableView* strengthenTableView = CCTableView::create(this, CCSizeMake(strengthenTestButton->getContentSize().width, tableViewPanel->getContentSize().height));
	strengthenTableView->setDirection(kCCScrollViewDirectionVertical);
	strengthenTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

	selectFrameImageView = dynamic_cast<UIImageView*>(uiLayer->getWidgetByName("SelectFrameImageView"));

	CCPoint strengthenTestPosition = strengthenTestButton->getPosition();
	strengthenTableView->setPosition(ccp(strengthenTestPosition.x, strengthenTestPosition.y - tableViewPanel->getContentSize().height));
	strengthenTableView->setDelegate(this);
	strengthenTableView->reloadData();

	for (int i = 0; i < 4; i++)
	{
		const char* s = CCString::createWithFormat("EquipmentButton_%d", i+1)->getCString();
		UIButton* equipmentButton = dynamic_cast<UIButton*>(uiLayer->getWidgetByName(s));
		equipmentButton->addTouchEventListener(this, toucheventselector(UIStrengthen::equipmentButtonClicked));
	}

	addChild(uiLayer);
	addChild(strengthenTableView);
	setVisible(false);

	refresh();
	return true;
}

void UIStrengthen::onEnter()
{
	CCLayer::onEnter();
	setTouchEnabled(true);
}

void UIStrengthen::refresh()
{
	int maxPageNum = strengthenManager->getMaxPageNum();

	// ����ͼƬ
	int num;

	if (strengthenManager->pageNum < (maxPageNum-1))
	{
		num = 4;
	}
	else
	{
		if (strengthenManager->selectGeneralId == 0)
		{
			num = itemManager->equipmentVector.size() - strengthenManager->pageNum * 4;
		}
		else
		{
			num = formationManager->generalVector[strengthenManager->selectGeneralId-1]->equipmentVector.size() - strengthenManager->pageNum * 4;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (i < num)
		{
			int j = strengthenManager->pageNum * 4 + i;
			const char* s = NULL;

			if (strengthenManager->selectGeneralId == 0)
			{
				s = CCString::createWithFormat("png/equipment/%s.png", itemManager->equipmentVector[j]->attribute.tuPian)->getCString();
			}
			else
			{
				s = CCString::createWithFormat("png/equipment/%s.png", formationManager->generalVector[strengthenManager->selectGeneralId-1]->equipmentVector[j]->attribute.tuPian)->getCString();
			}

			equipmentImageView[i]->loadTexture(s);
			equipmentImageView[i]->setVisible(true);
		}
		else
		{
			equipmentImageView[i]->setVisible(false);
		}
	}

	if (strengthenManager->selectGeneralId == 0)
	{
		const char* s = CCString::createWithFormat("png/equipment/%s.png", itemManager->equipmentVector[strengthenManager->selectEquipmentId]->attribute.tuPian)->getCString();
		featureImageView->loadTexture(s);
		featureImageView->setVisible(true);
	}
	else
	{
		const char* s = CCString::createWithFormat("png/equipment/%s.png", formationManager->generalVector[strengthenManager->selectGeneralId-1]->equipmentVector[strengthenManager->selectEquipmentId]->attribute.tuPian)->getCString();
		featureImageView->loadTexture(s);
		featureImageView->setVisible(true);
	}

	for (int i = 0; i < 3; i++)
	{
		const char* s = CCString::createWithFormat("png/gem/%s.png", itemManager->gemVector[i]->attribute.tuPian)->getCString();
		gemImageView[i]->loadTexture(s);
		gemImageView[i]->setVisible(true);
	}
}

void UIStrengthen::scrollViewDidScroll( CCScrollView* view )
{
}

void UIStrengthen::scrollViewDidZoom( CCScrollView* view )
{
}

void UIStrengthen::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	CCLOG("cell touched at index: %i", cell->getIdx());

	strengthenManager->selectGeneralId = cell->getIdx();
	strengthenManager->pageNum = 0;
	strengthenManager->selectEquipmentId = 0;

	UIButton* button = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("EquipmentButton_1"));;
	selectFrameImageView->setPosition(button->getPosition());

	refresh();
}

cocos2d::CCSize UIStrengthen::cellSizeForTable( CCTableView* table )
{
	return  CCSizeMake(strengthenTestButton->getContentSize().width, strengthenTestButton->getContentSize().height);
}

CCTableViewCell* UIStrengthen::tableCellAtIndex( CCTableView* table, unsigned int idx )
{
	CCTableViewCell* cell = table->cellAtIndex(idx);

	if (!cell)
	{
		cell = new CCTableViewCell();
		cell->autorelease();

		CCSprite* generalNameBGSprite;

		if (idx == 0)
		{
			generalNameBGSprite = CCSprite::create("png/IdleEquipment.png");
		}
		else
		{
			generalNameBGSprite = CCSprite::create("png/GeneralNameBG.png");
		}

		generalNameBGSprite->setAnchorPoint(CCPointZero);
		generalNameBGSprite->setPosition(CCPointZero);
		cell->addChild(generalNameBGSprite);

		if (idx != 0)
		{
			CCLabelTTF* generalNameLabel = CCLabelTTF::create();
			generalNameLabel->setPosition(ccp(generalNameBGSprite->getContentSize().width/2, generalNameBGSprite->getContentSize().height/2));
			generalNameLabel->setString(formationManager->generalVector[idx-1]->attribute.tuPian);
			cell->addChild(generalNameLabel);
		}

		CCSprite* generalSelectSprite = CCSprite::create("png/GeneralNameSelect.png");
		generalSelectSprite->setPosition(CCPointZero);
		generalSelectSprite->setAnchorPoint(CCPointZero);
		generalSelectSprite->setTag(8);
		generalSelectSprite->setVisible(false);
		cell->addChild(generalSelectSprite);

		generalSelectSpriteVector.push_back(generalSelectSprite);
	}

	return cell;
}

unsigned int UIStrengthen::numberOfCellsInTableView( cocos2d::extension::CCTableView *table )
{
	return formationManager->generalVector.size() + 1;
}

void UIStrengthen::tableCellHighlight( CCTableView* table, extension::CCTableViewCell* cell )
{
	for (int i = 0; i < generalSelectSpriteVector.size(); i++)
	{
		generalSelectSpriteVector[i]->setVisible(false);
	}

	CCSprite* generalSelectSprite =(CCSprite*)cell->getChildByTag(8);
	generalSelectSprite->setVisible(true);
}

void UIStrengthen::tableCellUnhighlight( CCTableView* table, extension::CCTableViewCell* cell )
{
}

void UIStrengthen::closeButtonClicked( CCObject* sender, TouchEventType type )
{
	TianXiaDiYi::getTheOnlyInstance()->removeChild(TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiStrengthen, true);
	TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiStrengthen->release();
	TianXiaDiYi::getTheOnlyInstance()->uiMainCity->uiStrengthen = NULL;
}

void UIStrengthen::equipmentButtonClicked( CCObject* sender, TouchEventType type )
{
	UIButton* button = (UIButton*)sender;

	for (int i = 0; i < 4; i++)
	{
		const char* s = CCString::createWithFormat("EquipmentButton_%d", i+1)->getCString();

		if (strcmp(button->getName(), s) == 0)
		{
			selectFrameImageView->setPosition(button->getPosition());
			int n = strengthenManager->pageNum * 4 + i;

			if (strengthenManager->selectGeneralId == 0)
			{
				if (n < itemManager->equipmentVector.size())
				{
					strengthenManager->selectEquipmentId = n;
					refresh();
				}
			}
			else
			{
				if (n < formationManager->generalVector[strengthenManager->selectGeneralId-1]->equipmentVector.size())
				{
					strengthenManager->selectEquipmentId = n;
					refresh();
				}
			}
			
			break;
		}
	}
}

void UIStrengthen::pageLeftButtonClicked( CCObject* sender, TouchEventType type )
{
	if (type == CCTOUCHBEGAN)
	{
		strengthenManager->pageNum--;

		if (strengthenManager->pageNum < 0)
		{
			strengthenManager->pageNum = 0;
			return;
		}

		CCLOG("formationManager->pageNum: %d", strengthenManager->pageNum);

		strengthenManager->selectEquipmentId = strengthenManager->pageNum * 4;
		UIButton* button = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("EquipmentButton_1"));;
		selectFrameImageView->setPosition(button->getPosition());

		refresh();
	}
}

void UIStrengthen::pageRightButtonClicked( CCObject* sender, TouchEventType type )
{
	if (type == CCTOUCHBEGAN)
	{
		strengthenManager->pageNum++;

		int maxPageNum = strengthenManager->getMaxPageNum();

		if (strengthenManager->pageNum > maxPageNum - 1)
		{
			strengthenManager->pageNum = maxPageNum - 1;
			return;
		}

		CCLOG("formationManager->pageNum: %d", strengthenManager->pageNum);

		strengthenManager->selectEquipmentId = strengthenManager->pageNum * 4;
		UIButton* button = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("EquipmentButton_1"));;
		selectFrameImageView->setPosition(button->getPosition());

		refresh();
	}
}