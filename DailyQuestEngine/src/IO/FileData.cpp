#include "FileData.h"

void FileData::SetString(const std::string value, const int index)
{
	if (m_content.size() <= index)
		m_content.resize(index + 1);
	m_content[index] = value;
}

void FileData::SetDouble(const double value, const int index)
{
	SetString(std::to_string(value), index);
}

void FileData::SetInt(const int value, const int index)
{
	SetString(std::to_string(value), index);
}

void FileData::SetBool(const bool value, const int index)
{
	SetString((value ? "TRUE" : "FALSE"), index);
}

std::string FileData::GetString(const int index) const
{
	if (index < 0 || index >= m_content.size())
		return "";
	return m_content[index];
}

double FileData::GetDouble(const int index) const
{
	std::string str = GetString(index);
	return std::atof(str.c_str());
}

int FileData::GetInt(const int index) const
{
	std::string str = GetString(index);
	return std::atoi(str.c_str());
}

bool FileData::GetBool(const int index) const
{
	std::string str = GetString(index);
	return std::strcmp("TRUE", str.c_str()) == 0;
}

int FileData::GetCount() const
{
	return m_content.size();
}

FileData& FileData::operator[](const std::string& name)
{
	if (m_mapObjects.count(name) == 0)
	{
		m_mapObjects[name] = m_vecObjects.size();
		FileData* d = new FileData();
		m_vecObjects.push_back({ name, *d });
	}
	return m_vecObjects[m_mapObjects[name]].second;
}

bool FileData::Has(const std::string& name) const
{
	return m_mapObjects.count(name) != 0;
}

int FileData::GetChildCount() const
{
	return m_vecObjects.size();
}

