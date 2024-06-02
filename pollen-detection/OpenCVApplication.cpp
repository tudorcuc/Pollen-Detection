#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;
const int CLASSES = 2;


bool thresholdIsEmpty = true;

enum tag
{
    POLLEN,
    NOPOLLEN
} tagEnum;

const char* tagList[] =
{
    "pollen",
    "nopollen"
};

map<string, int> tagToInt = {
    {"pollen", POLLEN},
    {"nopollen", NOPOLLEN}

};

char folderPath[] = ".\\dataset";

struct HSVThreshold {
    vector<double> mean;
    int count;
    HSVThreshold() : mean(0), count(0) {}
};

vector<string> testSet;
vector<string> trainSet;
vector<string> imgSet;
vector<int> tagTestSet;
vector<int> tagTrainSet;
vector<HSVThreshold> classThresholds(CLASSES);

int nrImg;
int nrTag;
int accMat[CLASSES][CLASSES] = { 0 };
int confusionMatrix[CLASSES][CLASSES] = { 0 };


void wait() {
    int c;
    cout << endl << "Press enter to continue.";
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

void createTagSet() {
    nrTag = 0;
    tagTrainSet.clear();
    tagTestSet.clear();

    for (const auto& img : trainSet) {
        stringstream str(img);
        string tagName, lastWord, word;
        while (getline(str, word, '\\')) {
            tagName = lastWord;
            lastWord = word;
        }
        tagTrainSet.push_back(tagToInt[tagName]);
        nrTag++;
    }
    for (const auto& img : testSet) {
        stringstream str(img);
        string tagName, lastWord, word;
        while (getline(str, word, '\\')) {
            tagName = lastWord;
            lastWord = word;
        }
        tagTestSet.push_back(tagToInt[tagName]);
        nrTag++;
    }
}

void openImages()
{
    char file[MAX_PATH];
    char subfolder[MAX_PATH];

    nrImg = 0;
    trainSet.clear();
    testSet.clear();

    for (int i = 0; i < sizeof(tagList) / sizeof(char*); i++) {
        strcpy(subfolder, folderPath);
        strcat(subfolder, "\\");
        strcat(subfolder, tagList[i]);

        imgSet.clear();

        FileGetter fg(subfolder, "jpg");
        while (fg.getNextAbsFile(file))
        {

            nrImg++;
            imgSet.push_back(file);
        }
        sort(imgSet.begin(), imgSet.end());

        int index = 0;
        for (const auto& img : imgSet) {
            if (index++ < imgSet.size() / 2)
                trainSet.push_back(img);
            else
                testSet.push_back(img);
        }
    }
    sort(trainSet.begin(), trainSet.end());
    sort(testSet.begin(), testSet.end());

    createTagSet();
}

void openImagesBatch() {
    char file[MAX_PATH];
    char subfolder[MAX_PATH];

    nrImg = 0;

    int imgCount = 0;

    for (int i = 0; i < sizeof(tagList) / sizeof(char*); i++) {
        strcpy(subfolder, folderPath);
        strcat(subfolder, "\\");
        strcat(subfolder, tagList[i]);

        imgSet.clear();

        FileGetter fg(subfolder, "jpg");
        while (fg.getNextAbsFile(file)) {
            if (imgCount >= 5)
                return;

            Mat src;
            src = imread(file);
            imshow(fg.getFoundFileName(), src);
            imgCount++;

            if (waitKey() == 27) // ESC pressed
                break;
        }

    }
}

const int PROPERTIES = 6;

void verifyNbOfImages() {
    openImages();
    if (nrImg == 20 && nrTag == 20) {
        printf("\nTest verify nb of images/tags successfully\n");
    }
    else {
        printf("\nYou made a mistake in counting the files\nnrImg = %d; nrTag = %d.", nrImg, nrTag);
    }
}


vector<int> generateRandomTags(const vector<string>& images) {
    vector<int> randomTags;
    randomTags.reserve(images.size());

    for (const auto& img : images) {
        int randomTag = rand() % 2;
        randomTags.push_back(randomTag);
    }

    return randomTags;
}




void accuracy(vector<int> newTagTestSet) {
    double ok = 0;

    for (size_t i = 0; i < newTagTestSet.size(); i++) {
        if (newTagTestSet[i] == tagTestSet[i]) {
            ok++;
        }
    }

    double acc = (ok / newTagTestSet.size()) * 100.0;
    printf("Accuracy: %f\n", acc);
    printf("TagTest:%d", newTagTestSet.size());

}

void testAccuracy() {

    if (tagTestSet.size() == 0) {
        openImages();
    }

    vector<int> newTagTestSet = generateRandomTags(testSet);
    accuracy(newTagTestSet);
}

void printLines(int index) {
    printf("\n___________|");
    for (int i = 0; i < 2; i++) {
        if (i == index) {
            printf("|______||");
        }
        else {
            printf("________|");
        }
    }
    printf("\n");
}

void printAccuracyMat() {
    printf("           |");
    for (int i = 0; i < CLASSES; i++) {
        printf("%8.7s|", tagList[i]);
    }
    for (int i = 0; i < CLASSES; i++) {
        printLines(i - 1);
        printf("%10s |", tagList[i]);
        for (int j = 0; j < CLASSES; j++) {
            if (i == j) {
                printf("|%6d||", accMat[i][j]);
            }
            else {
                printf("%7d |", accMat[i][j]);
            }
        }
    }
    printLines(10);
}

void printConfusionMatrix() {
    cout << "Confusion Matrix:\n";
    for (int i = 0; i < CLASSES; ++i) {
        for (int j = 0; j < CLASSES; ++j) {
            cout << confusionMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

bool hasPollen(const std::string& imagePath) {
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return false;
    }


    cv::Mat hsvColor(1, 1, CV_8UC3, cv::Scalar(255, 255, 152));
    cv::cvtColor(hsvColor, hsvColor, cv::COLOR_BGR2HSV);
    cv::Vec3b hsvPixel = hsvColor.at<cv::Vec3b>(0, 0);
    int hue = hsvPixel[0];
    int saturation = hsvPixel[1];
    int value = hsvPixel[2];


    int hueThreshold = 10;
    int saturationThreshold = 50;
    int valueThreshold = 50;


    int lowerHue = (hue > hueThreshold) ? hue - hueThreshold : 0;
    int upperHue = (hue < 180 - hueThreshold) ? hue + hueThreshold : 180;


    cv::Scalar lowerColor(lowerHue, saturation - saturationThreshold, value - valueThreshold);
    cv::Scalar upperColor(upperHue, saturation + saturationThreshold, value + valueThreshold);

    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    cv::Mat mask;
    cv::inRange(hsvImage, lowerColor, upperColor, mask);

    double colorPercentage = cv::countNonZero(mask) / (double)(image.rows * image.cols);


    return (colorPercentage > 0.3);
}
int generateTagFromImage(const std::string& imagePath) {

    cv::Scalar dominantColor = hasPollen(imagePath);


    if (dominantColor[0] > 240 && dominantColor[1] < 240 && dominantColor[2] < 150) {
        return POLLEN;
    }
    else {
        return NOPOLLEN;
    }
}

void resetAccMat() {
    for (int i = 0; i < CLASSES; i++) {
        for (int j = 0; j < CLASSES; j++) {
            accMat[i][j] = 0;
        }
    }
}

double euclideanDistance(const vector<double>& v1, const vector<double>& v2) {
    double sum = 0;
    for (int i = 0; i < PROPERTIES; i++) {
        double dif = v1.at(i) - v2.at(i);
        sum += dif * dif;
    }
    return std::sqrt(sum);
}

int closestHSVandRGB(const vector<double>& imageProp) {
    int index = 0;
    double minDist = euclideanDistance(imageProp, classThresholds[0].mean);
    for (int i = 1; i < CLASSES; i++) {
        double nextDist = euclideanDistance(imageProp, classThresholds[i].mean);
        if (nextDist < minDist) {
            minDist = nextDist;
            index = i;
        }
    }
    return index;
}

void saveImage(const string& img, int actual, int predicted) {
    Mat image = imread(img);
    if (image.empty()) {
        printf("Could not read the image: %s\n", img.c_str());
        return;
    }

    string imgName = img.substr(img.find_last_of("\\/") + 1);
    string folderPath = ".\\wrongPredictions";
    string path = folderPath + "\\" + to_string(actual) + "" + to_string(predicted) + "" + imgName;

    if (!imwrite(path, image)) {
        printf("Could not save the image at path: %s\n", path.c_str());
    }
}

void predictAndUpdateAccMat() {
    if (testSet.empty()) {
        openImages();
    }

    for (size_t i = 0; i < testSet.size(); ++i) {
        int actualTag = tagTestSet[i];
        string img = testSet[i];
        Mat image = imread(img);
        if (image.empty()) {
            printf("Could not read the img\n");
            continue;
        }

        Mat hsvImage;
        cvtColor(image, hsvImage, COLOR_BGR2HSV);
        Scalar meanHSV = mean(hsvImage);
        Scalar meanRGB = mean(image);

        vector<double> mean;
        for (int j = 0; j < 3; j++) {
            mean.push_back(meanHSV[j]);
        }
        for (int j = 0; j < 3; j++) {
            mean.push_back(meanRGB[j]);
        }

        int predictedTag = closestHSVandRGB(mean);

        accMat[predictedTag][actualTag]++;

        if (actualTag != predictedTag) {
            saveImage(img, actualTag, predictedTag);
        }
    }
}

void calculateMeanHSVMeanRGBForTags() {
    if (thresholdIsEmpty) {
        thresholdIsEmpty = false;

        if (trainSet.empty()) {
            openImages();
        }

        for (auto& threshold : classThresholds) {
            threshold.mean.clear();
            for (int i = 0; i < PROPERTIES; i++) {
                threshold.mean.push_back(0);
            }
            threshold.count = 0;
        }

        for (size_t i = 0; i < trainSet.size(); i++) {
            Mat image = imread(trainSet[i]);
            if (image.empty()) {
                printf("Could not read the img\n");
                continue;
            }

            Mat hsvImage;
            cvtColor(image, hsvImage, COLOR_BGR2HSV);

            Scalar meanHSV = mean(hsvImage);
            Scalar meanRGB = mean(image);

            int tagIndex = tagTrainSet[i];
            for (int j = 0; j < 3; j++) {
                classThresholds[tagIndex].mean.at(j) += meanHSV(j);
                classThresholds[tagIndex].mean.at(j + 3) += meanRGB(j);
            }
            classThresholds[tagIndex].count++;

        }

        for (auto& threshold : classThresholds) {
            if (threshold.count > 0) {
                for (int i = 0; i < PROPERTIES; i++) {
                    threshold.mean.at(i) /= threshold.count;
                }
            }
        }
    }
}

void testAccuracyPerClass() {
    resetAccMat();
    calculateMeanHSVMeanRGBForTags();
    predictAndUpdateAccMat();
    printAccuracyMat();
}


int main()
{
    srand(time(NULL));
    int op;
    do
    {
        system("cls");
        printf("Menu:\n");
        printf(" 1 - Main program\n");
        printf(" 2 - Testing number of images\n");
        printf(" 3 - Calculate accuracy\n");
        printf(" 4 - Calculate accuracy per class\n");
        printf("Option: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            openImagesBatch();
            break;
        case 2:
            verifyNbOfImages();
            break;
        case 3:
            testAccuracy();
            break;
        case 4:
            testAccuracyPerClass();
            break;
        }
        wait();

    } while (op != 0);

    return 0;
}
