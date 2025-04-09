//#include <windows.h>
//#include <commctrl.h>
//#include <string>
//#include "..\ObjectDetectionDLL\ObjectDetectionDLL.h" // Ensure this path matches your setup
//
//#pragma comment(lib, "comctl32.lib")
//
//#define IDC_BROWSE 101
//#define IDC_DETECT 102
//#define IDC_GRID   103
//
//HWND hGrid;
//std::string imagePath;
//
//// Window procedure
//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//    switch (msg) {
//    case WM_CREATE: {
//        INITCOMMONCONTROLSEX icex = { sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES };
//        InitCommonControlsEx(&icex);
//
//        CreateWindowW(L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//            10, 10, 100, 30, hwnd, (HMENU)IDC_BROWSE, NULL, NULL);
//
//        CreateWindowW(L"BUTTON", L"Detect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//            120, 10, 100, 30, hwnd, (HMENU)IDC_DETECT, NULL, NULL);
//
//        hGrid = CreateWindowW(WC_LISTVIEWW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
//            10, 50, 500, 300, hwnd, (HMENU)IDC_GRID, NULL, NULL);
//
//        LVCOLUMNW col = { 0 };
//        col.mask = LVCF_TEXT | LVCF_WIDTH;
//        col.cx = 100;
//        col.pszText = (LPWSTR)L"Label"; ListView_InsertColumn(hGrid, 0, &col);
//        col.pszText = (LPWSTR)L"Confidence"; col.cx = 80; ListView_InsertColumn(hGrid, 1, &col);
//        col.pszText = (LPWSTR)L"X"; col.cx = 60; ListView_InsertColumn(hGrid, 2, &col);
//        col.pszText = (LPWSTR)L"Y"; col.cx = 60; ListView_InsertColumn(hGrid, 3, &col);
//        col.pszText = (LPWSTR)L"Width"; col.cx = 60; ListView_InsertColumn(hGrid, 4, &col);
//        col.pszText = (LPWSTR)L"Height"; col.cx = 60; ListView_InsertColumn(hGrid, 5, &col);
//        break;
//    }
//    case WM_COMMAND:
//        switch (LOWORD(wParam)) {
//        case IDC_BROWSE: {
//            OPENFILENAMEW ofn = { 0 };
//            wchar_t szFile[MAX_PATH] = { 0 };
//            ofn.lStructSize = sizeof(ofn);
//            ofn.hwndOwner = hwnd;
//            ofn.lpstrFile = szFile;
//            ofn.nMaxFile = MAX_PATH;
//            ofn.lpstrFilter = L"Image Files\0*.jpg;*.png\0All Files\0*.*\0";
//            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//            if (GetOpenFileNameW(&ofn)) {
//                char narrowPath[MAX_PATH];
//                WideCharToMultiByte(CP_ACP, 0, szFile, -1, narrowPath, MAX_PATH, NULL, NULL);
//                imagePath = narrowPath;
//                MessageBoxW(hwnd, L"Image loaded successfully!", L"Success", MB_OK);
//            }
//            break;
//        }
//        case IDC_DETECT: {
//            if (imagePath.empty()) {
//                MessageBoxW(hwnd, L"Please load an image first!", L"Error", MB_OK);
//                break;
//            }
//
//            DetectionResult* results = nullptr;
//            int resultCount = 0;
//            imagePath = "D:/projects_iklab/a.jpg";
//            bool success = DetectObjects("D:/projects_iklab/a.jpg", &results, &resultCount);
//
//            if (!success || resultCount == 0) {
//                MessageBoxW(hwnd, L"No objects detected or detection failed!", L"Info", MB_OK);
//                if (results) delete[] results; // Free memory if allocated
//                break;
//            }
//
//            ListView_DeleteAllItems(hGrid);
//            for (int i = 0; i < resultCount; i++) {
//                LVITEMW item = { 0 };
//                item.mask = LVIF_TEXT;
//                item.iItem = i;
//                std::wstring wLabel(results[i].label.begin(), results[i].label.end());
//                item.pszText = (LPWSTR)wLabel.c_str();
//                ListView_InsertItem(hGrid, &item);
//
//                wchar_t buffer[32];
//                swprintf_s(buffer, L"%.2f", results[i].confidence); ListView_SetItemText(hGrid, i, 1, buffer);
//                swprintf_s(buffer, L"%d", results[i].x); ListView_SetItemText(hGrid, i, 2, buffer);
//                swprintf_s(buffer, L"%d", results[i].y); ListView_SetItemText(hGrid, i, 3, buffer);
//                swprintf_s(buffer, L"%d", results[i].width); ListView_SetItemText(hGrid, i, 4, buffer);
//                swprintf_s(buffer, L"%d", results[i].height); ListView_SetItemText(hGrid, i, 5, buffer);
//            }
//
//            // Free the memory allocated by the DLL
//            if (results) delete[] results;
//            break;
//        }
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProcW(hwnd, msg, wParam, lParam);
//    }
//    return 0;
//}
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//    if (!InitializeModel("D:/projects_iklab/GROK/ObjectDetectionApp/x64/Debug/best.onnx")) { // Replace with your model path
//        MessageBoxW(NULL, L"Failed to initialize model!", L"Error", MB_OK | MB_ICONERROR);
//        return 1;
//    }
//
//    WNDCLASSW wc = { 0 };
//    wc.lpfnWndProc = WndProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = L"ObjectDetectionAppClass";
//    RegisterClassW(&wc);
//
//    HWND hwnd = CreateWindowW(L"ObjectDetectionAppClass", L"Object Detection App",
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 550, 400,
//        NULL, NULL, hInstance, NULL);
//    if (!hwnd) {
//        return 1;
//    }
//
//    ShowWindow(hwnd, nCmdShow);
//    UpdateWindow(hwnd);
//
//    MSG msg;
//    while (GetMessageW(&msg, NULL, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessageW(&msg);
//    }
//    return (int)msg.wParam;
//}
//
//



