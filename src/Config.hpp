#ifndef __INCLUDE_CONFIG_HPP_
#define __INCLUDE_CONFIG_HPP_

#include <string>
#include <unordered_map>

#define CFG_KEY_SAVED_MIDI_INTERFACE "midi.interface.saved"

class Config
{
    public:
        Config(std::string filename);
        virtual ~Config();

        std::string getString(std::string name);
        long getInt(std::string name);
        double getDouble(std::string name);
        bool getBoolean(std::string name);

        void put(std::string name, std::string value);
        void put(std::string name, long value);
        void put(std::string name, double value);
        void put(std::string name, bool value);

        void save();

    protected:

    private:
        std::unordered_map<std::string, std::string> content;
        std::string filename;

};

#endif // __INCLUDE_CONFIG_HPP_
