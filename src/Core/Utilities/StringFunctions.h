#ifndef STRINGFUNCTIONS_H
#define STRINGFUNCTIONS_H

#include <string>
#include <vector>
#include "../Math/Vector.h"


inline std::vector<std::string> Split(std::string str, std::string delimiter) {
    std::vector<std::string> words;
    size_t pos = 0;
    std::string word;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        word = str.substr(0, pos);
        words.push_back(word);
        str.erase(0, pos + delimiter.length());
    }
    words.push_back(str);
    return words;
}


template<typename ComponentType, size_t Dimensions>
inline  Vector<ComponentType, Dimensions> StringToVector(std::string str, int firstWordIndex = 0, std::string delimiter = " ") {
    std::vector<std::string> words = Split(str, delimiter);
    ComponentType components[Dimensions];
    for(int i=0; i < Dimensions; i++) {
        components[i] = std::stod(words[i + firstWordIndex]);
    }

    return Vector<ComponentType, Dimensions>(components);
}


inline bool StartsWith(std::string str, std::string prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}


inline bool EndsWith(std::string str, std::string suffix) {
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

#endif