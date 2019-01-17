#pragma once

#include <memory>
#include <functional>

namespace Cook
{
    class TcpConn;
    class Buffer;

    using ConnPtr = std::shared_ptr<TcpConn>;
    using ConnectionCallback = std::function<void(const ConnPtr&)>;
    using CloseCallback = std::function<void(const ConnPtr&)>;
    using WriteCompleteCallback = std::function<void(const ConnPtr&)>;
    using MessageCallback = std::function<void(const ConnPtr&, Buffer*)>;
    using NewConnectionCallback = std::function<void(int)>;
    using ReadEventCallback = std::function<void()>;
    using WriteEventCallback = ReadEventCallback;

}

