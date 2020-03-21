/**
 * Tr(inkets) ScreenShell
 *
 * Copyright (C) 2020 by Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "ScreenShell.h"

using namespace BlackOS::Trinkets;

// thanks to Sarah Vessels for Pipe/Redirect:
// https://www.3till7.net/2008/11/29/c-shell-with-forks-and-pipes/index.html
int main() {

  auto termSz = BlackOS::DisplayKernel::TERMINAL_SIZE();
  size_t termSzY = termSz[0];
  size_t termSzX = termSz[1];

  // use stdscreen
  auto display = generateScreen();
  display->hideBorder();

  ScreenShell shell(display);
  shell.initShell();

  while (1) {

    shell.displayPrompt();
    int result = shell.readArgs();
    auto const &argv = shell.argv();
    if (!argv.empty()) {
      // quit on user prompt
      std::string firstArg = argv[0];
      if (firstArg == "exit" || firstArg == "quit")
        break;

      shell.runCommand(argv); // neither
      shell.logResult();
      shell.resetArgs();
    }
  }

  return ExitStatus::USER_EXIT;
}
