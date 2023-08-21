#pragma once
#include <qapplication.h>
#include <qmainwindow.h>
#include <qlabel.h>
#include <qstackedlayout.h>
#include <qlayout.h>
#include <qmenu.h>
#include <qaction.h>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include <qpixmap.h>
#include <qbrush.h>
#include <qtoolbar.h>
#include <qslider.h>
#include <qevent.h>
#include "MapWidget.h"
#include "GraphicsView.h"
#include "Toolbars.h"

namespace dndmap {
	class MainWindow: public QMainWindow {
		Q_OBJECT
	public:
		MainWindow();
		int scale = 30; // holds universal scaleFactor of all widgets //
		std::vector<QPixmap> savedSprites; // array of currently added sprites //

	private:
		graphics::MyGraphicsScene* scene;
		graphics::MyGraphicsView* view;
		mytoolbars::GridToolbar* gridToolBar;
		mytoolbars::SpriteToolbar* spriteToolbar;
		QStackedLayout* layout;
		QMenu* fileMenu;
		QAction* newMap;
		QMenu* aboutMenu;

	public slots:
		void openNewMap();
		void setMap(const char* url);
		void zoom(float scale);
		void addSprite(int sprite);
		void saveSprite(const QPixmap& sprite);
		void removeSprite(int index);
		void showLicense();

	};
} // end namespace main
