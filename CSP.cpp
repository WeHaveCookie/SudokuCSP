#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "State.h"
#include "Link.h"
#include <assert.h>
#include "QueueSet.h"
#include "Reader.h"
#include <cmath>

#define sudokuSize 9
#define squareNbr 3

int backtrack = 0;

typedef std::priority_queue<State, std::vector<State>, StateCompare> cspPriorityQueue;

void solveCSP(const int sudoku[sudokuSize][sudokuSize]);

void createLinks(State* states[sudokuSize][sudokuSize], QueueSet* queue, State* state)
{
	int row = state->getRow();
	int col = state->getColumn();
	for (int i = 0; i < sudokuSize; i++)
	{
		if (col != i)
		{
			auto tmp = Link(states[row][col], states[row][i]);
			queue->push(tmp);
		}
		if (row != i)
		{
			queue->push(Link(states[row][col], states[i][col]));
		}
	}

	int rowB, colB;
	rowB = (int)std::floor(row / squareNbr);
	colB = (int)std::floor(col / squareNbr);
	rowB *= squareNbr;
	colB *= squareNbr;

	for (int i = rowB; i < (rowB + squareNbr); i++)
	{
		for (int j = colB; j < (colB + squareNbr); j++)
		{
			if (i != row && j != col)
			{
				queue->push(Link(states[row][col], states[i][j]));
			}
		}
	}

}

void initializeStates(State* states[sudokuSize][sudokuSize], const int sudoku[sudokuSize][sudokuSize])
{
	for (int i = 0; i < sudokuSize; i++){
		for (int j = 0; j < sudokuSize; j++)
		{
			states[i][j] = new State(sudoku[i][j], i, j);
		}
	}
}

QueueSet* initialLinks(State* states[sudokuSize][sudokuSize]){
	QueueSet* queue = new QueueSet;
	for (int i = 0; i < sudokuSize; i++){
		for (int j = 0; j < sudokuSize; j++)
		{
			State* state = (State*)states[i][j];
			// If state is already initialized, we don't check dependencies. We assumed solvable sudoku
			if (!state->isFinal())
			{
				createLinks(states, queue, state);
			}
		}
	}
	return queue;
}

bool removeInconsistentValues(Link link)
{
	bool removed = false;
	State* tail = link.getTail();
	State* head = link.getHead();

	if (head->isFinal() || head->isForced()){
		if (tail->numberAvailable(head->assignedValue())){
			tail->removeFromDomain(head->assignedValue());
			removed = true;
		}
		return removed;
	}

	for (int i = 1; i <= sudokuSize; i++)
	{
		if (!tail->numberAvailable(i))
		{
			continue;
		}
		for (int j = 1; j <= sudokuSize; j++)
		{
			if (i != j && head->numberAvailable(j))
			{
				i++;
				if (!tail->numberAvailable(i))
				{
					continue;
				}
			}
		}
		tail->removeFromDomain(i);
		removed = true;
	}
	return removed;
}

void addNeighborsToQueueOf(State* states[sudokuSize][sudokuSize], QueueSet* queue, State* state)
{
	int row = state->getRow();
	int col = state->getColumn();
	for (int i = 0; i < sudokuSize; i++)
	{
		if (col != i) 
		{
			if (!states[row][i]->isFinal())
			{
				queue->push(Link(states[row][i], states[row][col]));
			}
		}
		if (row != i) 
		{
			if (!states[i][col]->isFinal()){
				queue->push(Link(states[i][col], states[row][col]));
			}
		}
	}
	int rowB, colB;
	rowB = (int)std::floor(row / squareNbr);
	colB = (int)std::floor(col / squareNbr);
	rowB *= squareNbr;
	colB *= squareNbr;

	for (int i = rowB; i < (rowB + squareNbr); i++)
	{
		for (int j = colB; j < (colB + squareNbr); j++)
		{
			if (i != row && j != col)
			{
				if (!states[i][j]->isFinal())
				{
					queue->push(Link(states[i][j], states[row][col]));
				}
			}
		}
	}
}

