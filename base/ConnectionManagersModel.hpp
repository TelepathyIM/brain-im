#pragma once

#include <QAbstractTableModel>

#include <TelepathyQt/Types>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/AccountManager>

class ConnectionManagersModel : public QAbstractTableModel
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
