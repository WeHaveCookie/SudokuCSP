#pragma once
#include "State.h"

class Link
{
public:
	Link(State* tail, State* head);
	State* getTail() const { return m_tail; }
	State* getHead() const { return m_head; }
	static size_t hash(const Link& link);
	static const bool equalTo(const Link& linkA, const Link& linkB) { return linkA.m_head == linkB.m_head && linkA.m_tail == linkB.m_tail; }

private:
	State * m_tail;
	State * m_head;
};