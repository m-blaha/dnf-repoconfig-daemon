
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__server_glue_h__adaptor__H__
#define __sdbuscpp__server_glue_h__adaptor__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace rpm {
namespace dnf {

class Repository1_adaptor
{
public:
    static constexpr const char* INTERFACE_NAME = "org.rpm.dnf.Repository1";

protected:
    Repository1_adaptor(sdbus::IObject& object)
        : object_(object)
    {
        object_.registerMethod("List").onInterface(INTERFACE_NAME).withInputParamNames("id").withOutputParamNames("repos").implementedAs([this](const std::string& id){ return this->List(id); });
    }

    ~Repository1_adaptor() = default;

private:
    virtual std::vector<std::string> List(const std::string& id) = 0;

private:
    sdbus::IObject& object_;
};

}}} // namespaces

#endif
