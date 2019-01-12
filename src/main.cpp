#include "MyInclude.h"
#include "getopt.h"

Setting setting;
Database db;

void read(std::istream& is) {
}

int main(int argc, char** argv) {
	int opt;
	string usage = "Usage: " + std::string(argv[0]) + "" + " data_file";
	while ((opt = getopt(argc, argv, "a:b")) != -1) {
		switch (opt) {
			case 'a':
				//a = std::atoi(optarg);
				break;
			case 'b':
				//b = true;
				break;
			default:
				std::cerr << usage << std::endl;
				return -1;
		}
	}
	//cout << a << " " << b << endl;

	if (argc - optind != 1) {
		std::cerr << usage << std::endl;
		return -1;
	}
	string filename(argv[optind++]);
	std::ifstream file(filename);
	if (file.fail()) {
		std::cerr << "File not found : " << filename << endl;
		return -1;
	}
	read(file);

	std::cout << "\e[38;5;0m\e[48;5;40m --- end ---  \e[m" << std::endl; // debug
	return 0;
}
