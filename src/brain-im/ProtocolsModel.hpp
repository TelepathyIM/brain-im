#ifndef BRAIN_IM_PROTOCOLS_MODEL
#define BRAIN_IM_PROTOCOLS_MODEL

#include "global.h"

#include <QAbstractTableModel>

#include <TelepathyQt/Types>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ProtocolInfoList>
#include <TelepathyQt/ConnectionManager>

class BRAIN_IM_EXPORT ProtocolsModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QStringList managers READ managers WRITE setManagers NOTIFY managersChanged)
public:
    enum Role {
        Name,
        CmName,
        RolesCount,
        InvalidRole
    };

    explicit ProtocolsModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList managers() const { return m_managers; }
    void setManagers(const QStringList &managers);

signals:
    void managersChanged(const QStringList &managers);

protected slots:
    void onReady();

protected:
    QVariant getData(int index, Role role) const;

protected:
    QStringList m_managers;
    Tp::ProtocolInfoList m_protocols;
    QVector<Tp::ConnectionManagerPtr> m_managers2;

};

#endif // BRAIN_IM_PROTOCOLS_MODEL