bool FileData::Read(FileData& data, const std::string& fileName, const std::string indent, const char listSep, bool useSizes)
{
	//Could read using a prefilled data (data["scene1"]["obj1"], data["scene1"]["obj2"]) 
	//so that it would limit memory usage by skipping over huge chunks of data, but only if specified

	//TWO PROBLEMS
	//Creates code inside code instead of filling
	//Can't go back in it's reccursion

	std::ifstream file(fileName, std::ios::binary);
	if (file.is_open())
	{
		std::string propertyName = "";
		std::string propertyValue = "";

		std::stack<std::reference_wrapper<FileData>> stackPath;
		std::stack<std::string> stackName;
		std::stack<int> stackNameID;
		std::map<FileData*, size_t> fileDataMaxPoses;
		std::map<FileData*, int> fileDataChildCount;
		stackPath.push(data);
		stackName.push("_ROOT_");
		stackNameID.push(0);

		int lastNameID = 0;
		std::stack<int> stackIndentNameID;

		size_t curPos = 0;

		size_t indent = 0;

		int nameIDCount = 0;

		while (!file.eof())
		{
			FileData& data = stackPath.top().get();
			std::string dataName = stackName.top();
			//could have errors due to ongoing modification implying not empty child count
			if (fileDataChildCount.count(&data) == 0)
			{
				fileDataChildCount[&data] = data.GetChildCount();
			}
			int childCount = fileDataChildCount[&data];
			if (childCount > 0)
			{
				size_t length;
				std::string name;
				//might be wrong dude
				curPos = file.tellg();
				
				if (useSizes && fileDataMaxPoses.count(&data))
				{
					size_t maxPos = fileDataMaxPoses[&data];
					if (curPos >= maxPos)
					{
						stackPath.pop();
						stackName.pop();
						stackNameID.pop();
						continue;
					}
				}
				int foundOpeningBrackets = 0;
				int foundClosingBrackets = 0;

				ExtractLengthFromFileString(file, curPos, useSizes, length, name, foundOpeningBrackets, foundClosingBrackets);

				int nameID = -1;
				if (name != "")
				{
					nameIDCount++;
					nameID = nameIDCount;
				}

				/*
				if (foundOpeningBrackets == 0 && foundClosingBrackets == 0 && stackIndentName.size() > 0)
				{
					stackIndentName.pop();
					stackIndentName.push(lastName);
				}
				*/

				int stackFirst = 0;
				int stackSecond = 0;
				if (stackIndentNameID.size() >= 2)
				{
					stackFirst = stackIndentNameID.top();
					stackIndentNameID.pop();
					stackSecond = stackIndentNameID.top();
					stackIndentNameID.push(stackFirst);
				}

				//PROBLEM
				//STRING == STRING
				//BUT SHOULD STILL SEE DIFFERENCE BETWEEN STRING POINTERS
				if (foundClosingBrackets > 0 
					//Tries to reduce error by only allowing a stack pop if known names
					&& stackSecond == stackNameID.top()) //&& !useSizes)
				{
					indent--;
					if (stackPath.size() == 1)
					{
						length = -1;
						break;
					}
					stackPath.pop();
					stackName.pop();
					stackNameID.pop();
					stackIndentNameID.pop();
					//data = stackPath.top().get();
					//dataName = stackName.top();
					foundClosingBrackets--;
					continue;
				}
				if (foundClosingBrackets > 0)
				{
					indent--;
					stackIndentNameID.pop();
					continue;
				}

				if ((useSizes && length <= 0) || length == -1)
					break;

				if (!useSizes && foundOpeningBrackets > 0)
				{
					//indent being added one too many times here at child2 of type cube (the last one)
					indent += foundOpeningBrackets;
					if (stackIndentNameID.size() > 0)
					{
						stackIndentNameID.pop();
						stackIndentNameID.push(lastNameID);
					}
					else if (stackIndentNameID.size() == 0 && lastNameID != -1)
						stackIndentNameID.push(lastNameID);

					//stackIndentName.push(lastName);
					stackIndentNameID.push(nameID);
					//length = 1;
				}
				if (stackIndentNameID.size() >= 2)
				{
					stackFirst = stackIndentNameID.top();
					stackIndentNameID.pop();
					stackSecond = stackIndentNameID.top();
					stackIndentNameID.push(stackFirst);
				}

				//indent++;
				int stackSize = stackIndentNameID.size();// stackPath.size() - 1;
				if (data.Has(name) && stackSecond == stackNameID.top())//(stackSize == indent))
				{
					FileData& fileData = data[name];
					fileDataMaxPoses[&fileData] = curPos + length;
					stackPath.push(fileData);
					stackName.push(name);
					stackNameID.push(nameID);
					file.seekg(-1, file.cur);
					propertyName = name;
				}
				else
				{
					//indent--;
					curPos += length;// - 1;
					file.seekg(curPos, file.beg);
				}
				lastNameID = nameID;
			}
			else
			{
				propertyName = dataName;

				bool isTarget = propertyName.size() != 0;
				std::string target = propertyName;
				bool isProperty = false;
				bool isPropertySpaceDone = false;
				bool isName = false;
				bool isNameDone = false;
				bool isChildNodeStart = false;
				bool isChildNodeEnd = false;
				bool isTextInQuotes = false;
				bool isEndl = false;
				std::string name = propertyName;
				int properties = 0;
				std::string property = "";
				bool removedSize = !useSizes;
				while (!file.eof()) 
				{
					FileData& curData = stackPath.top().get();
					std::string curName = stackName.top();
					char c;

					//std::cout << curData.ToString() << std::endl;
					while (file.get(c))
					{
						if (!isProperty && c == '=') { isProperty = true; isPropertySpaceDone = false; continue; }
						if (!isProperty && (isNameDone || isTarget) && c == '{') { isChildNodeStart = true; break; }
						if (!isProperty && c == '}') { isChildNodeEnd = true; break; }
						if (!isTextInQuotes && c == ';') { isEndl = true; break; }
						if (isProperty)
						{
							if (!isPropertySpaceDone && (IsProperty(c) || isEndl))
							{
								isPropertySpaceDone = true;
							}
							if (isPropertySpaceDone)
							{
								if (!isTextInQuotes && (isEndl || c == listSep))
									break;
								property += c;
								//DIS AIN'T WORKIN' RIGHT
								//quotes are getting an extra separation then it just breaks (for that line at least)
								if (property.size() == 1 && c == '"')
								{
									isTextInQuotes = true;
								}
								else if (isTextInQuotes && c == '"' && (property.size() < 2 || property[property.size() - 2] != '\\'))
								{
									isTextInQuotes = false;
									//isEndl = true;
									//break;
									continue;
								}
							}
							else if (c == ',' || isEndl)
							{
								break;
							}
							continue;
						}

						if (!isNameDone && IsName(c))
						{
							if (propertyName != "")
							{
								propertyName = "";
								name = "";
							}
							isName = true;
							name += c;
							if (!removedSize && name.size() == 8)
							{
								name = "";
								removedSize = true;
							}
							continue;
						}
						else if (isName && !isNameDone)
						{
							isNameDone = true;
							file.seekg(-1, file.cur);
							//propertyName = name;
							//break;
						}
					}

					if (isProperty)
					{
						isTarget = false;
						//read exception here if already initialized before calling read it seems, needs futher checking
						if (target == name)
							curData.SetString(property, properties);
						else
							curData[name].SetString(property, properties);
						properties++;
						property = "";

						//
						isPropertySpaceDone = false;
						//

						if (isEndl)
						{
							//indent--;

							isProperty = false;
							properties = 0;
							isEndl = false;

							isName = false;
							isNameDone = false;
							propertyName = "";
							removedSize = !useSizes;

							if (target == name)
							{
								stackPath.pop();
								stackName.pop();
								stackNameID.pop();
								break;
							}

							name = "";
						}
					}
					else if (isChildNodeStart)
					{
						//indent++;
						if (useSizes && !isTarget || target != name)//!isTarget)
						{
							stackPath.push(curData[name]);
							stackName.push(name);
							stackNameID.push(nameIDCount++);
						}
						else if (!useSizes && !isTarget)
						{
							stackPath.push(curData[name]);
							stackName.push(name);
							stackNameID.push(nameIDCount++);
						}
						isTarget = false;

						//propertyName = "";

						isProperty = false;
						isPropertySpaceDone = false;
						isName = false;
						isNameDone = false;
						isChildNodeStart = false;
						isChildNodeEnd = false;
						isTextInQuotes = false;
						isEndl = false;
						name = "";
						propertyName = "";
						removedSize = !useSizes;
						properties = 0;
						property = "";
					}
					else if (isChildNodeEnd)
					{
						//indent--;
						isTarget = false;

						isProperty = false;
						isPropertySpaceDone = false;
						isName = false;
						isNameDone = false;
						isChildNodeStart = false;
						isChildNodeEnd = false;
						isTextInQuotes = false;
						isEndl = false;
						name = "";
						propertyName = "";
						removedSize = !useSizes;
						properties = 0;
						property = "";
						// ?

						//stackPath.pop();
						//stackName.pop();
						if (stackName.top() == target)
						{
							stackPath.pop();
							stackName.pop();
							stackNameID.pop();

							break;
						}
						stackPath.pop();
						stackName.pop();
						stackNameID.pop();

						//stackPath.pop();
						//stackName.pop();
					}
					else
					{

					}
					//read stuff here
					// //check if has =
					//check if has {
					//check if has }
					//wut is dis then
				}
			}
		}


		file.close();
		return true;
	}

	return false;
}

