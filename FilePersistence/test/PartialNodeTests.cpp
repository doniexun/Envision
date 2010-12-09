/***********************************************************************************************************************
 * PartialNodeTests.cpp
 *
 *  Created on: Dec 8, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "filepersistence.h"
#include "FileStore.h"
#include "PartialList.h"
#include "SelfTest/headers/SelfTestSuite.h"
#include "ModelBase/headers/Model.h"
#include "ModelBase/headers/nodes/Text.h"
#include "ModelBase/headers/nodes/List.h"

#include <QtCore/QTextStream>
#include <QtCore/QFile>

namespace FilePersistence {

TEST(FilePersistence, LoadingList)
{
	QString testDir = ":/FilePersistence/test/persisted";
	Model::Model model;
	FileStore store;
	store.setBaseFolder(testDir);

	model.load(store, "partial");
	PartialList* root = dynamic_cast<PartialList*> (model.getRoot());
	CHECK_CONDITION(root != NULL);

	Model::List* list = root->list();

	CHECK_CONDITION(list != NULL);
	CHECK_STR_EQUAL("List", list->getTypeName() );
	CHECK_STR_EQUAL("TheList", list->getReferenceName());
	CHECK_CONDITION(list->isFullyLoaded() == false);
	CHECK_INT_EQUAL(1, list->getId());

	list->loadFully(store);

	CHECK_CONDITION(list->isFullyLoaded());
	CHECK_INT_EQUAL(4, list->size());

	Model::Text* one = dynamic_cast<Model::Text*> ((*list)[0]);
	Model::Text* two = dynamic_cast<Model::Text*> ((*list)[1]);
	Model::Text* three = dynamic_cast<Model::Text*> ((*list)[2]);
	Model::Text* four = dynamic_cast<Model::Text*> ((*list)[3]);

	CHECK_CONDITION(one != NULL);
	CHECK_STR_EQUAL("one", one->get());
	CHECK_INT_EQUAL(3, one->getId());

	CHECK_CONDITION(two != NULL);
	CHECK_STR_EQUAL("two", two->get());
	CHECK_INT_EQUAL(4, two->getId())

	CHECK_CONDITION(three != NULL);
	CHECK_STR_EQUAL("three", three->get());
	CHECK_INT_EQUAL(5, three->getId())

	CHECK_CONDITION(four != NULL);
	CHECK_STR_EQUAL("four", four->get());
	CHECK_INT_EQUAL(6, four->getId());

}

TEST(FilePersistence, SaveList)
{
	QString testDir = QDir::tempPath() + "/Envision/FilePersistence/tests";
	Model::Model model;
	FileStore store;
	store.setBaseFolder(testDir);

	PartialList* root = dynamic_cast<PartialList*> (model.createRoot("PartialList"));

	model.beginModification(root, "create ");
	root->list()->setReferenceName("TheList");
	Model::Text* t = NULL;
	t = dynamic_cast<Model::Text*> (root->list()->append("Text"));
	t->set("one");
	t = dynamic_cast<Model::Text*> (root->list()->append("Text"));
	t->set("two");
	t = dynamic_cast<Model::Text*> (root->list()->append("Text"));
	t->set("three");
	t = dynamic_cast<Model::Text*> (root->list()->append("Text"));
	t->set("four");
	model.endModification();

	store.saveModel(model, "partial");

	CHECK_TEXT_FILES_EQUAL(":/FilePersistence/test/persisted/partial/partial", testDir + "/partial/partial");

}

}
