#ifndef WEATHER_HPP
#define WEATHER_HPP

using namespace std;

class Coordinate {
    public:
        float longitude;
        float latitude;
};

class Detail {
    public:
        int id;
        std::string main;
        std::string description;
        std::string icon;
};

class Main {
    public:
        float temperature;
        float feelsLike;
        float tempMin;
        float tempMax;
        int pressure;
        int humidity;
};

class Wind {
    public:
        float speed;
        int degree;
        float gust;
};

class Cloud {
    public:
        int all;
};

class System {
    public:
        int type;
        int id;
        std::string country;
        int sunrise;
        int sunset;
};

class Weather {
    public:
        Coordinate coordinate;
        Detail detail;
        string base;
        Main main;
        int visibility;
        Wind wind;
        Cloud clouds;
        long dt;
        System system;
        int timezone;
        int id;
        string timezoneString;
        string name;
        int cod;
};

#endif