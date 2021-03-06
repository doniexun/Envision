/***********************************************************************************************************************
**
** Copyright (c) 2011, 2014 ETH Zurich
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

#include "../visualizationbase_api.h"

#include "../items/Mipmap.h"
#include "ShapeStyle.h"

namespace Visualization {

class VISUALIZATIONBASE_API BoxStyle : public Super<ShapeStyle>
{
	public:
		enum class CornerType : int {RightAngle, Round, Cut};

		virtual ~BoxStyle() override;

		void paint(QPainter* painter, int xOffset, int yOffset, int contentBoxWidth, int contentBoxHeight,
					  QColor customColor) const;

		Property<QBrush> background{this, "backgroundBrush"};
		Property<CornerType> corner{this, "cornerType"};
		Property<int> cornerRadius{this, "cornerRadius"};

		static QBrush fixGradient(const QBrush& gradientBrush, int x, int y);

	protected:
		QPainterPath getRectanglePath(qreal x, qreal y, int width, int height) const;

		void unoptimizedPaint(QPainter* painter, int xOffset, int yOffset, int contentBoxWidth,
						int contentBoxHeight, QColor customColor) const;
		void simplifiedPaint(QPainter* painter, int xOffset, int yOffset, int contentBoxWidth,
								int contentBoxHeight, QColor customColor) const;
};

inline QBrush BoxStyle::fixGradient(const QBrush& gradientBrush, int x, int y)
{
	QBrush result;
	if ( gradientBrush.style() == Qt::LinearGradientPattern
			&& gradientBrush.gradient()->coordinateMode() == QGradient::LogicalMode )
	{
		QLinearGradient g = *(static_cast<const QLinearGradient*> (gradientBrush.gradient()));
		g.setStart(x + g.start().x(), y + g.start().y());
		g.setFinalStop(x + g.finalStop().x(), y + g.finalStop().y());
		result = g;

	}
	else if ( gradientBrush.style()  == Qt::RadialGradientPattern
			&& gradientBrush.gradient()->coordinateMode() == QGradient::LogicalMode )
	{
		QRadialGradient g = *(static_cast<const QRadialGradient*> (gradientBrush.gradient()));
		g.setCenter(x + g.center().x(), y + g.center().y());
		g.setFocalPoint(x + g.focalPoint().x(), y + g.focalPoint().y());
		result = g;
	}
	else result = gradientBrush;

	return result;
}

}
