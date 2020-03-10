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

#ifndef DNFDAEMON_REPOSITORY_HPP
#define DNFDAEMON_REPOSITORY_HPP

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

#endif
