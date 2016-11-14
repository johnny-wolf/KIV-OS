#pragma once

#include <string>

struct node
{
	std::string name;
	node* parent;
};

struct File : node
{
	std::string data;
};

struct Directory : node
{
	node* childs[];
};