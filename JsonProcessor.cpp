#ifndef JsonProcessor
#define JsonProcessor
#include <string.h>
#include <vector>
#include "Weather.hpp"
using namespace std;

#define QUOTE '\"'
#define OPEN_BRACKET '{'
#define CLOSE_BRACKET '}'
#define COMMA ','
#define COLON ':'

class KeyValue
{
public:
  string key;
  string value;
};

class JsonArray
{
public:
  string key;
  vector<string> values;
};

class JsonObject
{
public:
  string GetValueAtKey(string key);
  string key;
  vector<KeyValue> values;
};

string JsonObject::GetValueAtKey(string key)
{
  string value = "";
  for (int index = 0; index < values.size(); index++)
  {
    string currentKey = values.at(index).key;
    if (currentKey.compare(key) == 0)
    {

      value = values.at(index).value;
      index = values.size();
    }
  }

  return value;
}

class JsonProcess
{
public:
  JsonProcess();
  JsonProcess(string json);
  string GetValue(string key);
  int GetJsonObject(string key, JsonObject *jsonObject);
  int GetKeyValuePair(string key, KeyValue *keyValue);
  Weather GetWeather();

private:
  string m_json;
  int GetKeyCount();
  string GetQuoteString(int index);
  string GetValue(int index);
  KeyValue ParseKeyValue(string jsonSubstring);
};

JsonProcess::JsonProcess() {}

JsonProcess::JsonProcess(string json)
{
  this->m_json = json;
}

int JsonProcess::GetKeyCount()
{
  int count = 0;
  bool innerOpenBracket = false;
  for (int index = 1; index < this->m_json.size() - 1; index++)
  {
    if (this->m_json.at(index) == '{')
    {
      innerOpenBracket = true;
    }
    else if (this->m_json.at(index) == '}')
    {
      innerOpenBracket = false;
    }

    if (!innerOpenBracket && this->m_json.at(index) == ',')
    {
      count++;
    }
  }
  return count + 1;
}

int JsonProcess::GetJsonObject(string key, JsonObject *jsonObject)
{
  int status = -1;
  int jsonLength = m_json.length();
  bool outerOpenQuote = false;
  string constructedKey = "";
  int keyIndex = -1;
  int substringStartIndex = -1;
  int substringLength = 0;

  // First, find the key, if it exists; offsetting values by one to skip encompasing brackets
  for (int index = 1; index < m_json.size(); index++)
  {
    if (m_json.at(index) == QUOTE)
    {
      outerOpenQuote = true;

      int innerIndex = index + 1; // push index past first quote
      while (outerOpenQuote)
      {
        if (m_json.at(innerIndex) == QUOTE)
        {
          outerOpenQuote = false;
        }
        else
        {
          constructedKey += m_json.at(innerIndex);
          innerIndex++;
        }

      } // end while
      if (constructedKey.compare(key) == 0)
      {
        status = 1;
        // Key has been found within the json
        jsonObject->key = constructedKey;
        // Get the length of the substring
        int subJsonLength = 0;
        bool innerOpenBracket = false;
        // Get the starting index of the json
        keyIndex = index;
        index = m_json.size();
      }
      else
      {
        status = 0;
        constructedKey = "";
      }

    } // end if
  }   // end for

  // Have the key index, need to find the {...} portion and create a substring
  if (keyIndex > 0)
  {
    bool openBracket = false;
    while (!openBracket)
    {
      char currentChar = m_json.at(keyIndex);
      if (currentChar == OPEN_BRACKET)
      {
        openBracket = true;
        substringStartIndex = keyIndex;
      }
      else
      {
        keyIndex++;
      }
    }
    // Moved index to the beginning, now find the end/length

    bool closeBracket = false;
    while (!closeBracket)
    {
      char currentChar = m_json.at(keyIndex);
      if (currentChar == CLOSE_BRACKET)
      {
        closeBracket = true;
        substringLength = (keyIndex - substringStartIndex) + 1;
      }
      else
      {
        keyIndex++;
      }
    }
    // Have the substring length
    string jsonSubstring = m_json.substr(substringStartIndex, substringLength);
    //cout << jsonSubstring << endl;
    // determine the key locations
    vector<int> delinineatorLocation;
    int keyCount;
    for (int index = 0; index < jsonSubstring.length(); index++)
    {
      char currentChar = jsonSubstring.at(index);
      if (currentChar == COMMA || currentChar == CLOSE_BRACKET)
      {
        delinineatorLocation.push_back(index);
      }
    }
    keyCount = delinineatorLocation.size();
    // Have the key count and their locations
    vector<string> keyValueSubstrings(keyCount);
    int prevSubstring = 0;
    for (int delineator = 0; delineator < delinineatorLocation.size(); delineator++)
    {
      for (int index = prevSubstring; index < delinineatorLocation.at(delineator); index++)
      {
        char currentChar = jsonSubstring.at(index);
        if (index == delinineatorLocation.at(delineator) - 1)
        {
          prevSubstring = delinineatorLocation.at(delineator);
        }
        if (currentChar != OPEN_BRACKET && currentChar != CLOSE_BRACKET && currentChar != COMMA)
        {
          keyValueSubstrings.at(delineator) += jsonSubstring.at(index);
        }
      }
    }
    vector<KeyValue> keyValuePairs;
    for (int index = 0; index < keyValueSubstrings.size(); index++)
    {
      keyValuePairs.push_back(ParseKeyValue(keyValueSubstrings.at(index)));
      //cout << keyValuePairs.at(index).key << " " << keyValuePairs.at(index).value << endl;
    }

    jsonObject->key = constructedKey;
    for( int index = 0; index < keyValuePairs.size(); index++)
    {
      jsonObject->values.push_back(keyValuePairs.at(index));
    }
  }
  return status;
}