bool FileData::Write(const FileData& data, const std::string& fileName, const std::string sIndent, const char listSep, bool useSizes)
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

void FileData::Write(const FileData& data, std::ofstream& file, std::string& tmpResult, int nIndent, const std::string sIndent, const char listSep, bool useSizes, int& size)
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
			auto v = std::count(dtResult.begin(), dtResult.end(), '\n');
			lineCount += v - 1;
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

void FileData::FormatString(const std::string& unformatedString, const size_t sizeOffset, std::string& formatedString)
{
	unsigned long length = unformatedString.size() + GetSizeOfLength() + sizeOffset;
	char tx_data[9];
	sprintf_s(tx_data, "%08lX", length);
	formatedString = tx_data + unformatedString;
}

void FileData::SkipUntilName(std::ifstream& file, int& foundOpeningBrackets, int& foundClosingBrackets)
{
	size_t startAt = file.tellg();
	bool startedAt0 = false;
	if (startAt == 0)
		startedAt0 = true;
	int readStep = 0;
	char lstC = ' ';
	bool skippedThrough = false;
	size_t ignoredCharsAt = 0;
	foundOpeningBrackets = 0;
	foundClosingBrackets = 0;
	while (!file.eof())
	{
		char c;
		if (file.get(c))
		{
			if (readStep == 0 && std::strchr(" \t\n\r\f\v", c))
			{
				skippedThrough = true;
				continue;
			}
			if (readStep != 3 && c == '}')
			{
				foundClosingBrackets++;
				break;
			}
			if (readStep == 0 && c == '{')
			{
				foundOpeningBrackets++;
			}

			switch (readStep)
			{
			case 0: 
			{ 
				if (!IsName(c))
				{
					readStep = 1;
				}
				else
				{
					if (skippedThrough && ignoredCharsAt == 0)
					{
						ignoredCharsAt = file.tellg();
						ignoredCharsAt--;
						skippedThrough = true;
						if (startedAt0)
						{
							ignoredCharsAt = startAt;
						}
					}
					break;
				}
			}
			case 1:
			{ 
				if (foundClosingBrackets > 0 && IsName(c))
				{
					readStep = 5;
					//file.seekg(-1, file.cur);
					break;
				}
				if (c == '=') readStep = 2; 
				if (c == '{')
				{
					readStep = 5;
					if (skippedThrough && (startedAt0 || ignoredCharsAt > 0))
					{
						file.seekg(ignoredCharsAt);
						if (!startedAt0)
							foundOpeningBrackets = 0;
					}
				} 
				if (c == ';')
				{
					readStep = 5;
					if (startedAt0)
					{
						file.seekg(ignoredCharsAt);
					}
				}
				if (c == '"') readStep = 3;
				break;
			}
			case 2: 
			{ if (c == '"') readStep = 3; else readStep = 4; break; }
			case 3: 
			{ 
				if (c == '"' && lstC != '\\')
				{
					readStep = 5;
					if (startedAt0)
					{
						file.seekg(ignoredCharsAt);
					}
				}
				break; 
			}
			case 4:
			{ 
				if (c == ';')
				{
					readStep = 5;
					if (startedAt0)
					{
						file.seekg(ignoredCharsAt);
					}
				}
				break; 
			}
			}
			//Final break
			if (readStep == 5 && IsName(c))
			{
				file.seekg(-1, file.cur);
				break;
			}
		}
		else
			break;
		lstC = c;
	}
}

