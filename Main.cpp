#include "Main.h"
#include <qdesktopservices.h>
#include <qmenubar.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <iostream>
#include <qkeysequence.h>
#include <qgraphicsitem.h>


int MAGIC_NUMBER = 0x23464217;
int FILE_VERSION = 100;

namespace dndmap {
	MainWindow::MainWindow()
	{
		this->resize(800, 600);

		scene = new graphics::MyGraphicsScene(this);

		view = new graphics::MyGraphicsView(scene, this, scale);
		view->setDragMode(QGraphicsView::ScrollHandDrag);

		fileMenu = new QMenu("File", this);
		newMap = new QAction("New", this);
		fileMenu->addAction(newMap);
		QObject::connect(newMap, &QAction::triggered, this, &MainWindow::openNewMap);
		this->menuBar()->addMenu(fileMenu);

		aboutMenu = new QMenu("About", this);
		QAction* license = new QAction("License", this);
		aboutMenu->addAction(license);
		QObject::connect(license, &QAction::triggered, this, &MainWindow::showLicense);
		this->menuBar()->addMenu(aboutMenu);

		gridToolBar = new mytoolbars::GridToolbar(this);
		this->addToolBar(gridToolBar);

		QObject::connect(gridToolBar->gSlider(), &QSlider::valueChanged, scene, &graphics::MyGraphicsScene::setScale);
		QObject::connect(gridToolBar->xSlider(), &QSlider::valueChanged, scene, &graphics::MyGraphicsScene::setXOffset);
		QObject::connect(gridToolBar->ySlider(), &QSlider::valueChanged, scene, &graphics::MyGraphicsScene::setYOffset);

		spriteToolbar = new mytoolbars::SpriteToolbar(this);
		this->addToolBar(Qt::RightToolBarArea, spriteToolbar);

		QObject::connect(spriteToolbar, &mytoolbars::SpriteToolbar::spriteAdded, this, &MainWindow::addSprite);
		QObject::connect(spriteToolbar, &mytoolbars::SpriteToolbar::saveSprite, this, &MainWindow::saveSprite);
		QObject::connect(spriteToolbar, &mytoolbars::SpriteToolbar::spriteDeleted, this, &MainWindow::removeSprite);

		layout = new QStackedLayout(this);

		layout->setStackingMode(QStackedLayout::StackAll);

		layout->addWidget(view);

		QWidget* widget = new QWidget();
		widget->setLayout(layout);

		this->setCentralWidget(widget);
	}

	void MainWindow::addSprite(int index)
	{
		mapwidget::Sprite* newSprite = new mapwidget::Sprite(savedSprites[index], scene->getscale());
		QObject::connect(scene, &graphics::MyGraphicsScene::scaleChanged, newSprite, &mapwidget::Sprite::setScale);
		QObject::connect(gridToolBar->xSlider(), &QSlider::valueChanged, newSprite, &mapwidget::Sprite::setXOffset);
		QObject::connect(gridToolBar->ySlider(), &QSlider::valueChanged, newSprite, &mapwidget::Sprite::setYOffset);
		newSprite->setFlag(QGraphicsItem::ItemIsMovable);
		newSprite->setPos(scene->sceneRect().center());
		scene->addItem(newSprite);
	}

	void MainWindow::saveSprite(const QPixmap& sprite) // creates and adds sprite to scene
	{	
		savedSprites.push_back(sprite);
	}

	void MainWindow::removeSprite(int index)  //generates iterator, moved to index, erases pixmap
	{
		auto it = savedSprites.begin();
		std::advance(it, index);
		savedSprites.erase(it);
	}

	void MainWindow::openNewMap()
	{
		QString dir = QFileDialog::getOpenFileName(this, tr("Select Image"), "/home", tr("Images (*.png *.xpm *.jpg)"));
		auto p = QPixmap(dir);
		p.fill(Qt::transparent);
		scene->addPixmap(p);
		scene->setMap(dir);
	}

	void MainWindow::setMap(const char* url)
	{
		std::cout << "test";
	}

	void MainWindow::zoom(float factor) // calls MyGraphicsView->zoom //
	{
		view->zoom(factor);
	}

	void MainWindow::showLicense()
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile("LICENSE.txt"));
	}

} // end namespace main

int main(int argc, char* argv[]) {
	QApplication app = QApplication(argc, argv);
	dndmap::MainWindow window;
	window.show();
	return app.exec();
}
