#include <gtest/gtest.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "propertytree.hpp"
#include "rtfw/config.hpp"


namespace pt = rtfw::detail::propertytree;
namespace bpt = boost::property_tree;

const std::string INI_DATA = R"(
[Mod1]
foo=bar
[Mod2]
foo=baz
bar=1
)";

TEST(PropertyTree, Ctor){
	pt::Node n{};

	rtfw::Config conf{"Mod", {"key0", "key1"}};
	pt::Node n2{conf};

	ASSERT_TRUE(n2.as_dict()["Mod"].as_dict().count("key0"));
	ASSERT_TRUE(n2.as_dict()["Mod"].as_dict().count("key1"));
	ASSERT_FALSE(n2.as_dict()["Mod"].as_dict().count("key2"));

	std::stringstream ss{INI_DATA};
	bpt::ptree ini_tree;
	bpt::read_ini(ss, ini_tree);
	pt::Node n3{ini_tree};

	pt::print(n3);
	ASSERT_EQ(n3.as_dict()["Mod1"].as_dict()["foo"].as_scalar().cast<std::string>(), "bar");
	ASSERT_EQ(n3.as_dict()["Mod2"].as_dict()["bar"].as_scalar().cast<int>(), 1);
}


