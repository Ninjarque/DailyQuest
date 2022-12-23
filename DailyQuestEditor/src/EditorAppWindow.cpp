#include "EditorAppWindow.h"
#include "IO/Data.h"

using namespace std;

#include <fstream>

void test_file(std::string fileName, int at)
{

	std::ifstream file(fileName, std::ios::binary);
	if (file.is_open()) {
		char* test = new char[at + 1];
		file.read(test, at);
		test[at] = '\0';
		std::cout << "Here is what's being written up to char " << at << "\n\n" << test << std::endl << std::endl;
		file.seekg(at, file.beg);
		test = new char[10001];
		file.read(test, 10001);
		test[10000] = '\0';
		std::cout << "Here is what's after char " << at << "\n\n" << test << std::endl;
	}
}

void EditorAppWindow::OnInit()
{
	Data d;

	/*
	Data& scene = d["Scene"];

	Data& obj1 = scene["Obj1"];
	obj1["CSGType"].SetString("Intersection");
	obj1["CSGValue"].SetDouble(0.5);

	Data& child1 = obj1["Child1"];
	child1["CSGType"].SetString("Minus");
	child1["CSGValue"].SetDouble(1.0);

	Data& sphere1 = child1["Child1"];
	Data& sphere2 = child1["Child2"];

	sphere1["Type"].SetString("Sphere");
	sphere1["Position"].SetDouble(0.0, 0);
	sphere1["Position"].SetDouble(0.0, 1);
	sphere1["Position"].SetDouble(-10.0, 2);
	sphere1["Radius"].SetDouble(1.0);

	sphere2["Type"].SetString("Sphere");
	sphere2["Position"].SetDouble(1.0, 0);
	sphere2["Position"].SetDouble(1.0, 1);
	sphere2["Position"].SetDouble(-10.0, 2);
	sphere2["Radius"].SetDouble(2.0);

	Data& child2 = obj1["Child2"];
	child2["Type"].SetString("Cube");
	child2["Position"].SetDouble(-1.0, 0);
	child2["Position"].SetDouble(0.0, 1);
	child2["Position"].SetDouble(-10.0, 2);
	child2["Size"].SetDouble(0.75);

	// */
	/*
	d["test"].SetString("I would like to quote \\\"Dis, is a test text; Dude Ducken\\\" from ME");

	d["user"]["name"].SetString("Ninjarque");
	d["user"]["birthday"].SetString("Today");
	
	d["user"]["age"].SetInt(42);

	d["user"]["house"]["owner"].SetString("Ninja");
	
	d["user"]["house"]["size"].SetDouble(5.0, 0);
	d["user"]["house"]["size"].SetDouble(10.0, 1);

	d["user"]["code"]["language"].SetString("C#");
	
	d["user"]["code"]["framework"].SetString(".NET");
	
	d["user"]["code"]["version"].SetDouble(10.2);

	d["user"]["projects"]["leader"].SetString("Me da best");
	
	d["user"]["projects"]["sub-leader"].SetString("Me again... but I ain't workin under myself tough!");
	
	d["user"]["projects"]["quotes"]["motivational"].SetString("\"One day I'll be, the best trainee!!!\"");

	d["user"]["projects"]["names"]["minecraft"].SetString("To mine, and to craft");
	
	d["user"]["projects"]["names"]["pokemon"].SetString("To be or not to be homeless");
	d["user"]["projects"]["names"]["Yu-Gi-Ho"].SetString("Is this even the right way to write it?");

	//*/
	std::string fileName = "C:\\Users\\ninja\\Desktop\\Data.txt";
	std::string fileName2 = "C:\\Users\\ninja\\Desktop\\Data2.txt";
	std::string fileName3 = "C:\\Users\\ninja\\Desktop\\Data3.txt";
	
	bool useSizes = false;

	//Data::Write(d, fileName, "\t", ',', useSizes);

	Data read;

	//read["Histoire"];
	// /*
	read["Histoire"];
	read["Monde"]["Nom"];
	read["Monde"]["Villes"]["Roiark"]["Type"];
	read["Monde"]["Villes"]["Doyon"]["Personnages_Secondaires"];
	// */

	//read["Scene"]["Obj1"]["Child2"];

	/*
	read["user"]["age"];
	read["user"]["house"]["owner"];
	read["user"]["code"];
	read["user"]["projects"]["quotes"];
	//*/

	//test_file(fileName, 102);
	//test_file(fileName, 467);

	Data::Read(read, fileName, "\t", ',');
	Data::Write(read, fileName2, "\t", ',', useSizes);
	
	//std::cout << d.ToString() << std::endl;
	std::cout << read.ToString() << std::endl;
}

void EditorAppWindow::OnDispose()
{
	cout << "Disposed of editor I see" << endl;
}

void EditorAppWindow::OnUpdate(float deltaTime)
{

}

void EditorAppWindow::OnDraw()
{
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glVertex2f(0.5f, -0.5f);
	glEnd();
}

void EditorAppWindow::OnImGUIDraw()
{
}
