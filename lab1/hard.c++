#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>

std::string strip(const std::string & input)
{
	auto start = input.find_first_not_of(' ');
	if(start == input.npos)	return "";

	auto end = input.find_last_not_of(' ');
	return input.substr(start, end - start + 1);
}


int main(int argc, const char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Spatny pocet argumentu" << std::endl;
		return 1;
	}
	std::ofstream out{argv[1]};
	if (!out)
	{
		std::cout << "Soubor nelze otevrit." << std::endl;
		return 1;
	}
	std::vector<std::string> text;
	std::string line;
	while(std::getline(std::cin, line))
	{
		if(strip(line) != "") text.push_back(line);
	}
	std::ranges::sort(text, std::greater<>{});

	for(auto && line : text) out << line << std::endl;

	out.close();
	if(!out) {
		std::cout << "Soubor se nepovedlo uzavrit" << std::endl;
		return 1;
	}

	return 0;
}