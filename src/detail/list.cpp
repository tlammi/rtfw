#include "rtfw/detail/list.hpp"

#include <utility>
#include <cstddef>
#include <stdexcept>

namespace rtfw{
namespace detail{


ListHead::ListHead(){}
ListHead::~ListHead(){
	free_();
}

void ListHead::free_() noexcept {
	if(next_) next_->prev_ = prev_;
	if(prev_) prev_->next_ = next_;
	next_ = nullptr;
	prev_ = nullptr;
}

List::List(){
		before_.next_ = &after_;
		after_.prev_ = &before_;
	}


List::~List(){
	auto* iter = before_.next_;
	while(iter != &after_){
		auto* deletee = iter;
		iter = iter->next_;
		pop(*deletee);
	}
}


void List::pop(ListHead& head) noexcept {
	head.free_();
}

void List::push_back(ListHead& head)  noexcept {
	head.free_();
	insert_before(after_, head);
}

void List::push_front(ListHead& head) noexcept {
	head.free_();
	insert_after(before_, head);
}

void List::insert_before(ListHead& before, ListHead& head) noexcept {
	head.free_();
	auto* first = before.prev_;
	auto* second = &head;
	auto* third = &before;
	insert_between(first, third, second);
}

void List::insert_after(ListHead& after, ListHead& head) noexcept {
	head.free_();
	auto* first = &after;
	auto* second = &head;
	auto* third = after.next_;
	insert_between(first, third, second);
}

bool List::find(ListHead& head) noexcept {
	auto* iter = before_.next_;
	while(iter != &after_ && iter != &head) iter = iter->next_;
	return iter != &after_;
}

ListHead* List::next(ListHead& head) {
	if(!head.prev_ || !head.next_) throw std::runtime_error("Head is not a member of a list");
	auto* next = head.next_;
	// after_ is opaque to the user
	if(next->next_) return next;
	return nullptr;
}

ListHead* List::prev(ListHead& head) {
	if(!head.prev_ || !head.next_) throw std::runtime_error("Head is not a member of a list");
	auto* prev = head.prev_;
	// before_ is opaque to the user
	if(prev->prev_) return prev;
	return nullptr;
}

ListHead* List::first() noexcept {
	if(before_.next_ != &after_)
		return before_.next_;
	return nullptr;
}

ListHead* List::last() noexcept {
	if(after_.prev_ != &before_)
		return after_.prev_;
	return nullptr;
}

size_t List::size() const noexcept{
	size_t count=0;
	auto* iter = before_.next_;
	while(iter != &after_){
		++count;
		iter = iter->next_;
	}
	return count;
}

void List::insert_between(ListHead* first, ListHead* last, ListHead* head) noexcept {
	first->next_ = head;
	head->prev_ = first;
	last->prev_ = head;
	head->next_ = last;
}

}
}
