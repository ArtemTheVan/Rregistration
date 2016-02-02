#ifndef ESUREGISTRATIONVIEWER_H
#define ESUREGISTRATIONVIEWER_H

#include <QDeclarativeView>


class ESURegistrationViewerPrivate;

class ESURegistrationViewer : public QDeclarativeView
{
    Q_OBJECT

public:
    explicit ESURegistrationViewer(QWidget *parent = 0);
    ~ESURegistrationViewer();

protected:
    ESURegistrationViewerPrivate* d;

private:
    Q_DECLARE_PRIVATE_D(ESURegistrationViewer::d, ESURegistrationViewer)
    Q_DISABLE_COPY(ESURegistrationViewer)
};

#endif // ESUREGISTRATIONVIEWER_H
