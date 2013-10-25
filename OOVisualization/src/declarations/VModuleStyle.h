/***********************************************************************************************************************
**
** Copyright (c) 2011, 2013 ETH Zurich
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

#pragma once

#include "../oovisualization_api.h"

#include "VisualizationBase/src/items/TextStyle.h"
#include "VisualizationBase/src/items/StaticStyle.h"
#include "VisualizationBase/src/items/VListStyle.h"
#include "VisualizationBase/src/layouts/PanelBorderLayoutStyle.h"
#include "VisualizationBase/src/layouts/PositionLayoutStyle.h"
#include "VisualizationBase/src/layouts/SequentialLayoutStyle.h"

namespace OOVisualization {

class OOVISUALIZATION_API VModuleStyle : public Visualization::ItemStyle
{
	private:
		Visualization::PanelBorderLayoutStyle layout_;
		Visualization::SequentialLayoutStyle header_;
		Visualization::TextStyle name_;
		Visualization::StaticStyle icon_;
		Visualization::PositionLayoutStyle body_;
		Visualization::SequentialLayoutStyle content_;
		Visualization::VListStyle fields_;
		Visualization::VListStyle declarations_;

	public:
		void load(Visualization::StyleLoader& sl);

		const Visualization::PanelBorderLayoutStyle& layout() const;
		const Visualization::SequentialLayoutStyle&  header() const;
		const Visualization::TextStyle&  name() const;
		const Visualization::StaticStyle&  icon() const;
		const Visualization::PositionLayoutStyle& body() const;
		const Visualization::SequentialLayoutStyle& content() const;
		const Visualization::VListStyle& fields() const;
		const Visualization::VListStyle& declarations() const;
};

inline const Visualization::PanelBorderLayoutStyle& VModuleStyle::layout() const {return layout_; }
inline const Visualization::SequentialLayoutStyle& VModuleStyle::header() const {return header_; }
inline const Visualization::TextStyle& VModuleStyle::name() const {return name_; }
inline const Visualization::StaticStyle& VModuleStyle::icon() const {return icon_; }
inline const Visualization::PositionLayoutStyle& VModuleStyle::body() const { return body_; }
inline const Visualization::SequentialLayoutStyle& VModuleStyle::content() const { return content_; }
inline const Visualization::VListStyle& VModuleStyle::fields() const { return fields_; }
inline const Visualization::VListStyle& VModuleStyle::declarations() const { return declarations_; }

}