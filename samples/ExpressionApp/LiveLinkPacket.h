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

class LiveLinkPacket {
private:
    struct FrameTime {
        uint32_t frame_number;
        uint32_t sub_frame;
        uint32_t numerator;
        uint32_t denominator;
    };

    std::string device_id;
    std::string subject_name;
    FrameTime frame_time;

    std::string header; // Store serialized header and timer as a single string
    std::string data; // Store serialized data as string
    uint8_t paload_size;

    uint32_t floatToBigEndian(float value) {
        uint32_t intValue;
        std::memcpy(&intValue, &value, sizeof(float)); // 安全地复制内存
        return TO_BIG_ENDIAN(intValue);
    }
public:
    LiveLinkPacket() {
        device_id = "Maxine";
        subject_name = "Maxine";
        paload_size = 61;
        frame_time = {0, 0, 30, 1};
        data = "";
        header = "";
    }

    void setupHeader() {
        uint8_t version = 6;
        header.append(CAST2CHAR(&version), sizeof(version));

        // Serialize device_id
        uint32_t device_id_length = TO_BIG_ENDIAN(static_cast<uint32_t>(device_id.size()));
        header.append(CAST2CHAR(&device_id_length), sizeof(device_id_length));
        header.append(device_id.c_str(), device_id.size());

        // Serialize subject_name
        uint32_t subject_name_length = TO_BIG_ENDIAN(static_cast<uint32_t>(subject_name.size()));
        header.append(CAST2CHAR(&subject_name_length), sizeof(subject_name_length));
        header.append(subject_name.c_str(), subject_name.size());

        uint32_t frame_number = TO_BIG_ENDIAN(frame_time.frame_number);
        uint32_t sub_frame = TO_BIG_ENDIAN(frame_time.sub_frame);
        uint32_t numerator = TO_BIG_ENDIAN(frame_time.numerator);
        uint32_t denominator = TO_BIG_ENDIAN(frame_time.denominator);
        header.append(CAST2CHAR(&frame_number), sizeof(uint32_t));
        header.append(CAST2CHAR(&sub_frame), sizeof(uint32_t));
        header.append(CAST2CHAR(&numerator), sizeof(uint32_t));
        header.append(CAST2CHAR(&denominator), sizeof(uint32_t));
        header.append(CAST2CHAR(&paload_size), sizeof(uint8_t));

        data = header;
    }

    // Method to set the payload
    void setPayload(uint32_t frame_num, const std::vector<float>& new_payload) {
        frame_time.frame_number = frame_num;
        paload_size = new_payload.size();
        for (size_t i = 0; i < paload_size; i++) {
            uint32_t bigEndianValue = floatToBigEndian(new_payload[i]);
            data.append(CAST2CHAR(&bigEndianValue), sizeof(bigEndianValue));
        }
    }

    const std::string& get_data() const {
        return data;
    }
};
