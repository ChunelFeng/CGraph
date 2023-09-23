/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: E01-AutoPilot.cpp
@Time: 2023/9/15 20:03
@Desc: 本example主要展示，Camera 每隔1000ms，采集一张图片
       LaneDetector 和 CarDetector同时消费这张图片，计算对应的内容
       并且最终在Show中展示对应的结果信息
***************************/

#include <iostream>
#include <cstring>
#include <memory>

#include "CGraph.h"

using namespace CGraph;

static const int DEFAULT_IMAGE_SIZE = 64;
static const int DEFAULT_MESSAGE_BUF_SIZE = 16;
static const char* EXAMPLE_IMAGE_TOPIC = "/example/image/topic";
static const char* EXAMPLE_PARAM_KEY = "example-param-key";


struct ImageMParam : public GMessageParam {
    int frame_id_ = 0;
    char image_buf_[DEFAULT_IMAGE_SIZE] = {0};

    explicit ImageMParam() = default;

    ImageMParam(const ImageMParam& param) : GMessageParam(param) {
        this->frame_id_ = param.frame_id_;
        memcpy(image_buf_, param.image_buf_, DEFAULT_IMAGE_SIZE);
    }

    ImageMParam& operator=(const ImageMParam& param) {
        if (this == &param) {
            return *this;
        }

        this->frame_id_ = param.frame_id_;
        memcpy(image_buf_, param.image_buf_, DEFAULT_IMAGE_SIZE);
        return *this;
    }
};


struct DetectResultGParam : public GParam {
    int lane_num_ = 0;
    int car_num_ = 0;
    int frame_id_ = 0;
};


class CameraGDaemon : public GDaemon {
public:
    CVoid daemonTask(GDaemonParamPtr param) override {
        ImageMParam image;
        image.frame_id_ = cur_index_;
        std::string info = "this is " + std::to_string(cur_index_) + " image";
        memcpy(image.image_buf_, info.c_str(), info.length());
        cur_index_++;

        CGRAPH_PUB_MPARAM(ImageMParam, EXAMPLE_IMAGE_TOPIC, image, GMessagePushStrategy::WAIT);
    }

private:
    int cur_index_ = 0;
};


class LaneDetectorGNode : public GNode {
public:
    CStatus init() override {
        // 订阅 EXAMPLE_IMAGE_TOPIC 消息。每次 bind的返回值，是不一样的，用于区分
        conn_id_ = CGRAPH_BIND_MESSAGE_TOPIC(ImageMParam, EXAMPLE_IMAGE_TOPIC, DEFAULT_MESSAGE_BUF_SIZE)
        return CStatus();
    }

    CStatus run() override {
        std::unique_ptr<ImageMParam> image = nullptr;
        auto status = CGRAPH_SUB_MPARAM(ImageMParam, conn_id_, image);
        if (status.isErr()) {
            return status;
        }

        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(DetectResultGParam, EXAMPLE_PARAM_KEY)
        // detector lane in image.image_buf_ ...
        CGRAPH_ECHO("detecting lane in frame [%d], info is [%s]", image->frame_id_, image->image_buf_);
        param->frame_id_ = image->frame_id_;
        param->lane_num_ = std::abs((int)std::random_device{}()) % 10;
        return CStatus();
    }

private:
    int conn_id_ = 0;
};


class CarDetectorGNode : public GNode {
public:
    CStatus init() override {
        conn_id_ = CGRAPH_BIND_MESSAGE_TOPIC(ImageMParam, EXAMPLE_IMAGE_TOPIC, DEFAULT_MESSAGE_BUF_SIZE)
        return CStatus();
    }

    CStatus run() override {
        std::unique_ptr<ImageMParam> image = nullptr;
        auto status = CGRAPH_SUB_MPARAM(ImageMParam, conn_id_, image);
        if (status.isErr()) {
            return status;
        }

        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(DetectResultGParam, EXAMPLE_PARAM_KEY);
        CGRAPH_ECHO("finding car in frame [%d], info is [%s]", image->frame_id_, image->image_buf_);
        // find car in image.image_buf_ ...
        param->car_num_ = std::abs((int)std::random_device{}()) % 5;
        return CStatus();
    }

private:
    int conn_id_ = 0;
};


class ShowGNode : public GNode {
    CStatus run() override {
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(DetectResultGParam, EXAMPLE_PARAM_KEY);
        CGRAPH_ECHO("find [%d] car and [%d] lane in frame [%d] \n", param->car_num_, param->lane_num_, param->frame_id_);
        return CStatus();
    }
};


void example_auto_pilot() {
    GElementPtr lane, car, show = nullptr;
    auto pipeline = GPipelineFactory::create();

    pipeline->registerGElement<LaneDetectorGNode>(&lane, {}, "lane");
    pipeline->registerGElement<CarDetectorGNode>(&car, {}, "car");
    pipeline->registerGElement<ShowGNode>(&show, {lane, car}, "show");

    pipeline->createGParam<DetectResultGParam>(EXAMPLE_PARAM_KEY);
    pipeline->addGDaemon<CameraGDaemon>(1000);    // 模拟相机，每间隔1000ms，生成一张图片

    pipeline->process(10);
    GPipelineFactory::clear();
    CGRAPH_CLEAR_MESSAGES()
}


int main() {
    example_auto_pilot();
    return 0;
}
