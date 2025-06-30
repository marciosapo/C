#include <QApplication>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

std::vector<std::string> keys;
std::string textToDetect;
std::string soundFilePath;
bool isRunning = false;
int delayTime = 500;

// Press a key using Windows API
void pressKey(const std::string& key) {
    for (char c : key) {
        INPUT input{};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = VkKeyScanA(c); // Convert char to virtual key code
        SendInput(1, &input, sizeof(INPUT));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Release key
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &input, sizeof(INPUT));
    }
}

// Function to play a sound file
void playSound(const std::string& filePath) {
    PlaySound(filePath.c_str(), nullptr, SND_FILENAME | SND_ASYNC);
}

// Function to detect text using Tesseract OCR
bool detectText(const std::string& windowTitle, const std::string& text) {
    HWND hwnd = FindWindow(nullptr, windowTitle.c_str());
    if (!hwnd) {
        QMessageBox::warning(nullptr, "Error", "Window not found.");
        return false;
    }

    RECT rect;
    GetWindowRect(hwnd, &rect);

    // Capture screenshot
    HDC hdcScreen = GetDC(nullptr);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, rect.right - rect.left, rect.bottom - rect.top);
    SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcScreen, rect.left, rect.top, SRCCOPY);

    // Save to file
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = rect.right - rect.left;
    bi.biHeight = rect.bottom - rect.top;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((rect.right - rect.left) * bi.biBitCount + 31) / 32 * 4 * (rect.bottom - rect.top);
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);
    GetDIBits(hdcScreen, hBitmap, 0, (UINT)(rect.bottom - rect.top), lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    std::ofstream file("screenshot.bmp", std::ios::binary);
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfType = 0x4D42; // BM
    file.write((char*)&bmfHeader, sizeof(BITMAPFILEHEADER));
    file.write((char*)&bi, sizeof(BITMAPINFOHEADER));
    file.write(lpbitmap, dwBmpSize);
    file.close();

    // Release resources
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(nullptr, hdcScreen);

    // Use Tesseract OCR
    tesseract::TessBaseAPI ocr;
    if (ocr.Init(nullptr, "eng")) {
        QMessageBox::warning(nullptr, "Error", "Could not initialize Tesseract.");
        return false;
    }

    Pix* image = pixRead("screenshot.bmp");
    ocr.SetImage(image);
    std::string extractedText = ocr.GetUTF8Text();
    pixDestroy(&image);

    // Check if text is found
    return extractedText.find(text) != std::string::npos;
}

// Main GUI class
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow() {
        auto* centralWidget = new QWidget;
        auto* layout = new QVBoxLayout;

        // Window title
        windowTitleInput = new QLineEdit;
        layout->addWidget(new QLabel("Window Title:"));
        layout->addWidget(windowTitleInput);

        // Key input
        keyInput = new QLineEdit;
        layout->addWidget(new QLabel("Keys (comma-separated):"));
        layout->addWidget(keyInput);

        auto* addKeyButton = new QPushButton("Add Keys");
        connect(addKeyButton, &QPushButton::clicked, this, &MainWindow::addKeys);
        layout->addWidget(addKeyButton);

        // Text to detect
        textInput = new QLineEdit;
        layout->addWidget(new QLabel("Text to Detect:"));
        layout->addWidget(textInput);

        auto* detectTextButton = new QPushButton("Detect Text");
        connect(detectTextButton, &QPushButton::clicked, this, &MainWindow::detectText);
        layout->addWidget(detectTextButton);

        // Sound file
        auto* chooseSoundButton = new QPushButton("Choose Sound");
        connect(chooseSoundButton, &QPushButton::clicked, this, &MainWindow::chooseSoundFile);
        layout->addWidget(chooseSoundButton);

        soundLabel = new QLabel("Sound File: None");
        layout->addWidget(soundLabel);

        // Delay
        delayInput = new QLineEdit("500");
        layout->addWidget(new QLabel("Delay (ms):"));
        layout->addWidget(delayInput);

        auto* setDelayButton = new QPushButton("Set Delay");
        connect(setDelayButton, &QPushButton::clicked, this, &MainWindow::setDelay);
        layout->addWidget(setDelayButton);

        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);
    }

private slots:
    void addKeys() {
        keys.clear();
        QString input = keyInput->text();
        for (const auto& key : input.split(',')) {
            keys.push_back(key.trimmed().toStdString());
        }
        QMessageBox::information(this, "Keys Added", "Keys have been added successfully.");
    }

    void detectText() {
        QString windowTitle = windowTitleInput->text();
        QString searchText = textInput->text();
        if (detectText(windowTitle.toStdString(), searchText.toStdString())) {
            QMessageBox::information(this, "Text Found", "The text was detected.");
            if (!soundFilePath.empty()) {
                playSound(soundFilePath);
            }
        } else {
            QMessageBox::warning(this, "Text Not Found", "The text was not detected.");
        }
    }

    void chooseSoundFile() {
        QString filePath = QFileDialog::getOpenFileName(this, "Choose Sound File", "", "Audio Files (*.wav *.mp3)");
        if (!filePath.isEmpty()) {
            soundFilePath = filePath.toStdString();
            soundLabel->setText("Sound File: " + filePath);
        }
    }

    void setDelay() {
        bool ok;
        int delay = delayInput->text().toInt(&ok);
        if (ok) {
            delayTime = delay;
            QMessageBox::information(this, "Delay Set", "Delay has been updated.");
        } else {
            QMessageBox::warning(this, "Invalid Input", "Please enter a valid delay.");
        }
    }

private:
    QLineEdit* windowTitleInput;
    QLineEdit* keyInput;
    QLineEdit* textInput;
    QLineEdit* delayInput;
    QLabel* soundLabel;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
