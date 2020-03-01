// NavigateDir 2020 by takudzwa Makoni (c)

#include "DisplayHelpers.h" // TERMINAL_SIZE()
#include "Kmenu.h"
#include "NavigationHelpers.h"
#include "PathController.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace BlackOS::Trinkets;

int main(int argc, char const **argv) {

  /*
  ====================================================================
  SYSTEM ARGUMENTS
  ====================================================================
  */

  bool withHidden;
  size_t cursor_pos_y;
  size_t cursor_pos_x;
  std::string initPath;

  withHidden = strtol(argv[1], nullptr, 10);

  // uncomment to allow externally specified window position
  // cursor_pos_y = strtol(argv[2], nullptr, 10);
  // cursor_pos_x = strtol(argv[3], nullptr, 10);
  // initPath = argv[4];

  auto const termSz = BlackOS::DisplayKernel::TERMINAL_SIZE();
  size_t const ROWS = termSz[0];
  size_t const COLS = termSz[1];

  // uncomment if using externally specified window position
  cursor_pos_y = ROWS / 2;
  cursor_pos_x = 0;
  initPath = argv[2];

  // remove trailing '/' if any
  if (initPath.back() == '/') {
    initPath.pop_back();
  }

  /*
  ====================================================================
  NAVIGATION CONTROLLER VARIABLES
  ====================================================================
  */

  std::filesystem::path parentPath(initPath);
  std::string chosenPath;
  size_t fieldIdx;
  size_t fieldSz;
  std::vector<std::string> fields;
  std::vector<std::filesystem::path> children;

  /*
  ====================================================================
  NAVIGATION MENU VARIABLES
  ====================================================================
  */

  std::string const menuID = "NavigationMenu";
  std::string title;
  size_t menuWidth;
  size_t menuHeight;
  size_t pagination;

  // break conditions for menu display.
  // 'a' key to navigate up a directory.
  // 'd' key to navigate into highlighted directory.
  // 'h' key to toggle hidden folders view.
  // 'ENTER' key to change directory, or open file with EDITOR environment
  // variable.
  // 'q' or 'ESC' key to exit navigation menu.
  std::vector<int> breakConditions = {(int)'a', (int)'d',     (int)'q',
                                      (int)'h', 10 /*ENTER*/, 27 /*ESC*/};

  // create menu object
  BlackOS::DisplayKernel::Kmenu NavigationMenu(
      menuID, ROWS - cursor_pos_y, COLS, cursor_pos_y, cursor_pos_x);

  // create path navigator object;
  PathController pathController;

  NavigationMenu.setWin(1);
  while (1) {

    pathController.showHidden(withHidden);

    try {
      pathController.loadParent(parentPath);
    } catch (std::filesystem::filesystem_error &e) {
      NavigationMenu.clear();
      NavigationMenu.insert(e.what(), 0, 0);
      NavigationMenu.refresh();
      NavigationMenu.pause();
      if (parentPath == initPath) {
        // user initialised parent directory without access
        NavigationMenu.setWin(0);
        exit(2); // leave here
      } else {
        // user navigated into directory without permissions
        // return to parent directory.
        // user is in curses mode.
        parentPath = parentPath.parent_path();
      }
      continue;
    }

    fields = pathController.generateFields();
    title = pathController.generateTitle();
    children = pathController.children();
    fieldSz = fields.size();

    // include 1 additional space.
    menuWidth = title.length() + 1;
    menuHeight = ROWS - cursor_pos_y;
    pagination = menuHeight - 3;

    NavigationMenu.resize(menuHeight, menuWidth);
    NavigationMenu.reposition(cursor_pos_y /*maintain cursor y position*/,
                              cursor_pos_x /*left of screen*/);

    if (fieldSz == 0) {
      std::string message;
      if (withHidden) {
        message = "no entries to show.";
      } else {
        message = "no entries to show (excl. hidden paths).";
      }
      NavigationMenu.clear(); // clear previous output
      NavigationMenu.insert(message.c_str(), cursor_pos_y, 0);
      NavigationMenu.refresh(); // present message to screen
      wgetch(stdscr);
      // go back up a level and skip iteration.
      parentPath = parentPath.parent_path(); // go up a dir
      NavigationMenu.clear(); // clear message for next iteration.
      continue;
    }

    // load all menu attributes first
    NavigationMenu.loadTitle(title,
                             BlackOS::DisplayKernel::TitleStyle::underline);
    NavigationMenu.loadFields(fields);
    NavigationMenu.loadFieldAlignment(-1, 1); // top left of window
    NavigationMenu.paginate(pagination);

    NavigationMenu.hideBorder();
    NavigationMenu.showTitle();

    std::string showingHidden = withHidden ? "t" : "f";

    // TODO: duplicate inserting is a hack, better to implement KCanvas and have
    // separate window as attribute panel on bottom of screen
    NavigationMenu.insert("showing hidden paths: " + showingHidden,
                          menuHeight - 1, 0);
    NavigationMenu.display(breakConditions);

    NavigationMenu.insert("showing hidden paths: " + showingHidden,
                          menuHeight - 1, 0);

    // retrieve last key entered by user on exit display call
    int lastKey = NavigationMenu.lastKeyPressed();

    if (lastKey == (int)'q' || lastKey == 27 /*ESC*/) {
      // user exited program
      NavigationMenu.setWin(0);
      exit(1); // leave here
    } else if (lastKey == (int)'a' /*out of directory*/) {
      // user navigated up a directory
      parentPath = parentPath.parent_path();
      NavigationMenu.clear();
    } else if (lastKey == (int)'d' /*into dir*/) {

      // user navigated into a child directory
      // get index pertaining to field chosen by user
      fieldIdx = NavigationMenu.selectedFieldIndex();

      // assign new path to chosen path in children,
      // mapped by fieldIdx.
      auto newPath = children[fieldIdx];

      // check the path is a directory
      // set parent to this path if true
      if (pathType(newPath) == "directory") {
        parentPath = newPath;
      }
      // clear window for next iteration
      NavigationMenu.clear();
    } else if (lastKey == (int)'h') {
      // toggle hide files
      if (withHidden) {
        withHidden = 0;
      } else {
        withHidden = 1;
      }
      NavigationMenu.clear();
    } else {
      // last key pressed is enter
      fieldIdx = NavigationMenu.selectedFieldIndex();
      chosenPath = children[fieldIdx];
      NavigationMenu.setWin(0);
      break; // break from while loop
    }
  }

  // write to tmp file for shell access
  std::ofstream navigationDir("/tmp/navigationDirectory.txt");
  navigationDir << chosenPath;

  return 0;
}
