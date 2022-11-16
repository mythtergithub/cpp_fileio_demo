#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

struct Person {
	int id;
	string name;
	int age;
};

void printPerson(Person n)
{
	cout << "ID  : " << n.id << endl
		 << "Name: " << n.name << endl
		 << "Age : " << n.age << endl;
}

int main ()
{
	ifstream in;
	char *filename = "main.in";
	
	in.open(filename);
	if (!in.is_open()) {
		cout << "Cannot open file: " << filename;
		exit(1);
	}
	int T;
	char c;
	in >> T >> c;
	cout << "Data size: " << T << "\n\n";	
	
	Person list[T];
	string line;
	int count = 3;
	stringstream ss;
	int index = 0;
	while (getline(in, line)) {
		if (count == 3) {
			ss << line; 
			ss >> list[index].id;
			ss.clear();
			ss.str("");
		} else if (count == 2) {
			list[index].name = line;
		} else if (count == 1) {
			ss << line;
			ss >> list[index].age;
			ss.clear();
			ss.str("");
		} else {
			printPerson(list[index]);
			index++;
			count = 4;
			cout << endl;
		}
		count--;
	}
	return 0;
}
