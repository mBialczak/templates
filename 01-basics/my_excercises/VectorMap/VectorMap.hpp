#pragma once

#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

template <typename KeyType, typename ValueType>
class VectorMap
{
    static_assert(std::is_default_constructible_v<ValueType>,
                  "Value type should be default constructible");

  public:
    constexpr std::pair<KeyType, bool> insert(const KeyType& key, const ValueType& val);
    constexpr std::pair<KeyType, bool> insert(KeyType&& key, ValueType&& val);

    constexpr ValueType& operator[](const KeyType& key);
    constexpr ValueType& operator[](KeyType&&);

    constexpr const ValueType& at(const KeyType& key) const;
    constexpr ValueType& at(const KeyType& key);

    constexpr const std::vector<KeyType>& keys() const { return keys_; }
    constexpr std::vector<KeyType>& keys() { return keys_; }

    constexpr const std::vector<ValueType> values() const { return values_; }
    constexpr const std::vector<ValueType> values() { return values_; }

    static inline constexpr bool isIntKey();

    static constexpr bool is_int_key = std::is_same_v<int, KeyType>;

  private:
    std::vector<KeyType> keys_;
    std::vector<ValueType> values_;
};

// template <typename KeyType, typename ValueType>
template <typename KeyType>
constexpr bool is_int_key_v = VectorMap<KeyType, KeyType>::is_int_key;

template <typename KeyType, typename ValueType>
constexpr std::pair<KeyType, bool> VectorMap<KeyType, ValueType>::insert(const KeyType& key, const ValueType& val)
{
    auto keyIt = std::find(keys_.begin(), keys_.end(), key);
    if (keyIt != keys_.end()) {
        return std::make_pair(key, false);
    }
    keys_.emplace_back(key);
    values_.emplace_back((val));

    return std::make_pair(key, true);
}

template <typename KeyType, typename ValueType>
constexpr std::pair<KeyType, bool> VectorMap<KeyType, ValueType>::insert(KeyType&& key, ValueType&& val)
{
    auto keyIt = std::find(keys_.begin(), keys_.end(), key);
    if (keyIt != keys_.end()) {
        return std::make_pair(std::move(key), false);
    }
    keys_.emplace_back(std::move(key));
    values_.emplace_back(std::move(val));

    return std::make_pair(key, true);
}

template <typename KeyType, typename ValueType>
constexpr ValueType& VectorMap<KeyType, ValueType>::operator[](const KeyType& key)
{
    auto keyIt = std::find(keys_.begin(), keys_.end(), key);
    if (keyIt != keys_.end()) {
        auto position = std::distance(keys_.begin(), keyIt);
        return values_[position];
    }

    keys_.emplace_back(key);
    values_.emplace_back(ValueType {});

    return values_.back();
}

template <typename KeyType, typename ValueType>
constexpr ValueType& VectorMap<KeyType, ValueType>::operator[](KeyType&& key)
{
    auto keyIt = std::find(keys_.begin(), keys_.end(), key);
    if (keyIt != keys_.end()) {
        auto position = std::distance(keys_.begin(), keyIt);
        return values_[position];
    }

    keys_.emplace_back(std::move(key));
    values_.emplace_back(ValueType {});

    return values_.back();
}

template <typename KeyType, typename ValueType>
constexpr const ValueType& VectorMap<KeyType, ValueType>::at(const KeyType& key) const
{
    auto keyIt = std::find(keys_.begin(), keys_.end(), key);
    if (keyIt == keys_.end()) {
        throw std::out_of_range("value for non existing key requested.");
    }

    auto position = std::distance(keys_.begin(), keyIt);
    return values_[position];
}

template <typename KeyType, typename ValueType>
constexpr ValueType& VectorMap<KeyType, ValueType>::at(const KeyType& key)
{
    auto keyIt = std::find(keys_.begin(), keys_.end(), key);
    if (keyIt == keys_.end()) {
        throw std::out_of_range("value for non existing key requested.");
    }

    auto position = std::distance(keys_.begin(), keyIt);
    return values_[position];
}

template <typename KeyType, typename ValueType>
inline constexpr bool VectorMap<KeyType, ValueType>::isIntKey()
{
    return std::is_same_v<int, KeyType>;
}

// Specializations

template <typename ValueType>
class VectorMap<bool, ValueType>
{
    static_assert(std::is_default_constructible_v<ValueType>,
                  "Value type should be default constructible");

  public:
    constexpr std::pair<bool, bool> insert(bool key, const ValueType& val);
    constexpr std::pair<bool, bool> insert(bool key, const ValueType&& val);

    constexpr ValueType& operator[](bool key);
    constexpr const ValueType& at(bool key) const;
    constexpr ValueType& at(bool key);

    static inline constexpr bool isIntKey() { return false; }
    static constexpr bool is_int_key = false;

  private:
    ValueType trueVal_;
    ValueType falseVal_;
};

template <typename ValueType>
constexpr std::pair<bool, bool> VectorMap<bool, ValueType>::insert(bool key, const ValueType& val)
{
    if ((key && trueVal_ != ValueType {})
        || (!key && falseVal_ != ValueType {})) {
        return std::make_pair(key, false);
    }

    if (key) {
        trueVal_ = val;
    }
    else {
        falseVal_ = val;
    }

    return std::make_pair(key, true);
}

template <typename ValueType>
constexpr std::pair<bool, bool> VectorMap<bool, ValueType>::insert(bool key, const ValueType&& val)
{
    if ((key && trueVal_ != ValueType {})
        || (!key && falseVal_ != ValueType {})) {
        return std::make_pair(key, false);
    }

    if (key) {
        trueVal_ = std::move(val);
    }
    else {
        falseVal_ = std::move(val);
    }

    return std::make_pair(key, true);
}

template <typename ValueType>
constexpr ValueType& VectorMap<bool, ValueType>::operator[](bool key)
{
    return key ? trueVal_ : falseVal_;
}

template <typename ValueType>
constexpr const ValueType& VectorMap<bool, ValueType>::at(bool key) const
{
    if (key && trueVal_ == ValueType {}) {
        throw std::out_of_range("value for true not specified");
    }

    if (!key && falseVal_ == ValueType {}) {
        throw std::out_of_range("value for false not specified");
    }

    return key ? trueVal_ : falseVal_;
}

template <typename ValueType>
constexpr ValueType& VectorMap<bool, ValueType>::at(bool key)
{
    if (key && trueVal_ == ValueType {}) {
        throw std::out_of_range("value for true not specified");
    }

    if (!key && falseVal_ == ValueType {}) {
        throw std::out_of_range("value for false not specified");
    }

    return key ? trueVal_ : falseVal_;
}
