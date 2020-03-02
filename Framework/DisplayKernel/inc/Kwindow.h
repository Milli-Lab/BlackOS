#ifndef BLACKOS_KWINDOW_H
#define BLACKOS_KWINDOW_H

/**
 * Kwindow
 *
 * Copyright (C) 2019-07-27, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
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

#include "ncurses.h"
#include <memory>
#include <vector>

namespace BlackOS {
namespace DisplayKernel {
class Kwindow {
public:
  virtual void display(std::vector<int> const &breakConditions) = 0;
  virtual void borderStyle(int const ch) = 0;
  virtual void borderStyle(int const L, int const R, int const T, int const B,
                           int const TL, int const TR, int const BL,
                           int const BR) = 0;
  virtual void label(std::string const &label) const = 0;
  virtual std::vector<size_t> maxSize() const = 0;
  virtual std::string winType() const = 0;
  virtual std::string name() const = 0;
  virtual void setWin(bool const initWin) = 0;
  virtual void erase(size_t const y1, size_t const x1, size_t const y2,
                     size_t const x2) = 0;
  virtual void eraseExcept(size_t const y1, size_t const x1, size_t const y2,
                           size_t const x2) = 0;
  virtual void erase(std::vector<size_t> const &elements) = 0;
  virtual void erase(bool titlebar) = 0;
  virtual void clear() = 0;
  virtual void eraseExcept(std::vector<size_t> const &elements) = 0;
  virtual void refresh() = 0;
  virtual char getCharFromWin(size_t const y, size_t const x,
                              bool const preserve_cursor_pos) const = 0;
  virtual void fill(char ch, bool titleBar = false) = 0;
  virtual void insert(std::string const &str, size_t const y,
                      size_t const x) = 0;
  virtual void insert(char const *ch, size_t const y, size_t const x) = 0;
  virtual void insert(char const ch, size_t const y, size_t const x) = 0;
  virtual void pause() const = 0;
  virtual bool windowSet() const = 0;
  virtual int resize(size_t const y, size_t const x) = 0;
  virtual int reposition(size_t const y, size_t const x) = 0;
};
} // namespace DisplayKernel
} // namespace BlackOS
#endif // BLACKOS_KWINDOW_H
