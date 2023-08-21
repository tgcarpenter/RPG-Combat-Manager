#pragma once
#include "MapWidget.h"
#include <qaction.h>
#include <qlayout.h>
#include <qmenu.h>
#include <qpushbutton.h>
#include <qscrollarea.h>
#include <qslider.h>
#include <qtoolbar.h>
#include <qwidget.h>


namespace mytoolbars{

	class GridToolbar : public QToolBar {
	private:
		QAction* offsetAction;
		QMenu* offsetMenu;
		QSlider* gridSlider;
		QSlider* xOffsetSlider;
		QSlider* yOffsetSlider;
		QAction* zoomIn;
		QAction* zoomOut;

	public:
		GridToolbar(QWidget* parent);
		QSlider* gSlider() const { return gridSlider; }
		QSlider* xSlider() const { return xOffsetSlider; }
		QSlider* ySlider() const { return yOffsetSlider; }

	};

	class SpriteButton : public QPushButton {
		Q_OBJECT
	private:
		int position;
		QMenu* context;

	public:
		SpriteButton(const int pos, QWidget* parent);
		int getPos() const { return position; }
		void setPosition(int pos);

	private slots:
		void emitButtonPressed();
		void emitDeleteSelf();
		void contextMenu();

	signals:
		void buttonPressed(int);
		void deleteSelf(int);
	};

	class SpriteToolbar : public QToolBar {
		Q_OBJECT

	private:
		std::vector<mapwidget::Sprite> sprites;
		QGridLayout* spriteLayout;
		QWidget* spriteMenu;
		QPixmap sprite;
		QScrollArea* scrollArea;

	public:
		SpriteToolbar(QWidget* parent);
		void addSprite();
		void removeSprite(int index);
		void loadSprite(int sprite);

	signals:
		void saveSprite(const QPixmap&);
		void spriteAdded(int);
		void spriteDeleted(int);
	};

} // end namespace mytoolbars
