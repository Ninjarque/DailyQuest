#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <stack>

#include <iostream>
#include <fstream>

class FileData
{
public:
	void SetString(const std::string value, const int index = 0);
	void SetDouble(const double value, const int index = 0);
	void SetInt(const int value, const int index = 0);
	void SetBool(const bool value, const int index = 0);

	std::string GetString(const int index = 0) const;
	double GetDouble(const int index = 0) const;
	int GetInt(const int index = 0) const;
	bool GetBool(const int index = 0) const;

	int GetCount() const;

	FileData& operator[](const std::string& name);
	bool Has(const std::string& name) const;

	int GetChildCount() const;

	static bool Read(FileData& data, const std::string& fileName, const std::string indent = "\t", const char listSep = ',', bool useSizes = true);
	static bool Write(const FileData& data, const std::string& fileName, const std::string indent = "\t", const char listSep = ',', bool useSizes = true);

	std::string ToString() const;

	FileData()
	{
		m_content = std::vector<std::string>();
		m_vecObjects = std::vector<std::pair<std::string, FileData&>>();
		m_mapObjects = std::unordered_map<std::string, size_t>();
	}
	~FileData()
	{
		m_content.clear();
		m_mapObjects.clear();
		for (auto property : m_vecObjects)
		{
			delete &property.second;
		}
		m_vecObjects.clear();
	}

private:
	static void FormatString(const std::string& unformatedString, const size_t sizeOffset, std::string& formatedString);
	static void ExtractLengthFromFileString(std::ifstream& file, size_t& startAt, bool useSizes, size_t& length, std::string& name, int& foundOpeningBrackets, int& foundClosingBrackets);
	static int GetSizeOfLength();

	static void SkipUntilName(std::ifstream& file, int& foundOpeningBrackets, int& foundClosingBrackets);

	static bool IsName(char c);
	static bool IsSize(char c);
	static bool IsProperty(char c);

	static const std::string Indent(const std::string& sIndent, int count)
	{
		std::string res = "";
		for (size_t i = 0; i < count; i++)
			res += sIndent;
		return res;
	}

	static void Write(const FileData& data, std::ofstream& file, std::string& tmpResult, int nIndent, const std::string sIndent, const char listSep, bool useSizes, int& size);

	std::string ToString(int indent) const;

	std::vector<std::string> m_content = std::vector<std::string>();

	std::vector<std::pair<std::string, FileData&>> m_vecObjects = std::vector<std::pair<std::string, FileData&>>();
	std::unordered_map<std::string, size_t> m_mapObjects = std::unordered_map<std::string, size_t>();
};

