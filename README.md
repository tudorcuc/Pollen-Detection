# Pollen Detection

## Overview

This program is designed to classify images into two categories: pollen and non-pollen, using the HSV and RGB color properties. It leverages color feature extraction techniques to differentiate between images containing pollen and those that do not. By calculating and analyzing the mean HSV and RGB values of images, the program builds a classification model that can be used to identify the presence of pollen in new images.

## Experimental Setup

### Data Preparation

The program expects a dataset organized into a specific directory structure. Within a main directory named `dataset`, there should be two subdirectories:

- `pollen`: This folder should contain images with pollen.
- `nopollen`: This folder should contain images without pollen.

All images should be in `.jpg` format. This organization helps the program easily identify and label images during the loading process.

## Experimental Details

### Methodology

The program follows a systematic approach to classify the images:

#### Data Loading

- The program loads images from the specified dataset path.
- Images are read using OpenCV and stored for processing.

#### Labeling

- Images are labeled based on the directory names (`pollen` or `nopollen`).
- This labeling is crucial for supervised learning, where each image's label is used to train the classification model.

#### Feature Calculation

- For each image, the program calculates the mean HSV (Hue, Saturation, Value) and RGB (Red, Green, Blue) values.
- These mean values serve as the primary features for distinguishing between pollen and non-pollen images.

#### Training

- The program uses the training set to compute the average mean HSV and RGB values for each class (pollen and non-pollen).
- These averaged values are used to establish threshold values that serve as decision boundaries for classification.

#### Testing

- Images in the test set are classified based on their mean HSV and RGB values.
- The classification is done by comparing each test image's feature values to the nearest class thresholds calculated during training.

## Functions

The program includes several key functions to perform its tasks:

### Confusion Matrix

- The confusion matrix displays the classification results, showing true positives, true negatives, false positives, and false negatives.
- This matrix is essential for evaluating the performance of the classification model.

### Accuracy Calculation

- Accuracy is calculated based on the test set results.
- It is displayed as a percentage, representing the proportion of correctly classified images out of the total test images.

## Observations

### Threshold Calculation

- The program calculates and stores threshold values for HSV and RGB features based on the training set.
- These thresholds are critical for the classification process, as they define the boundaries between the pollen and non-pollen classes.

### Image Classification

- Each image is classified based on how its mean HSV and RGB values compare to the calculated thresholds.
- The program assigns the class label (pollen or non-pollen) that has the nearest feature values.

### Accuracy

- The accuracy of the classification model is calculated and displayed.
- Higher accuracy indicates better performance of the model in distinguishing between pollen and non-pollen images.

## Conclusion

The program effectively classifies images into pollen and non-pollen categories using HSV and RGB color properties. By leveraging mean color values as features, it creates a simple yet effective classification model. However, there is always room for improvement. Future enhancements could include incorporating additional features such as texture, shape, or more advanced machine learning algorithms to further optimize the classification performance.
