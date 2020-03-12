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

#include "repository/context.hpp"
#include "dbus/repository.hpp"

#include <iostream>
#include <sdbus-c++/sdbus-c++.h>


int xmain(int argc, char **argv)
{
    Context ctx;

    ctx.configure();

    std::cout << "XXX installonly_limit: " << ctx.cfgMain.installonly_limit().getValue() << std::endl;
    for (auto &r: ctx.repos) {
        std::cout << "XXX repo id: " << r->getId() << std::endl;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Create D-Bus connection to the system bus and requests name on it.
    const char* serviceName = "org.rpm.dnf.v1.conf.Repos";
    auto connection = sdbus::createSystemBusConnection(serviceName);

    // Create D-Bus object.
    const char* objectPath = "/org/rpm/dnf/v1/conf/Repos";
    Repos repos(*connection, objectPath);

    // Run the loop on the connection.
    connection->enterEventLoop();
    return 0;
}

