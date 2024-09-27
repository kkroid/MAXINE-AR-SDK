#include "NvMaxine.h"
#include "App.h"
#include <memory>
#include <thread>

class NvMaxine::Impl
{
private:
    NvCV_Status err = NVCV_SUCCESS;
    std::unique_ptr<App> app;
public:
    Impl() 
    {
        app = std::make_unique<App>();
    }
    ~Impl()
    {
        app->setOnExpressionCallback(nullptr);
    }

    int32_t setInputVideo(const std::string &file)
    {
        err = app->setInputVideo(file);
        return err;
    }

    int32_t setInputCamera(int index, const std::string &resStr)
    {
        err = app->setInputCamera(index, resStr);
        return err;
    }

    void setModelDir(const std::string &dir)
    {
        app->setModelDir(dir);
    }

    int32_t init()
    {
        err = app->init();
        return err;
    }

    void setOnExpressionCallback(OnExpressionCallback callback)
    {
        app->setOnExpressionCallback(callback);
    }

    void start()
    {
        app->run();
    }

    void stop()
    {
        app->stop();
    }
};

NvMaxine::NvMaxine() : pImpl(new Impl) {}

NvMaxine::~NvMaxine()
{
    delete pImpl;
    pImpl = nullptr;
}

int32_t NvMaxine::setInputVideo(const std::string &file)
{
    return pImpl->setInputVideo(file);
}

int32_t NvMaxine::setInputCamera(int index)
{
    return pImpl->setInputCamera(index, "");
}

void NvMaxine::setModelDir(const std::string &dir)
{
    pImpl->setModelDir(dir);
}

void NvMaxine::setOnExpressionCallback(OnExpressionCallback callback)
{
    pImpl->setOnExpressionCallback(callback);
}

void NvMaxine::start()
{
    pImpl->init();
    pImpl->start();
}

void NvMaxine::startAsync()
{
    std::thread([this] {
        start();
    }).detach();
}

void NvMaxine::stop()
{
    pImpl->stop();
}
