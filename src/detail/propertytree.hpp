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

template<class, class, class=void>
struct eq_comparable: std::false_type{};

template<class T, class U>
struct eq_comparable<T, U, std::void_t<decltype(std::declval<T>() == std::declval<U>())>>: std::true_type{};

template<class T, class U>
constexpr bool eq_comparable_v = eq_comparable<T, U>::value;

template<class Variant, class T>
struct variant_eq_comparable: std::false_type{};

template<class T, class... Ts>
struct variant_eq_comparable<std::variant<Ts...>, T>{
	static constexpr bool value = std::disjunction_v<eq_comparable<Ts, T>...>;
};

using ScalarBase = std::variant<int, double, std::string>;


class Scalar: public ScalarBase {
public:
	using ScalarBase::ScalarBase;

	template<class T>
	bool operator==(T&& t) const noexcept {
		static_assert(variant_eq_comparable<std::variant<int, double, std::string>, T>::value,
				"No known comparison with any variant type");

		if constexpr (eq_comparable_v<int, T>){
			if(std::holds_alternative<int>(*this))
				return std::get<int>(*this) == t;
		}
		if constexpr (eq_comparable_v<double, T>){
			if(std::holds_alternative<double>(*this))
				return std::get<double>(*this) == t;
		}

		if constexpr (eq_comparable_v<std::string, T>){
			if(std::holds_alternative<std::string>(*this))
				return std::get<std::string>(*this) == t;
		}
		return false;
	}
	
	template<class T>
	bool operator!=(T&& t) const noexcept {
		return !((*this) == t);
	}

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

	Node(const Config& conf);

	Node(const boostpt::ptree& tree);

	Node(const Node&) = default;
	Node& operator=(const Node&) = default;

	Node(Node&&) = default;
	Node& operator=(Node&&) = default;

	Type type() const;
	
	bool is_scalar() const noexcept;

	bool is_list() const noexcept;

	bool is_dict() const noexcept;
	
	Scalar& as_scalar();
	
	const Scalar& as_scalar() const;

	List& as_list();

	const List& as_list() const;

	Dict& as_dict();

	const Dict& as_dict() const;
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
