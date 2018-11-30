#pragma once

namespace XCr
{

    class Service;
    class ServiceMgr;
    class MemoryMgr;
    class DataStream;
    class FileDataStream;
    class File;
    class CFile;
    class CTextFile;
    
    class TaskProcessor;
    class TaskThread;
    class Task;
    class TaskAllocator;
    class TaskHandler;
    class EventHandler;
    
    class MessageService;
    class AssetLocatorService;
    class AssetService;
    class ServiceMgr;
    class Service;
    class PropertyDescriptor;
    class IPropertyValue;
    class IPropertyConstraint;
    class IAsset;
    class SettingMessage;
    class CameraMessage;
    class AssetMessage;
    class EntityMessage;
    class SessionMessage;


    class EventExecutor;
    class FrameExecutor;
    class CPUThreadPoolExecutor;
    class IOThreadPoolExecutor;

}

namespace folly
{
    class ManualExecutor;
}