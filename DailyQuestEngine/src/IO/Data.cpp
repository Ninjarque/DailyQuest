#include "Data.h"

void Data::SetString(const std::string value, const int index)
{
	if (m_content.size() <= index)
		m_content.resize(index + 1);
	m_content[index] = value;
}

void Data::SetDouble(const double value, const int index)
{
	SetString(std::to_string(value), index);
}

void Data::SetInt(const int value, const int index)
{
	SetString(std::to_string(value), index);
}

void Data::SetBool(const bool value, const int index)
{
	SetString((value ? "TRUE" : "FALSE"), index);
}

std::string Data::GetString(const int index) const
{
	if (index < 0 || index >= m_content.size())
		return "";
	return m_content[index];
}

double Data::GetDouble(const int index) const
{
	std::string str = GetString(index);
	return std::atof(str.c_str());
}

int Data::GetInt(const int index) const
{
	std::string str = GetString(index);
	return std::atoi(str.c_str());
}

bool Data::GetBool(const int index) const
{
	std::string str = GetString(index);
	return std::strcmp("TRUE", str.c_str()) == 0;
}

int Data::GetCount() const
{
	return m_content.size();
}

Data& Data::operator[](const std::string& name)
{
	if (m_mapObjects.count(name) == 0)
	{
		m_mapObjects[name] = m_vecObjects.size();
		Data* d = new Data();
		m_vecObjects.push_back({ name, *d });
	}
	return m_vecObjects[m_mapObjects[name]].second;
}

bool Data::Has(const std::string& name) const
{
	return m_mapObjects.count(name) != 0;
}

int Data::GetChildCount() const
{
	return m_vecObjects.size();
}

bool Data::Read(Data& data, const std::string& fileName, const std::string indent, const char listSep)
{
	std::ifstream file(fileName, std::ios::binary);
	if (file.is_open())
	{
		Read(file, data, indent, listSep);
		file.close();
		return true;
	}
	return false;
}

void Data::Read(std::ifstream& file, Data& data, const std::string indent, const char listSep)
{
	std::size_t size = 0;
	std::string prop = "";
	std::size_t prop_pos = 0;
	std::size_t size_pos = 0;

	bool stop = false;
	std::string equal_property = "";
	bool started_equal = false;
	bool equal_done = false;
	int index = 0;
	char lst_c = '\0';
	int quoteStep = 0;

	bool skip = false;
	int skipIndent = 0;

	bool startedEmpty = data.GetChildCount() == 0;
	while (!file.eof() && !stop)
	{
		std::string info;
		DataType type;
		ExtractInfo(file, info, type);
		if (skip)
		{
			if (type == DataType::OpeningBracket) skipIndent++;
			if (type == DataType::ClosingBracket) skipIndent--;
			if (skipIndent == 0)
				skip = false;
			continue;
		}
		switch (type)
		{
		case DataType::Size:
			size = std::stol(info, nullptr, 16);
			size_pos = file.tellg();
			size_pos -= GetSizeOfLength();
			break;
		case DataType::Property:
			prop = info;
			prop_pos = file.tellg();
			break;
		case DataType::Equal:
			equal_done = false;
			equal_property = "";
			started_equal = false;
			quoteStep = 0;
			lst_c = '\0';
			index = 0;
			while (!equal_done && !file.eof())
			{
				char c;
				file.get(c);
				if (IsText(c) || c == '"' || c == listSep || c == ';') started_equal = true;
				if (started_equal)
				{
					if (c == '"' && lst_c != '\\')
					{
						equal_property += c;
						quoteStep++;
						if (quoteStep == 2)
						{
							quoteStep = 0;
							started_equal = false;
						}
						continue;
					}
					if (quoteStep > 0)
					{
						equal_property += c;
						continue;
					}
					if (c == ';')
					{
						if (startedEmpty || data.Has(prop))
							data[prop].SetString(equal_property, index);
						equal_property = "";
						equal_done = true;
					}
					else if (c == listSep)
					{
						if (startedEmpty || data.Has(prop))
							data[prop].SetString(equal_property, index);
						equal_property = "";
						started_equal = false;
						index++;
					}
					else
						equal_property += c;
				}
				lst_c = c;
			}
			break;
		case DataType::Semicolon:
			break;
		case DataType::OpeningBracket:
			if (startedEmpty || data.Has(prop))
			{
				Data& d = data[prop];
				Read(file, d, indent, listSep);
			}
			else if (size > 0)
			{
				file.seekg(size_pos + size, file.beg);
			}
			else
			{
				skip = true;
				skipIndent = 1;
			}
			break;
		case DataType::ClosingBracket:
			stop = true;
			break;
			//return;
		}
	}
}

