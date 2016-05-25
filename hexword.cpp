#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

string get_dictionary_raw(const char* filename) {
	FILE* fp = fopen(filename, "rb");
	if (fp) {
		string contents;
		fseek(fp, 0, SEEK_END);
		contents.resize(ftell(fp));
		rewind(fp); fread(&contents[0], 1, contents.size(), fp);
		fclose(fp);
		return (contents);
	}
	//throw(errno);
}

vector<string> split(const string &s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, delim)) {
		tokens.push_back(item);
	}
	return tokens;
}

vector<string> get_dictionary() {
	string dict_raw = get_dictionary_raw("/usr/share/dict/american-english");
	return split(dict_raw, '\n');
}

vector<string> hex_combos() {
	vector<string> hex;
	for (int i = 0x0000FF; i < 0xFFFFFF; i++) {
		if (i > 0xFFF && i < 0x100000) continue;

		stringstream ss;
		ss << std::hex << i;
		hex.push_back(ss.str());
	}
	return hex;
}

string hex_to_eng(string hex) {
	vector<string> possibilities;

	string c = "";
	for (string::iterator iter = hex.begin(); iter != hex.end(); iter++) {
		char ch = *iter;
		if (ch == '0') {
			c += "o";
		}
		else if (ch == '1') {
			c += "i";
		}		
		else if (ch == '2') {
			c += "z";
		}
		else if (ch == '3') {
			c += "e";
		}
		else if (ch == '4') {
			c += "h";
		}
		else if (ch == '5') {
			c += "s";
		}
		else if (ch == '6') {
			c += "g";
		}
		else if (ch == '7') {
			c += "l";
		}
		else if (ch == '8') {
			c += "b";
		}
		else if (ch == '9') {
			c += "g";
		}
		else {
				c += ch;
		}
		/*
		else if (ch == "A") {
			c += "w";
		}
		else if (ch == "B") {
			c += "q";
		}
		else if (ch == "C") {
			c += ")";
		}
		case "D":
			c += "p";
			break;
		case "E":
			c += "3";
			break;
		case "F":
			c += "s";
			break;
		default:
			c += *iter;
			break;
		*/
	}
	return c;
}

bool is_english_word(string s) {
	static vector<string> valid = get_dictionary();
	return (find(valid.begin(), valid.end(), s) != valid.end());
}

vector<string> find_valid_words(ofstream& output) {
	vector<string> valid;

	vector<string> hex = hex_combos();
	for (vector<string>::iterator iter = hex.begin(); iter != hex.end(); iter++) {
		string english = hex_to_eng(*iter);
		if (is_english_word(english)) {
			cout << "#" << *iter << "(" << english << ") is valid" << endl;
			valid.push_back(*iter);
			output << *iter << endl;
		}				
	}
	return valid;
}

int main() {
	ofstream output("valid.txt");
	vector<string> valid = find_valid_words(output);
	output.close();
	
	return 0;
}
