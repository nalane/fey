#pragma once

/*
 * Contains functions for initializing and running the game.
 */

#include <string>
#include <vector>
#include <map>

#include "paths.hpp"
#include "camera.hpp"
#include "glHeaders.hpp"
#include "shaderProgram.hpp"
#include "resourceHandler.hpp"
#include "resource.hpp"

void engineLoad(const std::string& configFile);
void endEngine();
void runGame();

