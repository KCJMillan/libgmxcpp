
#include "Index.h"

Index::Index() {
	return;
}

Index::Index(string ndxfile) {
	Set(ndxfile);
	return;
}

// TODO: figure out how to use maps
// Reads in the index file, first getting the headers so that the locations
// vector can be properly sized. Then the index file is read again; if the line
// is a header, increment the group counter. Any other line is read in with a
// string stream into integers that are added to the vector located in this
// location in the locations vector.
bool Index::Set(string ndxfile) {

    ifstream iFS;
    istringstream linestream;
    string line, header;
    int num;
    int groupNum = 0;

	cout << "Reading in index file " << ndxfile << "...";

    if (!IsIndexFile(ndxfile)) throw runtime_error("Not a valid index file.");
    

    iFS.open(ndxfile.c_str());

	if (!iFS.is_open()) throw runtime_error("Cannot open index file.");

    while (getline(iFS,line)) {
        if (isHeader(line)) { 
            header = line.substr(2,line.length()-4);
            headers.push_back(header);
            groupNum++;
        }
    }
    locations.resize(groupNum);
    iFS.close();

    groupNum = -1;
    iFS.open(ndxfile.c_str());
    while (getline(iFS,line)) {
        if (isHeader(line)) { 
            groupNum++;
        } else if (line.length() != 0) {
            linestream.clear();
            linestream.str(line);
            while (linestream >> num) {
                locations.at(groupNum).push_back(num);
            }
        }
    }
    iFS.close();

    cout << "OK" << endl;

	cout << "Found the following groups: " << endl;
	PrintInfo();
	cout << "Finished reading in index file." << endl;

    return true;

}

void Index::PrintInfo() {
	for (int i=0; i<headers.size(); i++) {
		cout << "  " << headers.at(i);
		for (int j=0; j<10-headers.at(i).length(); j++) {
			cout << " ";
		}
		cout << "(" << locations.at(i).size() << " particles)" << endl;
	}
	return;

}

bool Index::IsIndexFile(string ndxfile) {
    ifstream iFS;
    string line;

    iFS.open(ndxfile.c_str());
    getline(iFS,line);
    iFS.close();

    if (isHeader(line)) return true;

    return false;
}

// Translates the string name of a header to its correct index integer
int Index::GetHeaderIndex(string header) {
    for (int i=0; i<headers.size(); i++) {
        if (headers.at(i) == header) {
            return i;
        }
    }
	cout << "ERROR: Group '" << header << "' not found in index file." << endl;
    return -1;
}

int Index::GetGroupSize(string header) {
    int locationIndex = GetHeaderIndex(header);
	if (locationIndex == -1) return -1;
	return locations.at(locationIndex).size();
}

// Gets the location of the ith atom in the header group specified
int Index::GetLocation(string header, int i) {
    int locationIndex = GetHeaderIndex(header);
	if (locationIndex == -1) return -1;
    return locations.at(locationIndex).at(i)-1;
}

bool Index::isHeader(string line) {
    return (line[0] == '[' && line[line.length()-1] == ']');
}
