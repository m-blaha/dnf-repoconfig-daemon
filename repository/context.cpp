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

#include "context.hpp"

#include "libdnf/conf/ConfigMain.hpp"
#include "libdnf/conf/ConfigParser.hpp"
#include "libdnf/conf/ConfigRepo.hpp"
#include "libdnf/dnf-context.h" // for find_base_arch

#include <glob.h>
#include <iostream>
#include <rpm/rpmlib.h>

bool Context::libRpmInitiated = false;

void Context::configure()
{
    setSubstitutions();
    readMainConfig();
    readRepoConfigs();
}

void Context::setSubstitutions()
{
    if (!libRpmInitiated) {
        if (rpmReadConfigFiles(NULL, NULL) != 0) {
            throw std::runtime_error("Failed to read rpm config files\n");
        }
        libRpmInitiated = true;
    }
    const char *arch;
    rpmGetArchInfo(&arch, NULL);
    substitutions["arch"] = std::string(arch);
    const auto basearch = find_base_arch(arch);
    substitutions["basearch"] = std::string(basearch);
    //TODO code for detecting releasever
    substitutions["releasever"] = "31";
}

void Context::readMainConfig()
{
    cfgMainParser.setSubstitutions(substitutions);
    cfgMainParser.read(cfgMain.config_file_path().getValue());
    const auto & cfgParserData = cfgMainParser.getData();
    const auto cfgParserDataIter = cfgParserData.find("main");
    if (cfgParserDataIter != cfgParserData.end()) {
        auto optBinds = cfgMain.optBinds();
        for (const auto &opt : cfgParserDataIter->second) {
            const auto optBindsIter = optBinds.find(opt.first);
            if (optBindsIter != optBinds.end()) {
                optBindsIter->second.newString(
                    libdnf::Option::Priority::MAINCONFIG,
                    cfgMainParser.getSubstitutedValue("main", opt.first));
            }
        }
    }
}

void Context::readRepoConfigs()
{
    for (auto reposDir : cfgMain.reposdir().getValue()) {
        const std::string pattern = reposDir + "/*.repo";
        glob_t globResult;
        glob(pattern.c_str(), GLOB_MARK, NULL, &globResult);
        for (size_t i = 0; i < globResult.gl_pathc; ++i) {
            std::unique_ptr<libdnf::ConfigParser> repo_parser(new libdnf::ConfigParser);
            std::string filePath=globResult.gl_pathv[i];
            repo_parser->setSubstitutions(substitutions);
            repo_parser->read(filePath);
            const auto & cfgParserData = repo_parser->getData();
            for (const auto & cfgParserDataIter : cfgParserData) {
                if (cfgParserDataIter.first != "main") {
                    auto section = cfgParserDataIter.first;
                    std::unique_ptr<libdnf::ConfigRepo> cfgRepo(new libdnf::ConfigRepo(cfgMain));
                    auto optBinds = cfgRepo->optBinds();
                    for (const auto &opt : cfgParserDataIter.second) {
                        if (opt.first[0] == '#') {
                            continue;
                        }
                        const auto optBindsIter = optBinds.find(opt.first);
                        if (optBindsIter != optBinds.end()) {
                            optBindsIter->second.newString(
                                libdnf::Option::Priority::REPOCONFIG,
                                repo_parser->getSubstitutedValue(section, opt.first));
                        }
                    }
                    std::unique_ptr<RepoInfo> repoinfo(new RepoInfo());
                    repoinfo->repoid = std::move(section);
                    repoinfo->filePath = std::string(filePath);
                    repoinfo->parser = repo_parser.get();
                    repoinfo->repoconfig = std::move(cfgRepo);
                    repos.push_back(std::move(repoinfo));
                }
            }
            cfgRepoParsers.push_back(std::move(repo_parser));
        }
        globfree(&globResult);
    }
}
