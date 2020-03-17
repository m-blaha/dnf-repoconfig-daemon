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

#include "repoconf-server-glue.hpp"
#include "repository.hpp"
#include "../repository/context.hpp"

#include <algorithm>
#include <sdbus-c++/sdbus-c++.h>

std::map<std::string, sdbus::Variant> repo_to_dbus(const Context::RepoInfo &repo)
{
    std::map<std::string, sdbus::Variant> dict;
    dict.emplace(std::make_pair("id", repo.repoid));
    for (const auto &section: repo.parser->getData()) {
        if (section.first == repo.repoid) {
            for (const auto &line: section.second) {
                if (line.first[0] != '#') {
                    dict.emplace(std::make_pair(line.first, line.second));
                }
            }
        }
    }
    return dict;
}

std::vector<std::map<std::string, sdbus::Variant>> RepoConf::list(const std::vector<std::string>& ids)
{
    Context ctx;
    ctx.configure();

    bool empty_ids=ids.empty();
    std::vector<std::map<std::string, sdbus::Variant>> out;
    for (auto &repo: ctx.repos) {
        if (empty_ids || std::find(ids.begin(), ids.end(), repo->repoid) != ids.end()) {
            out.push_back(repo_to_dbus(*repo));
        }
    }
    return out;
}

std::map<std::string, sdbus::Variant> RepoConf::get(const std::string& id)
{
    auto lst=list(std::vector<std::string>{id});
    if (lst.empty()) {
        throw sdbus::Error("org.rpm.dnf.v1.rpm.RepoConf.Error", "Repository not found");
    } else if (lst.size() > 1) {
        throw sdbus::Error("org.rpm.dnf.v1.rpm.RepoConf.Error", "Multiple repositories found");
    } else {
        return lst[0];
    }
}

std::vector<std::string> enable_disable_repos(const std::vector<std::string> &ids, bool enable) {
    Context ctx;
    ctx.configure();

    std::vector<std::string> out;
    for (auto &repoinfo: ctx.repos) {
        if (std::find(ids.begin(), ids.end(), repoinfo->repoid) != ids.end()
            && repoinfo->repoconfig->enabled().getValue()!=enable) {
            repoinfo->parser->setValue(repoinfo->repoid, "enabled", enable ? "1" : "0");
            // try / catch and return proper dbus error
            repoinfo->parser->write(repoinfo->filePath, false);
            out.push_back(repoinfo->repoid);
        }
    }
    return out;
}

std::vector<std::string> RepoConf::enable(const std::vector<std::string>& ids)
{
    return enable_disable_repos(ids, true);
}

std::vector<std::string> RepoConf::disable(const std::vector<std::string>& ids)
{
    return enable_disable_repos(ids, false);
}
