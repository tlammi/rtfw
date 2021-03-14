#pragma once

#include <utility>
#include <cstddef>

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
	List(){}


	void pop(ListHead& head) noexcept {
		head.free_();
	}

	void push_back(ListHead& head)  noexcept {
		head.free_();
		if(!first_){
			first_ = &head;
		} else {
			last_->next_ = &head;
			head.prev_ = last_;
		}
		last_ = &head;
	}

	void push_front(ListHead& head) noexcept {
		head.free_();
		if(!first_){
			last_ = &head;
		} else{
			first_->prev_ = &head;
			head.next_ = first_;
		}
		first_ = &head;
	}


	void insert_before(ListHead& before, ListHead& head) noexcept {
		head.free_();
		auto* first = before.prev_;
		auto* second = &head;
		auto* third = &before;
		if(!first){
			push_front(head);
		} else {
			insert_between(first, third, second);
		}
	}
	
	void insert_after(ListHead& after, ListHead& head) noexcept {
		head.free_();
		auto* first = &after;
		auto* second = &head;
		auto* third = after.next_;
		if(!third){
			push_back(head);
		} else {
			insert_between(first, third, second);
		}
	}

	ListHead* next(ListHead& head) noexcept {
		return head.next_;
	}

	ListHead* prev(ListHead& head) noexcept {
		return head.prev_;
	}

	ListHead* first() noexcept {
		return first_;
	}
	
	ListHead* last() noexcept {
		return last_;
	}

	size_t size() const noexcept{
		size_t count=0;
		auto* iter = first_;
		while(iter){
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

	ListHead* first_{nullptr};
	ListHead* last_{nullptr};
};

}
}
