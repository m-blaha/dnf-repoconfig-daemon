/*
 * Copyright (C) 2020 Red Hat, Inc.
 *
 * Licensed under the GNU Lesser General Public License Version 2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef DNFDAEMON_REPOCONF_HPP
#define DNFDAEMON_REPOCONF_HPP

#include <sdbus-c++/sdbus-c++.h>

#include <string>
#include <tuple>

namespace org {
namespace rpm {
namespace dnf {
namespace v1 {
namespace rpm {

class RepoConf_adaptor
{
public:
    static constexpr const char* INTERFACE_NAME = "org.rpm.dnf.v1.rpm.RepoConf";

protected:
    RepoConf_adaptor(sdbus::IObject& object)
        : object_(object)
    {
        object_.registerMethod("list").onInterface(INTERFACE_NAME).withInputParamNames("ids").withOutputParamNames("repos").implementedAs([this](const std::vector<std::string>& ids){ return this->list(ids); });
        object_.registerMethod("get").onInterface(INTERFACE_NAME).withInputParamNames("id").withOutputParamNames("repos").implementedAs([this](const std::string& id){ return this->get(id); });
        object_.registerMethod("enable").onInterface(INTERFACE_NAME).withInputParamNames("ids").withOutputParamNames("changed_ids").implementedAs([this](const std::vector<std::string>& ids){ return this->enable(ids); });
        object_.registerMethod("disable").onInterface(INTERFACE_NAME).withInputParamNames("ids").withOutputParamNames("changed_ids").implementedAs([this](const std::vector<std::string>& ids){ return this->disable(ids); });
    }

    ~RepoConf_adaptor() = default;

private:
    virtual std::vector<std::map<std::string, sdbus::Variant>> list(const std::vector<std::string>& ids) = 0;
    virtual std::map<std::string, sdbus::Variant> get(const std::string& id) = 0;
    virtual std::vector<std::string> enable(const std::vector<std::string>& ids) = 0;
    virtual std::vector<std::string> disable(const std::vector<std::string>& ids) = 0;

private:
    sdbus::IObject& object_;
};

}}}}} // namespaces


class RepoConf : public sdbus::AdaptorInterfaces<org::rpm::dnf::v1::rpm::RepoConf_adaptor /*, more adaptor classes if there are more interfaces*/>
{
public:
    RepoConf(sdbus::IConnection& connection, std::string objectPath)
        : AdaptorInterfaces(connection, std::move(objectPath))
    {
        registerAdaptor();
    }

    ~RepoConf()
    {
        unregisterAdaptor();
    }

private:
    std::vector<std::map<std::string, sdbus::Variant>> list(const std::vector<std::string>& ids) override;
    std::map<std::string, sdbus::Variant> get(const std::string& id) override;
    std::vector<std::string> enable(const std::vector<std::string>& ids) override;
    std::vector<std::string> disable(const std::vector<std::string>& ids) override;
};

#endif
