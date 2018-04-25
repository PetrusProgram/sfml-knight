#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include "Utilities.hpp"

template<typename Derived, typename T>
class ResourceManager 
{
public:
	ResourceManager(const std::string& pathsFile) {
		LoadPaths(pathsFile);
	}

	T* GetResource(const std::string& id) 
	{
		auto res{ Find(id) };
		return(res ? res->first.get() : nullptr);
	}

	std::string GetPath(const std::string& id)
	{
		auto path{ m_paths.find(id) };
		return(path != m_paths.end() ? path->second : "");
	}

	bool RequireResource(const std::string& id)
	{
		auto res{ Find(id) };
		if (res) 
		{
			++res->second;
			return true;
		}
		auto path{ m_paths.find(id) };
		if (path == m_paths.end()) 
			return false; 

		auto resource{ Load(path->second) };
		if (!resource)  
			return false; 

		m_resources.emplace(id, std::make_pair(std::move(resource), 1));
		return true;
	}

	bool ReleaseResource(const std::string& id) 
	{
		auto res{ Find(id) };
		if (!res) 
			return false;

		--res->second;
		if (!res->second) 
			Unload(id); 

		return true;
	}
protected:
	std::unique_ptr<T> Load(const std::string& path) 
	{
		return static_cast<Derived*>(this)->Load(path);
	}
private:
	std::pair<std::unique_ptr<T>, unsigned int>* Find(const std::string& id)
	{
		auto itr{ m_resources.find(id) };
		return (itr != m_resources.end() ? &itr->second : nullptr);
	}

	bool Unload(const std::string& id) 
	{
		auto itr{ m_resources.find(id) };
		if (itr == m_resources.end())  
			return false; 
		std::cout << "unload" << std::endl;
		m_resources.erase(itr);
		return true;
	}

	void LoadPaths(const std::string& pathFile)
	{
		std::ifstream paths(Utils::GetWorkingDirectory() + pathFile);
		if (paths) 
		{
			std::string line;
			while (std::getline(paths, line)) 
			{
				std::stringstream keystream(line);
				std::string pathName;
				std::string path;
				keystream >> pathName;
				keystream >> path;
				m_paths.emplace(pathName, path);
			}
			return;
		}
		std::cerr << "! Failed loading the path file: " << pathFile << std::endl;
	}

	std::unordered_map<std::string,	std::pair<std::unique_ptr<T>, unsigned int>> m_resources;
	std::unordered_map<std::string, std::string> m_paths;
};