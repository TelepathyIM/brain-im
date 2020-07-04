#include "AccountsModel.hpp"

class DeclarativeAccountsModel : public AccountsModel
{
    Q_OBJECT
public:
    explicit DeclarativeAccountsModel(QObject *parent = nullptr);
};
