#include "NvMaxine.h"
#include "LiveLinkPacket.h"
#include "asio.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// 固定的索引映射关系
std::vector<int> index_mapping = {
10, 12, 14, 16, 18, 20, 22, 11, 13, 15,
17, 19, 21, 23, 24, 25, 27, 26, 28, 33,
39, 34, 40, 45, 46, 31, 32, 29, 30, 47,
48, 41, 42, 43, 44, 37, 38, 35, 36, 49,
50, 0, 1, 2, 4, 5, 6, 8, 9, 51, 52
};

int main(int argc, char **argv)
{
    NvMaxine NvMaxine;
    NvMaxine.setModelDir("..\\samples\\ExpressionApp\\models");
    NvMaxine.setInputCamera(0);
    // NvMaxine.setInputVideo("C:\\Users\\admin\\Videos\\004340_CH0.mp4");
    int32_t PackageNum = 0;
    // 创建 io_context 对象
    asio::io_context Context;
    // 创建 UDP socket
    asio::ip::udp::socket Socket(Context);
    Socket.open(asio::ip::udp::v4());
    // 设置服务器地址
    asio::ip::udp::endpoint ServerEndpoint(asio::ip::make_address("127.0.0.1"), 11111);
    NvMaxine.setOnExpressionCallback([&PackageNum, &Socket, &ServerEndpoint](std::vector<float> expressions) mutable {
        LiveLinkPacket Packet;
        // printf("expressions: %d\n", expressions.size());
        size_t size = index_mapping.size();
        std::vector<float> new_expressions(61, 0.0f);
        for (size_t i = 0; i < size; i++)
        {
            new_expressions[i] = expressions[index_mapping[i]];
            // printf("%d -> %d, value:%f", i, index_mapping[i], expressions[index_mapping[i]]);
        }
        // printf("\n");
        // printf("jawOpen: %f, eyeBlinkLeft: %f\n", new_expressions[17], new_expressions[0]);
        Packet.setPayload(PackageNum, new_expressions);
        Packet.serialize();
        auto SentBytes = Socket.send_to(asio::buffer(Packet.get_data()), ServerEndpoint);
        // printf("Sent %llu bytes\n", SentBytes);
        PackageNum++;
    });
    NvMaxine.start();
    return 0;
}
