#include "app.h"

int main() {
    SimpleShapeApplication app(650, 480, PROJECT_NAME, true, 1);
    app.run(1);
    return 0;
}