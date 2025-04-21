#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <vector>

class JSONReader
{
    boost::property_tree::ptree pt;

public:
    JSONReader(const std::string &file_name);
    const boost::property_tree::ptree get_ptree();
}