#pragma once
#include "Toolbars.h"
#include <qevent.h>
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qpixmap.h>
#include <qslider.h>
#include <qmenu.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qrect.h>


namespace graphics {
	// MyGraphicsScene //
	class MyGraphicsScene : public QGraphicsScene {
		Q_OBJECT

	private:
		int scale = 50;
		int xOffset = 0;
		int yOffset = 0;
		float zoomFactor = 1.00;
		bool mapSet = false;

	public:
		QPixmap grid;
		QPixmap map;

	public:
		MyGraphicsScene(QWidget* parent);
		void setMap(QPixmap newMap);
		int getscale() const { return scale; }
		bool hasMap() const { return mapSet; }

	protected:
		void drawBackground(QPainter* painter, const QRectF& rect) override;

	public slots:
		void setScale(int factor);
		void setXOffset(int offset);
		void setYOffset(int offset);
		void setZoomFactor(float factor);

	signals:
		void scaleChanged(int);
	};

	// MyGraphicsView //
	class MyGraphicsView : public QGraphicsView {

		Q_OBJECT

	public:
		MyGraphicsView(QGraphicsScene* scene, QWidget* parent, int sFactor);
		void wheelEvent(QWheelEvent* event) override;
		void zoom(float scale);
		void setScene(QGraphicsScene* scene);

	private:
		int bscale;
		int curXOffset = 95;
		int curYOffset = 100;
		float zoomFactor = 1.00;
		QPixmap baseGrid;
		QPixmap grid;
		mytoolbars::GridToolbar* gridToolBar;
		mytoolbars::SpriteToolbar* spriteToolbar;

	protected:
		void drawForeground(QPainter* painter, const QRectF& rect) override;
		//bool event(QEvent* event) override;

	public slots:
		void setXOffset(int offset);
		void setYOffset(int offset);
		void scaleGrid(int height);

	signals:
		void scaleChanged(int);
		void xOffsetChanged(int);
		void yOffsetChanged(int);
		void zoomFactorChanged(float);
	};
} // end namespace graphics