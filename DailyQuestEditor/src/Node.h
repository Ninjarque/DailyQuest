#pragma once

#include <string>

#include "IO/Data.h"
#include "IO/Serializable.h"

class Node : public Serializable
{
public:
	Node() : Node("Default") { }
	Node(std::string name) : Node(name, nullptr, nullptr) { }
	Node(std::string name, Node* childA, Node* childB);

	void Save(Data& data) override;
	void Load(Data& data) override;

	std::string ToString();

	std::string Name;
	Node* ChildA;
	Node* ChildB;
	bool hasChilds();
};
class NodeList : public Serializable
{
public:
	NodeList() : NodeList(std::vector<Node*>()) { }
	NodeList(std::vector<Node*> nodes)
	{
		Nodes = nodes;
	}

	void Save(Data& data) override
	{
		int count = Nodes.size();
		data["Count"].SetInt(count);
		for (int i = 0; i < count; i++)
		{
			std::string name = "Node" + std::to_string(i);
			Nodes[i]->Save(data[name]);
		}
	}
	void Load(Data& data) override 
	{
		int count = data["Count"].GetInt(0);
		for (int i = 0; i < count; i++)
		{
			std::string name = "Node" + std::to_string(i);
			Node* n = new Node();
			n->Load(data[name]);
			Nodes.push_back(n);
		}
	}
	std::string ToString()
	{
		std::string txt = "NodeList, size (" + std::to_string(Nodes.size()) + ")\n";
		for (int i = 0; i < Nodes.size(); i++)
		{
			txt += "[" + std::to_string(i) + "]\n{\n" + Nodes[i]->ToString() + "}\n";
		}
		return txt + "\n";
	}

	std::vector<Node*> Nodes;
};