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

#ifndef DNFDAEMON_CONTEXT_HPP
#define DNFDAEMON_CONTEXT_HPP

#include "libdnf/conf/ConfigMain.hpp"
#include "libdnf/conf/ConfigParser.hpp"
#include "libdnf/conf/ConfigRepo.hpp"

#include <map>
#include <memory>
#include <vector>


class Context {
public:
    struct RepoInfo {
        std::string filePath;
        std::unique_ptr<libdnf::ConfigRepo> repoconfig;
    };
    libdnf::ConfigMain cfgMain;

    void read_configuration();
    const std::map<std::string, std::unique_ptr<RepoInfo>> &getRepos() {return repos;}
    RepoInfo* findRepo(const std::string &repoid);
    libdnf::ConfigParser* findParser(const std::string &filepath);

private:
    libdnf::ConfigParser cfgMainParser;
    // repoid: repoinfo
    std::map<std::string, std::unique_ptr<RepoInfo>> repos;
    // repo_config_file_path: parser
    std::map<std::string, std::unique_ptr<libdnf::ConfigParser>> configFiles;

    std::map<std::string, std::string> substitutions;
    static bool libRpmInitiated;

    void setSubstitutions();
    void readMainConfig();
    void readRepoConfigs();
};


#endif
