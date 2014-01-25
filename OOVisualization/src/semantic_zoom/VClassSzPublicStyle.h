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
#include "VisualizationBase/src/layouts/SequentialLayoutStyle.h"
#include "VisualizationBase/src/layouts/PositionLayoutStyle.h"

namespace OOVisualization {

class OOVISUALIZATION_API VClassSzPublicStyle : public Visualization::DeclarativeItemBaseStyle
{
	private:
		Visualization::TextStyle nameDefault_;
		Visualization::TextStyle namePublic_;
		Visualization::TextStyle namePrivate_;
		Visualization::TextStyle nameProtected_;

		Visualization::StaticStyle classIcon_;
		Visualization::StaticStyle interfaceIcon_;
		Visualization::StaticStyle structIcon_;
		Visualization::StaticStyle unionIcon_;
		Visualization::StaticStyle enumIcon_;
		Visualization::StaticStyle annotationIcon_;

		Visualization::SequentialLayoutStyle header_;
		Visualization::VListStyle typeArguments_;
		Visualization::VListStyle baseClasses_;
		Visualization::VListStyle annotations_;
		Visualization::VListStyle enumerators_;
		Visualization::StaticStyle friendsSymbol_;
		Visualization::VListStyle friends_;
		Visualization::VListStyle declarations_;
		Visualization::PositionLayoutStyle body_;
		Visualization::SequentialLayoutStyle content_;
		Visualization::SequentialLayoutStyle fieldContainer_;
		Visualization::SequentialLayoutStyle publicFieldArea_;
		Visualization::SequentialLayoutStyle privateFieldArea_;
		Visualization::SequentialLayoutStyle protectedFieldArea_;
		Visualization::SequentialLayoutStyle defaultFieldArea_;

	public:
		void load(Visualization::StyleLoader& sl);

		const Visualization::TextStyle& nameDefault() const;
		const Visualization::TextStyle& namePublic() const;
		const Visualization::TextStyle& namePrivate() const;
		const Visualization::TextStyle& nameProtected() const;

		const Visualization::StaticStyle& classIcon() const;
		const Visualization::StaticStyle& interfaceIcon() const;
		const Visualization::StaticStyle& structIcon() const;
		const Visualization::StaticStyle& unionIcon() const;
		const Visualization::StaticStyle& enumIcon() const;
		const Visualization::StaticStyle& annotationIcon() const;

		const Visualization::SequentialLayoutStyle& header() const;
		const Visualization::VListStyle& typeArguments() const;
		const Visualization::VListStyle& baseClasses() const;
		const Visualization::VListStyle& annotations() const;
		const Visualization::VListStyle& enumerators() const;
		const Visualization::StaticStyle& friendsSymbol() const;
		const Visualization::VListStyle& friends() const;
		const Visualization::VListStyle& declarations() const;
		const Visualization::PositionLayoutStyle& body() const;
		const Visualization::SequentialLayoutStyle& content() const;
		const Visualization::SequentialLayoutStyle& fieldContainer() const;
		const Visualization::SequentialLayoutStyle& publicFieldArea() const;
		const Visualization::SequentialLayoutStyle& privateFieldArea() const;
		const Visualization::SequentialLayoutStyle& protectedFieldArea() const;
		const Visualization::SequentialLayoutStyle& defaultFieldArea() const;
};

inline const Visualization::TextStyle& VClassSzPublicStyle::nameDefault() const { return nameDefault_; }
inline const Visualization::TextStyle& VClassSzPublicStyle::namePublic() const { return namePublic_; }
inline const Visualization::TextStyle& VClassSzPublicStyle::namePrivate() const { return namePrivate_; }
inline const Visualization::TextStyle& VClassSzPublicStyle::nameProtected() const { return nameProtected_; }

inline const Visualization::StaticStyle& VClassSzPublicStyle::classIcon() const { return classIcon_; }
inline const Visualization::StaticStyle& VClassSzPublicStyle::interfaceIcon() const { return interfaceIcon_; }
inline const Visualization::StaticStyle& VClassSzPublicStyle::structIcon() const { return structIcon_; }
inline const Visualization::StaticStyle& VClassSzPublicStyle::unionIcon() const { return unionIcon_; }
inline const Visualization::StaticStyle& VClassSzPublicStyle::enumIcon() const { return enumIcon_; }
inline const Visualization::StaticStyle& VClassSzPublicStyle::annotationIcon() const { return annotationIcon_; }

inline const Visualization::SequentialLayoutStyle& VClassSzPublicStyle::header() const { return header_; }
inline const Visualization::VListStyle& VClassSzPublicStyle::typeArguments() const { return typeArguments_; }
inline const Visualization::VListStyle& VClassSzPublicStyle::baseClasses() const { return baseClasses_; }
inline const Visualization::VListStyle& VClassSzPublicStyle::annotations() const { return annotations_; }
inline const Visualization::VListStyle& VClassSzPublicStyle::enumerators() const { return enumerators_; }
inline const Visualization::StaticStyle& VClassSzPublicStyle::friendsSymbol() const{ return friendsSymbol_; }
inline const Visualization::VListStyle& VClassSzPublicStyle::friends() const { return friends_; }
inline const Visualization::VListStyle& VClassSzPublicStyle::declarations() const { return declarations_; }
inline const Visualization::PositionLayoutStyle& VClassSzPublicStyle::body() const { return body_; }
inline const Visualization::SequentialLayoutStyle& VClassSzPublicStyle::content() const { return content_; }
inline const Visualization::SequentialLayoutStyle& VClassSzPublicStyle::fieldContainer() const { return fieldContainer_; }
inline const Visualization::SequentialLayoutStyle& VClassSzPublicStyle::publicFieldArea() const { return publicFieldArea_; }
inline const Visualization::SequentialLayoutStyle& VClassSzPublicStyle::privateFieldArea() const { return privateFieldArea_; }
inline const Visualization::SequentialLayoutStyle& VClassSzPublicStyle::protectedFieldArea() const { return protectedFieldArea_; }
inline const Visualization::SequentialLayoutStyle& VClassSzPublicStyle::defaultFieldArea() const { return defaultFieldArea_; }

}
