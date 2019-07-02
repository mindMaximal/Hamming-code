#include <iostream>
#include <string>
#include <fstream>

using namespace std;
//Получение двоичного представления символа
string get_binary(unsigned char val)
{
	int binary = 0;
	string line_temp;

	for (int i = 7; i >= 0; --i)
	{
		binary *= 10;

		if (val & (1 << i))
			binary += 1;
	}

	line_temp = to_string(binary);

	if (line_temp.length() < 8)
	{
		for (int d = line_temp.length(); d < 8; d++)
		{
			line_temp = "0" + line_temp;
		}
	}

	return line_temp;
}
//Получение размера текстового блока
int get_block_size()
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
		<< endl;
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
	default:
		break;
	}
	return variant;
}
//Вызов основного меню
int	get_main_menu()
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
//Декодирование файла
int encryption(string file_name_default_in, string file_name_default_out)
{
	string line;
	int i;
	int sum = 0;
	int block_size = get_block_size();
	int bz = block_size;
	int size = 0;
	while (bz >>= 1) size++;
	int* p_darr = new int[size];

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
			p_darr[i] = degree;
		}

		//Обработка меню
		if (block_size != 8 && block_size != 12 && block_size != 16 && block_size != 24 && block_size != 32 && block_size != 48 && block_size != 64)
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
				if ( block_size == 12 )
				{
					i++;

					if ( i % 3 == 0 )
					{
						out << get_binary(simbol) << endl;
					}
					else if ( i % 3 == 1 )
					{
						out << get_binary(simbol);
					} 
					else if ( i % 3 == 2 )
					{
						string str_t = get_binary(simbol);
						out << str_t.substr(0, 4) << endl << str_t.substr(4, 4);
					}
				}
				else
				{
					i += 8;
					out << get_binary(simbol);
					if (i % block_size == 0)
					{
						out << endl;
					}
				}
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
			getline(file, line);
			if ( line.length() != 0 )
			{	
				if ( (int)line.length() < block_size )
				{
					for (int d = line.length(); d < block_size; d++)
					{
						line += "0";
					}
				}
				for (int d = 0; d <= size; d++)
				{
					//Установка контрольних битов
					line.insert(p_darr[d] - 1, "0");
				}
				for (int d = 0; d <= size; d++)
				{
					for (int k = p_darr[d] - 1; k < (int)line.size(); k = k + p_darr[d] + 1)
					{
						//Счетчик кол-ва символов, пройденных следующим циклом for
						int f = 0;
						for (int j = k; j <= k + p_darr[d] - 1; j++)
						{
							if (j >= (int)line.size())
							{
								break;
							} 
							//Если это не контрольный бит, то считаем сумму
							for (i = 0; i < size; i++) {
								if (p_darr[i] == j + 1 ) {
									break;
								}
							}
							if (i >= size )
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
					line[p_darr[d] - 1] = sum + '0';
					sum = 0;
				}
				out << line << endl;
				line = "";
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
//Кодирование файла
int decoding(string file_name_default_in, string file_name_default_out)
{
	int p_darr[7] = { 1, 2, 4, 8, 16, 32, 64 };
	string line, text, line_q;
	int i = 0;
	int size = 0;
	int misstake_count = 0;
	int line_count = 0;

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
				if (line_count == 0)
				{
					float size_q = log(line.length()) / log(2);
					size_q = floor(size_q);
					size = (int)size_q + 1;
				}

				//Если длина строки совпадает с степенью 2, то не считаем последний бит, как контрольный
				for (i = 0; i < size; i++) {
					if ( p_darr[i] == line.length() ) {
						break;
					}
				}
				if (i < size)
				{
					//Элемент найден в массиве
					size--;
				}

				line_count++;
				int misstake_pos = 0;
				for (int d = 0; d < size; d++)
				{
					int sum = 0;
		
					for (int k = p_darr[d] - 1; k < (int)line.size(); k = k + p_darr[d] + 1)
					{
						//Счетчик кол-ва символов, пройденных следующим циклом for
						int f = 0;
						for (int j = k; j <= k + p_darr[d] - 1; j++)
						{
							if (j >= (int)line.size())
							{
								break;
							}
							//Если это не контрольный бит, то считаем сумму
							for (i = 0; i < size; i++) {
								if (p_darr[i] == j + 1) {
									break;
								}
							}
							if (i >= size)
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
					if (line[p_darr[d] - 1] != sum) misstake_pos += p_darr[d];

				}

				//Исправление ошибок
				if ( misstake_pos != 0)
				{
					cout << "Найдена ошибка, в строке " << line_count << " ее позиция - " << misstake_pos << endl;
					int misstake = line[misstake_pos - 1] - '0';
					if (misstake == 1)
					{
						line[misstake_pos - 1] = 0 + '0';
					}
					else if (misstake == 0)
					{
						line[misstake_pos - 1] = 1 + '0';
					}
					misstake_count++;
				}
				//Удаление контрольных битов
				for (int d = size - 1; d >= 0; d--)
				{
					line.erase(p_darr[d] - 1, 1);
				}
				text += line;
			}
		}
		line = "";
		i = 0;
		for (char simbol : text)
		{
			i++;
			line_q += simbol;
			//Перевод символа из двоичного кода
			if (i == 8)
			{
				int val = 0;
				for (char sim : line_q)
				{
					i--;
					int sim_i = sim - '0';
					val += pow(2, i) * sim_i;
				}
				char ch = static_cast<char>(val);
				line += ch;
				line_q = "";
			}
		}
		cout << "\n- - - - - - - - - - - - - - - - -\n" 
			<< "Количество исправленных ошибок: "
			<< misstake_count 
			<< endl 
			<< endl;
		out << line << endl;
		cout << "Текст: \n \n"
			<< line
			<< endl;
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
//Вызов вспомогательного меню
int get_menu()
{
	string file_name_in, file_name_out;
	int menu_res = get_main_menu();
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
		if (file_name_out == "d")
		{
			file_name_out = "text_encrypted.txt";
		}
		result = encryption(file_name_in, file_name_out);
		if (result == 1)
		{
			cout << endl;
			system("pause");
			get_menu();
		}
		else {
			cout << "Что-то пошло не так" << endl;
			cout << endl;
			system("pause");
			get_menu();
		}
	}
	else if (menu_res == 2)
	{
		cout << "Пожалуйста, введите имя файла, который необходимо расшифролвать\n"
			<< "(Введите \"d\", если хотите использовать стандартные имена)\n"
			<< "--> ";
		cin >> file_name_in;
		if (file_name_in == "d")
		{
			file_name_in = "text_encrypted.txt";
		}
		cout << "Пожалуйста, введите имя файла, в который сохранится результат\n"
			<< "(Введите \"d\", если хотите использовать стандартные имена)\n"
			<< "--> ";
		cin >> file_name_out;
		if (file_name_out == "d")
		{
			file_name_out = "text_decoded.txt";
		}
		result = decoding(file_name_in, file_name_out);
		if (result == 1)
		{
			system("pause");
			get_menu();
		}
		else {
			cout << "Что-то пошло не так" << endl;
			system("pause");
			get_menu();
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
	get_menu();
	return 0;
}
