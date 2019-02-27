#ifndef BRAIN_IM_PEERS_MODEL_HPP
#define BRAIN_IM_PEERS_MODEL_HPP

#include "global.h"

#include <QObject>
#include <QAbstractListModel>

#include <QSet>

#include "Types.hpp"

namespace BrainIM {

class BRAIN_IM_EXPORT PeersModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PeersModel(QObject *parent = nullptr);

    Q_INVOKABLE virtual bool hasPeer(const BrainIM::Peer peer) const = 0;
    Q_INVOKABLE virtual QString getName(const BrainIM::Peer peer) const = 0;

signals:
    void nameChanged(const Peer peer);
};

} // BrainIM namespace

#endif // BRAIN_IM_PEERS_MODEL_HPP
