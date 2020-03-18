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


std::vector<std::map<std::string, sdbus::Variant>> RepoConf::list(const std::vector<std::string>& ids)
{
    Context ctx;
    ctx.configure();
    bool empty_ids=ids.empty();
    std::vector<std::map<std::string, sdbus::Variant>> out;
    for (auto &repo: ctx.repos) {
        if (empty_ids || std::find(ids.begin(), ids.end(), repo.first) != ids.end()) {
            auto parser = ctx.configFiles.find(repo.second->filePath)->second.get();
            std::map<std::string, sdbus::Variant> dbus_repo;
            dbus_repo.emplace(std::make_pair("repoid", repo.first));
            for (const auto &section: parser->getData()) {
                if (section.first == repo.first) {
                    for (const auto &line: section.second) {
                        if (line.first[0] != '#') {
                            dbus_repo.emplace(std::make_pair(line.first, line.second));
                        }
                    }
                }
            }
            out.push_back(dbus_repo);
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
    std::vector<std::string> changed_config_files;

    for (auto &repoid: ids) {
        auto &repoinfo=ctx.repos[repoid];
        if (repoinfo->repoconfig->enabled().getValue() != enable) {
            ctx.configFiles[repoinfo->filePath]->setValue(repoid, "enabled", enable ? "1" : "0");
            changed_config_files.push_back(repoinfo->filePath);
            out.push_back(repoid);
        }
    }
    for (auto &config_file: changed_config_files) {
        // try / catch and return proper dbus error
        ctx.configFiles[config_file]->write(config_file, false);
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
