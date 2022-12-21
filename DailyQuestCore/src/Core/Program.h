#pragma once


class Program
{
public:
	virtual int Run() = 0;
	virtual void Close() = 0;

	virtual bool ShouldRestart() = 0;
};

