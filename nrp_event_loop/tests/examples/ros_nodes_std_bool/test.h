#include <string>
#include <iostream>
#include "std_msgs/msg/bool.hpp"

using namespace std;

bool my_function(const std_msgs::msg::Bool* i1, std_msgs::msg::Bool& o1) {
    if(i1 != nullptr) {
        cout << "got data" << endl;
        o1 = *i1;
        return true;
    }
    else {
        cout << "got null pointer" << endl;
        return false;
    }
}
