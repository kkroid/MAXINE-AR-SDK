#include "NvMaxine.h"

int main(int argc, char **argv)
{
    NvMaxine NvMaxine;
    NvMaxine.setModelDir("..\\samples\\ExpressionApp\\models");
    NvMaxine.setInputCamera(0);
    // NvMaxine.setInputVideo("C:\\Users\\admin\\Videos\\004340_CH0.mp4");
    NvMaxine.setOnExpressionCallback([](const std::vector<float> &expressions) {
        for (int i = 0; i < expressions.size(); i++)
        {
            printf("Expression %d: %f\n", i, expressions[i]);
        }
    });
    NvMaxine.start();
    return 0;
}
