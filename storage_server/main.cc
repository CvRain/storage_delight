#include <drogon/drogon.h>
int main() {
    //Set HTTP listener address and port
    drogon::app().addListener("0.0.0.0", 3000);
    drogon::app().run();
    return 0;
}
