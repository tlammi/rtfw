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

	ASSERT_EQ(n3.as_dict()["Mod1"].as_dict()["foo"].as_scalar().cast<std::string>(), "bar");
	ASSERT_EQ(n3.as_dict()["Mod2"].as_dict()["bar"].as_scalar().cast<int>(), 1);
}

TEST(PropertyTree, Union){
	pt::Node n0{pt::Dict()};
	pt::Node n1{pt::Dict()};
	
	n0.as_dict()["Mod1"] = pt::Dict();
	n0.as_dict()["Mod1"].as_dict()["key0"] = 42;
	n0.as_dict()["Mod1"].as_dict()["key1"] = "baz";

	n1.as_dict()["Mod1"] = pt::Dict();
	n1.as_dict()["Mod1"].as_dict()["key1"] = 100;
	n1.as_dict()["Mod1"].as_dict()["key2"] = "asdf";
	
	pt::Dict joined;
	joined.left_union(n0.as_dict());
	joined.left_union(n1.as_dict());

	ASSERT_EQ(joined["Mod1"].as_dict()["key0"].as_scalar(), 42);
	ASSERT_EQ(joined["Mod1"].as_dict()["key1"].as_scalar(), "baz");
	ASSERT_EQ(joined["Mod1"].as_dict()["key2"].as_scalar(), "asdf");

	pt::Dict joined2;
	joined2.right_union(n1.as_dict());
	joined2.right_union(n0.as_dict());
	ASSERT_EQ(joined, joined2);

}

TEST(PropertyTree, Intersection){
	pt::Dict d0{};
	pt::Dict d1{};
	
	d0["Mod1"] = pt::Dict();
	d1["Mod1"] = pt::Dict();

	d0["Mod1"].as_dict()["key0"] = 10;
	d0["Mod1"].as_dict()["key1"] = "foo";

	d1["Mod1"].as_dict()["key1"] = 42;
	d1["Mod1"].as_dict()["key2"] = "bar";


	pt::Dict intersected0{d0};
	intersected0.left_intersect(d1);

	pt::Dict intersected1{d1};
	intersected1.right_intersect(d0);

	pt::print(intersected0);
	pt::print(intersected1);
	ASSERT_EQ(intersected0["Mod1"].as_dict()["key1"].as_scalar(), "foo");
	ASSERT_EQ(intersected1["Mod1"].as_dict()["key1"].as_scalar(), "foo");
	ASSERT_EQ(intersected0["Mod1"].as_dict().size(), 1);
	ASSERT_EQ(intersected1["Mod1"].as_dict().size(), 1);

}
