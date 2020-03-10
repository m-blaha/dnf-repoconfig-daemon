#include <sdbus-c++/sdbus-c++.h>
#include "server-glue.hpp"
#include "Repository.hpp"

std::vector<std::string> Repository1::List(const std::string& id)
{
    std::vector<std::string> out;
    out.push_back("ahoj");
    out.push_back("cau");
    return out;
}
