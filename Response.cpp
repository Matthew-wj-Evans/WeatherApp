#ifndef RESPONSE_CPP
#define RESPONSE_CPP

#define HEADER_STATUS_STRING "HTTP/1.1"
#define HEADER_STATUS_SIZE 8
#define HEADER_LENGTH_STRING "Content-Length"
#define HEADER_LENGTH_SIZE 14

#include "./poco/Weather.hpp"
#include "./poco/Response.hpp"
#include <vector>
#include <locale>
#include <codecvt>
#include <string>
using namespace std;

int GetEndOfLineIndexOffset(std::vector<char> response, int indexStart);
Response GetResponseObject(std::vector<char> response);
vector<char> GetLine(int lineStartIndex, int lineEndIndex, vector<char> response);
void SetStatus(Response* response, vector<char> line);
void SetTesting(Response* response, vector<char> line);

bool PrintToFile(Response response, string fileName);
bool PrintToFile(vector<char> response, string fileName);
bool PrintToFile(int response, string fileName);

Response GetResponseObject(vector<char> response) 
{
    PrintToFile(response, "response_dump_output");
    Response responseObj;
    int lineStartIndex = 0;
    int lineEndIndex = GetEndOfLineIndexOffset(response, lineStartIndex);
    
    // First header line; HTTTP/1.1 ...
    vector<char>line = GetLine(lineStartIndex, lineEndIndex, response);
    SetStatus(&responseObj, line);
    
    // Second Header Line; Server: ...
    lineStartIndex = lineEndIndex + 2; // move to the next line
    lineEndIndex = GetEndOfLineIndexOffset(response, lineStartIndex);
    line = GetLine(lineStartIndex, lineEndIndex, response);
    SetTesting(&responseObj, line);

    PrintToFile(responseObj, "response_obj_output");
    return responseObj;
}

/*
    Returns the index offset from the start position to the current end of line character.
*/
int GetEndOfLineIndexOffset(vector<char> response, int indexStart) 
{
    int index = indexStart;
    do {
        index++;
    } while (response.at(index) != '\n');
    
    return index - indexStart;
}

vector<char> GetLine(int lineStartIndex, int lineEndIndex, vector<char> response) 
{
    int vectorSize = (lineEndIndex - lineStartIndex) + 1;
    vector<char> line(vectorSize, 0);

    for (int index = lineStartIndex; index < lineEndIndex; index++) {
        line.at(index) = response.at(lineStartIndex + index);
    }
    return line;
}

void SetStatus(Response* response, vector<char> line) 
{
    // Example : HTTP/1.1 200 Ok
    int indexOffset = HEADER_STATUS_SIZE + 1; // + 2 to force the index to a non-space character
    // Get the error code length
    int errorCodeLength = 0;
    int statusMessageLength = 0;

    // Get the length until the next space char
    for (int index = indexOffset; line.at(index) != ' '; index++ ) {
        errorCodeLength++;
    }

    vector<char> errorCode(errorCodeLength + 1, 0);

    for (int index = 0; index <  errorCodeLength; index++) {
        errorCode.at(index) = line.at(index + indexOffset);
    }    

    indexOffset = indexOffset + errorCodeLength + 1; // +1 to push index to non-space char
    
    // Remaining line is the status code.
    for (int index = indexOffset; line.at(index) != '\r' ; index++) {
        statusMessageLength++;
    }

    vector<char> statusMessage(statusMessageLength + 1, 0);

    for (int index = 0; index < statusMessageLength; index++) {
        statusMessage.at(index) = line.at(index + indexOffset);
    }

    response->statusCode = atoi(errorCode.data());

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wide = converter.from_bytes(statusMessage.data());
    response->statusMessage = wide;
}

void SetTesting(Response* response, vector<char> line)
{
    response->testing = wstring(line.begin(), line.end());
}

bool PrintToFile(Response response, string fileName)
{
    string OutputfilePath = "./donotpush/" + fileName + ".txt";
    ofstream file;
    file.open(OutputfilePath);
    bool operationResult = false;

    if (!file.is_open()) {
        cerr << "Could not open file" << endl;
    } else {
        file << response.statusCode << "\n" << response.statusMessage.c_str() << "\n" <<  response.testing.data() << endl;
        operationResult = true;
    }
    file.close();
    return operationResult;
}

bool PrintToFile(vector<char> response, string fileName)
{
    string OutputfilePath = "./donotpush/" + fileName + ".txt";
    ofstream file;
    file.open(OutputfilePath);
    bool operationResult = false;

    if (!file.is_open()) {
        cerr << "Could not open file" << endl;
    } else {
        file << response.data() << endl;
        operationResult = true;
    }
    file.close();
    return operationResult;
}

bool PrintToFile(int response, string fileName)
{
    string OutputfilePath = "./donotpush/" + fileName + ".txt";
    ofstream file;
    file.open(OutputfilePath);
    bool operationResult = false;

    if (!file.is_open()) {
        cerr << "Could not open file" << endl;
    } else {
        file << response << endl;
        operationResult = true;
    }
    file.close();
    return operationResult;
}
#endif