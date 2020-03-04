#include <fstream>

class ShaderReader {
	class String {
		char* addArray;
	public:
		char* word;
		int size = 0;

		String() {
			word = new char[size];
		}

		void add(int character) {
			addArray = new char[size];

			for (int i = 0; i < size; ++i) {
				addArray[i] = word[i];
			}

			delete[] word;

			size += 1;
			word = new char[size];

			for (int i = 0; i < size - 1; ++i) {
				word[i] = addArray[i];
			}
			delete[] addArray;

			word[size - 1] = character;
		}

		void add(char wrd[], int length) {
			addArray = new char[size];

			for (int i = 0; i < size; ++i) {
				addArray[i] = word[i];
			}

			delete[] word;

			word = new char[size + length];

			for (int i = 0; i < size; ++i) {
				word[i] = addArray[i];
				//
				std::cout << word[i] << "(" << (int)word[i] << ")";
			}

			delete[] addArray;
			for (int i = size; i < size + length; ++i) {
				word[i] = wrd[i - size];
				//
				std::cout << word[i] << "(" << (int)word[i] << ")";
			}

			//
			std::cout << "\n\n";

			size += length;
		}

		void add(const char* wrd, int length) {
			addArray = new char[size];

			for (int i = 0; i < size; ++i) {
				addArray[i] = word[i];
			}

			delete[] word;

			word = new char[size + length];

			for (int i = 0; i < size; ++i) {
				word[i] = addArray[i];
			}

			delete[] addArray;
			for (int i = size; i < size + length; ++i) {
				word[i] = wrd[i - size];
			}

			size += length;

		}

		~String() {
			delete[] word;
		}
	};
public:
	String source;
	bool error = false;

	ShaderReader(const char* path) {
		std::ifstream is(path);
		if (is.is_open()) {
			char* line;

			char c;
			do {
				c = is.get();
				if (c != -1) {
					source.add(c);
				}
			} while (c != -1);

			source.add(0);

			is.close();
		}
		else {
			error = true;
		}
	}
};
