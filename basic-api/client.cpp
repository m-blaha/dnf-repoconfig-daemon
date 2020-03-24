#include <sdbus-c++/sdbus-c++.h>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[])
{
    auto connection = sdbus::createSystemBusConnection();
    std::cout << "Client name: " << connection->getUniqueName() << std::endl;
    const char* destinationName = "org.rpm.dnf.v1.rpm.RepoConf";
    const char* objectPath = "/org/rpm/dnf/v1/rpm/RepoConf";
    auto repoconf_proxy = sdbus::createProxy(*connection, destinationName, objectPath);
    const char* interfaceName = "org.rpm.dnf.v1.rpm.RepoConf";
    repoconf_proxy->finishRegistration();

    // Invoke test on given interface of the object
    {
        auto method = repoconf_proxy->createMethodCall(interfaceName, "test");
        auto reply = repoconf_proxy->callMethod(method);
        std::string result;
        reply >> result;
        std::cout << "Result: " << result << std::endl;
    }

    return 0;
}
