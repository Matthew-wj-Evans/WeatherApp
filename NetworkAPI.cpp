#ifndef NETWORKAPI_CPP
#define NETWORKAPI_CPP
#include <bits/stdc++.h>

/*
    This class is responsible for getting and returning HTTP requests
    all data returned will be 
*/
class NetworkAPI {      
    public:
        const wchar_t DEFAULT_BASE_URL[41]={L"api.openweathermap.org/data/2.5/weather?"};
        // some place in the US
        const wchar_t DEFAULT_LAT[11]=L"lat=37.39&";
        const wchar_t DEFAULT_LON[13]=L"lon=-122.08&";
        // Default Constructor
        NetworkAPI()
        { 
            wchar_t path[] = {*DEFAULT_BASE_URL};
            _base_path = path;

            wchar_t lat[] = {*DEFAULT_LAT};
            _lat_arg = lat;

            wchar_t lon[] = {*DEFAULT_LON};
            _lon_arg = lon;

            
        }
        // Custom URL Constructor
        NetworkAPI(const wchar_t basePath[],const wchar_t latitude[],const wchar_t longitude[],const wchar_t apiKey[])
        {}
        wchar_t GetURL();
        bool SetURL(wchar_t[], wchar_t[], wchar_t[], wchar_t[]);
    private:
        wchar_t* _base_path;
        wchar_t* _lat_arg;
        wchar_t* _lon_arg;
        wchar_t* api_key_arg;
} network;

#endif