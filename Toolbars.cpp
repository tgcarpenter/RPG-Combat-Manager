#include "Toolbars.h"
#include <qcursor.h>
#include <qfiledialog.h>
#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qscrollbar.h>
#include <qsize.h>
#include <qsizepolicy.h>
#include <qtoolbutton.h>
namespace mytoolbars {
	GridToolbar::GridToolbar(QWidget* parent) :
		QToolBar(parent)
	{
		this->layout()->setSpacing(10);
		this->setAutoFillBackground(true);

		gridSlider = new QSlider(Qt::Horizontal, this);
		gridSlider->setFixedHeight(24);
		gridSlider->setFixedWidth(250);
		gridSlider->setMinimum(30);
		gridSlider->setMaximum(150);
		gridSlider->setValue(50);

		xOffsetSlider = new QSlider(Qt::Horizontal, this);
		xOffsetSlider->setFixedWidth(100);
		xOffsetSlider->setMaximum(100);
		xOffsetSlider->setValue(100);

		yOffsetSlider = new QSlider(Qt::Vertical, this);
		yOffsetSlider->setFixedHeight(100);
		yOffsetSlider->setMaximum(100);
		yOffsetSlider->setValue(100);

		offsetAction = new QAction("Adjust Grid", this);
		QToolButton* offsetButton = new QToolButton();
		offsetMenu = new QMenu(offsetButton);
		offsetButton->setFixedWidth(80);
		offsetButton->setPopupMode(QToolButton::InstantPopup);
		offsetButton->setDefaultAction(offsetAction);
		offsetButton->setMenu(offsetMenu);
		offsetButton->setAutoFillBackground(true);

		QHBoxLayout* offsetLayout = new QHBoxLayout();
		offsetLayout->addWidget(xOffsetSlider);
		offsetLayout->addWidget(yOffsetSlider);

		offsetMenu->setLayout(offsetLayout);

		zoomIn = new QAction("+", this);
		QObject::connect(zoomIn, &QAction::triggered, gridSlider, [&]() {
			for (int i; i < 25; i++)
				gridSlider->setValue(gridSlider->value() + 1); });

		zoomOut = new QAction("-", this);
		QObject::connect(zoomOut, &QAction::triggered, gridSlider, [&]() {
			for (int i; i < 25; i++)
				gridSlider->setValue(gridSlider->value() - 1); });

		this->addWidget(offsetButton);
		this->addWidget(gridSlider);
		this->addAction(zoomIn);
		this->addSeparator();
		this->addAction(zoomOut);
	}

	// SpriteButton //
	SpriteButton::SpriteButton(const int pos, QWidget* parent = (QWidget*)nullptr) :
		QPushButton(parent), position(pos)
	{
		this->setContextMenuPolicy(Qt::CustomContextMenu);
		QObject::connect(this, &QPushButton::customContextMenuRequested, this, &SpriteButton::contextMenu);

		QObject::connect(this, &QPushButton::clicked, this, &SpriteButton::emitButtonPressed);

		context = new QMenu(this);
		QAction* del = new QAction("Delete", this);
		QObject::connect(del, &QAction::triggered, this, &SpriteButton::emitDeleteSelf);

		context->addAction(del);
	}

	void SpriteButton::setPosition(int pos)
	{
		position = pos;
	}

	void SpriteButton::contextMenu()
	{
		context->popup(QCursor::pos());
	}

	void SpriteButton::emitDeleteSelf()
	{
		emit deleteSelf(position);
	}

	void SpriteButton::emitButtonPressed()
	{
		emit buttonPressed(position);
	}

	// SpriteToolbar //
	SpriteToolbar::SpriteToolbar(QWidget* parent):
		QToolBar(parent)
	{
		this->setAutoFillBackground(true);
		this->setFixedWidth(105);
		this->setAllowedAreas(Qt::RightToolBarArea);
		this->setMovable(false);

		QLabel* l = new QLabel("Sprites:", this);
		l->setAlignment(Qt::AlignHCenter);

		spriteLayout = new QGridLayout();
		spriteLayout->setContentsMargins(5, 0, 0, 0);
		spriteLayout->setSpacing(5);
		spriteLayout->setVerticalSpacing(5);

		spriteMenu = new QWidget(this);
		spriteMenu->setLayout(spriteLayout);

		scrollArea = new QScrollArea(this);
		scrollArea->setWidget(spriteMenu);
		scrollArea->setWidgetResizable(true);
		scrollArea->setFrameShape(QFrame::NoFrame);
		scrollArea->verticalScrollBar()->hide();

		QPushButton* addSprite = new QPushButton("+", this);
		addSprite->setFixedSize(30, 40);
		addSprite->setFlat(true);
		auto f = addSprite->font();
		f.setPointSizeF(30);
		f.setBold(true);
		addSprite->setFont(f);
		QObject::connect(addSprite, &QPushButton::clicked, this, &SpriteToolbar::addSprite);

		QWidget* addSWidget = new QWidget(this);
		QHBoxLayout* addSLayout = new QHBoxLayout;
		QWidget* spacer = new QWidget(this);
		spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		addSLayout->addWidget(spacer);
		addSLayout->addWidget(addSprite, Qt::AlignRight);
		addSWidget->setLayout(addSLayout);


		QWidget* s = new QWidget(this);
		s->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

		//spriteLayout->addWidget(s);
		//spriteLayout->addWidget(s);

		this->addWidget(l);
		this->addWidget(scrollArea);
		this->addWidget(addSWidget);
	}

	void SpriteToolbar::addSprite()
	{
		auto ret = QFileDialog::getOpenFileName(this, tr("Select Image"), "/home", tr("Images (*.png *.xpm *.jpg)"));
		if (ret.isEmpty()) return;
		QPixmap sprite = QPixmap(ret);
		QIcon icon = QIcon(sprite);

		SpriteButton* action = new SpriteButton(spriteLayout->count(), this);
		action->setFixedHeight(40);
		action->setFixedWidth(40);
		action->setFlat(true);
		action->setIcon(icon);
		action->setIconSize(QSize(40, 40));

		int column = spriteLayout->count() % 2;
		int row = spriteLayout->count() / 2;
		spriteLayout->addWidget(action, row, column, Qt::AlignTop);
		spriteLayout->setAlignment(Qt::AlignTop);
		QObject::connect(action, &SpriteButton::buttonPressed, this, &SpriteToolbar::loadSprite);
		QObject::connect(action, &SpriteButton::deleteSelf, this, &SpriteToolbar::removeSprite);

		emit saveSprite(sprite);
	}

	void SpriteToolbar::removeSprite(int index) //doesn't quite work
	{
		int count = spriteLayout->count();
		SpriteButton* widget = (SpriteButton*)spriteLayout->itemAtPosition(index / 2, index % 2)->widget();
		widget->deleteLater();
		for (int i = index + 1; i < count; i++) {
			SpriteButton* sprite = (SpriteButton*)spriteLayout->itemAtPosition(i / 2, i % 2)->widget();
			sprite->setPosition(i - 1);
			spriteLayout->addWidget(sprite, (i - 1) / 2, (i - 1) % 2);
		}
		this->repaint();
		emit spriteDeleted(index);
	}

	void SpriteToolbar::loadSprite(int sprite)
	{
		emit spriteAdded(sprite);
	}

} // end namespace mytoolbars