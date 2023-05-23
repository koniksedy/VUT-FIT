/**
 * @file matrix.hpp
 * @author Michal Šedý (xsedym02@vutbr.cz)
 * @brief A header file defining matrix template.
 * @date 04.05.2023
 */

#include <vector>

#pragma once


template <typename T>
using matrix = std::vector<std::vector<T>>;