bool arcConsistency(State* states[sudokuSize][sudokuSize], QueueSet * queue)
{
	while (queue->size() > 0)
	{
		Link link = queue->pop();
		if (removeInconsistentValues(link))
		{
			if (link.getTail()->isDirty())
			{
				backtrack++;
				return false;
			}
			addNeighborsToQueueOf(states, queue, link.getTail());
		}
	}
	return true;
}

void printResult(State* states[sudokuSize][sudokuSize]){
	std::cout << std::endl;
	for (int i = 0; i < sudokuSize; i++)
	{
		for (int j = 0; j < sudokuSize; j++)
		{
			std::cout << states[i][j]->assignedValue() << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void consolidateStates(State* states[sudokuSize][sudokuSize])
{
	for (int i = 0; i < sudokuSize; i++)
	{
		for (int j = 0; j < sudokuSize; j++)
		{
			if (states[i][j]->assignedValue() > 0)
			{
				states[i][j]->consolidate();
			}
		}
	}
}

void preprocessing(State* states[sudokuSize][sudokuSize], const int sudoku[sudokuSize][sudokuSize])
{
	initializeStates(states, sudoku);
	QueueSet* queue = initialLinks(states);
	if (!arcConsistency(states, queue))
	{
		std::cout << "Unsolvable sudoku" << std::endl;
	}
	consolidateStates(states);
}

bool recursiveBacktrack(State* orStates[sudokuSize][sudokuSize], cspPriorityQueue queue)
{
	if (queue.empty()){
		return true;
	}
	State currentState = queue.top(); 
	queue.pop();
	for (int i = 1; i <= sudokuSize; i++)
	{
		if (currentState.numberAvailable(i))
		{
			State* states[sudokuSize][sudokuSize];
			State::copyStateArray(orStates, states);
			State* varToAssign = new State(currentState);
			varToAssign->forceAssignment(i);
			states[currentState.getRow()][currentState.getColumn()] = varToAssign;
			QueueSet linkqueue;
			createLinks(states, &linkqueue, varToAssign);
			addNeighborsToQueueOf(states, &linkqueue, varToAssign);
			if (arcConsistency(states, &linkqueue) && recursiveBacktrack(states, queue))
			{
				State::copyStateArray(states, orStates);
				return true;
			}
			else
			{
				continue;
			}
		}
	}
	return false;
}

bool bactrackSearch(State* states[sudokuSize][sudokuSize])
{
	cspPriorityQueue queue;
	for (int i = 0; i < sudokuSize; i++)
	{
		for (int j = 0; j < sudokuSize; j++)
		{
			if (!states[i][j]->isFinal())
			{
				assert(states[i][j]->assignedValue() == 0);
				assert(!states[i][j]->isDirty());
				queue.push(*states[i][j]);
			}
		}
	}
	return recursiveBacktrack(states, queue);
}

void solveCSP(const int sudoku[sudokuSize][sudokuSize]){
	State* states[sudokuSize][sudokuSize];
	preprocessing(states, sudoku);
	std::cout << "\nSolution after initial filtering:" << std::endl;
	printResult(states);
	if (!bactrackSearch(states))
	{
		std::cout << "Unsolvable" << std::endl;
	}
	else
	{
		std::cout << "Solution:" << std::endl;
		printResult(states);
	}
}

int main() {
	std::cout << "Enter your sudoku : " << std::endl;
	std::string buffer = "sudokuWiki";
	//getline(std::cin, buffer);
	IntegerReader reader(buffer);

	int sudoku[sudokuSize][sudokuSize];
	for (int i = 0; i < sudokuSize; i++){
		for (int j = 0; j < sudokuSize; j++)
		{
			sudoku[i][j] = reader.nextInt();
		}
	}

	solveCSP(sudoku);
	std::cout << backtrack << " backtracks had to be done" << std::endl;
	std::cout << "press Enter to exit" << std::endl;
	getline(std::cin, buffer);
	return 0;
}