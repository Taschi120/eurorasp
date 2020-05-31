#include "Config.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>

#define SEPARATOR "="

using namespace std;

Config::Config(std::string filename)
{
    this->filename = filename;

    filesystem::path path(filename);
    if (!filesystem::exists(path)) {
        cout << "Configuration file at " << filename << "does not exist. Creating it." << endl;
        filesystem::create_directories(path.parent_path());
        ofstream out (filename);
        if (out.is_open()) {
            out << endl;
            out.close();
        } else {
            cerr << "Could not open configuration file at " << filename << "for initial writing!" << endl;
        }
    }

    ifstream in (filename);
    string line;
    if (in.is_open()) {
        while(getline(in, line)) {
            int separator = line.find_first_of(SEPARATOR);
            if (separator != string::npos) {
                string name = line.substr(0, separator);
                string value = line.substr(separator + 1, string::npos);

                if (content.find(name) == content.end()) {
                    content[name] = value; // TODO trim
                } else {
                    cerr << "Ignoring duplicate config entry: " << name << endl;
                }

            } else {
                cout << "Skipping invalid or empty line: " << line << endl;
            }
        }
        cout << "Configuration file read" << endl;
    } else {
        cerr << "Could not read config file at \"" << filename << "\"" << endl;
    }
}

Config::~Config()
{

}

void Config::save()
{
    ofstream out (filename);
    if (out.is_open()) {
        for (unordered_map<string, string>::iterator it = content.begin(); it != content.end(); ++it) {
            out << it->first << SEPARATOR << it->second << endl;
        }

        cout << "Configuration saved." << endl;
    } else {
        cerr << "Could not write configuration to " << filename << endl;
    }
}


string Config::getString(std::string name)
{
    return content[name];
}

long Config::getInt(std::string name)
{
    return 0;
}

double Config::getDouble(std::string name)
{
    return 0.0;
}

bool Config::getBoolean(std::string name)
{
    return false;
}

void Config::put(std::string name, std::string value)
{
    content[name] = value;
}

void Config::put(std::string name, long value)
{

}

void Config::put(std::string name, double value)
{

}

void Config::put(std::string name, bool value)
{

}
