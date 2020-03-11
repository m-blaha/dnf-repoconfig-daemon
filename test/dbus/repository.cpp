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

#include "server-glue.hpp"
#include "repository.hpp"
#include "../repository/context.hpp"

#include <sdbus-c++/sdbus-c++.h>

std::vector<std::map<std::string, sdbus::Variant>> Repository1::list()
{
    Context ctx;
    ctx.configure();
    std::vector<std::map<std::string, sdbus::Variant>> out;
    for (auto &repo: ctx.repos) {
        std::map<std::string, sdbus::Variant> repoitem;
        repoitem.emplace(std::make_pair(std::string("id"), repo->getId()));
        out.push_back(repoitem);
    }
    return out;
}

std::map<std::string, sdbus::Variant> Repository1::info(const std::string& id)
{
    std::map<std::string, sdbus::Variant> out;
    return out;
}

std::vector<std::string> Repository1::enable(const std::vector<std::string>& ids)
{
    std::vector<std::string> out;
    return out;
}

std::vector<std::string> Repository1::disable(const std::vector<std::string>& ids)
{
    std::vector<std::string> out;
    return out;
}
