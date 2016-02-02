#ifndef STAFFUIAPPLICATION_H
#define STAFFUIAPPLICATION_H

#include <QtCore>
#include <QDeclarativeContext>


class StaffUIModel;
class StaffStructureUIModel;
class StaffUIPathModel;

class StaffUIApplicationPrivate;

class StaffUIApplication : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool checkMode READ checkMode WRITE setCheckMode NOTIFY checkModeChanged)
    Q_PROPERTY(bool staffMode READ staffMode WRITE setStaffMode NOTIFY staffModeChanged)
    Q_PROPERTY(int viewModeUI READ viewModeUI WRITE setViewModeUI NOTIFY viewModeUIChanged)
    Q_PROPERTY(int userAction READ userAction WRITE setUserAction NOTIFY userActionChanged)
    Q_PROPERTY(QString formHeader READ formHeader WRITE setFormHeader NOTIFY formHeaderChanged)

public:
    explicit StaffUIApplication(QObject *parent = 0);
    ~StaffUIApplication();

    static void declareQML();

    void initData();
    void declareQmlModels(QDeclarativeContext* context);

public:
    // [ PROPERTIES ]:
    bool checkMode() const;
    void setCheckMode(bool enable = true);

    bool staffMode() const;
    void setStaffMode(bool enabled = true);

    int viewModeUI() const;
    void setViewModeUI(int mode);

    int userAction() const;
    void setUserAction(int userAction);

    void setStaffAddress(const QString& address);

    QString formHeader() const;
    void setFormHeader(const QString& header);

    // [ UI METHODS ]:
    Q_INVOKABLE void closeUI();
    Q_INVOKABLE void createStaff(int index);

    // [ MODELS PROPERTIES ]:

    StaffUIModel &staffModel() const;
    StaffStructureUIModel &structureModel() const;

    StaffUIPathModel &staffPathModel() const;
    StaffUIPathModel &structurePathModel() const;


Q_SIGNALS:
    void checkModeChanged();
    void staffModeChanged();
    void viewModeUIChanged();
    void userActionChanged();
    void formHeaderChanged();

    void close();
    void staffCreated(QString address, QString name);

private:
    StaffUIApplicationPrivate* d;
};

#endif // STAFFUIAPPLICATION_H
