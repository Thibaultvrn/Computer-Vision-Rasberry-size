
/*
 * Algorith.C (C++ version avec OpenCV 4)
 * Computer Vision pour détecter la maturité et la taille d'une framboise
 *
 * Compilation Windows:
 *   compile_simple.bat
 *
 * Usage:
 *   algorith.exe raspberry.jpg [scale_mm_per_pixel]
 *
 * Compatible avec Arduino via communication série
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

// Seuils HSV pour le rouge (OpenCV H range: 0-180)
// Le rouge entoure 0, donc deux intervalles
const Scalar RED_LOW1(0, 70, 50);
const Scalar RED_HIGH1(10, 255, 255);
const Scalar RED_LOW2(170, 70, 50);
const Scalar RED_HIGH2(180, 255, 255);

// Seuils HSV pour le vert (arrière-plan)
const Scalar GREEN_LOW(35, 40, 40);
const Scalar GREEN_HIGH(85, 255, 255);

// Paramètres de taille attendue
const double EXPECTED_MIN_MM = 10.0;
const double EXPECTED_MAX_MM = 30.0;
const double RIPENESS_THRESHOLD = 0.5;

int main(int argc, char** argv)
{
    const char* filename = "raspberry.jpg";
    double scale_mm_per_pixel = 0.5; // Valeur par défaut

    if (argc >= 2) filename = argv[1];
    if (argc >= 3) scale_mm_per_pixel = atof(argv[2]);

    // Charger l'image
    Mat src = imread(filename, IMREAD_COLOR);
    if (src.empty()) {
        cerr << "Erreur: impossible de charger l'image '" << filename << "'" << endl;
        return 1;
    }

    cout << "Image chargée: " << src.cols << "x" << src.rows << " pixels" << endl;

    // Convertir en HSV
    Mat hsv;
    cvtColor(src, hsv, COLOR_BGR2HSV);

    // Créer les masques
    Mat red_mask1, red_mask2, red_mask;
    inRange(hsv, RED_LOW1, RED_HIGH1, red_mask1);
    inRange(hsv, RED_LOW2, RED_HIGH2, red_mask2);
    bitwise_or(red_mask1, red_mask2, red_mask);

    Mat green_mask, fruit_mask;
    inRange(hsv, GREEN_LOW, GREEN_HIGH, green_mask);
    bitwise_not(green_mask, fruit_mask);

    // Nettoyage morphologique
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(red_mask, red_mask, MORPH_CLOSE, kernel);
    morphologyEx(red_mask, red_mask, MORPH_OPEN, kernel);

    // Trouver les contours
    vector<vector<Point>> contours;
    findContours(red_mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (contours.empty()) {
        cout << "Raspberry detected: NONE" << endl;
        imshow("Source", src);
        waitKey(0);
        return 0;
    }

    cout << contours.size() << " contour(s) rouge(s) détecté(s)" << endl;

    // Trouver la framboise la plus large
    double best_width = 0.0;
    Rect best_rect;
    double best_color_ratio = 0.0;
    int best_idx = -1;

    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        
        // ROI pour calculer le ratio
        Mat roi_red = red_mask(rect);
        Mat roi_fruit = fruit_mask(rect);
        
        int red_pixels = countNonZero(roi_red);
        int fruit_pixels = countNonZero(roi_fruit);
        
        double ratio = (fruit_pixels > 0) ? (double)red_pixels / fruit_pixels : 0.0;
        
        if (rect.width > best_width) {
            best_width = rect.width;
            best_rect = rect;
            best_color_ratio = ratio;
            best_idx = i;
        }
    }

    if (best_width <= 0.0) {
        cout << "Raspberry detected: NONE" << endl;
        return 0;
    }

    // Déterminer la maturité
    bool is_ripe = (best_color_ratio >= RIPENESS_THRESHOLD);
    
    // Calculer la largeur en mm
    double width_mm = best_width * scale_mm_per_pixel;
    
    // Confiance couleur (normalisée 0-1)
    double conf_color = min(1.0, max(0.0, best_color_ratio));
    
    // Confiance taille
    double conf_size;
    if (width_mm < EXPECTED_MIN_MM) {
        conf_size = width_mm / EXPECTED_MIN_MM;
    } else if (width_mm > EXPECTED_MAX_MM) {
        conf_size = 1.0 - min(1.0, (width_mm - EXPECTED_MAX_MM) / EXPECTED_MAX_MM);
    } else {
        conf_size = (width_mm - EXPECTED_MIN_MM) / (EXPECTED_MAX_MM - EXPECTED_MIN_MM);
    }
    conf_size = min(1.0, max(0.0, conf_size));

    // Affichage résultats
    cout << "\nRaspberry detected: " << (is_ripe ? "RIPE" : "UNRIPE") << endl;
    cout << "Estimated width: " << width_mm << " mm (" << best_width << " px)" << endl;
    cout << "Confidence (color): " << conf_color << endl;
    cout << "Confidence (size): " << conf_size << endl;

    // Dessiner les résultats
    drawContours(src, contours, -1, Scalar(0, 255, 255), 2);
    rectangle(src, best_rect, Scalar(0, 0, 255), 3);
    
    // Texte
    string label = string(is_ripe ? "RIPE" : "UNRIPE") + " (" + 
                   to_string((int)width_mm) + "mm)";
    putText(src, label, Point(best_rect.x, best_rect.y - 10),
            FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);

    // Afficher
    imshow("Detected", src);
    imshow("Red Mask", red_mask);
    cout << "\nAppuyez sur une touche pour fermer..." << endl;
    waitKey(0);

    return 0;
}