int JsonProcess::GetKeyValuePair(string key, KeyValue *keyValue)
{
  bool locatingKey = true;
  bool openBracket = false;
  bool openQuote = false;
  string constructedKey = "";
  string value = "";

  for (int index = 1; index < m_json.size() - 1; index++)
  {
    if (m_json.at(index) == OPEN_BRACKET && locatingKey)
    {
      openBracket = true;
      constructedKey = ""; // Clear the key, it's not a straight key-value pair
      value = "";
    }
    else if (m_json.at(index) == CLOSE_BRACKET)
    {
      openBracket = false;
    }

    if (!openBracket)
    {
      if (m_json.at(index) == QUOTE)
      {
        if (openQuote)
        {
          openQuote = false; // Flip the quote flag, it's a closing quote
        }
        else
        {
          openQuote = true; // Flip the quote flag, it's an opening quote
          if (locatingKey)
          {
            constructedKey = GetQuoteString(index);
            if (constructedKey.compare(key) == 0)
            {
              // Found the key, get the value next
              value = GetValue(index);
              index = m_json.size();
            }
          }
        }
      }
    }
  }

  keyValue->key = constructedKey;
  keyValue->value = value;
  return 1;
}

string JsonProcess::GetQuoteString(int index)
{
  string substring = "";
  bool closingQuote = false;

  while (!closingQuote)
  {
    index++; // This method is called at a quote character
    if (m_json.at(index) == QUOTE)
    {
      closingQuote = true;
    }
    else
    {
      substring += m_json.at(index);
    }
  }
  return substring;
}

string JsonProcess::GetValue(int index)
{
  string value = "";
  bool openQuote = true;
  bool foundClosing = false;
  bool foundColon = false;
  bool closingQuote = false;
  char currentChar = '\0';

  while (!foundColon)
  {
    if (m_json.at(index) != COLON)
    {
      index++;
    }
    else
    {
      foundColon = true;
    }
  }
  index++; // Shift to one beyond the colon character
  while (!foundClosing)
  {
    currentChar = m_json.at(index);
    if (currentChar == COMMA || currentChar == CLOSE_BRACKET || (currentChar == QUOTE && closingQuote))
    {
      // Found the terminating character
      foundClosing = true;
    }
    else
    {
      if (currentChar == QUOTE && !closingQuote)
      {
        // We're handling a string
        openQuote == true;
      }
      else if (currentChar == QUOTE && openQuote)
      {
        openQuote = false;
        closingQuote = true;
      }

      if (currentChar != QUOTE)
      {
        value += currentChar;
      }
      index++;
    } // End (currentChar == COMMA || currentChar == CLOSE_BRACKET || (currentChar == QUOTE && closingQuote))

  } // End while
  return value;
}

KeyValue JsonProcess::ParseKeyValue(string jsonSubstring)
{
  KeyValue keyValue;
  string key = "";
  string value = "";
  int delineatorIndex = 0;

  for (int index = 0; index < jsonSubstring.length(); index++)
  {
    char currentChar = jsonSubstring.at(index);
    if (currentChar != QUOTE && currentChar != COLON)
    {
      key += currentChar;
    }
    if (currentChar == COLON)
    {
      delineatorIndex = index;
      index = jsonSubstring.length();
    }
  }
  
  for (int index = delineatorIndex; index < jsonSubstring.length(); index++)
  {
    char currentChar = jsonSubstring.at(index);

    if (currentChar != QUOTE && currentChar != COLON && currentChar != CLOSE_BRACKET && currentChar != COMMA)
    {
      value+= currentChar;
    }
  }

  keyValue.key = key;
  keyValue.value = value;

  return keyValue;
}
#endif