#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <tuple>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include "rtfw/config.hpp"

namespace rtfw {
namespace detail {
namespace propertytree {

namespace boostpt = boost::property_tree;

using ScalarBase = std::variant<int, double, std::string>;

class Scalar: public ScalarBase {
public:
	using ScalarBase::ScalarBase;

	template<class T>
	T& as(){
		return std::get<T>(*this);
	}

	template<class T>
	const T& as() const {
		return std::get<T>(*this);
	}

	template<class T>
	T cast() const {
		auto visitor = [](const auto& item){
			return boost::lexical_cast<T>(item);
		};
		return std::visit(visitor, *static_cast<const ScalarBase*>(this));
	}

	template<class T>
	bool is_current_type() const {
		return std::holds_alternative<T>(*this);
	}
};

class Node;

using List = std::vector<Node>;
using Dict = std::map<Scalar, Node>;

using NodeBase = std::variant<Scalar, List, Dict>;

class Node: public NodeBase {
public:
	using NodeBase::NodeBase;

	enum class Type{
		Scalar,
		List,
		Dict,
	};

	Node(const Config& conf): NodeBase{Dict()} {
		as_dict()[std::string(conf.name())] = Dict();
		for(const auto& [key, value]: conf){
			as_dict()[std::string(conf.name())].as_dict()[key] = value;
		}
	}

	Node(const boostpt::ptree& tree){
		if(tree.empty()){
			// Leaf node
			*this = Scalar(tree.get_value<std::string>());

		} else if(tree.find("") != tree.not_found()){
			// List
			*this = List();
			for(const auto& [key, subtree]: tree){
				(void)key;
				as_list().emplace_back(subtree);
			}
		} else{
			// Dict
			*this = Dict();
			for(const auto& [key, subtree]: tree){
				as_dict()[key] = Node(subtree);
			}
		}
	}

	Type type() const {
		if(is_scalar()) return Type::Scalar;
		if(is_list()) return Type::List;
		if(is_dict()) return Type::Dict;
		throw std::runtime_error("Unknown state");
	}
	
	bool is_scalar() const noexcept {
		return std::holds_alternative<Scalar>(*this);
	}

	bool is_list() const noexcept {
		return std::holds_alternative<List>(*this);
	}

	bool is_dict() const noexcept {
		return std::holds_alternative<Dict>(*this);
	}
	
	Scalar& as_scalar() {
		return std::get<Scalar>(*this);
	}
	
	const Scalar& as_scalar() const {
		return std::get<Scalar>(*this);
	}

	List& as_list() {
		return std::get<List>(*this);
	}

	const List& as_list() const {
		return std::get<List>(*this);
	}

	Dict& as_dict() {
		return std::get<Dict>(*this);
	}

	const Dict& as_dict() const {
		return std::get<Dict>(*this);
	}
};

std::string indent_str(int indent){
	return std::string(indent*4, ' ');
}


void print(const Scalar& s, int indent){
	std::cerr << indent_str(indent) << s.cast<std::string>() << '\n';
}

void print(const Node& node, int indent=0){
	if(node.is_scalar()){
		print(node.as_scalar(), indent);
	}
	else if(node.is_list()){
		std::cerr << indent_str(indent) <<  "[\n";
		for(const auto& subnode: node.as_list()){
			print(subnode, indent+1);
		}
		std::cerr << indent_str(indent) << "]\n";
	}
	else if(node.is_dict()){
		std::cerr << indent_str(indent) << "{\n";
		for(const auto& [key, value] : node.as_dict()){
			std::cerr << indent_str(indent+1) << key.cast<std::string>() << ":\n";
			print(value, indent+2);
		}
		std::cerr << indent_str(indent) << "}\n";
	}
	else {
		throw std::runtime_error("oh no");
	}
}

}
}
}
