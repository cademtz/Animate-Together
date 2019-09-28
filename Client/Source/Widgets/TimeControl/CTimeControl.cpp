/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 9/19/2019 10:07:34 AM
 */

#include "CTimeControl.h"
#include <qframe.h>
#include <qshortcut.h>

#include "Projects/CProject.h"
#include "Projects/OnionSkin/COnionSkin.h"

// Just for now, I promise. It'll be cleaned up in the future ... :|
class CDivider : public QFrame
{
public:
	CDivider(QWidget* Parent, bool Vertical = true) : QFrame(Parent)
	{
		setFrameShape(Vertical ? QFrame::VLine : QFrame::HLine);
		setLineWidth(2);
		if (Vertical)
			setFixedWidth(10);
		else
			setFixedHeight(10);
	}
};

CTimeControl::CTimeControl(QWidget * Parent) : QWidget(Parent)
{
	setFixedHeight(24);

	m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	m_svgplay = QIcon(":/Icons/Play.svg");
	m_svgpause = QIcon(":/Icons/Pause.svg");

	m_play = new QPushButton(this), m_bkstep = new QPushButton(this), m_fwdstep = new QPushButton(this), m_appleskin = new QPushButton(this);
	m_fps = new QSpinBox(this);

	QSize btnsize(20, 20), iconsize = btnsize / 2;
	m_play->setFixedSize(btnsize);
	m_bkstep->setFixedSize(btnsize);
	m_fwdstep->setFixedSize(btnsize);
	m_appleskin->setFixedSize(btnsize);
	m_fps->setFixedHeight(btnsize.height());

	m_play->setIcon(m_svgplay);
	m_fwdstep->setIcon(QIcon(":/Icons/FwdStep.svg"));
	m_bkstep->setIcon(QIcon(":/Icons/BackStep.svg"));
	m_play->setIconSize(iconsize);
	m_fwdstep->setIconSize(iconsize);
	m_bkstep->setIconSize(iconsize);

	m_appleskin->setText("O");
	m_appleskin->setCheckable(true);

	m_fps->setMinimum(1);
	m_fps->setMaximum(120);
	m_fps->setSuffix(" fps");
	m_fps->setButtonSymbols(QSpinBox::ButtonSymbols::NoButtons);
	
	setStyleSheet(
		"QPushButton {"
			"color: white;"
			"border: none;"
			"background-color: none"
		"}"
		"QPushButton:hover {"
			"border: 1px solid #4B4B4B;"
			"background-color: #3C3C3C;"
		"}"
		"QPushButton:checked, QPushButton:pressed {"
			"background-color: #191919;"
			"border: 1px solid #4B4B4B;"
		"}"
		"QSpinBox {"
			"color: #C8C8C8;"
			"border: none;"
		"}"
		"QFrame { color: #202020; }");
			
	m_layout->addWidget(m_fps);
	m_layout->addWidget(new CDivider(this));
	m_layout->addWidget(m_bkstep);
	m_layout->addWidget(m_play);
	m_layout->addWidget(m_fwdstep);
	m_layout->addWidget(new CDivider(this));
	m_layout->addWidget(m_appleskin);

	m_play->setShortcut(Qt::Key_Return);
	m_bkstep->setAutoRepeat(true);
	m_fwdstep->setAutoRepeat(true);
	m_bkstep->setAutoRepeatInterval(30);
	m_fwdstep->setAutoRepeatInterval(30);

	// Qt's auto repeat for button shortcuts is too slow and has no setting (That I know of). Normal shortcuts will do.
	QShortcut* decframe = new QShortcut(Qt::Key_Comma, this), *incframe = new QShortcut(Qt::Key_Period, this);
	connect(decframe, &QShortcut::activated, [this] { ButtonEvent(m_bkstep); });
	connect(incframe, &QShortcut::activated, [this] { ButtonEvent(m_fwdstep); });

	connect(m_play, &QPushButton::pressed, [this] { ButtonEvent(m_play); });
	connect(m_bkstep, &QPushButton::pressed, [this] { ButtonEvent(m_bkstep); });
	connect(m_fwdstep, &QPushButton::pressed, [this] { ButtonEvent(m_fwdstep); });
	connect(m_fps, QOverload<int>::of(&QSpinBox::valueChanged), [this] { ChangeFramerate(); });
	connect(m_appleskin, &QPushButton::toggled, [this](bool checked) { COnionSkin::Enable(checked); });

	CProject::Listen([this](CProjectEvent* e) { ProjectEvent(e); });
}

void CTimeControl::ButtonEvent(QPushButton * Btn)
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	if (Btn == m_play)
			proj->IsPlaying() ? proj->Pause() : proj->Play();
	else if (Btn == m_bkstep || Btn == m_fwdstep)
	{
		int step = Btn == m_bkstep ? -1 : 1;
		size_t frame = proj->ActiveFrame() + step;
		if (frame >= 0 && frame <= proj->LastFrame()->Index())
			proj->SetActiveFrame(frame);
	}
}

void CTimeControl::ProjectEvent(CProjectEvent * Event)
{
	if (!Event->Project() || Event->Project() != CProject::ActiveProject())
		return;

	switch (Event->Action())
	{
	case CProjectEvent::Play:
	case CProjectEvent::Pause:
		m_play->setIcon(Event->Action() == CProjectEvent::Play ? m_svgpause : m_svgplay);
		break;
	case CProjectEvent::ActiveProject:
	case CProjectEvent::Framerate:
		m_fps->blockSignals(true);
		m_fps->setValue(Event->Project()->Framerate());
		m_fps->blockSignals(false);
	}
}

void CTimeControl::ChangeFramerate()
{
	if (CProject* proj = CProject::ActiveProject())
		if (proj->Framerate() != m_fps->value())
			proj->SetFramerate(m_fps->value());
}