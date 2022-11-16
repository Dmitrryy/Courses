#include "utils.h"

#include <algorithm>
#include <numeric>

namespace ezg 
{

const std::string class_prefix = "s.";

std::string mangl_class_name(const std::string &class_name) {
    return class_prefix + class_name;
}

std::string demangl_class_name(const std::string &class_name) {
    std::string res = class_name;
    if(res.starts_with(class_prefix)) {
        res = res.substr(class_prefix.size());
    }
    return res;
}

std::string mangl_method_name(const std::string &class_name, const std::string &method_name, const std::vector< std::string > &args_types) {
    std::string res = demangl_class_name(class_name) + "." + method_name + "_";
    for(auto&& arg_ty : args_types) {
        res += demangl_class_name(arg_ty);
    }
    return res;
}

}