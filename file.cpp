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

Person parseData(string line)
{
	// parse data-line, using [space] as delimiter
	// source: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	size_t pos = 0;
	size_t npos = -1;
	string delimiter = ",";
	string data[5] = {""};
	int data_index = 0;
	while ((pos = line.find(delimiter)) != npos) {
	    data[data_index] = line.substr(0, pos);
	    line.erase(0, pos + delimiter.length());
	    data_index++;
	}
	data[data_index] = line;
	// set Person data
	stringstream ss;
	Person temp;
	for (int i = 0; i < 5; i++) {
		string d = *(data + i);
		if (i == 0 || i == 2) {
			ss << d;
			if (i == 0) {
				ss >> temp.id;
			} else if (i == 2) {
				ss >> temp.age;
			}
			ss.clear();
			ss.str("");
		} else if (i == 1) {
			temp.name = d;
		}
	}
	return temp;
}

void printPerson(Person n)
{
	cout << "ID  : " << n.id << endl
		 << "Name: " << n.name << endl
		 << "Age : " << n.age << endl
		 << "===========================================\n";
}

int main ()
{
	// Open the input file
	ifstream in;
	char *filename = "file.in";
	in.open(filename);
	if (!in.is_open()) {
		cout << "Cannot open file: " << filename;
		exit(1);
	}
	// Read the record size
	int T;
	char c;
	in >> T;
	cout << "Record Size: " << T << endl;
	cout << "===========================================\n";
	// instanstiate queue
	Person list[T];
	string line;
	int index = 0;
	// For each succeeding lines of strings...
	while (getline(in, line)) {
		if (line.length() > 0) {
			// Parse data and store to list
			list[index] = parseData(line);
			// Print Person data
			printPerson(list[index]);
			index++;
		}
	}
	return 0;
}
