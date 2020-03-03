/**
 * Tr(inkets) Shell
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

#include "Shell.h"

using namespace BlackOS::Trinkets;

// thanks to Sarah Vessels for Pipe/Redirect:
// https://www.3till7.net/2008/11/29/c-shell-with-forks-and-pipes/index.html
int main() {

  char *argv[MAX_ARGS], *cmd1[MAX_ARGS], *cmd2[MAX_ARGS];
  PipeRedirect pipe_redirect;
  int argc;

  static int first_time = 1;
  if (first_time) {
    const char *CLEAR_SCREEN_ANSI = "\e[0;0H\e[2J";
    std::cout << CLEAR_SCREEN_ANSI;

    std::string title = "Tr(inkets) Shell (C)";
    std::string version = "1.0";
    std::string repo = "Dark Horse (C)";
    std::string license = "GPL v3.0 licensed";
    std::string year = "2020";
    std::string language = "C++";
    std::string author = "Takudzwa Makoni";
    std::string git = "https://github.com/TakudzwaMakoni";

    std::vector<std::string> v{title, version,  repo,   license,
                               year,  language, author, git};
    splashScreen(v);
  }

  // Keep returning the user to the prompt ad infinitum unless they enter
  // 'quit' or 'exit' (without quotes).

  while (true) {
    // Display a prompt.
    char buf[1024];
    std::cout << "Tr " << getcwd(buf, sizeof buf) << "> ";

    // Read in a command from the user.
    argc = read_args(argv);

    // Decipher the command we just read in and split it, if necessary, into
    // cmd1 and cmd2 arrays.  Set pipe_redirect to a PipeRedirect enum value to
    // indicate whether the given command pipes, redirects, or does neither.
    pipe_redirect = parse_command(argc, argv, cmd1, cmd2);

    // Determine how to handle the user's command(s).
    if (pipe_redirect == PipeRedirect::PIPE) // piping
      pipe_cmd(cmd1, cmd2);
    else if (pipe_redirect == PipeRedirect::REDIRECT) // redirecting
      redirect_cmd(cmd1, cmd2);
    else
      run_cmd(argc, argv); // neither

    // Reset the argv array for next time.
    for (int i = 0; i < argc; i++)
      argv[i] = NULL;
  }

  return 0;
}
