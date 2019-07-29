//
// Created by Takudzwa Makoni on 2019-07-27.
//


#include "../inc/Directives.h"
#include "../inc/Kmenu.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

namespace {
const int PADDING = 1;
//WINDOW * DEFAULT_WINDOW = newwin(0,0,0,0);

}

using namespace BlackOSDisplay;



Kmenu::Kmenu(const size_t &id, std::string &name, int sizeY, int sizeX ,int posY, int posX) : m_id(id) {

    m_size = {sizeY,sizeX};
    m_position = {posY,posX};
    setWin();

}

/// return Window Object
WINDOW * Kmenu::window() const {
  return m_privateWin;
}
/// return fields in Kmenu instance
std::vector<Kfield> Kmenu::fields() const { return m_fields; }
/// return window size of instance
std::vector<int> Kmenu::size() const { return m_size; }
/// return type of Kwindow instance
std::string Kmenu::winType() const { return "Kmenu"; }
/// return name of Kwindow instance
std::string Kmenu::name() const { return m_name; }
/// switch for cursor blinking
void Kmenu::cursor(const int &code) const { curs_set(code); }
/// set style of corners of BlackOS Window
void Kmenu::setCornerStyle(const char &ch) {
    wborder(m_privateWin, 0, 0, 0, 0, (int)ch, (int)ch, (int)ch, (int)ch);
}
/// set style for each corner of BlackOS Window
void Kmenu::setCornerStyle(const char &ch1, const char &ch2,
                           const char &ch3, const char &ch4) {
    wborder(m_privateWin, 0, 0, 0, 0, (int)ch1, (int)ch2, (int)ch3, (int)ch4);
}
///set border style
void Kmenu::setBorderStyle(const char &ch){
    box(m_privateWin, ch, ch);
}
/// set corner label of BlackOS Window
void Kmenu::setLabel(const std::string &label) const {
    int labellocy = m_size[0] - 1;
    int labellocx = m_size[1] - (3 + (int)label.length());
    mvwaddstr(m_privateWin, labellocy, labellocx, label.c_str());
}
/// return the maximum size of the local window
std::vector<int> Kmenu::maxSize() const {
    int yMax, xMax;
    getmaxyx(m_privateWin, yMax, xMax);
    std::vector<int> size{yMax, xMax};
    return size;
}
/// set the position of the menu inside the local window
void Kmenu::setPos(Eigen::Vector2i &v){

}
/// set animation codes for window start and finish
void Kmenu::setAnimation(const int &start, const int &finish) const {
  m_startAnim = start;
  m_finishAnim = finish;
}
/// get position of window top left corner
Eigen::Vector2i Kmenu::position() const { return m_position;}
/// set the private window to a window
void Kmenu::setWin() {
  m_privateWin = newwin(m_size[0], m_size[1], m_size[2], m_size[3]);
}
/// add child display object to this instances private window
void Kmenu::addDisplayObj(BlackOSDisplay::Kwindow obj) const {
  std::vector<int> childSize = m_size;
  std::for_each(childSize.begin(), childSize.end(), [](const int &i){return i - PADDING;});
  //WINDOW * win = subwin(m_privateWin, childSize[0],childSize[1],childSize[2],childSize[3]);
}
/// display the menu onto private window
void Kmenu::display() const {
setAnimation(0,0);
  cursor(0);

   setLabel(m_name);

    int yMax, xMax;
    getmaxyx(m_privateWin, yMax, xMax);
    move(yMax / 2, xMax / 2);

    keypad(m_privateWin, true);
    box(m_privateWin, 0, 0);

    int selection;
    int highlighted = 0;
    int numOfFields = m_fields.size();

    while (true) {
        for (int i = 0; i < numOfFields; i++) {
            if (i == highlighted)
                wattron(m_privateWin, A_REVERSE);

            int yAlign = 0;
            int xAlign = 0;
            int left, right, top, bottom, v_centre, h_centre;

            // left align
            left = 1;

            // right align
            int longest_string_len = 0;
            std::for_each(m_fields.begin(), m_fields.end(), [&longest_string_len](const Kfield &field){

                std::string str = field.name();

                int len = str.length();
                if (len > longest_string_len)
                    longest_string_len = len;

            });
            right = xMax - longest_string_len - 1;

            // centre align
            v_centre = i + (yMax / 2) - numOfFields;
            h_centre = (xMax - m_fields[i].name().length()) / 2;

            // top align
            top =  i + 1;
            bottom = yMax - 2  - numOfFields + i;

            if(m_xAlignment == 1 )
                xAlign = right;
            else if(m_xAlignment == 0)
                xAlign = h_centre;
            else if(m_xAlignment == -1)
                xAlign = left;

            if(m_yAlignment == 1)
                yAlign = top;
            else if(m_yAlignment == 0)
                yAlign = v_centre;
            else if (m_yAlignment == -1)
                yAlign = bottom;

            mvwprintw(m_privateWin, yAlign, xAlign, m_fields[i].name().c_str());
            wattroff(m_privateWin, A_REVERSE);
        }
        selection = wgetch(m_privateWin);

        switch (selection) {
            case KEY_UP:
                highlighted--;
                if (highlighted == -1) {
                    highlighted = 0;
                }
                break;
            case KEY_DOWN:
                highlighted++;
                if (highlighted == m_fields.size()) {
                    highlighted = (int)m_fields.size() - 1;
                }
            default:
                break;
        }
        if (selection == 10) {
            break;
        }
        clear();

        mvprintw((yMax / 2), (xMax - m_fields[highlighted].message().length())/2, m_fields[highlighted].message().c_str());
        auto fieldScript = m_fields[highlighted].script();
        fieldScript(); // performs the script.
    }
    wrefresh(m_privateWin);
    getch();
}

Kmenu::~Kmenu(){
    
    delwin(m_privateWin);
}

