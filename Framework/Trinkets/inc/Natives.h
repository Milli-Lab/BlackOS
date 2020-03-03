#ifndef NATIVES_H
#define NATIVES_H

/**
 * Natives
 *
 * Copyright (C) 2020, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
 *
 * This Program is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Program is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This Program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#include "../inc/PathController.h" // NavigateDir, ListChildren
#include "DisplayHelpers.h"        //(libDisplayKernel) NavigateDir
#include "Kmenu.h"                 //(libDisplayKernel) NavigateDir
#include "NavigationHelpers.h"     //(libDisplayKernel) NavigateDir

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

namespace BlackOS {
namespace Trinkets {
int navigateDir(int, char **);
int listChildren(int, char **);
int changeDir(char const *path = nullptr);
} // namespace Trinkets
} // namespace BlackOS


#endif