//#include <windows.h>
//#include <commctrl.h>
//#include <string>
//#include <opencv2/opencv.hpp>
//#include "..\ObjectDetectionDLL\ObjectDetectionDLL.h"
//
//#pragma comment(lib, "comctl32.lib")
////#pragma comment(lib, "opencv_world490d.lib")
//
//#define IDC_BROWSE 101
//#define IDC_DETECT 102
//#define IDC_GRID   103
//
//HWND hGrid;
//std::string imagePath;
//
//std::string FixPath(const std::string& path) {
//    std::string fixedPath = path;
//    for (size_t i = 0; i < fixedPath.length(); ++i) {
//        if (fixedPath[i] == '\\') {
//            fixedPath.replace(i, 1, "\\\\");
//            i++;
//        }
//    }
//    return fixedPath;
//}
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//    switch (msg) {
//    case WM_CREATE: {
//        INITCOMMONCONTROLSEX icex = { sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES };
//        InitCommonControlsEx(&icex);
//
//        CreateWindowW(L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//            10, 10, 100, 30, hwnd, (HMENU)IDC_BROWSE, NULL, NULL);
//
//        CreateWindowW(L"BUTTON", L"Detect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
//            120, 10, 100, 30, hwnd, (HMENU)IDC_DETECT, NULL, NULL);
//
//        hGrid = CreateWindowW(WC_LISTVIEWW, L"", WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
//            10, 50, 500, 300, hwnd, (HMENU)IDC_GRID, NULL, NULL);
//
//        LVCOLUMNW col = { 0 };
//        col.mask = LVCF_TEXT | LVCF_WIDTH;
//        col.cx = 100;
//        col.pszText = (LPWSTR)L"Label"; ListView_InsertColumn(hGrid, 0, &col);
//        col.pszText = (LPWSTR)L"Confidence"; col.cx = 80; ListView_InsertColumn(hGrid, 1, &col);
//        col.pszText = (LPWSTR)L"X"; col.cx = 60; ListView_InsertColumn(hGrid, 2, &col);
//        col.pszText = (LPWSTR)L"Y"; col.cx = 60; ListView_InsertColumn(hGrid, 3, &col);
//        col.pszText = (LPWSTR)L"Width"; col.cx = 60; ListView_InsertColumn(hGrid, 4, &col);
//        col.pszText = (LPWSTR)L"Height"; col.cx = 60; ListView_InsertColumn(hGrid, 5, &col);
//        break;
//    }
//    case WM_COMMAND:
//        switch (LOWORD(wParam)) {
//        case IDC_BROWSE: {
//            OPENFILENAMEW ofn = { 0 };
//            wchar_t szFile[MAX_PATH] = { 0 };
//            ofn.lStructSize = sizeof(ofn);
//            ofn.hwndOwner = hwnd;
//            ofn.lpstrFile = szFile;
//            ofn.nMaxFile = MAX_PATH;
//            ofn.lpstrFilter = L"Image Files\0*.jpg;*.png\0All Files\0*.*\0";
//            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//            if (GetOpenFileNameW(&ofn)) {
//                char narrowPath[MAX_PATH];
//                WideCharToMultiByte(CP_ACP, 0, szFile, -1, narrowPath, MAX_PATH, NULL, NULL);
//                imagePath = FixPath(narrowPath);
//                MessageBoxW(hwnd, L"Image loaded successfully!", L"Success", MB_OK);
//            }
//            break;
//        }
//        case IDC_DETECT: {
//            if (imagePath.empty()) {
//                MessageBoxW(hwnd, L"Please load an image first!", L"Error", MB_OK);
//                break;
//            }
//
//            DetectionResult* results = nullptr;
//            int resultCount = 0;
//            bool success = DetectObjects(imagePath.c_str(), &results, &resultCount);
//
//            if (!success || resultCount == 0) {
//                MessageBoxW(hwnd, L"No drone detections found!", L"Info", MB_OK);
//                if (results) delete[] results;
//                break;
//            }
//
//            // Load and resize image to 640x640 for drawing
//            cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
//            if (img.empty()) {
//                MessageBoxW(hwnd, L"Failed to reload image for drawing!", L"Error", MB_OK);
//                if (results) delete[] results;
//                break;
//            }
//            cv::resize(img, img, cv::Size(640, 640));
//
//            // Draw raw bounding boxes
//            for (int i = 0; i < resultCount; i++) {
//                int x = results[i].x;
//                int y = results[i].y;
//                int w = results[i].width;
//                int h = results[i].height;
//                cv::rectangle(img, cv::Point(x, y), cv::Point(x + w, y + h), cv::Scalar(0, 255, 0), 2);
//                char label[32];
//                sprintf_s(label, "drone %.2f", results[i].confidence);
//                cv::putText(img, label, cv::Point(x, y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
//            }
//
//            cv::imshow("Raw Drone Detections", img);
//            cv::waitKey(0);
//            cv::destroyWindow("Raw Drone Detections");
//
//            // Update grid
//            ListView_DeleteAllItems(hGrid);
//            for (int i = 0; i < resultCount; i++) {
//                LVITEMW item = { 0 };
//                item.mask = LVIF_TEXT;
//                item.iItem = i;
//                std::wstring wLabel(results[i].label.begin(), results[i].label.end());
//                item.pszText = (LPWSTR)wLabel.c_str();
//                ListView_InsertItem(hGrid, &item);
//
//                wchar_t buffer[32];
//                swprintf_s(buffer, L"%.2f", results[i].confidence); ListView_SetItemText(hGrid, i, 1, buffer);
//                swprintf_s(buffer, L"%d", results[i].x); ListView_SetItemText(hGrid, i, 2, buffer);
//                swprintf_s(buffer, L"%d", results[i].y); ListView_SetItemText(hGrid, i, 3, buffer);
//                swprintf_s(buffer, L"%d", results[i].width); ListView_SetItemText(hGrid, i, 4, buffer);
//                swprintf_s(buffer, L"%d", results[i].height); ListView_SetItemText(hGrid, i, 5, buffer);
//            }
//
//            if (results) delete[] results;
//            break;
//        }
//        }
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    default:
//        return DefWindowProcW(hwnd, msg, wParam, lParam);
//    }
//    return 0;
//}
//
//int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
//    if (!InitializeModel("D:/projects_iklab/GROK/ObjectDetectionApp/x64/Debug/best.onnx")) { // Update this path
//        MessageBoxW(NULL, L"Failed to initialize model!", L"Error", MB_OK | MB_ICONERROR);
//        return 1;
//    }
//
//    WNDCLASSW wc = { 0 };
//    wc.lpfnWndProc = WndProc;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = L"ObjectDetectionAppClass";
//    RegisterClassW(&wc);
//
//    HWND hwnd = CreateWindowW(L"ObjectDetectionAppClass", L"Drone Detection App",
//        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 550, 400,
//        NULL, NULL, hInstance, NULL);
//    if (!hwnd) {
//        return 1;
//    }
//
//    ShowWindow(hwnd, nCmdShow);
//    UpdateWindow(hwnd);
//
//    MSG msg;
//    while (GetMessageW(&msg, NULL, 0, 0)) {
//        TranslateMessage(&msg);
//        DispatchMessageW(&msg);
//    }
//    return (int)msg.wParam;
//}













