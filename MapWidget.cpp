#include "MapWidget.h"
#include <iostream>
#include <qapplication.h>
#include <qcolor.h>
#include <qcursor.h>
#include <qdebug.h>
#include <qdrag.h>
#include <qfiledialog.h>
#include <qgraphicssceneevent.h>
#include <qimage.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter>
#include <qpixmap.h>
#include <qrect.h>
#include <qslider.h>
#include <qtoolbutton.h>
#include <qtransform.h>

namespace mapwidget {

	// MapOverlay //
	MapOverlay::MapOverlay(int factor, QWidget* parent):
		QWidget(parent)
	{
		scale = 1;
		this->setMouseTracking(true);
	}

	void MapOverlay::paintEvent(QPaintEvent* event)
	{
		QWidget::paintEvent(event);
	}

	// Sprite //
	Sprite::Sprite(const QPixmap& image, int factor):
		QGraphicsPixmapItem()
	{
		scale = factor;

		if (image.height() > image.width()) this->setPixmap(image.scaledToHeight(scale));
		else this->setPixmap(image.scaledToWidth(scale));

		pixmap = image;
		highlight = makeHighlightPixmap(image);

		popupMenu = new QMenu();

		QLabel* size = new QLabel("Size: Medium", popupMenu);
		QSlider* sizeBar = new QSlider(Qt::Horizontal, popupMenu);
		sizeBar->setMaximum(9);
		sizeBar->setMinimum(1);
		sizeBar->setFixedWidth(100);
		sizeBar->setSingleStep(1);
		sizeBar->setValue(5);
		QObject::connect(sizeBar, &QSlider::valueChanged, this, &Sprite::setScaleFactor);
		QVBoxLayout* layout= new QVBoxLayout(popupMenu);
		layout->addWidget(size);
		layout->addWidget(sizeBar);

		popupMenu->setLayout(layout);

		this->show();
	}

	void Sprite::setScaleFactor(int size) // offsetFactor not set Properly
	{
		QLabel* l = static_cast<QLabel*>(popupMenu->layout()->itemAt(0)->widget());
		switch (size) {
		case 1:
			offsetFactor = (float)FineOffset / 100;
			scaleFactor = (float)Fine / 100;
			l->setText("Size: Fine");
			break;
		case 2:
			offsetFactor = (float)DiminutiveOffset / 100;
			scaleFactor = (float)Diminutive / 100;
			l->setText("Size: Diminutive");
			break;
		case 3:
			offsetFactor = (float)TinyOffset / 100;
			scaleFactor = (float)Tiny / 100;
			l->setText("Size: Tiny");
			break;
		case 4:
			offsetFactor = (float)SmallOffset / 100;
			scaleFactor = (float)Small / 100;
			l->setText("Size: Small");
			break;
		case 5:
			offsetFactor = (float)MediumOffset / 100;
			scaleFactor = (float)Medium / 100;
			l->setText("Size: Medium");
			break;
		case 6:
			offsetFactor = (float)LargeOffset / 100;
			scaleFactor = (float)Large / 100;
			l->setText("Size: Large");
			break;
		case 7:
			offsetFactor = (float)HugeOffset / 100;
			scaleFactor = (float)Huge / 100;
			l->setText("Size: Huge");
			break;
		case 8:
			offsetFactor = (float)GargantuanOffset / 100;
			scaleFactor = (float)Gargantuan / 100;
			l->setText("Size: Gargantuan");
			break;
		case 9:
			offsetFactor = (float)ColossalOffset / 100;
			scaleFactor = (float)Colossal / 100;
			l->setText("Size: Colossal");
			break;
		default:
			return;
		}
		setScale(scale);
	}

	void Sprite::setScale(int factor)
	{
		QPointF p = this->scenePos();
		//calculate grid index
		float x = p.x() / scale;
		float y = p.y() / scale;
		//sets position to index's new position 
		p.setX(x * factor + 5);
		p.setY(y * factor + scale * offsetFactor + 5);

		scale = factor;
		if (pixmap.height() > pixmap.width()) this->setPixmap(pixmap.scaledToHeight(scale * scaleFactor));
		else this->setPixmap(pixmap.scaledToWidth(scale * scaleFactor));
		this->setPos(getGridCenter(p));
	}

	void Sprite::setXOffset(int offset)
	{
		xOffset = 100 - offset;
		this->setPos(getGridCenter(this->scenePos()));
	}

	void Sprite::setYOffset(int offset)
	{
		yOffset = 100 - offset;
		this->setPos(getGridCenter(this->scenePos()));
	}

	void Sprite::mousePressEvent(QGraphicsSceneMouseEvent* event)
	{
		if (highlight.height() > highlight.width()) this->setPixmap(highlight.scaledToHeight(scale * scaleFactor));
		else this->setPixmap(highlight.scaledToWidth(scale * scaleFactor));

		QGraphicsPixmapItem::mousePressEvent(event);

	}

	void Sprite::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
	{

		if (pixmap.height() > pixmap.width()) this->setPixmap(pixmap.scaledToHeight(scale * scaleFactor));
		else this->setPixmap(pixmap.scaledToWidth(scale * scaleFactor));

		if (event->button() == Qt::RightButton) {
			this->contextMenuEvent(new QGraphicsSceneContextMenuEvent());
		}
		else if (event->button() == Qt::LeftButton) {

			this->setPos(getGridCenter(event->scenePos()));
		}

		QGraphicsPixmapItem::mouseReleaseEvent(event);
	}

	void Sprite::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
	{
		popupMenu->popup(QCursor::pos());
	}

	QPointF Sprite::getGridCenter(QPointF scenePos) const // finds the center of a grid square in relation to SpriteSize
	{
		QPointF anchor = scenePos;

		int x = anchor.x();
		int xOff = xOffset;
		if (x % scale != scale / 2) {
			if (x % scale + xOffset > scale) xOff = xOffset - 100;  // checks if anchor point if to the left or right of default anchor
			anchor.setX((x - (x % scale) + scale / 2) - xOff);
		}

		int y = anchor.y();
		int yOff = yOffset;
		if (y % scale != scale / 2) {
			if (y % scale + yOffset > scale) yOff = yOffset - 100;
			anchor.setY((y - (y % scale) + scale / 2) - yOff);
		}

		QPointF center = this->boundingRect().center();

		center.setY((center.y() + scale * offsetFactor));

		return anchor - center;
	}

	QPixmap Sprite::makeHighlightPixmap(const QPixmap& pixmap)
	{
		QImage newMap = QImage((pixmap.scaled(pixmap.width() + 8, pixmap.height() + 8)).toImage());
		QPainter p = QPainter(&newMap);
		for (int x = 0; x < newMap.width(); x++) {
			for (int y = 0; y < newMap.height(); y++) {
				if (newMap.pixelColor(x, y).alpha() != 0) {
					newMap.setPixelColor(x, y, QColor(116, 124, 246));
				}
			}
		}
		p.drawPixmap(4, 4, pixmap);
		return QPixmap::fromImage(newMap);
	}
}