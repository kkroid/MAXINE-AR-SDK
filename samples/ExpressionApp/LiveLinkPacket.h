#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#define CAST2CHAR reinterpret_cast<const char*>

#define TO_BIG_ENDIAN(value) \
    ((((value) >> 24) & 0x000000FF) | \
     (((value) >> 16) & 0x0000FF00) | \
     (((value) << 8) & 0x00FF0000)  | \
     (((value) << 24) & 0xFF000000))

uint32_t floatToBigEndian(float value) {
    uint32_t intValue;
    // 使用 memcpy 以避免潜在的类型别名问题
    std::memcpy(&intValue, &value, sizeof(float)); // 安全地复制内存
    return TO_BIG_ENDIAN(intValue);
}
class LiveLinkPacket {
public:

    LiveLinkPacket() 
        : data_count(61), device_id("WebCam"), subject_name("BasicRole") {
        frame_time = {0, 0, 30, 1};
        _premade_header.clear();
        _premade_timer.clear();
        payload.resize(data_count, 0.0f); // Initialize with zeros
    }

    // Method to set the payload
    void setPayload(uint32_t frame_num, const std::vector<float>& new_payload) {
        frame_time.frame_number = frame_num;
        payload = new_payload;
        // for (size_t i = 0; i < data_count; i++)
        // {
        //     payload[i] = floatToBigEndian(new_payload[i]);
        // }
    }

    void serialize() {
        serialize_header();
        serialize_timer();
        data.clear(); // Clear previous data
        data.append(_premade_header); // Append header
        data.append(_premade_timer);   // Append timer
        // 将payload以二进制形式写入data
        data.append(CAST2CHAR(payload.data()), data_count * sizeof(float));
        // printf("Data size: %zu\n", data.size());
    }

    const std::string& get_data() const {
        return data;
    }

private:
    struct FrameTime {
        uint32_t frame_number;
        uint32_t sub_frame;
        uint32_t numerator;
        uint32_t denominator;
    };

    void serialize_header() {
        _premade_header.clear(); // Clear previous header data
        uint8_t version = 6;
        _premade_header.append(CAST2CHAR(&version), sizeof(version));

        // Serialize device_id
        uint32_t device_id_length = TO_BIG_ENDIAN(static_cast<uint32_t>(device_id.size()));
        _premade_header.append(CAST2CHAR(&device_id_length), sizeof(device_id_length));
        _premade_header.append(device_id.c_str(), device_id.size());

        // Serialize subject_name
        uint32_t subject_name_length = TO_BIG_ENDIAN(static_cast<uint32_t>(subject_name.size()));
        _premade_header.append(CAST2CHAR(&subject_name_length), sizeof(subject_name_length));
        _premade_header.append(subject_name.c_str(), subject_name.size());
        // printf("Header size: %zu\n", _premade_header.size());
    }

    void serialize_timer() {
        _premade_timer.clear(); // Clear previous timer data
        uint32_t frame_number = TO_BIG_ENDIAN(frame_time.frame_number);
        uint32_t sub_frame = TO_BIG_ENDIAN(frame_time.sub_frame);
        uint32_t numerator = TO_BIG_ENDIAN(frame_time.numerator);
        uint32_t denominator = TO_BIG_ENDIAN(frame_time.denominator);
        _premade_timer.append(CAST2CHAR(&frame_number), sizeof(uint32_t));
        _premade_timer.append(CAST2CHAR(&sub_frame), sizeof(uint32_t));
        _premade_timer.append(CAST2CHAR(&numerator), sizeof(uint32_t));
        _premade_timer.append(CAST2CHAR(&denominator), sizeof(uint32_t));
        _premade_timer.append(CAST2CHAR(&data_count), sizeof(uint8_t));
        // printf("frame_number: %u, sub_frame: %u, numerator: %u, denominator: %u, data_count: %u\n", frame_time.frame_number, frame_time.sub_frame, frame_time.numerator, frame_time.denominator, data_count);
        // printf("Timer size: %zu\n", _premade_timer.size());
    }

    std::string data; // Store serialized data as string
    uint8_t data_count;

    std::string device_id;
    std::string subject_name;
    FrameTime frame_time;

    std::vector<float> payload; // Renamed from np_data to payload

    std::string _premade_header; // Store serialized header as string
    std::string _premade_timer;  // Store serialized timer as string
};
