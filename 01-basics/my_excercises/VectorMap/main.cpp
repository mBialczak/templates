#include "VectorMap.hpp"

#include <cassert>
#include <iostream>

template <typename KeyType, typename ValueType>
void printVectorMap(const VectorMap<KeyType, ValueType>& vecMap)
{
    auto keys = vecMap.keys();
    auto values = vecMap.values();
    assert(keys.size() == values.size());
    for (std::size_t i = 0; i < keys.size(); ++i) {
        std::cout << "key: " << keys[i]
                  << " value: " << values[i] << "\n";
    }
}

struct nonDefaultConstructible
{
    nonDefaultConstructible() = delete;
};

void checkInsertForVectorMapOfBool(std::vector<std::pair<bool, std::string>> testExamples)
{
    std::cout << "\nChecking VectorMap<bool, std::string>::insert()\n----------------\n";
    VectorMap<bool, std::string> boolToString;
    for (auto&& [key, value] : testExamples) {

        std::pair<bool, bool> insertionResult = boolToString.insert(key, value);
        std::cout << std::boolalpha
                  << "for: " << key << ", \"" << value << "\" --> "
                  << (insertionResult.second ? " inserted\n" : " not inserted\n");
    }
}

void checkInsertRvalForVectorMapOfBool()
{
    std::cout << "\nChecking VectorMap<bool, std::string>::insert()\n----------------\n";

    VectorMap<bool, std::string> boolToString;
    boolToString.insert(true, "yes");
    boolToString.insert(false, "no");
    boolToString.insert(true, "tak");
    boolToString.insert(true, "nie");

    std::cout << "After insertion and reinsertion attempts,\n"
              << "value for true is: " << boolToString[true] << '\n'
              << "value for false is: " << boolToString[false] << '\n';
}

void checkOperatorInsertionAndAccessingForVectorMapOfBool()
{
    VectorMap<bool, std::string> weekendsAre;
    weekendsAre[true] = "they are cool and always awaited";
    weekendsAre[false] = "long enough :(";

    std::cout << "\nThe truth about weekends is that: " << weekendsAre[true] << '\n'
              << "And they are definitely not " << weekendsAre[false] << '\n';
}

void checkAtForVectorMapOfBool()
{
    std::cout << "\n------------\nChecking at() access on empty VectorMap<bool,char>...\n";
    VectorMap<bool, char> trueToChar;
    try {
        trueToChar.at(true);
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }
    std::cout << std::endl;

    try {
        trueToChar.at(false);
    }
    catch (const std::exception& e) {
        std::cout << e.what();
    }

    std::cout << "\n----------\nChecking at() for reading of pre-filled VectorMap<bool,std::string>...";
    VectorMap<bool, std::string> trueOrFalseOtherWay;
    trueOrFalseOtherWay[true] = "yes, of course, naturally...";
    trueOrFalseOtherWay[false] = "no, no way, impossible";
    std::cout << "\nTrue said in another way: " << trueOrFalseOtherWay.at(true) << '\n'
              << "False said in another way: " << trueOrFalseOtherWay.at(false) << '\n';

    std::cout << "\n----------\nChecking change of stored values with at()...\n";
    trueOrFalseOtherWay.at(true) = "prawda";
    trueOrFalseOtherWay.at(false) = "nieprawda";

    std::cout << "'true' in polish: " << trueOrFalseOtherWay.at(true) << '\n'
              << "'false' in polish without polish letters ;): " << trueOrFalseOtherWay.at(false) << '\n';
}

int main()
{
    VectorMap<unsigned, char> map;
    for (auto [i, val] = std::make_pair(0, 'a'); i < 4; ++i, ++val) {
        auto [key, hasInserted] = map.insert(i, val);
        std::cout << "Value " << val
                  << (hasInserted ? " inserted into map"
                                  : " not inserted (existed before)")
                  << " with key: " << key << '\n';
    }
    printVectorMap(map);

    auto [key, result] = map.insert(3, 'd');
    std::cout << "Tried to insert again with key: " << key
              << " -> insertion result: " << std::boolalpha << result << std::endl;
    printVectorMap(map);

    std::cout << "Checking operator[]. Key: " << 1 << " value: " << map[1] << '\n';
    std::cout << "Checking operator[]. Key: " << 3 << " value: " << map[3u] << '\n';
    std::cout << "Checking operator[]. Key: " << 5 << " value: " << map[5u] << '\n';

    std::cout << "Checking at(). Key: " << 1 << " value: " << map.at(1) << '\n';
    std::cout << "Checking at(). Key: " << 3 << " value: " << map.at(3) << '\n';
    try {
        std::cout << "Checking at(). Key: " << 7 << " value: " << map.at(7) << '\n';
    }
    catch (const std::out_of_range& e) {
        std::cout << e.what() << std::endl;
    }

    printVectorMap(map);
    std::cout << "==============================" << std::endl;

    VectorMap<std::string, std::string> stringMap;

    stringMap["Tim"] = "Mayers";
    stringMap["John"] = "Smith";
    printVectorMap(stringMap);

    std::cout << stringMap["Tim"] << std::endl;

    stringMap.at("John") = "Changed name!";
    printVectorMap(stringMap);

    // VectorMap<char, nonDefaultConstructible> shouldNotBuildMap;
    std::cout << "Checking isKeyInt() method\n-----------------\n"
              << std::boolalpha
              << "VectorMap<unsigned, char>.isKeyInt():" << map.isIntKey() << '\n'
              << "VectorMap<std::string, std::string>.isKeyInt(): " << stringMap.isIntKey() << '\n'
              << "VectorMap<int, std::string>.isKeyInt(): " << VectorMap<int, std::string> {}.isIntKey() << '\n';

    std::cout << "Checking is_int_key variable \n-----------------\n"
              << std::boolalpha
              << "VectorMap<unsigned, char>::is_int_key: " << VectorMap<unsigned, char>::is_int_key << '\n'
              << "VectorMap<int, std::string>::is_int_key: " << VectorMap<int, std::string>::is_int_key << '\n';

    checkInsertForVectorMapOfBool({ std::make_pair(true, "yes"),
                                    std::make_pair(false, "no"),
                                    std::make_pair(true, "tak"),
                                    std::make_pair(false, "nie") });

    checkInsertRvalForVectorMapOfBool();
    checkOperatorInsertionAndAccessingForVectorMapOfBool();

    checkAtForVectorMapOfBool();

    std::cout << "VectorMap<bool,double>::isIntKey() -> " << VectorMap<bool, double> {}.isIntKey() << '\n';
    std::cout << "VectorMap<bool,double>::is_int_key -> " << VectorMap<bool, double>::is_int_key << '\n';

    std::cout << "is_int_key_v<int> -> " << std::boolalpha << is_int_key_v<int> << '\n'
              << "is_int_key_v<double> -> " << is_int_key_v<double> << '\n';
    return 0;
}
