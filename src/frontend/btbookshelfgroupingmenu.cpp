/*********
*
* In the name of the Father, and of the Son, and of the Holy Spirit.
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2010 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License
* version 2.0.
*
**********/

#include "frontend/btbookshelfgroupingmenu.h"

#include "util/cresmgr.h"
#include "util/directory.h"


namespace {
bool groupsInitialized = false;
BtBookshelfTreeModel::Grouping groupingNone(true);
BtBookshelfTreeModel::Grouping groupingCat(true);
BtBookshelfTreeModel::Grouping groupingCatLang;
BtBookshelfTreeModel::Grouping groupingLang(true);
BtBookshelfTreeModel::Grouping groupingLangCat(true);

inline void initializeGroups() {
    Q_ASSERT(groupingNone.empty());
    groupingCat.append(BtBookshelfTreeModel::GROUP_CATEGORY);
    Q_ASSERT(groupingCatLang.size() == 2);
    Q_ASSERT(groupingCatLang.at(0) == BtBookshelfTreeModel::GROUP_CATEGORY);
    Q_ASSERT(groupingCatLang.at(1) == BtBookshelfTreeModel::GROUP_LANGUAGE);
    groupingLang.append(BtBookshelfTreeModel::GROUP_LANGUAGE);
    groupingLangCat.append(BtBookshelfTreeModel::GROUP_LANGUAGE);
    groupingLangCat.append(BtBookshelfTreeModel::GROUP_CATEGORY);
    groupsInitialized = true;
}

inline void setActionRef(QAction *a, const BtBookshelfTreeModel::Grouping &g) {
    a->setProperty("groupingPointer", QVariant::fromValue((void*) &g));
}

inline const BtBookshelfTreeModel::Grouping &getActionRef(const QAction *a) {
    return *((const BtBookshelfTreeModel::Grouping*) a->property("groupingPointer").value<void*>());
}

} // anonymous namespace


void BtBookshelfGroupingMenu::initMenu(bool showNoGrouping) {
    namespace DU = util::directory;
    namespace RM = CResMgr::mainIndex;

    if (!groupsInitialized) initializeGroups();

    setIcon(DU::getIcon(RM::grouping::icon));

    m_groupingActionGroup = new QActionGroup(this);
    m_groupingActionGroup->setExclusive(true);
    connect(m_groupingActionGroup, SIGNAL(triggered(QAction*)),
            this, SLOT(slotGroupingActionTriggered(QAction*)));

    m_groupingCatLangAction = new QAction(this);
    m_groupingCatLangAction->setCheckable(true);
    setActionRef(m_groupingCatLangAction, groupingCatLang);
    m_groupingActionGroup->addAction(m_groupingCatLangAction);
    addAction(m_groupingCatLangAction);

    m_groupingCatAction = new QAction(this);
    m_groupingCatAction->setCheckable(true);
    setActionRef(m_groupingCatAction, groupingCat);
    m_groupingActionGroup->addAction(m_groupingCatAction);
    addAction(m_groupingCatAction);

    m_groupingLangCatAction = new QAction(this);
    m_groupingLangCatAction->setCheckable(true);
    setActionRef(m_groupingLangCatAction, groupingLangCat);
    m_groupingActionGroup->addAction(m_groupingLangCatAction);
    addAction(m_groupingLangCatAction);

    m_groupingLangAction = new QAction(this);
    m_groupingLangAction->setCheckable(true);
    setActionRef(m_groupingLangAction, groupingLang);
    m_groupingActionGroup->addAction(m_groupingLangAction);
    addAction(m_groupingLangAction);

    if (showNoGrouping) {
        m_groupingNoneAction = new QAction(this);
        m_groupingNoneAction->setCheckable(true);
        setActionRef(m_groupingNoneAction, groupingNone);
        m_groupingActionGroup->addAction(m_groupingNoneAction);
        addAction(m_groupingNoneAction);
    } else {
        m_groupingNoneAction = 0;
    }

    retranslateUi();
}

void BtBookshelfGroupingMenu::retranslateUi() {
    m_groupingCatLangAction->setText(tr("Category/Language"));
    m_groupingCatAction->setText(tr("Category"));
    m_groupingLangCatAction->setText(tr("Language/Category"));
    m_groupingLangAction->setText(tr("Language"));

    if (m_groupingNoneAction != 0) {
        m_groupingNoneAction->setText(tr("No grouping"));
    }
}

void BtBookshelfGroupingMenu::slotGroupingActionTriggered(QAction *action) {
    emit signalGroupingOrderChanged(getActionRef(action));
}