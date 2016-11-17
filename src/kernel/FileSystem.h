#pragma once

#include <string>
#include <vector>


// FileSystem node
struct fsNode
{	
	//Name of the node
	std::string name;
	// Parent of the node
	fsNode* parent;
};

// FileSystem node that reprezents file
struct fsFile : fsNode
{
	// Data of the file
	std::string data;

};

// FileSystem node that reprezents directory
struct fsDirectory : fsNode
{
	// Child of the directory
	std::vector<fsNode> childs;
};





class FileSystem
{
public:
	static const std::string CONST_ROOT;
public:
	static fsDirectory& getInstance()
	{
		// Guaranteed to be destroyed.

		// Instance of the filesystem, initialized only once.
		static fsDirectory instance;
		if (!instance.name.compare(CONST_ROOT)) {
			instance.name = CONST_ROOT;
		}
		return instance;
	}
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
	fsFile* createFile();
	fsDirectory* createDirectory();

	std::string getAbsolutePath();
	
	std::vector<fsFile> getFilesFromDir();
	std::vector<fsDirectory> getDirsFromDir();
	std::vector<fsNode> getAllFromDir();
	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status
};