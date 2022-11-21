#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

// data-structure for Queue Customer data
struct QueueCustomer {
	int custNum;
	int numItems;
	string packagingType;
	int numPacks;
	string paymentType;
	bool isExact;
};

// constants
const double ITEM_DECART_TIME = 0.5;
const double ITEM_SCAN_TIME = 0.5;
const double ITEM_PACK_TIME = 0.5;
const double BAG_PACKING_COMPLETION_TIME = 5;
const double BOX_PACKING_COMPLETION_TIME = 10;
const double CASH_NOT_EXACT_PAYMENT_TIME = 20;
const double CASH_EXACT_PAYMENT_TIME = 15;
const double DIGITAL_PAYMENT = 15;
const double ITEM_RECEIPT_PRINT_TIME = 0.3;
const double OTHERS_RECEIPT_PRINT_TIME = 2;
const double IDLE_TIME = 3;

// Function: Parse Data()
QueueCustomer parseData(int index, string line)
{
	// parse data-line, using [space] as delimiter
	// source: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
	size_t pos = 0;
	size_t npos = -1;
	string delimiter = " ";
	string data[5] = {""};
	int data_index = 0;
	while ((pos = line.find(delimiter)) != npos) {
	    data[data_index] = line.substr(0, pos);
	    line.erase(0, pos + delimiter.length());
	    data_index++;
	}
	data[data_index] = line;
	// set QueueCustomer data
	stringstream ss;
	QueueCustomer temp;
	temp.custNum = index + 1;
	for (int i = 0; i < 5; i++) {
		string d = *(data + i);
		if (i == 0 || i == 2) {
			ss << d;
			if (i == 0) {
				ss >> temp.numItems;
			} else if (i == 2) {
				ss >> temp.numPacks;
			}
			ss.clear();
			ss.str("");
		} else if (i == 1) {
			temp.packagingType = d;
		} else if (i == 3) {
			temp.paymentType = d;
		} else if (i == 4) {
			temp.isExact = d == "e";
		}
	}
	return temp;
}

// Function: Print Queue-Customer data
void printQueueCustomer(QueueCustomer q)
{
	string packTypeLabel = q.packagingType + (q.packagingType == "box" ? "es" : "s ");
	cout << "Customer #" << q.custNum << endl
		 << "No. of Cart Items          : " << q.numItems << endl
		 << "Preferred Packaging        : " << q.packagingType << endl
		 << "Number of " << packTypeLabel << "            : " << q.numPacks << endl
		 << "Preferred Payment Method   : " << q.paymentType;
	if (q.paymentType == "cash") {
		cout << " (" << (q.isExact ? "not " : "") << "exact)";
	}
	cout << "\n-------------------------------------------\n";
}

// Function: Calculate Lane-Queue Statistics
void calculateQueueStatistics(QueueCustomer q[], int q_size)
{
	double decartTime = 0, scanTime = 0, packingItemTime = 0, packCompletionRate = 0, packCompletionTime = 0, totalPackingTime = 0,
		   payingTime = 0, printingTime = 0, idleTime = 0, totalQueueTime = 0, overAllTotalQueueTime = 0;
	// For each queue data...
	for (int i = 0; i < q_size; i++) {
		printQueueCustomer(q[i]);
		// compute decarting time
		decartTime = q[i].numItems * ITEM_DECART_TIME;
		// compute scanning time
		scanTime = q[i].numItems * ITEM_SCAN_TIME;
		// compute packing time
		packingItemTime = q[i].numItems * ITEM_PACK_TIME;
		packCompletionRate = (q[i].packagingType == "bag" ? BAG_PACKING_COMPLETION_TIME : BOX_PACKING_COMPLETION_TIME);
		packCompletionTime = q[i].numPacks * packCompletionRate;
		totalPackingTime = packingItemTime + packCompletionTime;
		// compute paying time
		payingTime = CASH_NOT_EXACT_PAYMENT_TIME;
		if (q[i].paymentType == "digital") {
			payingTime = DIGITAL_PAYMENT;
		} else if (q[i].paymentType == "cash" && q[i].isExact) {
			payingTime = CASH_EXACT_PAYMENT_TIME;
		}
		// compute receipt-printing time
		printingTime = (q[i].numItems * ITEM_RECEIPT_PRINT_TIME) + OTHERS_RECEIPT_PRINT_TIME;
		// check idle time
		idleTime = 0;
		if (i > 0) {
			idleTime = IDLE_TIME;
		}
		// compute total customer queue time
		totalQueueTime = decartTime + scanTime + totalPackingTime + payingTime + printingTime + idleTime;
		// aggregate lane-queue time
		overAllTotalQueueTime += totalQueueTime;
		// print statistics
		cout << "Total De-Carting Time      : " << decartTime << endl
			 << "Total Scanning Time        : " << scanTime << endl
			 << "Total Packing Time         : " << totalPackingTime << endl
			 << "Total Paying Time          : " << payingTime << endl
			 << "Total Receipt Printing Time: " << printingTime << endl;
		cout << "Idle Time                  : " << idleTime << endl;
		cout << "Total Customer-Queue Time  : " << totalQueueTime << endl;
		if (i < q_size - 1) {
			cout << "+++++++++++++++++++++++++++++++++++++++++++\n";
		} else {
			cout << "===========================================\n";
		}
	}
	cout << "Total Lane Queue Time      : " << overAllTotalQueueTime << endl;
}

// Main Function
int main ()
{
	// Open the input file
	ifstream in;
	char *filename = "project.in";
	in.open(filename);
	if (!in.is_open()) {
		cout << "Cannot open file: " << filename;
		exit(1);
	}
	// Read the record size
	int T;
	char c;
	in >> T;
	cout << "No. of Customers: " << T << endl;
	cout << "===========================================\n";
	// instanstiate queue
	QueueCustomer queue[T];
	string line;
	int index = 0;
	// For each succeeding lines of strings...
	while (getline(in, line)) {
		if (line.length() > 0) {
			// Parse data and store to queue
			queue[index] = parseData(index, line);
			index++;
		}
	}
	// calculate lane-queue statistics
	calculateQueueStatistics(queue, T);
	return 0;
}
