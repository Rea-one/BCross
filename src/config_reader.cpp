#include "config_reader.h"

JSONReader::JSONReader(const std::string &file_name)
{
    boost::property_tree::read_json(file_name, pt);
}

boost::property_tree::ptree JSONReader::get_ptree()
{
    return pt;
}