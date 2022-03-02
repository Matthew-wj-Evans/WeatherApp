#ifndef WEATHER_HPP
#define WEATHER_HPP

using namespace std;

class Coordinates {
    public:
        float longitude;
        float latitude;
};

class Details {
    public:
        int id;
        std::string main;
        std::string description;
        std::string icon;
};

class MainWeather {
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
        Coordinates coordinates;
        MainWeather weather;
        string base;
        MainWeather main;
        int visibility;
        Wind wind;
        int dt;
        System system;
        int timezone;
        int id;
        string timezoneString;
        int cod;
};

#endif