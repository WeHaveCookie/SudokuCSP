#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

class Reader
{
    public:
        Reader(const std::string& filePath);
        virtual ~Reader();

    protected:
        void open(const std::string& filePath);
        bool endFile();
        virtual void read()=0;
        void nextChar();
        std::string nextWord();
        void skipSeparator();

        inline void close() {m_file.close();}

        std::fstream m_file;
        std::string m_filePath;
        char m_curChar;
        std::string m_curWord;
    private:

};

class IntegerReader : public Reader {
public:

	IntegerReader(const std::string& filePath) : Reader(filePath){}

	virtual void read() {}
	int nextInt();

private:
};