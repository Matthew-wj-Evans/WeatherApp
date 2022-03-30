#ifndef RESPONSE_CPP
#define RESPONSE_CPP

#define HEADER_STATUS_STRING "HTTP/1.1"
#define HEADER_STATUS_SIZE 8
#define HEADER_LENGTH_STRING "Content-Length: "
#define HEADER_LENGTH_SIZE 14 
#define HEADER_LINES_COUNT 11

#define HEADER_HTTP_STATUS 0
#define HEADER_SERVER 1
#define HEADER_DATE 2
#define HEADER_CONTENT_TYPE 3
#define HEADER_CONTENT_LENGTH 4
#define HEADER_CONNECTION 5
#define HEADER_CACHE_KEY 6
#define HEADER_ACCESS_ALLOW_ORIGIN 7
#define HEADER_ACCESS_ALLOW_CREDENTIALS 8
#define HEADER_ACCESS_ALLOW_METHODS 9
#define HEADER_JSON_DATA 10

#define DEBUG_FILE_RESPONSE "./debug/response/"

#include "./../classes/Weather.hpp"
#include "./../classes/Response.hpp"
#include <vector>
#include <locale>
#include <codecvt>
#include <string>
using namespace std;

vector<std::wstring> ProccessResponseToWsVector(vector<char> response, int* jsonStart);
int GetEndOfCurrentLineIndex(std::vector<char> response, int indexStart);
Response GetResponseObject(std::vector<char> response);
vector<char> GetLine(int lineStartIndex, int lineEndIndex, vector<char> response);

void SetStatus(Response* response, std::wstring);
void SetTesting(Response* response, std::wstring);
int SetContentLength(Response* response, std::wstring);
void SetJSON(Response* responseObj, std::vector<char> response);

bool PrintToFile(Response response, string fileName);
bool PrintToFile(vector<char> response, string fileName);
bool PrintToFile(int response, string fileName);
bool PrintToFile(wchar_t* line, string fileName);
void PrintToFile(std::vector<std::wstring> content, string fileName);

Response GetResponseObject(vector<char> response) 
{
    Response responseObj;
    int lineStartIndex = 0;
    int lineEndIndex = GetEndOfCurrentLineIndex(response, lineStartIndex);
    int jsonStart = -1;
    bool validLine = false;

    // Break the vector<char> into a two dimentional vector of wstrings for easier manipulation
    vector<std::wstring> headerLines = ProccessResponseToWsVector(response, &jsonStart);
    responseObj.jsonStart = jsonStart;

    if (jsonStart == -1) {
        PrintToFile(new wchar_t[]{L"Error, jsonStart has not been set"}, "error_dump");    
    } else {
      SetStatus(&responseObj, headerLines.at(HEADER_HTTP_STATUS));
      if (SetContentLength(&responseObj, headerLines.at(HEADER_CONTENT_LENGTH)) > 0) {
          SetJSON(&responseObj, response);
      } else {
        // Returned no content.
        responseObj.contentLength = -1;   
      }
    }
    PrintToFile(headerLines, "header_lines_vec");
    PrintToFile(response, "response_dump_output");
    PrintToFile(responseObj, "response_obj_output");
    return responseObj;
}

