#include "esu_registration_viewer.h"
#include "esu_registration_viewer_p.h"


ESURegistrationViewer::ESURegistrationViewer(QWidget *parent):
    QDeclarativeView(parent)
  , d(new ESURegistrationViewerPrivate(this))
{
    setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setRenderHint(QPainter::TextAntialiasing);
    setRenderHint(QPainter::SmoothPixmapTransform);
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags( windowFlags() | Qt::FramelessWindowHint );
    setStyleSheet(" background-color: rgba(0, 0, 0, 0); ");

    d->init();

    QUrl source("qrc:/mods/registration/ui/qml/UIRegistration.qml");
    setSource(source);
    //close();
}


ESURegistrationViewer::~ESURegistrationViewer()
{
    delete d;
}

