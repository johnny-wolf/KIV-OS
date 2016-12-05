#pragma once

#include <vector>
#include <iostream>


// FileSystem node
struct fsNode
{	
public:
	//Name of the node
	std::string name;
	// Parent of the node
	fsNode* parent;
};

// FileSystem node that represents file
struct fsFile : public fsNode
{
public:
	// Data of the file
	std::string data;

	fsFile(std::string name, fsNode* parent)
	{
		this->name = name;
		this->parent = parent;
	}
	fsFile(std::string name, fsNode* parent, std::string data)
	{
		this->name = name;
		this->parent = parent;
		this->data = data;
	}
};

// FileSystem node that reprezents directory
struct fsDirectory : public fsNode
{
public:
	// Child of the directory
	std::vector<fsNode> childs;

	fsDirectory(){}

	fsDirectory(std::string name, fsNode* parent)
	{
		this->name = name;
		this->parent = parent;
	}

	fsDirectory(std::string name, fsNode* parent, std::vector<fsNode> childs)
	{
		this->name = name;
		this->parent = parent;
		this->childs = childs;
	}

	int addChild(fsNode* child)
	{
		std::vector<fsNode>::iterator it = this->childs.begin();
		this->childs.insert(it, *child);
		child->parent = this;
		return 1;
	}
};





class FileSystem
{
public:
	static const std::string CONST_ROOT;

	static fsDirectory& getInstance()
	{
		// Guaranteed to be destroyed.

		// Instance of the filesystem, initialized only once.
		static fsDirectory instance;
		if (instance.name.size() == 0) {
			instance.name = CONST_ROOT;
		}
		return instance;
	}

	static fsFile createFile(std::string name);
	static fsDirectory createDirectory(std::string name);
	static std::string getAbsolutePath(fsNode node);
	static int addChild(fsDirectory* parent,fsNode* child);
	static bool FileSystem::isNodeFile(fsFile* node);
	static bool isNodeDirectory(fsDirectory node);

private:
	FileSystem() {}                    // Constructor? (the {} brackets) are needed here.

	// C++ 11
	// =======
	// We can use the better technique of deleting the methods
	// we don't want.
public:
	FileSystem(FileSystem const&) = delete;
	void operator=(FileSystem const&) = delete;

	//Functionality of filesystem

	
	
	std::vector<fsFile> getFilesFromDir();
	std::vector<fsDirectory> getDirsFromDir();
	std::vector<fsNode> getAllFromDir();

	int deleteNode();
	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status
};