#include <windows.h>
#include <commctrl.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "..\ObjectDetectionDLL\ObjectDetectionDLL.h"

#pragma comment(lib, "comctl32.lib")
//#pragma comment(lib, "opencv_world490d.lib")

#define IDC_BROWSE 101
#define IDC_DETECT 102
#define IDC_IMAGE  103

HWND hImageWnd;
std::string imagePath;
cv::Mat currentImage;

std::string FixPath(const std::string& path) {
    std::string fixedPath = path;
    for (size_t i = 0; i < fixedPath.length(); ++i) {
        if (fixedPath[i] == '\\') {
            fixedPath.replace(i, 1, "\\\\");
            i++;
        }
    }
    return fixedPath;

}




std::string GetExecutableDirectory() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring wExePath(exePath);
    size_t lastSlash = wExePath.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        wExePath = wExePath.substr(0, lastSlash + 1);
    }
    char narrowPath[MAX_PATH];
    WideCharToMultiByte(CP_ACP, 0, wExePath.c_str(), -1, narrowPath, MAX_PATH, NULL, NULL);
    return std::string(narrowPath);
}

void UpdateImageDisplay(HWND hwnd, const cv::Mat& img) {
    if (img.empty()) return;

    // Convert OpenCV Mat to HBITMAP
    BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), img.cols, -img.rows, 1, 24, BI_RGB, 0, 0, 0, 0, 0 };
    HDC hdc = GetDC(hwnd);
    HBITMAP hBitmap = CreateDIBitmap(hdc, &bi, CBM_INIT, img.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    ReleaseDC(hwnd, hdc);

    // Set bitmap to static control
    SendMessage(hImageWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

    // Clean up old bitmap if exists
    HBITMAP hOldBitmap = (HBITMAP)SendMessage(hImageWnd, STM_GETIMAGE, 0, 0);
    if (hOldBitmap && hOldBitmap != hBitmap) DeleteObject(hOldBitmap);

    currentImage = img.clone();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        CreateWindowW(L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 10, 100, 30, hwnd, (HMENU)IDC_BROWSE, NULL, NULL);

        CreateWindowW(L"BUTTON", L"Detect", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 10, 100, 30, hwnd, (HMENU)IDC_DETECT, NULL, NULL);

        hImageWnd = CreateWindowW(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP,
            10, 50, 640, 640, hwnd, (HMENU)IDC_IMAGE, NULL, NULL);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDC_BROWSE: {
            OPENFILENAMEW ofn = { 0 };
            wchar_t szFile[MAX_PATH] = { 0 };
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrFilter = L"Image Files\0*.jpg;*.png\0All Files\0*.*\0";
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if (GetOpenFileNameW(&ofn)) {
                char narrowPath[MAX_PATH];
                WideCharToMultiByte(CP_ACP, 0, szFile, -1, narrowPath, MAX_PATH, NULL, NULL);
                imagePath = FixPath(narrowPath);

                cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);
                if (!img.empty()) {
                    cv::resize(img, img, cv::Size(640, 640));
                    UpdateImageDisplay(hwnd, img);
                }
                else {
                    MessageBoxW(hwnd, L"Failed to load image!", L"Error", MB_OK | MB_ICONERROR);
                }
            }
            break;
        }
        case IDC_DETECT: {
            if (imagePath.empty() || currentImage.empty()) {
                MessageBoxW(hwnd, L"Please load an image first!", L"Error", MB_OK);
                break;
            }

            DetectionResult* results = nullptr;
            int resultCount = 0;
            bool success = DetectObjects(imagePath.c_str(), &results, &resultCount);

            if (!success || resultCount == 0) {
                MessageBoxW(hwnd, L"No drone detections found!", L"Info", MB_OK);
                if (results) delete[] results;
                break;
            }

            // Draw bounding box on the current image
            cv::Mat img = currentImage.clone();
            for (int i = 0; i < resultCount; i++) {
                int x = results[i].x;
                int y = results[i].y;
                int w = results[i].width;
                int h = results[i].height;
                cv::rectangle(img, cv::Point(x, y), cv::Point(x + w, y + h), cv::Scalar(0, 255, 0), 2);
            }

            UpdateImageDisplay(hwnd, img);

            if (results) delete[] results;
            break;
        }
        }
        break;
    case WM_DESTROY:
        if (!currentImage.empty()) currentImage.release();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    std::string exeDir = GetExecutableDirectory();
    std::string modelPath = exeDir + "best.onnx"; // Assumes model is named yolov11m.onnx

    if (!InitializeModel(modelPath.c_str())) {
        MessageBoxW(NULL, L"Failed to initialize model! Ensure yolov11m.onnx is in the same directory as the executable.",
            L"Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ObjectDetectionAppClass";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(L"ObjectDetectionAppClass", L"Drone Detection App",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 700, 750,
        NULL, NULL, hInstance, NULL);
    if (!hwnd) {
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}