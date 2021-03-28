#include "propertytree.hpp"

namespace rtfw {
namespace detail {
namespace propertytree {

Node::Node(const Config& conf): NodeBase{Dict()} {
	as_dict()[std::string(conf.name())] = Dict();
	for(const auto& [key, value]: conf){
		as_dict()[std::string(conf.name())].as_dict()[key] = value;
	}
}

Node::Node(const boostpt::ptree& tree){
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
Node::Type Node::type() const {
	if(is_scalar()) return Type::Scalar;
	if(is_list()) return Type::List;
	if(is_dict()) return Type::Dict;
	throw std::runtime_error("Unknown state");
}


bool Node::is_scalar() const noexcept {
	return std::holds_alternative<Scalar>(*this);
}

bool Node::is_list() const noexcept {
	return std::holds_alternative<List>(*this);
}

bool Node::is_dict() const noexcept {
	return std::holds_alternative<Dict>(*this);
}

Scalar& Node::as_scalar() {
	return std::get<Scalar>(*this);
}

const Scalar& Node::as_scalar() const {
	return std::get<Scalar>(*this);
}

List& Node::as_list() {
	return std::get<List>(*this);
}

const List& Node::as_list() const {
	return std::get<List>(*this);
}

Dict& Node::as_dict() {
	return std::get<Dict>(*this);
}

const Dict& Node::as_dict() const {
	return std::get<Dict>(*this);
}


}
}
}