void FileData::ExtractLengthFromFileString(std::ifstream& file, size_t& startAt, bool useSizes, size_t& length, std::string& name,
	int& foundOpeningBrackets, int& foundClosingBrackets)
{
	//std::string hex_string = "";
	//azerty file;
	file.seekg(startAt, file.beg);
	length = 0;
	char c;

	foundOpeningBrackets = 0;
	foundClosingBrackets = 0;

	if (file.eof())
	{
		length = -1;
		name = "";
		return;
	}

	foundClosingBrackets = 0;

	name = "";
	if (!useSizes)
	{
		SkipUntilName(file, foundOpeningBrackets, foundClosingBrackets);
		while (file.get(c))
		{
			if (!IsName(c))
				break;
			name += c;
		}
		length = name.size() > 0 ? 0 : -1;// name.size();//name.size();
		//file.seekg(-1, file.cur);
		startAt = file.tellg();
		return;
	}
	if (useSizes)
	{
		bool tried = false;
		while (file.get(c))
		{
			tried = true;
			if (file.eof())
			{
				length = -1;
				name = "";
				return;
			}
			if (c == '}')
			{
				length = 0;
				foundClosingBrackets = 1;
				return;
			}
			if (IsSize(c))
				break;
		}
		if (!tried || file.eof())
		{
			length = -1;
			name = "";
			return;
		}
		file.seekg(-1, file.cur);

		//test line
		startAt = file.tellg();


		char buffer[9];
		file.read(buffer, GetSizeOfLength());
		buffer[GetSizeOfLength()] = '\0';

		length = std::stol(buffer, nullptr, 16);
	}

	while (file.get(c))
	{
		if (!IsName(c))
			break;
		name += c;
	}
	//file.seekg(startAt, file.beg);
}

int FileData::GetSizeOfLength()
{
	return 8;
}

bool FileData::IsName(char c)
{
	return std::isalnum(c) || c == '_' || c == '-' || c == '[' || c == ']' || c == '.';
}

bool FileData::IsSize(char c)
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

bool FileData::IsProperty(char c)
{
	return (IsName(c) || c == '"' || c == '\'') || !(std::strchr(" \t\n\r\f\v", c));
}


std::string FileData::ToString() const
{
	return ToString(0);
}

std::string FileData::ToString(int indent) const
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
