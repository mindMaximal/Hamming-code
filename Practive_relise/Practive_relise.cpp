/*
	Pracite.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
	https://habr.com/ru/post/140611/
	http://forum.codenet.ru/q38614/ - Пример
	https://decodeit.ru/binary - бинарный декодер/кодер
	https://code-live.ru/forum/cpp/167/ - меню]
	https://generator-online.com/text/ - генератор текста
*/

#include <bitset>
#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <cstdio>

using namespace std;

int prompt_menu_item()
{
	// Выбранный вариант
	int variant;
	cout << "Введите размер текстового блока: \n" << endl;
	cout << "1. 8\n"
		<< "2. 12\n"
		<< "3. 16\n"
		<< "4. 24\n"
		<< "5. 32\n"
		<< "6. 48\n"
		<< "7. 64\n"
		<< "8. выйти\n" << endl;
	cout << ">>> ";
	cin >> variant;
	switch (variant)
	{
	case 1:
		variant = 8;
		break;
	case 2:
		variant = 12;
		break;
	case 3:
		variant = 16;
		break;
	case 4:
		variant = 24;
		break;
	case 5:
		variant = 32;
		break;
	case 6:
		variant = 48;
		break;
	case 7:
		variant = 64;
		break;
	case 8:
		variant = 0;
		break;
	default:
		break;
	}
	return variant;
}

int	prompt_menu_main()
{
	int variant;
	system("cls");
	cout << "Выберите действие: " << endl;
	cout << "1. Зашифровать файл\n"
		<< "2. Расшифровать файл\n"
		<< "3. Выйти\n" << endl;
	cout << ">>> ";
	cin >> variant;
	switch (variant)
	{
	case 1:
		variant = 1;
		break;
	case 2:
		variant = 2;
		break;
	case 3:
		variant = 3;
	default:
		break;
	}
	return variant;
}

int encryption(string file_name_default_in, string file_name_default_out)
{
	string line, text;
	int i;
	int sum = 0;
	int block_size = prompt_menu_item();
	int bz = block_size;
	int size = 0;
	while (bz >>= 1) size++;
	vector<int> degrees;


	ifstream file;
	ofstream out;
	out.open("text_encrypted_temp.txt");
	//Обработка файла
	file.open(file_name_default_in);
	if (file.is_open())
	{

		//Создание массива со степени двойки
		for (i = 0; i <= size; i++)
		{
			int degree = pow(2, i);
			degrees.push_back(degree);
		}

		//Обработка меню
		if (block_size == 0)
		{
			cout << "Выход из программы..." << endl;
			exit(EXIT_SUCCESS);
		}
		else if (block_size != 8 && block_size != 12 && block_size != 16 && block_size != 24 && block_size != 32 && block_size != 48 && block_size != 64)
		{
			cerr << "Вы выбрали неверный вариант" << endl;
			exit(EXIT_SUCCESS);
		}

		while (!file.eof())
		{
			//Построковое считывание
			getline(file, line);
			i = 0;
			//Посимвольный перевод в двоичный код и запись в файл
			for (char simbol : line)
			{
				bitset<8> bs(simbol);
				out << bs.to_string();
			}
		}

		i = 0;
		line = "";

		file.close();
		file.clear();
		file.open("text_encrypted_temp.txt ");

		out.close();
		out.clear();
		out.open(file_name_default_out);

		//Обработка текста
		while (!file.eof())
		{
			getline(file, text);
			for (char simbol : text)
			{
				i++;
				line += simbol;

				if (i % block_size == 0)
				{
					//cout << "Line (default): " << line << endl;
					for (int d = 0; d <= size; d++)
					{
						//Установка контрольних битов
						line.insert(degrees[d] - 1, "0");
					}
					//cout << "Line (control bits = 0): " << line << endl;
					for (int d = 0; d <= size; d++)
					{
						//cout << "Size: " << line.size() << endl;
						//cout << "-------------------" << endl << "degrees[d]: " << degrees[d] << endl;

						for (int k = degrees[d] - 1; k < line.size(); k = k + degrees[d] + 1)
						{
							//Счетчик кол-ва символов, пройденных следующим циклом for
							int f = 0;
							for (int j = k; j <= k + degrees[d] - 1; j++)
							{
								if (j >= line.size())
								{
									break;
								}
								//Если это не контрольный бит, то считаем сумму
								if (find(degrees.begin(), degrees.end(), j + 1) == degrees.end())
								{
									//Элемент не найден в массиве
									char c = line[j];
									sum += c - '0';
								}
								f++;
							}
							k += f - 1;
						}
						if (sum % 2 == 0)
						{
							//четное
							sum = 0;
						}
						else
						{
							//нечетное
							sum = 1;
						}
						line[degrees[d] - 1] = sum + '0';
						sum = 0;
					}
					out << line << endl;
					//cout << "Line (after): " << line << endl;
					line = "";
				}
			}
		}
		cout << "Файл зашифрован." << endl;
		file.close();
		file.clear();
		remove("text_encrypted_temp.txt");
		return 1;
	}
	else
	{
		cout << "Не удается открыть файл" << endl;
		return 0;
	}

	out.close();
	out.clear();
}

