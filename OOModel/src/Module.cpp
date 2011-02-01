/***********************************************************************************************************************
 * Module.cpp
 *
 *  Created on: Jan 27, 2011
 *      Author: Dimitar Asenov
 **********************************************************************************************************************/

#include "Module.h"

namespace OOModel {

EXTENDABLENODE_DEFINE_EMPTY_CONSTRUCTORS(Module, Model::ExtendableNode)
EXTENDABLENODE_DEFINE_TYPE_REGISTRATION_METHODS(Module, Model::ExtendableNode)

REGISTER_ATTRIBUTE(Module, name, Text, false, false, true)
REGISTER_ATTRIBUTE(Module, modules, TypedListOfModule, false, false, true)
REGISTER_ATTRIBUTE(Module, classes, TypedListOfClass, false, false, true)

QString Module::referenceName() const
{
	return name();
}

}
