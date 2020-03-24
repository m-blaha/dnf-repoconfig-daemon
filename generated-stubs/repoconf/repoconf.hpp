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

#include "repoconf-server-glue.hpp"

#include <sdbus-c++/sdbus-c++.h>

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
