#include <sdbus-c++/sdbus-c++.h>
#include "server-glue.hpp"

class Repository1 : public sdbus::AdaptorInterfaces<org::rpm::dnf::Repository1_adaptor /*, more adaptor classes if there are more interfaces*/>
{
public:
    Repository1(sdbus::IConnection& connection, std::string objectPath)
        : AdaptorInterfaces(connection, std::move(objectPath))
    {
        registerAdaptor();
    }

    ~Repository1()
    {
        unregisterAdaptor();
    }

protected:
    std::vector<std::string> List(const std::string& id) override;
};
