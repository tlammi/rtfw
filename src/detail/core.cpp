#include "rtfw/detail/core.hpp"
#include "rtfw/detail/util.hpp"
#include "propertytree.hpp"

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>



namespace rtfw{
namespace detail{
using namespace std::literals::chrono_literals;

namespace pt = propertytree;
namespace bpt = boost::property_tree;


namespace {

template<class Iter>
std::string join(Iter iter, Iter end){
	std::string out{};
	while(iter != end){
		out += *iter;
		++iter;
		if(iter != end)
			out.push_back('.');
		else
			break;
	}
	return out;
}

void print_keys(const pt::Dict& d, std::vector<std::string>& stack){
	for(const auto& [key, node] : d){
		stack.push_back(key.cast<std::string>());
		if(node.type() == pt::Node::Type::Dict){
			print_keys(node.as_dict(), stack);
		} else{
			std::cerr << join(stack.begin(), stack.end()) << '\n';
		}
		stack.pop_back();
	}
}

void validate_config_files(const pt::Dict& mod_confs, const pt::Dict& file_confs){
	auto tmp = file_confs;
	tmp -= mod_confs;
	if(!tmp.empty()){
		std::cerr << "\nWARNING: unsupported configurations in config files:\n";
		std::vector<std::string> stack;
		print_keys(tmp, stack);
		std::cerr << '\n';
	}
}


template<class Iter>
pt::Node read_ini_file(Iter iter, Iter end){
	pt::Node result{pt::Dict()};
	while(iter != end){
		bpt::ptree tree;
		bpt::read_ini(*iter, tree);
		result.as_dict().right_union(pt::Node{tree}.as_dict());
		++iter;
	}
	return result;
}

pt::Node populate_configs(pt::Node&& confs){
	std::vector ini_files{"demo.ini"};
	auto from_files = read_ini_file(ini_files.begin(), ini_files.end());
	validate_config_files(confs.as_dict(), from_files.as_dict());
	confs.as_dict().right_union(from_files.as_dict());
	return confs;
}

pt::Node invoke_configs(List& module_list){
	auto* iter = module_list.first();
	pt::Node dict{pt::Dict()};
	while(iter){
		auto* ptr = static_cast<ModuleHolder*>(iter);
		std::cerr << "configuring " << ptr->name() << '\n';
		auto conf = ptr->config();
		if(conf.name() == "")
			conf.set_name(ptr->name());

		if(dict.as_dict().count(conf.name()))
			throw std::runtime_error("Conflicting module names");

		pt::Node tmp{std::move(conf)};
		dict.as_dict().left_union(tmp.as_dict());
		iter = List::next(*iter);
	}
	return dict;
}

void init(List& init_stack, List& module_list){
	ListHead* iterator = module_list.first();
	while(iterator){
		auto* target = iterator;
		iterator = List::next(*iterator);
		init_stack.push_back(*target);
		std::cerr << "initializing " << static_cast<ModuleHolder*>(target)->name() << '\n';
		static_cast<ModuleHolder*>(target)->init();
		module_list.push_front(*target);
	}
}


void deinit(List& module_list){
	ListHead* iterator = module_list.first();
	while(iterator){
		auto* target = static_cast<ModuleHolder*>(iterator);
		std::cerr << "destroying " << target->name() << '\n';
		iterator = List::next(*iterator);
		target->clear();
	}

}

}

Core::Core(){}

Core::~Core(){}

void Core::run(){

	// Lambda is called in object destructor
	Defer defer{[&](){
		setup_ = false;
	}};

	setup_ = false;
	rtfw_thread_id_ = std::this_thread::get_id();
	auto configs = invoke_configs(ModuleHolder::instances);
	configs = populate_configs(std::move(configs));
	init(init_stack_, ModuleHolder::instances);
	setup_ = true;
	std::this_thread::sleep_for(1s);
	deinit(ModuleHolder::instances);
}

Core core{};
}
}

