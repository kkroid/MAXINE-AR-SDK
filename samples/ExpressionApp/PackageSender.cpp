#include "LiveLinkPacket.h" // 确保您有这个头文件
#include <iostream>
#include <string>
#include <asio.hpp>
#include <chrono>
#include <random>
#include <algorithm>
#include <functional> // 添加此行以使用 std::function

int main() {
    // 创建 io_context 对象
    asio::io_context io_context;
    // 创建 UDP socket
    asio::ip::udp::socket socket(io_context);
    socket.open(asio::ip::udp::v4());
    // 设置服务器地址
    asio::ip::udp::endpoint server_endpoint(asio::ip::make_address("127.0.0.1"), 11111);
    uint32_t frame_num = 0;
    // 使用 std::random 生成随机数，提供更好的随机性
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // 创建一个 61 维的 float 数组，用于测试
    std::vector<float> payload(61);

    // 创建一个定时器，每秒发送 30 个包
    asio::steady_timer timer(io_context, std::chrono::milliseconds(33));

    // 定义发送函数
    std::function<void()> send_packet = [&]() {
        LiveLinkPacket packet;
        // 使用 std::generate 填充 payload 数组
        std::generate(payload.begin(), payload.end(), [&]() { return dis(gen); });
        packet.setPayload(frame_num, payload);
        packet.serialize();
        frame_num++;

        // 发送数据包
        auto sent_bytes = socket.send_to(asio::buffer(packet.get_data()), server_endpoint);
        // 检查 send_to 的返回值，确保数据包成功发送
        if (sent_bytes < 0) {
            std::cerr << "Failed to send packet" << std::endl;
        }

        printf("Sent %llu bytes\n", sent_bytes);

        // 重启定时器
        timer.expires_after(std::chrono::milliseconds(33));
        timer.async_wait([&](const asio::error_code& error) {
            if (!error) {
                send_packet(); // 继续发送下一个数据包
            }
        });
    };

    // 启动发送
    send_packet();

    // 运行 io_context
    io_context.run();

    return 0;
}
