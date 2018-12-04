#ifndef BRAIN_IM_ACCOUNT_PARAMETER_MODEL
#define BRAIN_IM_ACCOUNT_PARAMETER_MODEL

#include "global.h"

#include <QAbstractTableModel>

#include <TelepathyQt/Types>
#include <TelepathyQt/PendingOperation>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ProtocolInfoList>
#include <TelepathyQt/ConnectionManager>
#include <TelepathyQt/Account>

class BRAIN_IM_EXPORT AccountParameterModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString manager READ manager NOTIFY managerChanged)
    Q_PROPERTY(QString protocol READ protocol NOTIFY protocolChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY displayNameChanged)
    Q_PROPERTY(bool creationMode READ creationMode NOTIFY creationModeChanged)
public:
    enum Role {
        Name,
        Value,
        Signature,
        Type,
        DefaultValue,
        Required,
        Secret,
        RequiredForRegistration,
        RolesCount,
        InvalidRole
    };

    explicit AccountParameterModel(QObject *parent = nullptr);

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QVariant getData(int index, Role role) const;
    Q_INVOKABLE bool setData(int index, const QVariant &value);
    Q_INVOKABLE bool setData(const QString &paramName, const QVariant &value);

    Q_INVOKABLE QVariantMap getVariantMap() const;

    QString manager() const { return m_manager; }
    QString protocol() const { return m_protocol; }
    QString displayName() const { return m_displayName; }
    bool creationMode() const { return m_creationMode; }

signals:
    void managerChanged(const QString &manager);
    void protocolChanged(const QString &protocol);
    void displayNameChanged(const QString &displayName);
    void creationModeChanged(bool creationMode);

public slots:
    void newAccount(const QString &manager, const QString &protocol);
    void setAccount(const QString &uniqueIdentifier);
    bool submit() override;

protected slots:
    void onReady(Tp::PendingOperation *operation);

protected:
    void setManager(const QString &manager);
    void setProtocol(const QString &protocol);
    void setProtocolParams(const Tp::ProtocolParameterList &params);
    void setDisplayName(const QString &displayName);
    void setCreationMode(bool creationMode);
    static Role getRealRole(const QModelIndex index, int role);
    void reinit();

    QString m_protocol;
    QString m_displayName;
    Tp::ProtocolParameterList m_protocolParams;
    QString m_manager;
    Tp::ConnectionManagerPtr m_managerObject;
    Tp::AccountPtr m_account;
    QVariantMap m_values;
    bool m_resetInProgress;
    bool m_creationMode;

};

#endif // BRAIN_IM_ACCOUNT_PARAMETER_MODEL
