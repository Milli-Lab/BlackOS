/**
 * DisplayMenu
 *
 * Copyright (C) 2019, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "Menu.h"

#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

// namespace BlackOS {
// namespace DisplayKernel {

using namespace BlackOS::DisplayKernel;

int main(int argc, const char *argv[]) {

  // get terminal screen dimensions
  auto termSz = TERMINAL_SIZE();

  std::string const fname0 = "FIELD 1";
  std::string const fname1 = "FIELD 2";
  std::string const fname2 = "FIELD 3";
  std::string const fname3 = "FIELD 4";
  std::string const fname4 = "FIELD 5";
  std::string const fname5 = "FIELD 6";
  std::string const fname6 = "FIELD 7";
  std::string const fname7 = "QUIT";

  std::vector<std::string> test_fields = {fname0, fname1, fname2, fname3,
                                          fname4, fname5, fname6, fname7};

  std::string mainMenuName = "BlackOS version 1.0 ";
  Menu main_menu(termSz[0], termSz[1], 0, 0);

  main_menu.loadTitle(mainMenuName, TextStyle::highlight);
  main_menu.loadFieldAlignment(-1, 1);
  main_menu.loadFields(test_fields);

  main_menu.setWin(1); // must set the window!

  main_menu.hideBorder();
  // main_menu.hideTitle();
  // main_menu.borderStyle();
  main_menu.showTitle();

  int pagination = 3;

  main_menu.fieldStyle("-"); // must do this before
  // displaying

  main_menu.paginate(pagination, 1);

  main_menu.display();

  std::string message;
  size_t winSzY = main_menu.winSzY();
  size_t winSzX = main_menu.winSzX();

  int exitStatus = main_menu.lastKeyPressed();
  if (exitStatus == 27) {
    message = "you exited with ESC";
  } else if (exitStatus == (int)'q') {
    message = "you exited with q";
  } else {
    message = "you selected: " + main_menu.selectedField();
  }

  size_t centreY = winSzY / 2;
  size_t centreX = (winSzX - message.length()) / 2;

  main_menu.clear();
  main_menu.insert(message, centreY, centreX);
  main_menu.refresh();
  main_menu.pause();

  main_menu.setWin(0);
  return 0;
}
//} // namespace DisplayKernel
//} // namespace BlackOS
