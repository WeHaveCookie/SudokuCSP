#include "Link.h"

Link::Link(State* tail, State* head)
{
	this->m_tail = tail;
	this->m_head = head;
}

size_t Link::hash(const Link& link)
{
	int hash = 0;
	hash += (*link.m_head).getRow();
	hash *= 10;
	hash += (*link.m_head).getColumn();
	hash *= 10;
	hash += (*link.m_tail).getRow();
	hash *= 10;
	hash += (*link.m_tail).getColumn();
	return hash;
}