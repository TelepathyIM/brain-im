#include <QtQml/qqmlextensionplugin.h>
#include <QtQml/qqml.h>

class BrainControlsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    BrainControlsPlugin(QObject *parent = nullptr) :
        QQmlExtensionPlugin(parent)
    {
    }

    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QByteArray(uri) == QByteArray("BrainControls"));
    }
};

#include "plugin.moc"
