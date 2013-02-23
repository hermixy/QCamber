#include "viewerwindow.h"
#include "ui_viewerwindow.h"

#include <QtGui>
#include <QDebug>

#include "context.h"

extern Context ctx;

ViewerWindow::ViewerWindow(QWidget *parent) :
  QMainWindow(parent), ui(new Ui::ViewerWindow)
{
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);

  loadColorConfig();

  m_cursorCoordLabel = new QLabel();
  m_featureDetailLabel = new QLabel();
  m_cursorCoordLabel->setAlignment(Qt::AlignRight);
  m_featureDetailLabel->setAlignment(Qt::AlignCenter);
  statusBar()->addPermanentWidget(m_featureDetailLabel);
  statusBar()->addPermanentWidget(m_cursorCoordLabel, 1);

  m_layout = new QVBoxLayout();
  ui->scrollWidget->setLayout(m_layout);

  connect(ui->viewWidget->scene(), SIGNAL(mouseMove(QPointF)), this,
      SLOT(updateCursorCoord(QPointF)));
  connect(ui->viewWidget->scene(), SIGNAL(featureSelected(Symbol*)), this,
      SLOT(updateFeatureDetail(Symbol*)));

  ui->viewWidget->setFocus(Qt::MouseFocusReason);
}

ViewerWindow::~ViewerWindow()
{
  delete ui;
}

void ViewerWindow::setStep(QString step)
{
  setWindowTitle(step);
  m_step = step;
}

void ViewerWindow::setLayers(const QStringList& layerNames)
{
  ui->viewWidget->clearScene();
  ui->viewWidget->loadProfile(m_step);
  ui->viewWidget->zoomToProfile();

  clearLayout(m_layout, true);
  QString pathTmpl = "steps/%1/layers/%2";

  for(int i = 0; i < layerNames.count(); ++i) {
    LayerSelector *l = new LayerSelector(layerNames[i], m_step, layerNames[i]);

    m_SelectorMap[layerNames[i]] = l;
    connect(l, SIGNAL(Clicked(LayerSelector*, bool)), this,
        SLOT(toggleShowLayer(LayerSelector*, bool)));
    m_layout->addWidget(l);
  }
}

void ViewerWindow::clearLayout(QLayout* layout, bool deleteWidgets)
{
  while (QLayoutItem* item = layout->takeAt(0))
  {
    if (deleteWidgets)
    {
      if (QWidget* widget = item->widget())
        delete widget;
    }
    else if (QLayout* childLayout = item->layout())
      clearLayout(childLayout, deleteWidgets);
    delete item;
  }
}

void ViewerWindow::showLayer(QString name)
{
  LayerSelector* selector = m_SelectorMap[name];
  selector->toggle();
}

void ViewerWindow::toggleShowLayer(LayerSelector* selector, bool selected)
{
  if (!selected) {
    if (!selector->item) {
      Layer* layer = new Layer(selector->step(), selector->layer());
      selector->item = layer;
      selector->item->setDoComposite(true);
    }
    selector->setColor(nextColor());
    ui->viewWidget->addItem(selector->item);
    m_actives.append(selector);
  } else {
    int index = m_colors.indexOf(selector->color());
    m_colorsMap[index] = false;
    ui->viewWidget->removeItem(selector->item);
    m_actives.removeOne(selector);
  }
}

QColor ViewerWindow::nextColor(void)
{
  for (int i = 0; i < m_colors.size(); ++i) {
    if (!m_colorsMap[i]) {
      m_colorsMap[i] = true;
      return m_colors[i];
    }
  }
  return Qt::red;
}

void ViewerWindow::loadColorConfig()
{
  ctx.bg_color = QColor(ctx.config->value("color/BG").toString());
  ui->viewWidget->setBackgroundColor(ctx.bg_color);

  QString config("color/C%1");
  m_colors.clear();

  for(int i = 1; i < N_COLOR + 1; i++) {
    m_colors << QColor(ctx.config->value(config.arg(i)).toString());
  }

  for (int i = 0; i < m_colors.size(); ++i) {
    m_colorsMap[i] = false;
  }

  for (int i = 0; i < m_actives.size(); ++i) {
    m_actives[i]->setColor(nextColor());
    m_actives[i]->item->update();
  }
}

void ViewerWindow::updateCursorCoord(QPointF pos)
{
  QString text;
  text.sprintf("(%f, %f)", pos.x(), pos.y());
  m_cursorCoordLabel->setText(text);
}

void ViewerWindow::updateFeatureDetail(Symbol* symbol)
{
  m_featureDetailLabel->setText(symbol->name());
}

void ViewerWindow::on_actionSetColor_triggered(void)
{
  ColorSettings dialog;
  dialog.exec();
  loadColorConfig();
}

void ViewerWindow::on_homeFunc_clicked(void)
{
  ui->viewWidget->zoomToProfile();
}

void ViewerWindow::on_areaZoomFunc_clicked(void)
{
  ui->viewWidget->setZoomMode(ODBPPGraphicsView::AreaZoom);
}

void ViewerWindow::on_mousePanFunc_clicked(void)
{
  ui->viewWidget->setZoomMode(ODBPPGraphicsView::MousePan);
}
