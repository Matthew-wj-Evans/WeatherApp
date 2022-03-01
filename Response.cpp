#ifndef RESPONSE_CPP
#define RESPONSE_CPP

#define HEADER_STATUS_STRING "HTTP/1.1"
#define HEADER_STATUS_SIZE 8
#define HEADER_LENGTH_STRING "Content-Length"
#define HEADER_LENGTH_SIZE 14

#include "./poco/Weather.hpp"
#include "./poco/Response.hpp"

int GetEndOfLineIndexOffset(vector<char> response, int indexStart);
Response GetResponseObject(vector<char> response);
void SetStatus(Response* response, char* line, int beginning, int end);

Response GetResponseObject(vector<char> response) {
    Response responseObj;
    int lineStartIndex = 0;
    int lineEndIndex = GetEndOfLineIndexOffset(response, lineStartIndex);
    
    // First header line; HTTTP/1.1 ...
    char line[(lineEndIndex - lineStartIndex) + 1] = {'\0'};

    for (int index = lineStartIndex; index < lineEndIndex; index++) {
        line[index] = response[lineStartIndex + index];
    }
    SetStatus(&responseObj, line, lineStartIndex, lineEndIndex);
    responseObj.statusMessage = line;
    return responseObj;
}

/*
    Returns the index offset from the start position to the current end of line character.
*/
int GetEndOfLineIndexOffset(vector<char> response, int indexStart) {
    int index = indexStart;

    do {
        index++;
    } while (response[index] != '\n');
    
    return index - indexStart;
}

void SetStatus(Response* response, char* line, int beginning, int end) {
    // Example : HTTP/1.1 200 Ok
    int indexOffset = beginning + HEADER_STATUS_SIZE + 2; // + 2 to force the index to a non-space character
    // Get the error code length
    int index = indexOffset;
    int errorCodeLength = 0;
    int statusMessageLength = 0;


}
#endif