#include "streamscanner.h"
StreamScanner::StreamScanner(){}

StreamScanner::~StreamScanner(){
	if(file.is_open())
		file.close();
	}

bool StreamScanner::openFile(std::string filename){
	file.open(filename.c_str(), std::ios::binary);

	if(!file.is_open())
		return false; ///@note Should throw an exception

	file.seekg(0, std::ios::beg);
	return true;
	}

int StreamScanner::invalidState(){
	return file.fail();
	}

void StreamScanner::prepareNextLine(int offset){
	if (fileType == Encoding::ASCII){
		std::string lineBuffer;
		std::getline(file, lineBuffer);
		}
	else {
		file.ignore(offset);
		}
	}

void StreamScanner::setCurrentEncoding(Encoding current){
	fileType = current;
	}

Encoding StreamScanner::getCurrentEncoding(){
	return fileType;
	}

void StreamScanner::closeFile(){
	if (file.is_open()){
		file.close();
		}
	}