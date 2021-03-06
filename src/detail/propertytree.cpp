#include "propertytree.hpp"

namespace rtfw {
namespace detail {
namespace propertytree {

namespace {

Node* try_at(Dict& d, const Scalar& k){
	if(d.count(k))
		return &d.at(k);
	return nullptr;
}


template<class Iter>
std::string join(Iter iter, Iter end, char sep){
	std::string str;
	while(iter != end){
		str += *iter;
		++iter;
		if(iter != end)
			str.push_back(sep);
	}
	return str;
}

void to_config(const Node& n, Config& c, std::vector<std::string>& stack, char separator='.'){
	switch(n.type()){
	case Node::Type::Dict:
		for(const auto& [key, node] : n.as_dict()){
			stack.push_back(key.as<std::string>());
			to_config(node, c, stack, separator);
			stack.pop_back();
		}
		break;
	case Node::Type::List:
		throw std::runtime_error("list support is not implemented yet");
		break;
	case Node::Type::Scalar:
		c[join(stack.begin(), stack.end(), separator)] = n.as_scalar().as<std::string>();
		break;
	}
}

}


bool operator<(std::string_view sv, const Scalar& s){
	return s > sv;
}



Node& Dict::operator[](std::string_view str){
	auto iter = find(str);
	if(iter == end()){
		auto [new_iter, success] = insert(Dict::value_type(std::string(str), Node{}));
		if(!success) throw std::runtime_error("Could not insert into the map");
		iter = new_iter;
	}
	return iter->second;
}


Dict& Dict::operator-=(const Dict& other){
	for(const auto& [key, onode]: other){
		if(auto* tnode = try_at(*this, key)){
			if(onode.type() == Node::Type::Dict && tnode->type() == Node::Type::Dict){
				tnode->as_dict() -= onode.as_dict();
			} else{
				erase(key);
			}
		}
	}
	return *this;
}

Dict& Dict::left_union(const Dict& other){
	for(const auto& [key, onode] : other){
		if(auto tnode = try_at(*this, key)){
			if(tnode->type() == Node::Type::Dict && onode.type() == Node::Type::Dict)
				tnode->as_dict().left_union(onode.as_dict());
		} else {
			(*this)[key] = onode;
		}
	}
	return *this;
}

Dict& Dict::right_union(const Dict& other){
	for(const auto& [key, onode] : other){
		if(auto tnode = try_at(*this, key)){
			if(tnode->type() == Node::Type::Dict && onode.type() == Node::Type::Dict)
				tnode->as_dict().right_union(onode.as_dict());
			else
				(*this)[key] = onode;
		} else {
			(*this)[key] = onode;
		}
	}
	return *this;
}

Dict& Dict::left_intersect(const Dict& other){
	Dict tmp;
	for(const auto& [key, onode]: other){
		(void)onode;
		if(this->count(key)){
			if(this->at(key).type() == Node::Type::Dict && onode.type() == Node::Type::Dict){
				tmp.insert(std::move(this->extract(key)));
				tmp.at(key).as_dict().left_intersect(onode.as_dict());
			}
			else{
				tmp.insert(std::move(this->extract(key)));
			}
		}
	}
	*this = std::move(tmp);
	return *this;
}

Dict& Dict::right_intersect(const Dict& other){
	Dict tmp;
	for(const auto& [key, onode]: other){
		if(this->count(key)){
			if(this->at(key).type() == Node::Type::Dict && onode.type() == Node::Type::Dict){
				tmp.insert(std::move(this->extract(key)));
				tmp.at(key).as_dict().right_intersect(onode.as_dict());
			}
			else{
				tmp[key] = onode;
			}
		}
	}
	*this = tmp;
	return *this;
}


std::vector<Config> Dict::to_configs() const {
	std::vector<Config> confs;
	for(const auto& [key, node]: *this){
		confs.emplace_back(key.as<std::string>());
		std::vector<std::string> key_stack;
		to_config(node, confs.back(), key_stack);
	}
	return confs;
}

Node::Node(const Config& conf): NodeBase{Dict()} {
	as_dict()[conf.name()] = Dict();
	for(const auto& [key, value]: conf){
		as_dict()[conf.name()].as_dict()[key] = value;
	}
}


Node::Node(Config&& conf): NodeBase{Dict()} {
	as_dict()[conf.name()] = Dict();
	for(auto&& [key, value] : conf){
		as_dict()[conf.name()].as_dict()[key] = std::move(value);
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