int decoding(string file_name_default_in, string file_name_default_out)
{
	vector<int> degrees;
	string line, text;
	int i = 0;

	ifstream file;
	ofstream out;
	out.open(file_name_default_out);
	//Обработка файла
	file.open(file_name_default_in);
	if (file.is_open())
	{

		//Обработка текста
		while (!file.eof())
		{
			getline(file, line);
			//Проверка на пустую строку
			if (line.length() > 0)
			{
				//Создание массива со степени двойки при первом проходе цикла
				if (i == 0)
				{
					float size_q = log(line.length()) / log(2);
					size_q = floor(size_q);
					int size = (int)size_q;
					for (int d = 0; d <= size; d++)
					{
						int degree = pow(2, d);
						degrees.push_back(degree);
					}
					//test
					//line = "010011011010"; //11-n misstake
				}
				i++;

				for (int d = 0; d < degrees.size(); d++)
				{
					//cout << "\n\nSize: " << line.size() << endl;
					//cout << "-------------------" << endl << "degrees[d]: " << degrees[d] << endl;
					int sum = 0;
					for (int k = degrees[d] - 1; k < line.size(); k = k + degrees[d] + 1)
					{
						//Счетчик кол-ва символов, пройденных следующим циклом for
						int f = 0;
						for (int j = k; j <= k + degrees[d] - 1; j++)
						{
							if (j >= line.size())
							{
								break;
							}
							//Если это не контрольный бит, то считаем сумму
							if (find(degrees.begin(), degrees.end(), j + 1) == degrees.end())
							{
								//Элемент не найден в массиве
								char c = line[j];
								sum += c - '0';
							}
							f++;
						}
						k += f - 1;
					}
					if (sum % 2 == 0)
					{
						//четное
						sum = 0;
					}
					else
					{
						//нечетное
						sum = 1;
					}
					sum = sum + '0';
					if (line[degrees[d] - 1] == sum)
					{
						cout << "Элемент [" << degrees[d] << "] совпадает" << endl;
					}
					else
					{
						cout << "Элемент [" << degrees[d] << "] не совпадает" << endl;
					}

				}

				cout << "Line [" << i << "] - " << line << endl;
			}
		}
		return 1;
	}
	else
	{
		cout << "Не удается открыть файл";
		return 0;
	}
	out.close();
	out.clear();
}

int menu()
{
	string file_name_in, file_name_out;
	int menu_res = prompt_menu_main();
	int result;
	if (menu_res == 1)
	{
		cout << "Пожалуйста, введите имя файла, который необходимо зашифровать\n"
			<< "(Введите \"d\", если хотите использовать стандартные имена)\n"
			<< "--> ";
		cin >> file_name_in;
		if (file_name_in == "d")
		{
			file_name_in = "text_original.txt";
		}
		cout << "Пожалуйста, введите имя файла, в который сохранится результат\n"
			<< "(Введите \"d\", если хотите использовать стандартные имена)\n"
			<< "--> ";
		cin >> file_name_out;
		//Default for test
		if (file_name_out == "d")
		{
			file_name_out = "text_encrypted.txt";
		}
		result = encryption(file_name_in, file_name_out);
		if (result == 1)
		{
			system("pause");
			menu();
		}
		else {
			cout << "Что-то пошло не так" << endl;
			system("pause");
			menu();
		}
	}
	else if (menu_res == 2)
	{
		cout << "Пожалуйста, введите имя файла, который необходимо расшифролвать\n"
			<< "(Введите \"d\", если хотите использовать стандартные имена)\n"
			<< "--> ";
		cin >> file_name_in;
		cout << "Пожалуйста, введите имя файла, в который сохранится результат\n"
			<< "(Введите \"d\", если хотите использовать стандартные имена)\n"
			<< "--> ";
		cin >> file_name_out;
		//Default for test
		file_name_in = "text_encrypted.txt";
		file_name_out = "text_decoded.txt";
		result = decoding(file_name_in, file_name_out);
		if (result == 1)
		{
			system("pause");
			menu();
		}
		else {
			cout << "Что-то пошло не так" << endl;
			system("pause");
			menu();
		}
	}
	else if (menu_res == 3)
	{
		cout << "Выход из программы..." << endl;
		exit(EXIT_SUCCESS);
	}
	else
	{
		cerr << "Вы выбрали неверный вариант" << endl;
		exit(EXIT_SUCCESS);
	}
}

int main()
{
	setlocale(LC_CTYPE, "rus");

	menu();

	//line = "000010000100001011101";

	//text = "";
	//line = "";
	//line_q = "";
	//file.open("text_encrypted.txt");
	//if (file.is_open())
	//{
	//	while (!file.eof())
	//	{
	//		getline(file, line);
	//		
	//		for ( i = 0; i < size; i++)
	//		{
	//			line.insert(degrees[i] - 1, "0");
	//		}

	//		text += line;
	//		text += "\n";
	//	}
	//}

	//cout << "Text: \n" << text << endl;

	////Обнуление переменных
	//text = "";
	//line = "";
	//line_q = "";

	////Обработка файла с двоичным кодом
	//file.open("text_encrypted.txt");
	//if (file.is_open())
	//{
	//	while (!file.eof())
	//	{

	//		getline(file, line);
	//		i = 0;
	//		text += line;
	//		//Посимвольный перебор
	//	}
	//	for (char simbol : text)
	//	{
	//		i++;
	//		line_q += simbol;

	//		//Перевод символа из двоичного кода
	//		if (i == 8)
	//		{
	//			i = 0;
	//			bitset<8> foo(line_q);
	//			unsigned char ch = static_cast<unsigned char> (foo.to_ulong());
	//			text += ch;
	//			line_q = "";
	//		}
	//	}
	//	file.close();
	//	file.clear();
	//}
	//cout << "File: " << text << endl;
	//system("pause");
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.