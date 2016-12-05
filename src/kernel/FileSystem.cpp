#include "FileSystem.h"
#include <iostream>

const std::string FileSystem::CONST_ROOT = "C:\\";


fsFile FileSystem::createFile(std::string name)
{
	fsFile file(name, nullptr);
	return file;
}

fsDirectory FileSystem::createDirectory(std::string name)
{
	fsDirectory directory(name, nullptr);
	return directory;
}

std::string FileSystem::getAbsolutePath(fsNode node)
{
	/**
	 * 	Run this to test
	fsDirectory fs = FileSystem::getInstance();

	fsDirectory dir = FileSystem::createDirectory("1");
	fsDirectory dir2 = FileSystem::createDirectory("2");
	fsFile file = FileSystem::createFile("file.txt");
	
	FileSystem::addChild(&fs,&dir);
	dir.addChild(&dir2);
	FileSystem::addChild(&dir2, &file);

	std::string path = FileSystem::getAbsolutePath(file);
	std::cout << path.c_str() << std::endl;
	 */
	fsNode* tmp = &node;
	std::string path;
	while(true)
	{
		if(tmp->parent == nullptr)
		{
			path = tmp->name + path;
			break;
		}
		if (path.size() != 0) {
			path = tmp->name + "\\" + path;
		} else
		{
			path = tmp->name;
		}
		tmp = tmp->parent;
	}
	
	return path;
}

int FileSystem::addChild(fsDirectory* parent, fsNode* child)
{
	std::vector<fsNode>::iterator it = parent->childs.begin();
	parent->childs.insert(it,*child);
	child->parent = parent;
	return 1;
}

/**
 * TODO untested if it actually deletes from childs too
 */
int deleteNode(fsNode* node)
{
	//fsDirectory * parent = (fsDirectory*) node->parent;
	delete node;
	return 1;
}

/**
 * TODO doesnt work
 */
bool FileSystem::isNodeFile(fsFile* node)
{
	fsFile* tmp = node;

	tmp->name;

	fsNode* b = dynamic_cast<fsNode*>(tmp);
	if (b)
	{
		// cast succeeded
		std::cout << "Pravdu diz" << std::endl;

		return true;
	}
	std::cout << "NAh A" << std::endl;
	return false;
}

bool FileSystem::isNodeDirectory(fsDirectory node)
{
	throw "Not implemented";
	return false;
}
