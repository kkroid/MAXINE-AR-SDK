#ifndef NVMAXINE_H
#define NVMAXINE_H

#include <string>
#include <functional>
#include <vector>

#ifdef BUILD_NVMAXINE
#define NVMAXINE_API __declspec(dllexport)
#else
#define NVMAXINE_API __declspec(dllimport)
#endif

class NVMAXINE_API NvMaxine
{
private:
    class Impl;
    Impl* pImpl;
    using OnExpressionCallback = std::function<void(const std::vector<float> &)>;
public:
    NvMaxine();
    
    ~NvMaxine();

    int32_t setInputVideo(const std::string &file);

    int32_t setInputCamera(int index);

    void setModelDir(const std::string &dir);

    /**
     * | ARKit                     | Maxine                |
     * |---------------------------|-----------------------|
     * | eyeBlinkLeft              | eyeBlink_L            |
     * | eyeLookDownLeft           | eyeLookDown_L         |
     * | eyeLookInLeft             | eyeLookIn_L           |
     * | eyeLookOutLeft            | eyeLookOut_L          |
     * | eyeLookUpLeft             | eyeLookUp_L           |
     * | eyeSquintLeft             | eyeSquint_L           |
     * | eyeWideLeft               | eyeWide_L             |
     * | eyeBlinkRight             | eyeBlink_R            |
     * | eyeLookDownRight          | eyeLookDown_R         |
     * | eyeLookInRight            | eyeLookIn_R           |
     * | eyeLookOutRight           | eyeLookOut_R          |
     * | eyeLookUpRight            | eyeLookUp_R           |
     * | eyeSquintRight            | eyeSquint_R           |
     * | eyeWideRight              | eyeWide_R             |
     * | jawForward                | jawForward            |
     * | jawLeft                   | jawLeft               |
     * | jawRight                  | jawRight              |
     * | jawOpen                   | jawOpen               |
     * | mouthClose                | mouthClose            |
     * | mouthFunnel               | mouthFunnel           |
     * | mouthPucker               | mouthPucker           |
     * | mouthLeft                 | mouthLeft             |
     * | mouthRight                | mouthRight            |
     * | mouthSmileLeft            | mouthSmile_L          |
     * | mouthSmileRight           | mouthSmile_R          |
     * | mouthFrownLeft            | mouthFrown_L          |
     * | mouthFrownRight           | mouthFrown_R          |
     * | mouthDimpleLeft           | mouthDimple_L         |
     * | mouthDimpleRight          | mouthDimple_R         |
     * | mouthStretchLeft          | mouthStretch_L        |
     * | mouthStretchRight         | mouthStretch_R        |
     * | mouthRollLower            | mouthRollLower        |
     * | mouthRollUpper            | mouthRollUpper        |
     * | mouthShrugLower           | mouthShrugLower       |
     * | mouthShrugUpper           | mouthShrugUpper       |
     * | mouthPressLeft            | mouthPress_L          |
     * | mouthPressRight           | mouthPress_R          |
     * | mouthLowerDownLeft        | mouthLowerDown_L      |
     * | mouthLowerDownRight       | mouthLowerDown_R      |
     * | mouthUpperUpLeft          | mouthUpperUp_L        |
     * | mouthUpperUpRight         | mouthUpperUp_R        |
     * | browDownLeft              | browDown_L            |
     * | browDownRight             | browDown_R            |
     * | browInnerUp               | browInnerUp_L         |
     * | browOuterUpLeft           | browOuterUp_L         |
     * | browOuterUpRight          | browOuterUp_R         |
     * | cheekPuff                 | cheekPuff_L           |
     * | cheekSquintLeft           | cheekSquint_L         |
     * | cheekSquintRight          | cheekSquint_R         |
     * | noseSneerLeft             | noseSneer_L           |
     * | noseSneerRight            | noseSneer_R           |
     * | tongueOut                 |                       |
     * | headYaw                   |                       |
     * | headPitch                 |                       |
     * | headRoll                  |                       |
     * | leftEyeYaw                |                       |
     * | leftEyePitch              |                       |
     * | leftEyeRoll               |                       |
     * | rightEyeYaw               |                       |
     * | rightEyePitch             |                       |
     * | rightEyeRoll              |                       |
     */
    void setOnExpressionCallback(OnExpressionCallback callback);

    void start();

    void startAsync();

    void stop();
};
#endif // NVMAXINE_H