void Data::ExtractInfo(std::ifstream& file, std::string& data, DataType& type)
{
	bool done = false;
	bool isText = false;
	char lst_c = '\0';
	while (!done && !file.eof())
	{
		char c;
		if (!file.get(c))
			break;

		switch (c)
		{
		case '=':
		{
			data = "=";
			type = DataType::Equal;
			done = true;
			break;
		}
		case '"':
		{
			if (lst_c == '\\')
				break;
			data = "=";
			type = DataType::Quote;
			done = true;
			break;
		}
		case ';':
		{
			data = "=";
			type = DataType::Semicolon;
			done = true;
			break;
		}
		case '{':
		{
			data = "{";
			type = DataType::OpeningBracket;
			done = true;
			break;
		}
		case '}':
		{
			data = "}";
			type = DataType::ClosingBracket;
			done = true;
			break;
		}
		default:
		{
			if (IsText(c))
			{
				data = "";
				isText = true;
				done = true;
			}
			break;
		}
		}

		lst_c = c;
	}

	if (isText)
	{
		char c = lst_c;
		bool haveSize = true;
		while (!file.eof() && IsText(c))
		{
			data += c;
			if (!IsSize(c) && data.size() <= GetSizeOfLength())
				haveSize = false;
			if (haveSize && data.size() == GetSizeOfLength())
				break;

			file.get(c);
			lst_c = c;
		}
		if (haveSize)
		{
			type = DataType::Size;
		}
		else
		{
			file.seekg(-1, file.cur);
			type = DataType::Property;
		}
	}
}


bool Data::Write(const Data& data, const std::string& fileName, const std::string sIndent, const char listSep, bool useSizes)
{
	std::ofstream file(fileName);
	if (file.is_open())
	{
		int size;
		std::string res;
		Write(data, file, res, 0, sIndent, listSep, useSizes, size);

		file.close();
		return true;
	}

	return false;
}

void Data::Write(const Data& data, std::ofstream& file, std::string& tmpResult, int nIndent, const std::string sIndent, const char listSep, bool useSizes, int& size)
{
	std::string listSperator = std::string(1, listSep) + " ";
	size = 0;
	tmpResult = "";
	for (auto const& property : data.m_vecObjects)
	{
		if (property.second.GetChildCount() == 0)
		{
			std::string dtResult = "";
			const std::string name = property.first;
			//file << name << " = ";

			size_t nItems = property.second.GetCount();
			for (size_t i = 0; i < property.second.GetCount(); i++)
			{
				std::string prop = property.second.GetString(i);
				prop = prop.erase(0, prop.find_first_not_of('"'));
				prop = prop.erase(prop.find_last_not_of('"') + 1);
				size_t x = property.second.GetString(i).find_first_of(listSep);
				std::string variable = "";
				//Has , and not defined as text explicitly, surround it
				if (x != std::string::npos)
				{
					//file << "\"" << property.second.GetString(i) << "\"" << ((nItems > 1) ? listSperator : "");
					variable = "\"" + prop + "\"" + ((nItems > 1) ? listSperator : ";");
				}
				else
				{
					variable = prop + ((nItems > 1) ? listSperator : ";");
				}
				//std::string formatedVariable = variable;
				//if (useSizes)
				//	FormatString(variable, 0, formatedVariable);
				dtResult += variable; // formatedVariable;

				nItems--;
			}

			dtResult = name + "=" + dtResult;
			std::string formatedResult = dtResult;
			if (useSizes)
				FormatString(dtResult, 0, formatedResult);
			dtResult = Indent(sIndent, nIndent) + formatedResult + "\n";

			size += dtResult.size();
			tmpResult += dtResult;
			//std::cout << tmpResult << " with indent " << nIndent << std::endl;

		}
		else
		{
			std::string dtResult = "";
			std::string name = property.first;
			dtResult += Indent(sIndent, nIndent) + "{\n";

			int nextSize;
			std::string nextTmpResult = "";
			Write(property.second, file, nextTmpResult, nIndent + 1, sIndent, listSep, useSizes, nextSize);
			//size += nextSize;

			//std::cout << "from : [" << name << "] next res : " << nextTmpResult << std::endl;

			dtResult += nextTmpResult + Indent(sIndent, nIndent) + "}\n";

			dtResult = name + "\n" + dtResult;
			std::string formatedName = dtResult;
			int lineCount = 0;

			if (ON_WINDOWS)
			{
				auto v = std::count(dtResult.begin(), dtResult.end(), '\n');
				lineCount += v - 1;
			}
			
			if (useSizes)
				FormatString(dtResult, lineCount, formatedName);

			dtResult = Indent(sIndent, nIndent) + formatedName;

			size += dtResult.size();

			tmpResult += dtResult;
			//std::cout << tmpResult << " with indent " << nIndent << std::endl;
		}

		if (nIndent == 0)
		{
			file << tmpResult;
			tmpResult = "";
		}
	}
}

std::string Data::ToString() const
{
	return ToString(0);
}

std::string Data::ToString(int indent) const
{
	std::string res = "";
	std::string sIndent = "";// indent != 0 ? "  " : "";
	int ind = indent;
	while (ind > 0)
	{
		ind--;
		sIndent += "\t";
	}
	for (int i = 0; i < m_vecObjects.size(); i++)
	{
		res += sIndent + m_vecObjects[i].first + "\n";
		res += sIndent + "{\n";
		res += m_vecObjects[i].second.ToString(indent + 1) + "\n";
		res += sIndent + "}" + "\n";
	}
	if (m_content.size() > 0)
	{
		res += sIndent + m_content[0];
		for (int i = 1; i < m_content.size(); i++)
		{
			res += ", " + m_content[i];
		}
	}

	return res;
}