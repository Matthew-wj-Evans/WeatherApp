#ifndef MapJsonToWeather
#define MapJsonToWeather
#include "./poco/Weather.hpp"
#include "JsonProcessor.cpp"
#include <string>
using namespace std;


Weather MapWeatherObject(string responseJson);

Weather MapWeatherObject(string responseJson)
{
    JsonProcess jProcess{responseJson};
    JsonObject jsonObject;
    KeyValue keyValue;
    Weather weatherData;
    string value;
    
    jProcess.GetJsonObject("coord", &jsonObject);
    //jsonObject currently holds Coords data
    Coordinate coords{0};
    value = jsonObject.GetValueAtKey("lon");
    coords.longitude = value == "" ? 0.00 : stof(value);
    value = jsonObject.GetValueAtKey("lat");
    coords.latitude = value == "" ? 0.00 : stof(value);
    weatherData.coordinate = coords;
    jsonObject.values.clear();

    jProcess.GetJsonObject("weather", &jsonObject);
    Detail details;
    value = jsonObject.GetValueAtKey("id");
    details.id = value == "" ? 0 : stoi(value);

    details.main = jsonObject.GetValueAtKey("main");
    details.description = jsonObject.GetValueAtKey("description");
    details.icon = jsonObject.GetValueAtKey("icon");
    weatherData.detail = details;
    jsonObject.values.clear();

    jProcess.GetKeyValuePair("base", &keyValue);
    weatherData.base = keyValue.value;

    jProcess.GetJsonObject("main", &jsonObject);
    Main mainWeather;
    value = jsonObject.GetValueAtKey("temp");
    mainWeather.temperature = value == "" ? 0.00 : stof(value);

    value = jsonObject.GetValueAtKey("feels_like");
    mainWeather.feelsLike = value == "" ? 0.00 : stof(value);

    value = jsonObject.GetValueAtKey("temp_min");
    mainWeather.tempMin = value == "" ? 0.00: stof(value);

    value = jsonObject.GetValueAtKey("temp_max");
    mainWeather.tempMax = value == ""? 0.00 : stof(value);

    value = jsonObject.GetValueAtKey("pressure");
    mainWeather.pressure = value == "" ? 0.00 : stof(value);

    value = jsonObject.GetValueAtKey("humidity");
    mainWeather.humidity = value == "" ? 0.00 : stof(value);

    weatherData.main = mainWeather;
    jsonObject.values.clear();

    jProcess.GetKeyValuePair("visibility", &keyValue);
    value = keyValue.value;
    weatherData.visibility = value == "" ? 0 : stoi(value);

    jProcess.GetJsonObject("wind", &jsonObject);
    Wind wind;
    value = jsonObject.GetValueAtKey("speed");
    wind.speed = value == "" ? 0.00 : stof(value);

    value = jsonObject.GetValueAtKey("deg");
    wind.degree = value == "" ? 0 : stoi(value);

    value = jsonObject.GetValueAtKey("gust");
    wind.gust = value == "" ? 0.00 : stof(value);

    weatherData.wind = wind;
    jsonObject.values.clear();

    jProcess.GetJsonObject("clouds", &jsonObject);
    Cloud cloud;
    value = jsonObject.GetValueAtKey("all");
    cloud.all = value == "" ? 0 : stoi(value);

    weatherData.clouds = cloud;
    jsonObject.values.clear();

    jProcess.GetKeyValuePair("dt", &keyValue);
    weatherData.dt = keyValue.value == "" ? 0 : stoi(keyValue.value);

    jProcess.GetJsonObject("sys", &jsonObject);
    System sys;
    value = jsonObject.GetValueAtKey("type");
    sys.type = value == "" ? 0 : stoi(value);

    value = jsonObject.GetValueAtKey("id");
    sys.id = value == "" ? 0 : stoi(value);

    sys.country = jsonObject.GetValueAtKey("country");

    value = jsonObject.GetValueAtKey("sunrise");
    sys.sunrise = value == "" ? 0 : stoi(value);

    value = jsonObject.GetValueAtKey("sunset");
    sys.sunset = value == "" ? 0 : stoi(value);
    weatherData.system = sys;
    jsonObject.values.clear();

    jProcess.GetKeyValuePair("timezone", &keyValue);
    weatherData.timezone = keyValue.value == "" ? 0 : stoi(keyValue.value);

    jProcess.GetKeyValuePair("id", &keyValue);
    weatherData.id = keyValue.value == "" ? 0 : stoi(keyValue.value);

    jProcess.GetKeyValuePair("name", &keyValue);
    weatherData.name = keyValue.value;

    jProcess.GetKeyValuePair("cod", &keyValue);
    weatherData.cod = keyValue.value == "" ? 0 : stoi(keyValue.value);

    return weatherData;
}
#endif