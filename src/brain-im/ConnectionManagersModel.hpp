#ifndef BRAIN_IM_CONNECTION_MANAGERS_MODEL
#define BRAIN_IM_CONNECTION_MANAGERS_MODEL

#include "global.h"

#include <QAbstractTableModel>

#include <TelepathyQt/Types>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/AccountManager>

class BRAIN_IM_EXPORT ConnectionManagersModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList managers READ managers NOTIFY managersChanged)
public:
    enum Role {
        Name,
        RolesCount,
        InvalidRole
    };

    explicit ConnectionManagersModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList managers() const { return m_managers; }

signals:
    void managersChanged(const QStringList &managers);

protected:
    QVariant getData(int index, Role role) const;

protected slots:
    void onReady();

protected:
    QStringList m_managers;

};

#endif // BRAIN_IM_CONNECTION_MANAGERS_MODEL
