#include "Node.h"

Node::Node(std::string name, Node* childA, Node* childB)
{
	Name = name;
	ChildA = childA;
	ChildB = childB;
}

void Node::Save(Data& data) 
{
	if (hasChilds())
	{
		data["Name"].SetString(Name);
		ChildA->Save(data["ChildA"]);
		ChildB->Save(data["ChildB"]);
	}
	else
	{
		data["Name"].SetString(Name);

	}
}

void Node::Load(Data& data) 
{
	if (data.Has("ChildA"))
	{
		Name = data["Name"].GetString(0);
		ChildA = new Node();
		ChildB = new Node();
		ChildA->Load(data["ChildA"]);
		ChildB->Load(data["ChildB"]);
	}
	else
	{
		Name = data["Name"].GetString(0);
	}
}

std::string Node::ToString()
{
	std::string txt = Name;
	if (hasChilds())
	{
		txt += "\n{\n";
		txt += ChildA->ToString();
		txt += ChildB->ToString();
		txt += "}\n";
	}
	else txt += ";\n";
	return txt;
}

bool Node::hasChilds() { return ChildA != nullptr && ChildB != nullptr; }
