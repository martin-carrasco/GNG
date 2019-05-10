#include <iostream>
#include "CImg.h"
#include <vector>

using namespace std;
using namespace cimg_library;

int main(){
    int mode = 1;
    static constexpr int RED[] = {255, 0, 0};
    
    CImg<unsigned char> background(640, 640, 1, 3, 0);
    CImg<unsigned char> text(640, 640, 1, 3, 0);
    CImgDisplay window(background, "Growing Neural Gas");    
    text.draw_text(640-50, 20, "1000", RED);
    
    vector<pair<int, int>> draw_pos;

    while(!window.is_closed()){
        
        pair<int, int> last_point = make_pair(-1, -1);
        for(auto tmp_pair : draw_pos){
            if(last_point.first == -1 && last_point.second == -1){
                last_point = tmp_pair;
                continue;
            }
            pair<int, int> current_point = tmp_pair;
            background.draw_line(last_point.first, last_point.second, current_point.first, current_point.second, RED);
            last_point = current_point;
        }
        if(window.button()){
            if(mode){
                pair<int, int> p = make_pair(window.mouse_x(), window.mouse_y());
                draw_pos.push_back(p);
            }
        }
        background.display(window);
        window.wait();
    }

}
