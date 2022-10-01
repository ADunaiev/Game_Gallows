﻿#include <iostream>
#include<time.h>
#include<vector>
#include<fstream>
#include<string>
using namespace std;

//Создайте консольную версию игры «Виселица».
//Правила игры можно посмотреть по этой ссылке.
//Важно:
//■ Слово выбирается компьютером из списка слов.
//■ Список слов находится в файле в зашифрованном виде.
//■ По завершении игры на экран выводится статистика игры :
//• количество времени;
//• количество попыток;
//• искомое слово;
//• буквы игрока.

//рисовалка виселицы в зависимости от количества ошибок
void Draw_Gallows(int temp)
{
	cout << "--------\n";
	cout << "|     ";
	if (temp > 0)
		cout << "| \n";
	else
		cout << endl;
	cout << "|     ";
	if (temp > 1)
		cout << "O\n";
	else
		cout << endl;
	cout << "|   ";
	if (temp > 2)
		cout << "/-";
	if (temp > 3)
		cout << "+";
	if (temp > 4)
		cout << "-/\n";
	else
		cout << endl;
	cout << "|     ";
	if (temp > 5)
		cout << "|\n";
	else
		cout << endl;
	cout << "|    ";
	if (temp > 6)
		cout << "| |\n";
	else
		cout << endl;
	cout << "|    ";
	if (temp > 7)
		cout << "| |\n";
	else
		cout << endl;
	cout << "|\n";
	cout << "+-----------\n";

}
int code_key = 10;

//функтор для подсчета ошибок
class mistake_counter
{
	int count;
public:
	mistake_counter();
	int get_count();
	int operator()();
};
int mistake_counter::get_count() {
	return count;
}
int mistake_counter::operator()()
{
	return ++count;
}
mistake_counter::mistake_counter(): count {0}{}

//кодирование/декодирование слов
string& code(string& str)
{
	int temp;

	for (size_t i = 0; i < str.length(); i++)
	{
		temp = (int)str[i];
		str[i] = (char)(temp + code_key);
	}
	return str;
}
string& decode(string& str)
{
	int temp;

	for (size_t i = 0; i < str.length(); i++)
	{
		temp = (int)str[i];
		str[i] = (char)(temp - code_key);
	}
	return str;
}

//работа с двоичным файлом
void SaveToFile(vector<string>& words)
{
    remove("Game_Words.txt");

    int size; char* temp = nullptr;

    fstream f("Game_Words.txt", ios::out | ios::binary | ios::app);

    if (!f)
    {
        throw "\nFile is not opened for writing!\n";
    }

    for (auto var : words)
    {
        if (temp != nullptr)
            delete[] temp;

        size = var.length();
        f.write((char*)&size, sizeof(int));
        temp = new char[size + 1];
        strcpy_s(temp, size + 1, code(var).c_str());
        f.write((char*)temp, size * sizeof(char));
    }

    f.close();
    delete[] temp;
}
vector<string>& LoadFromBinFile(vector<string>& object) {
	fstream f("Game_Words.txt", ios::in | ios::binary);
	if (!f) {
		throw "\nFile is not opened for reading!\n\n";
	}
	char* temp = nullptr;
	int size; 
	int i = 0;

	while (f.read((char*)&size, sizeof(int)))
	{

		if (temp != nullptr)
			delete[] temp;

		temp = new char[size + 1];
		f.read((char*)temp, size * sizeof(char));
		temp[size] = '\0';
		string s = temp;
		object.push_back(decode(s));

	}

	if (temp != nullptr)
		delete[] temp;

	return object;
}

//первичная загрузка слов в игру через текстовый файл
vector<string>& LoadFromTxtFile(vector<string>& words) {
	ifstream f;
	f.open("Words_Uncoded.txt");

    if (!f.is_open()) {
        throw "\nFile is not opened for reading!\n\n";
    }

	string str;

    while (!f.eof())
    {
		str = "";
		f >> str;
		words.push_back(str);
    }
	f.close();

    return words;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	mistake_counter mist_count;
	vector<string> words_library, wl2;

	try
	{
		/*LoadFromTxtFile(words_library);*/
		LoadFromBinFile(words_library);

		for (auto var : words_library)
			cout << var << "\t";

		SaveToFile(words_library);


	}
	catch (char* su)
	{
		cout << "\n\n" << su << "\n\n" << endl;
	}



	return 0;
}