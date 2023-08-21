#include "GraphicsView.h"
#include <iostream>
#include <qapplication.h>
#include <qrect.h>
#include <qpainter>
#include <qtransform.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qdebug.h>
#include <qgraphicsitem.h>
#include <qstyleoption.h>
#include <qscrollbar.h>

namespace graphics {

	MyGraphicsScene::MyGraphicsScene(QWidget* parent)
	{
		grid = QPixmap("Media/grid.png");
		//this->addPixmap(grid);
	}

	void MyGraphicsScene::setMap(QPixmap newMap)
	{
		map = newMap;
		mapSet = true;
	}

	void MyGraphicsScene::setScale(int factor) {
		scale = factor;
		emit scaleChanged(factor);
		this->setBackgroundBrush(QBrush());
	}

	void MyGraphicsScene::setXOffset(int offset)
	{
		xOffset = 100 - offset;
		this->setBackgroundBrush(QBrush());
	}

	void MyGraphicsScene::setYOffset(int offset)
	{
		yOffset = 100 - offset;
		this->setBackgroundBrush(QBrush());
	}

	void MyGraphicsScene::setZoomFactor(float factor)
	{
		zoomFactor = factor;
	}

	void MyGraphicsScene::drawBackground(QPainter* painter, const QRectF& rect)
	{
		if (!map.isNull()) {
			QGraphicsItem* item[1] = { new QGraphicsPixmapItem(map) };
			this->drawItems(painter, 1, item, new QStyleOptionGraphicsItem());
		}
		painter->save();
		for (int x = (scale * -100) - xOffset; x < rect.width() * 5; x += scale) {
			painter->drawLine(x, rect.height() * -5, x, rect.height() * 5);
		}
		for (int y = (scale * -100) - yOffset; y < rect.height() * 5; y += scale) {
			painter->drawLine(rect.width() * -5, y, rect.width() * 5, y);
		}
		painter->restore();
	}

	// MyGraphicsView //
	MyGraphicsView::MyGraphicsView(QGraphicsScene* scene, QWidget* parent, int sFactor) :
		QGraphicsView(scene, parent)
	{
		bscale = sFactor;

		baseGrid = QPixmap("Media/grid.png");
		grid = QPixmap("Media/grid.png");
		scaleGrid(30);
		this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	}

	void MyGraphicsView::zoom(float factor) // updates scaleFactor and rescales widget //
	{
		zoomFactor *= factor;
		emit zoomFactorChanged(zoomFactor);
		this->scale(factor, factor);
	}

	void MyGraphicsView::setScene(QGraphicsScene* scene)
	{
		QGraphicsView::setScene(scene);
	}

	void MyGraphicsView::setXOffset(int offset) // edits grid pixmap to move vertical bar along the x axis
	{  // fix making grid smaller
		curXOffset = offset;
		emit xOffsetChanged(offset);
	}

	void MyGraphicsView::setYOffset(int offset) // edits grid pixmap to move vertical bar along the y axis
	{
		curYOffset = offset;
		emit yOffsetChanged(offset);
	}

	void MyGraphicsView::scaleGrid(int height)
	{
		bscale = height;
		emit scaleChanged(height);
	}

	void MyGraphicsView::drawForeground(QPainter* painter, const QRectF& rect)
	{
		QGraphicsView::drawForeground(painter, rect);
	}

	void MyGraphicsView::wheelEvent(QWheelEvent* event)
	{
		auto modifiers = QApplication::keyboardModifiers();
		const ViewportAnchor anchor = this->transformationAnchor();
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		if (event->angleDelta().y() > 0 && modifiers == Qt::ControlModifier) {
			// Zoom In
			zoom(1.25);
			return;
		} else if (event->angleDelta().y() < 0 && modifiers == Qt::ControlModifier) {
			// Zooming out
			zoom(.8);
			return;
		}
		setTransformationAnchor(anchor);
		QGraphicsView::wheelEvent(event);
	}
} // end namespace graphics