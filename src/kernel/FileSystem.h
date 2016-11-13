#pragma once

#include <string>

struct Node
{
	std::string name;
	Node* parent;
};

struct File : Node
{
	std::string data;
};

struct Directory : Node
{
	Node* childs[];
};