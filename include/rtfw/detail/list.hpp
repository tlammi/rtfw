#pragma once

#include <utility>
#include <cstddef>
#include <stdexcept>

namespace rtfw{
namespace detail{

class List;

class ListHead{
	friend class List;
public:
	ListHead(){}
	~ListHead(){
		free_();
	}

	ListHead(const ListHead&) = delete;
	ListHead(ListHead&&) = delete;

	ListHead& operator=(const ListHead&) = delete;
	ListHead& operator=(ListHead&&) = delete;

private:
	void free_() noexcept {
		if(next_) next_->prev_ = prev_;
		if(prev_) prev_->next_ = next_;
		next_ = nullptr;
		prev_ = nullptr;
	}

	ListHead* next_{nullptr};
	ListHead* prev_{nullptr};
};

class List{
public:
	List(){
		before_.next_ = &after_;
		after_.prev_ = &before_;
	}


	~List(){
		auto* iter = before_.next_;
		while(iter != &after_){
			auto* deletee = iter;
			iter = iter->next_;
			pop(*deletee);
		}
	}


	void pop(ListHead& head) noexcept {
		head.free_();
	}

	void push_back(ListHead& head)  noexcept {
		head.free_();
		insert_before(after_, head);
	}

	void push_front(ListHead& head) noexcept {
		head.free_();
		insert_after(before_, head);
	}

	void insert_before(ListHead& before, ListHead& head) noexcept {
		head.free_();
		auto* first = before.prev_;
		auto* second = &head;
		auto* third = &before;
		insert_between(first, third, second);
	}
	
	void insert_after(ListHead& after, ListHead& head) noexcept {
		head.free_();
		auto* first = &after;
		auto* second = &head;
		auto* third = after.next_;
		insert_between(first, third, second);
	}

	bool find(ListHead& head) noexcept {
		auto* iter = before_.next_;
		while(iter != &after_ && iter != &head) iter = iter->next_;
		return iter != &after_;
	}

	static ListHead* next(ListHead& head) {
		if(!head.prev_ || !head.next_) throw std::runtime_error("Head is not a member of a list");
		auto* next = head.next_;
		// after_ is opaque to the user
		if(next->next_) return next;
		return nullptr;
	}

	static ListHead* prev(ListHead& head) {
		if(!head.prev_ || !head.next_) throw std::runtime_error("Head is not a member of a list");
		auto* prev = head.prev_;
		// before_ is opaque to the user
		if(prev->prev_) return prev;
		return nullptr;
	}

	ListHead* first() noexcept {
		if(before_.next_ != &after_)
			return before_.next_;
		return nullptr;
	}
	
	ListHead* last() noexcept {
		if(after_.prev_ != &before_)
			return after_.prev_;
		return nullptr;
	}

	size_t size() const noexcept{
		size_t count=0;
		auto* iter = before_.next_;
		while(iter != &after_){
			++count;
			iter = iter->next_;
		}
		return count;
	}
private:

	static void insert_between(ListHead* first, ListHead* last, ListHead* head) noexcept {
		first->next_ = head;
		head->prev_ = first;
		last->prev_ = head;
		head->next_ = last;
	}

	ListHead before_{};
	ListHead after_{};
};

}
}
