#ifndef NETWORKAPI_CPP
#define NETWORKAPI_CPP
#include <bits/stdc++.h>

/*
    This class is responsible for getting and returning HTTP requests
    all data returned will be 
*/
class NetworkAPI {
        wchar_t _base_path[128];
        wchar_t _lat_arg[128];
        wchar_t _prefix_lon_arg[128];
        wchar_t _app_id_arg[128];
    public:
        wchar_t* GetURL();
        bool SetURL(wchar_t[], wchar_t[], wchar_t[], wchar_t[]);
} network;

#endif