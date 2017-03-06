#pragma once
#include "Link.h"
#include <assert.h>
#include "QueueSet.h"

QueueSet::QueueSet()
{
	m_size = 0;
}

void QueueSet::push(Link element)
{
	if (!contains(element))
	{
		m_set.insert(element);
		m_queue.push(element);
		m_size++;
	}

	assert(m_set.size() == m_queue.size());
	assert(m_set.size() == m_size);
}

Link QueueSet::pop()
{
	Link tmp =  m_queue.front();
	m_queue.pop();
	m_set.erase(tmp);
	m_size--;
	assert(m_set.size() == m_queue.size());
	assert(m_set.size() == m_size);
	return tmp;
}