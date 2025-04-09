#pragma once

#ifdef OBJECTDETECTIONDLL_EXPORTS
#define OBJECTDETECTIONDLL_API __declspec(dllexport)
#else
#define OBJECTDETECTIONDLL_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

struct DetectionResult {
    int x, y, width, height;
    float confidence;
    std::string label;
};

extern "C" OBJECTDETECTIONDLL_API bool InitializeModel(const char* modelPath);

// Modified function declaration
extern "C" OBJECTDETECTIONDLL_API bool DetectObjects(const char* imagePath, DetectionResult** results, int* resultCount);
//extern "C" OBJECTDETECTIONDLL_API bool DetectObjects(const wchar_t* imagePath, DetectionResult** results, int* resultCount);
