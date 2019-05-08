#include <iostream>
#include <fstream>
#include <set>
#include <sstream>

std::string any_from(std::set<std::string> set) {
	return *set.begin();
}

int main() {

	
	std::cout << "File name: ";
	std::string file_name;
	std::cin >> file_name;

	std::ifstream file_stream(file_name);
	if (!file_stream.is_open()) {
		std::cout << "Can't open file";
		return 0;
	}

	
	std::set<std::string> shortest_words;
	std::set<std::string> longest_words;
	
	char c;
	std::string word; // будем считывать по символу и поддерживать текущий символ
	while (file_stream.get(c)) {
		if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')) { // если считали букву, увеличиваем слово
			word += c;
		}
		else if (word != "") { // если не букву, то возможно закончилось слово
			if (shortest_words.empty() || any_from(shortest_words).size() > word.size()) { // проверяем что новое слово самое короткое
				shortest_words.clear();
				shortest_words.insert(word);
			}
			else if (any_from(shortest_words).size() == word.size()) { // проверяем что самые короткие слова как раз таой длины
				shortest_words.insert(word);
			}

			if (longest_words.empty() || any_from(longest_words).size() < word.size()) { // проверяем что новое слово самое длинное
				longest_words.clear();
				longest_words.insert(word);
			}
			else if (any_from(longest_words).size() == word.size()) { // проверяем что самые длинные слова как раз таой длины
				longest_words.insert(word);
			}
			word = "";
		}
	}
	file_stream.close();
	std::cout << "The shortest words:" << std::endl;
	for (std::string word : shortest_words) {
		std::cout << word << std::endl;
	}
	std::cout << "The longest words:" << std::endl;
	for (std::string word : longest_words) {
		std::cout << word << std::endl;
	}




}