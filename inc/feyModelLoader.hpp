#pragma once

/*
 * Functions for getting a model from a .fey.model file
 */

#include <string>

#include "model.hpp"

model* loadFeyModel(std::string filename);
