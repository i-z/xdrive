 //
//  main.cpp
//  PddParser
//
//  Created by Igor Zubkov on 04/10/15.
//  Copyright (c) 2015 Igor Zubkov. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define TEST_TOKEN "Билет"
#define NUMBER_TOKEN '#' // №
#define QUESTION_TOKEN "Вопрос"
#define CHOICE_TOKEN1 "Варианты"
#define CHOICE_TOKEN2 "ответа:"
#define RIGHT_ANSWER1 "Правильный"
#define RIGHT_ANSWER2 "ответ:"

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
#define ROOT_NODE "Exam"
#define TEST_NODE "Test"
#define QUESTION_NODE "Question"
#define ANSWER_NODE "Answer"

#define CHOICES "Choices"
#define CHOICE "Choice"

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

int main(int argc, const char * argv[]) {
	
	std::ifstream inputFile;
	std::ofstream outputFile;
	
	inputFile.open("Data/tests.txt");
	outputFile.open("tests.xml");
	
	if (inputFile.fail()) {
		return 1;
	}
	
	if (outputFile.fail()) {
		return 1;
	}
	
	std::string s;
	std::stringstream out;
	
	out << XML_HEADER << '\n';
	out << '<' << ROOT_NODE << ">\n";
	
	outputFile << out.str();
	std::cout << out.str();
	
	out.str("");
	
	bool testNodeOpen = false;
	bool questionNodeOpen = false;
	bool answerNodeOpen = false;
	
	bool contin = false;
	
	do{
		
		if (s == TEST_TOKEN) {
			
			if (answerNodeOpen) {
				out << "</" << ANSWER_NODE<< ">\n";
				answerNodeOpen = false;
			}
			
			if (questionNodeOpen) {
				out << "\t\t</" << QUESTION_NODE << ">\n";
				questionNodeOpen = false;
			}
			
			if (testNodeOpen) {
				out << "\t</" << TEST_NODE << ">\n";
				testNodeOpen = false;
			}
			
			out << "\t<" << TEST_NODE;
			inputFile >> s;
			if (s[0] == NUMBER_TOKEN) {
				std::string ids = s.substr(1, s.length() - 1);
				long id = strtol(ids.c_str(), nullptr, 10);
				out << " ID=\"" << id << "\">\n";
				testNodeOpen = true;
			}
		}else if (s == QUESTION_TOKEN)
		{
			
			if (answerNodeOpen) {
				out << "</" << ANSWER_NODE<< ">\n";
				answerNodeOpen = false;
			}
			
			if (questionNodeOpen) {
				out << "\t\t</" << QUESTION_NODE << ">\n";
				questionNodeOpen = false;
			}
			
			out << "\t\t<" << QUESTION_NODE;
			inputFile >> s;
			
			
			if (s[0] == NUMBER_TOKEN) {
				std::string ids = s.substr(1, s.length() - 1);
				long id = strtol(ids.c_str(), nullptr, 10);
				out << " ID=\"" <<  id << "\" Text=\"";
				questionNodeOpen = true;
			}
		}else if(s == CHOICE_TOKEN1){
			inputFile >>s;
			if (s == CHOICE_TOKEN2) {
				out << "\">\n";
				out << "\t\t\t<" << CHOICES << ">\n";
				
				char ch;
				bool end = false;
				std::stringstream token;
				std::stringstream choicestream;
				std::vector<std::string> choices;
				
				do{

					do{
						ch = inputFile.get();
						if (ch == EOF) {
							end = true;
							break;
						}
						if (std::isspace(ch)) {
							if(token.str() == RIGHT_ANSWER1 || token.str() == QUESTION_TOKEN)
							{
								s = token.str();
								end = true;
								contin = true;
								break;
							}
							token.str("");
						}
						else
						{
							token << ch;
						}
						choicestream << ch;
					}while (ch != '\n');
					
						if (!end)
						{
							std::string choice = choicestream.str();
							trim(choice);
							
							if (!choice.empty()) {
								choices.push_back(choice);
						}
						token.str("");
						choicestream.str("");
					}
					
				}while(!end);
				
				long id;
				std::string txt;
				
				if (!choices.empty()) {
					for (std::vector<std::string>::const_iterator it = choices.cbegin(); it != choices.cend(); it++) {
						id = strtol(it->substr(0,1).c_str(), nullptr, 10);
						if ((*it)[2] == ' ') {
							txt = it->substr(3, it->length() - 3);
						}else{
							txt = it->substr(2, it->length() - 2);
						}
						
						out << "\t\t\t\t<" << CHOICE << " ID=\"" << id << "\" Text=\"" << txt << "\" />\n";
					}
				}
				
				out << "\t\t\t</" << CHOICES << ">\n";
				
				if (contin) {
					contin = false;
					continue;
				}
			}
		}
		else if(s == RIGHT_ANSWER1)
		{
			inputFile >> s;
			if(s == RIGHT_ANSWER2)
			{
				out << "\t\t\t<" << ANSWER_NODE << '>';
				answerNodeOpen = true;
			}
		}
		else{
			out << s << ' ';
		}
		
		outputFile << out.str();
		std::cout << out.str();
		out.str("");
		
		inputFile >> s;
	}while (!inputFile.eof());
	
	
	
	if (answerNodeOpen) {
		out << "</" << ANSWER_NODE<< ">\n";
		answerNodeOpen = false;
	}
	
	if (questionNodeOpen) {
		out << "\t\t</" << QUESTION_NODE << ">\n";
	}
	
	if (testNodeOpen) {
		out << "\t</" << TEST_NODE << ">\n";
	}
	
	out << "</" << ROOT_NODE << ">\n";
	
	
	outputFile << out.str();
	std::cout << out.str();
	out.str("");
	
	
	
	outputFile.close();
	inputFile.close();
	
	return 0;
}
