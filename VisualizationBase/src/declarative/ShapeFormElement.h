/***********************************************************************************************************************
 **
 ** Copyright (c) 2011, 2014 ETH Zurich
 ** All rights reserved.
 **
 ** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 ** following conditions are met:
 **
 **    * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 **      following disclaimer.
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
 **********************************************************************************************************************/

#pragma once

#include "../visualizationbase_api.h"
#include "FormElement.h"

namespace Visualization {

/**
 * The shape form element is a placeholder form element, stretching to encompass all the available space. It is used to
 * put the shape at a custom position.
 */
class VISUALIZATIONBASE_API ShapeFormElement
		: public SuperFormElement<ShapeFormElement, FormElement>
{
	public:

		ShapeFormElement() = default;
		ShapeFormElement(const ShapeFormElement&) = default;
		ShapeFormElement& operator=(const ShapeFormElement&) = delete;
		virtual ~ShapeFormElement();

		virtual ShapeFormElement* clone() const override;

		virtual QList<FormElement*> shapeElements() override;
		virtual void computeSize(Item* item, int availableWidth, int availableHeight) override;
		virtual bool sizeDependsOnParent(const Item* item) const override;
		virtual bool isEmpty(const Item* item) const override;

		// Do not forget to update the copy constructor if adding new members.
};

}
