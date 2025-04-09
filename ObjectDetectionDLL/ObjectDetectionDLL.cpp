#include "pch.h"
#define OBJECTDETECTIONDLL_EXPORTS
#include "ObjectDetectionDLL.h"
#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>
#include <memory>
#include <vector>
#include <algorithm>

static Ort::Env* env = nullptr;
static std::unique_ptr<Ort::Session> session = nullptr;
static std::vector<std::string> classNames = { "drone" };

extern "C" OBJECTDETECTIONDLL_API bool InitializeModel(const char* modelPath) {
    try {
        env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, "YOLOv11");
        Ort::SessionOptions sessionOptions;
        sessionOptions.SetIntraOpNumThreads(4);
        sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

#ifdef _WIN32
        std::wstring wModelPath(modelPath, modelPath + strlen(modelPath));
        session = std::make_unique<Ort::Session>(*env, wModelPath.c_str(), sessionOptions);
#else
        session = std::make_unique<Ort::Session>(*env, modelPath, sessionOptions);
#endif
        return true;
    }
    catch (const std::exception&) {
        return false;
    }
}

void NMSBoxes(std::vector<DetectionResult>& detections, float iouThreshold) {
    std::sort(detections.begin(), detections.end(),
        [](const DetectionResult& a, const DetectionResult& b) { return a.confidence > b.confidence; });

    std::vector<bool> keep(detections.size(), true);
    for (size_t i = 0; i < detections.size(); ++i) {
        if (!keep[i]) continue;
        for (size_t j = i + 1; j < detections.size(); ++j) {
            if (!keep[j]) continue;
            float x1 = std::max(detections[i].x, detections[j].x);
            float y1 = std::max(detections[i].y, detections[j].y);
            float x2 = std::min(detections[i].x + detections[i].width, detections[j].x + detections[j].width);
            float y2 = std::min(detections[i].y + detections[i].height, detections[j].y + detections[j].height);

            float inter = std::max(0.0f, x2 - x1) * std::max(0.0f, y2 - y1);
            float area_i = detections[i].width * detections[i].height;
            float area_j = detections[j].width * detections[j].height;
            float iou = inter / (area_i + area_j - inter);

            if (iou > iouThreshold) keep[j] = false;
        }
    }

    std::vector<DetectionResult> filtered;
    for (size_t i = 0; i < detections.size(); ++i) {
        if (keep[i]) filtered.push_back(detections[i]);
    }
    detections = filtered;
}

extern "C" OBJECTDETECTIONDLL_API bool DetectObjects(const char* imagePath, DetectionResult** results, int* resultCount) {
    *results = nullptr;
    *resultCount = 0;

    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        return false;
    }

    try {
        cv::Mat input_image;
        cv::resize(image, input_image, cv::Size(640, 640));
        input_image.convertTo(input_image, CV_32FC3, 1.0 / 255.0);

        std::vector<int64_t> input_shape = { 1, 3, 640, 640 };
        std::vector<float> input_data(1 * 3 * 640 * 640);
        for (int c = 0; c < 3; c++) {
            for (int h = 0; h < 640; h++) {
                for (int w = 0; w < 640; w++) {
                    input_data[c * 640 * 640 + h * 640 + w] = input_image.at<cv::Vec3f>(h, w)[c];
                }
            }
        }

        Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
        Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
            memory_info, input_data.data(), input_data.size(), input_shape.data(), input_shape.size());

        const char* input_names[] = { "images" };
        const char* output_names[] = { "output0" };
        std::vector<Ort::Value> output_tensors = session->Run(
            Ort::RunOptions{ nullptr }, input_names, &input_tensor, 1, output_names, 1);

        float* output_data = output_tensors[0].GetTensorMutableData<float>();
        auto shape = output_tensors[0].GetTensorTypeAndShapeInfo().GetShape();
        int num_features = static_cast<int>(shape[1]); // e.g., 5
        int num_detections = static_cast<int>(shape[2]); // e.g., 8400

        int num_classes = num_features - 4;
        std::vector<DetectionResult> temp_results;

        for (int d = 0; d < num_detections; ++d) {
            float center_x = output_data[0 * num_detections + d];
            float center_y = output_data[1 * num_detections + d];
            float width = output_data[2 * num_detections + d];
            float height = output_data[3 * num_detections + d];

            float max_score = -1.0f;
            int class_id = -1;
            for (int c = 0; c < num_classes; ++c) {
                float score = output_data[(4 + c) * num_detections + d];
                if (score > max_score) {
                    max_score = score;
                    class_id = c;
                }
            }

            if (max_score > 1.0f) max_score /= 10.0f;

            if (max_score > 0.5 && (num_classes == 1 || classNames[class_id] == "drone")) {
                DetectionResult result;
                result.x = static_cast<int>(center_x - width / 2);
                result.y = static_cast<int>(center_y - height / 2);
                result.width = static_cast<int>(width);
                result.height = static_cast<int>(height);
                result.confidence = max_score;
                result.label = "drone";

                temp_results.push_back(result);
            }
        }

        NMSBoxes(temp_results, 0.45f);

        if (temp_results.empty()) {
            return true;
        }

        *resultCount = static_cast<int>(temp_results.size());
        *results = new DetectionResult[*resultCount];
        for (int i = 0; i < *resultCount; ++i) {
            (*results)[i] = temp_results[i];
        }

        return true;
    }
    catch (const std::exception&) {
        return false;
    }
}