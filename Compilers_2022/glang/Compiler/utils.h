#pragma once

#include <string>
#include <vector>
namespace ezg
{

std::string mangl_class_name(const std::string &class_name);
std::string demangl_class_name(const std::string &class_name);
std::string mangl_method_name(const std::string &class_name, const std::string &method_name, const std::vector< std::string > &args_types);

}