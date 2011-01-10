/***********************************************************************************************************************
 * PartialList.cpp
 *
 *  Created on: Dec 9, 2010
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "test_nodes/PartialList.h"

namespace TestNodes {

EXTENDABLENODE_DEFINE_EMPTY_CONSTRUCTORS(PartialList, Model::ExtendableNode)
NODE_DEFINE_TYPE_REGISTRATION_METHODS(PartialList)

Model::ExtendableIndex PartialList::listIndex = Model::ExtendableIndex();

void PartialList::init()
{
	registerNodeType();
	listIndex = registerNewAttribute<PartialList> ("list", "List", true, false, true);
}

Model::List* PartialList::list()
{
	return static_cast<Model::List*> (get(listIndex));
}


}
