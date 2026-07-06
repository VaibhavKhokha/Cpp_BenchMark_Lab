#pragma once
#include <vector>

template<typename T>
std::vector<T> addVectors_baseline(const std::vector<T>& a, const std::vector<T>& b);

template <typename T>
std::vector<T> addVectors_reserving(const std::vector<T>& a, const std::vector<T>& b);

template <typename T>
void addVectors_prealloc(const std::vector<T>& a, const std::vector<T>& b, std::vector<T>& result);