/*
    Returns the index offset from the start position to the current end of line character.
*/
int GetEndOfCurrentLineIndex(vector<char> response, int indexStart) 
{
    int index = indexStart;
    do {
        index++;
    } while (response.at(index) != '\r');
    
    return index;
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

/*
Takes the vector<char> of the entire response and offloads
into a vector<std::wstring> for easier manipulation.
*/
vector<std::wstring> ProccessResponseToWsVector(vector<char> response, int* jsonStart)
{
    vector<std::wstring> header(HEADER_LINES_COUNT);
    int lineStartIndex = 0;
    int lineEndIndex = GetEndOfCurrentLineIndex(response, lineStartIndex);

    // Don't want to handle iterating throught the JSON characters in this for-loop as I don't have the content length yet
    //  hence offsetting the loop-condition by one.
    for (int headerIndex = 0; headerIndex < HEADER_LINES_COUNT - 1; headerIndex++)
    {
        std::wstring line = L"";
        for (int index = lineStartIndex; index <= lineEndIndex; index++)
        {
            line += response.at(index);
        }
        header.at(headerIndex) = line;
        if (headerIndex != 9)
        {
            lineStartIndex = lineEndIndex + 2; // Skip past the two /r and one /n characters
            lineEndIndex = GetEndOfCurrentLineIndex(response, lineStartIndex);
        }
    }
    *jsonStart = lineEndIndex + 4;
    return header;
}

void SetStatus(Response* response, std::wstring line) 
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

void SetTesting(Response* response, std::wstring line)
{
    response->testing = wstring(line.begin(), line.end());
}

int SetContentLength(Response* response, std::wstring contentHeader)
{
    int contentLength = -1;
    int index = strlen(HEADER_LENGTH_STRING);
    int contentLengthSize = 0;
    for (int headerIndex = index; headerIndex < contentHeader.length(); headerIndex++)
    {
        contentLengthSize++;
    }
    std::vector<char> cLength(contentLengthSize);
    for (int cLengthIndex = 0; cLengthIndex < contentLengthSize; cLengthIndex++)
    {
        cLength.at(cLengthIndex) = contentHeader.at(cLengthIndex + index);
    }
    contentLength = atoi(cLength.data());
    PrintToFile(contentLength, "content_len");
    response->contentLength = contentLength;
    return contentLength;
}

void SetJSON(Response* responseObj, std::vector<char> response)
{
    int jsonStart = responseObj->jsonStart;
    int jsonSize = responseObj->contentLength;
    std::vector<char> json(jsonSize);
    
    for (int index = 0; index < jsonSize; index++)
    {
        json.at(index) = response.at(index + jsonStart);
    }
    responseObj->JSON.assign(json.begin(), json.begin() + json.size());
}


// Debugging methods
bool PrintToFile(Response response, string fileName)
{
    
    string OutputfilePath = DEBUG_FILE_RESPONSE + fileName + ".txt";
    /* Response object strings are wstring, std::wofstream is needed else hex is output */
    std::wofstream file;
    file.open(OutputfilePath);
    bool operationResult = false;

    if (!file.is_open()) {
        cerr << "Could not open file" << endl;
    } else {
        file << response.statusCode << ',' << response.statusMessage << ',' << response.jsonStart << ',' <<  response.contentLength << '\n';
        for (int index = 0; index < response.JSON.size(); index++) {
            file << response.JSON.at(index);
        }

        operationResult = true;
    }
    file.close();
    return operationResult;
}

bool PrintToFile(vector<char> response, string fileName)
{
    string OutputfilePath = DEBUG_FILE_RESPONSE + fileName + ".txt";
    ofstream file;
    file.open(OutputfilePath);
    bool operationResult = false;

    if (!file.is_open()) {
        cerr << "Could not open file" << endl;
    } else {
        file << response.data();
        operationResult = true;
    }
    file.close();
    return operationResult;
}

bool PrintToFile(int response, string fileName)
{
    string OutputfilePath = DEBUG_FILE_RESPONSE + fileName + ".txt";
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

bool PrintToFile(wchar_t* line, string fileName)
{
    string OutputfilePath = DEBUG_FILE_RESPONSE + fileName + ".txt";
    std::wofstream file;
    file.open(OutputfilePath);
    bool operationResult = false;

    if (!file.is_open()) {
        cerr << "Could not open file" << endl;
    } else {
        file << line;
        operationResult = true;
    }
    file.close();
    return operationResult;
}

void PrintToFile(std::vector<std::wstring> content, string fileName)
{
    string OutputfilePath = "./debug/response/" + fileName + ".txt";
    std:wofstream file;
    file.open(OutputfilePath);

    if (file.is_open())
    {
        for (int index = 0; index < content.size(); index++)
        {
            file << content.at(index);
        }
        file << endl;
    }
    file.close();
}
#endif