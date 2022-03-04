class Response {
    public: 
        int statusCode;
        int contentLength;
        int jsonStart;
        std::wstring statusMessage;
        std::vector<char> JSON;
        std::wstring testing;
};