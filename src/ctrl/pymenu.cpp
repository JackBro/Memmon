#include "pymenu.h"

PYMenu::PYMenu(QWidget *parent) :
    QMenu(parent)
{
  fadeTimer = new QTimer(this);
  fadeTimer->setInterval(PYM_TIMER_INTERVAL);
  connect(fadeTimer,SIGNAL(timeout()),this,SLOT(DoFading()));

  m_opacity = 0.0;
  setWindowOpacity(m_opacity);
}

PYMenu::PYMenu(const QString &str, QWidget *parent):QMenu(str,parent)
{
  fadeTimer = new QTimer(this);
  fadeTimer->setInterval(PYM_TIMER_INTERVAL);
  connect(fadeTimer,SIGNAL(timeout()),this,SLOT(DoFading()));

  m_opacity = 0.0;

}

/*!
  \fn showEvent
*/
void PYMenu::showEvent(QShowEvent *)
{
  m_opacity = 0.0;
  setWindowOpacity(m_opacity);

  if(fadeTimer->isActive())
  {
    fadeTimer->stop();
  }
  fadeTimer->start();

  emit sig_aboutToShow();
}

void PYMenu::hideEvent(QHideEvent *)
{
  m_opacity = 0.0;
  setWindowOpacity(m_opacity);

  if(fadeTimer->isActive())
  {
    fadeTimer->stop();
  }
}

void PYMenu::closeEvent(QCloseEvent *)
{
  m_opacity = 0.0;
  setWindowOpacity(m_opacity);

  if(fadeTimer->isActive())
  {
    fadeTimer->stop();
  }
}


/*!
  private slot functions
*/
void PYMenu::DoFading()
{
  m_opacity += PYM_OPACITY_INCREMENT;

  if(m_opacity > 1.0)
  {
    m_opacity = 1.0;
    fadeTimer->stop();
  }
  setWindowOpacity(m_opacity);
}
