#pragma once
#include <qevent.h>
#include <qgraphicsitem.h>
#include <qmenu.h>
#include <qobject.h>
#include <qpoint.h>
#include <qstackedlayout.h>
#include <qwidget.h>

namespace mapwidget {
	// Sprite //
	class Sprite : public QObject, public QGraphicsPixmapItem
	{
		Q_OBJECT

	public:
		enum SpriteOffset // enum type held * 100
		{
			FineOffset = - 45,
			DiminutiveOffset = -37,
			TinyOffset = -25,
			SmallOffset = -12,
			MediumOffset = 0,
			LargeOffset = 25,
			HugeOffset = 50,
			GargantuanOffset = 100,
			ColossalOffset = 200
		};

		enum SpriteSize
		{
			Fine = 10,
			Diminutive = 26,
			Tiny = 50,
			Small = 76,
			Medium = 100,
			Large = 150,
			Huge = 200,
			Gargantuan = 400,
			Colossal = 800
		};

	private:
		int scale;
		float scaleFactor = 1.00;
		float offsetFactor = 0.00;
		int xOffset = 0;
		int yOffset = 0;
		QPixmap pixmap;
		QPixmap highlight;
		QMenu* popupMenu;

	public:
		Sprite(const QPixmap& image, int factor);
	
	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
		void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
		QPixmap makeHighlightPixmap(const QPixmap& pixmap);
		QPointF getGridCenter(QPointF scenePos) const;

	public slots:
		void setScale(int factor);
		void setScaleFactor(int size);
		void setXOffset(int offset);
		void setYOffset(int offset);
	};

	// MapOverlay //
	class MapOverlay : public QWidget
	{
	private:
		int scale;

	public:
		MapOverlay(int factor, QWidget* parent);

	protected:
		std::vector<Sprite*> loadedSprites;
		void paintEvent(QPaintEvent* event) override;


	};
} // end namespace mapwidget //