#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <stack>

#include <iostream>
#include <fstream>

enum DataType
{
	Size,
	Property,
	Equal,
	Quote,
	Semicolon,
	OpeningBracket,
	ClosingBracket,
	EndOfFile,
};

class Data
{
#define ON_WINDOWS 0
#define ON_LINUX 0
#ifdef _WIN32
#define ON_WINDOWS 1
#endif
#ifdef __unix__
#define ON_LINUX 1
#endif
public:
	Data()
	{
		m_content = std::vector<std::string>();
		m_vecObjects = std::vector<std::pair<std::string, Data&>>();
		m_mapObjects = std::unordered_map<std::string, size_t>();
	}
	~Data()
	{
		m_content.clear();
		m_mapObjects.clear();
		for (auto property : m_vecObjects)
		{
			delete& property.second;
		}
		m_vecObjects.clear();
	}

	void SetString(const std::string value, const int index = 0);
	void SetDouble(const double value, const int index = 0);
	void SetInt(const int value, const int index = 0);
	void SetBool(const bool value, const int index = 0);

	std::string GetString(const int index = 0) const;
	double GetDouble(const int index = 0) const;
	int GetInt(const int index = 0) const;
	bool GetBool(const int index = 0) const;

	int GetCount() const;

	Data& operator[](const std::string& name);
	bool Has(const std::string& name) const;

	Data* GetChild(int index);
	int GetChildCount() const;

	static bool Read(Data& data, const std::string& fileName, const std::string indent = "\t", const char listSep = ',');
	static bool Write(const Data& data, const std::string& fileName, const std::string indent = "\t", const char listSep = ',', bool useSizes = true);

	std::string ToString() const;

private:
	std::vector<std::string> m_content = std::vector<std::string>();

	std::vector<std::pair<std::string, Data&>> m_vecObjects = std::vector<std::pair<std::string, Data&>>();
	std::unordered_map<std::string, size_t> m_mapObjects = std::unordered_map<std::string, size_t>();

	static void Read(std::ifstream& file, Data& data, const std::string indent = "\t", const char listSep = ',');

	static void Write(const Data& data, std::ofstream& file, std::string& tmpResult, int nIndent, const std::string sIndent, const char listSep, bool useSizes, int& size);

	
	static void FormatString(const std::string& unformatedString, const size_t sizeOffset, std::string& formatedString)
	{
		unsigned long length = unformatedString.size() + GetSizeOfLength() + sizeOffset;
		char tx_data[9];
		sprintf_s(tx_data, "%08lX", length);
		formatedString = tx_data + unformatedString;
	}
	static const std::string Indent(const std::string& sIndent, int count)
	{
		std::string res = "";
		for (size_t i = 0; i < count; i++)
			res += sIndent;
		return res;
	}

	static void ExtractInfo(std::ifstream& file, std::string& info, DataType& type);
	static bool IsText(char c)
	{
		return std::isalnum(c) || c == '_' || c == '-' || c == '[' || c == ']' || c == '.';
	}
	static bool IsSize(char c)
	{
		switch (c)
		{
		case 'A': return true;
		case 'B': return true;
		case 'C': return true;
		case 'D': return true;
		case 'E': return true;
		case 'F': return true;
		}
		return std::isdigit(c);
	}
	static int GetSizeOfLength()
	{
		return 8;
	}


	std::string ToString(int indent) const;
};

