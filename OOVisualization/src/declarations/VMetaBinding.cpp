/***********************************************************************************************************************
**
** Copyright (c) 2011, 2015 ETH Zurich
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
** following conditions are met:
**
**    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following
**      disclaimer.
**    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
**      following disclaimer in the documentation and/or other materials provided with the distribution.
**    * Neither the name of the ETH Zurich nor the names of its contributors may be used to endorse or promote products
**      derived from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
***********************************************************************************************************************/

#include "VMetaBinding.h"
#include "../OOVisualizationException.h"

#include "VisualizationBase/src/items/Static.h"
#include "VisualizationBase/src/items/VList.h"
#include "VisualizationBase/src/declarative/DeclarativeItemDef.h"

using namespace Visualization;
using namespace OOModel;

namespace OOVisualization {

ITEM_COMMON_DEFINITIONS(VMetaBinding, "item")

VMetaBinding::VMetaBinding(Item* parent, NodeType* node, const StyleType* style) : Super{parent, node, style}
{}

void VMetaBinding::initializeForms()
{
	addForm(new GridLayoutFormElement())
		->setHorizontalSpacing(3)
		->setVerticalAlignment(LayoutStyle::Alignment::Center)
		->setNoBoundaryCursors([](Item*){return true;})
		->setNoInnerCursors([](Item*){return true;})
		->put(0, 0, item(&I::name_, [](I* v){return v->node()->nameNode();}))
		->put(1, 0, item<Static>(&I::assignmentSymbol_, [](I* v){ return &v->style()->assignmentSymbol();}))
		->put(2, 0, item(&I::input_, [](I* v){return v->node()->input();}))
		->put(3, 0, item<VList>(&I::mappings_, [](I* v){return v->node()->mappings();},
				[](I* v){return &v->style()->mappings();}));
}

}
